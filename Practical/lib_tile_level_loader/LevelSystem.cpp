#include "LevelSystem.h"
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

using namespace std;
using namespace sf;

std::map<LevelSystem::Tile, sf::Color> LevelSystem::_colours{
    {WALL, Color::White}, {END, Color::Red}, {EMPTY, Color::Transparent}};

sf::Color LevelSystem::getColor(LevelSystem::Tile t) {
    auto it = _colours.find(t);
    if (it == _colours.end()) {
        _colours[t] = Color::Transparent;  // Default to transparent if unknown
    }
    return _colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
    _colours[t] = c;
}

std::unique_ptr<LevelSystem::Tile[]> LevelSystem::_tiles;
size_t LevelSystem::_width;
size_t LevelSystem::_height;

float LevelSystem::_tileSize(100.f);
Vector2f LevelSystem::_offset(0.0f, 30.0f);
vector<std::unique_ptr<sf::RectangleShape>> LevelSystem::_sprites;

void LevelSystem::loadLevelFile(const std::string& path, float tileSize) {
    _tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    // Load in file to buffer
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    } else {
        throw string("Couldn't open level file: ") + path;
    }

    std::vector<Tile> temp_tiles;
    int widthCheck = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        if (c == '\0') break;
        if (c == '\n') {  // newline
            if (w == 0) {
                w = i;
            } else if (w != (widthCheck - 1)) {
                throw string("Non-uniform width: ") + path;
            }
            widthCheck = 0;
            h++;  // increment height
        } else {
            temp_tiles.push_back(static_cast<Tile>(c));
        }
        ++widthCheck;
    }

    if (temp_tiles.size() != (w * h)) {
        throw string("Can't parse level file: ") + path;
    }
    _tiles = std::make_unique<Tile[]>(w * h);
    _width = w;
    _height = h;
    std::copy(temp_tiles.begin(), temp_tiles.end(), &_tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    buildSprites(true);  // Enable compression
}

void LevelSystem::buildSprites(bool optimise) {
    _sprites.clear();

    struct tp {
        sf::Vector2f p;
        sf::Vector2f s;
        sf::Color c;
    };
    vector<tp> tps;
    const auto tls = Vector2f(_tileSize, _tileSize);

    for (size_t y = 0; y < _height; ++y) {
        for (size_t x = 0; x < _width; ++x) {
            Tile t = getTile({x, y});
            if (t == EMPTY) {
                continue;
            }
            tps.push_back({getTilePosition({x, y}), tls, getColor(t)});
        }
    }

    if (optimise) {
        vector<tp> tpo;

        // Compress horizontally
        for (size_t y = 0; y < _height; ++y) {
            for (size_t x = 0; x < _width; ++x) {
                Tile t = getTile({x, y});
                if (t == EMPTY) continue;

                auto color = getColor(t);
                auto position = getTilePosition({x, y});
                Vector2f size = tls;
                
                size_t x_end = x;
                while (x_end < _width && getTile({x_end, y}) == t) {
                    ++x_end;
                }

                // Expand width if contiguous tiles of the same type are found
                size.x = (x_end - x) * _tileSize;
                tpo.push_back({position, size, color});
                x = x_end - 1;  // Jump to the end of the sequence
            }
        }

        // Compress vertically on the existing horizontal compression
        vector<tp> compressed;
        for (size_t i = 0; i < tpo.size(); ++i) {
            auto base = tpo[i];
            size_t y_end = 1;
            while (i + y_end < tpo.size() && tpo[i + y_end].p.x == base.p.x &&
                   tpo[i + y_end].s == base.s && tpo[i + y_end].c == base.c) {
                ++y_end;
            }
            base.s.y *= y_end;
            compressed.push_back(base);
            i += y_end - 1;
        }

        // Copy compressed sprites back to tps
        tps.swap(compressed);
    }

    // Build sprites based on the compressed data
    for (auto& t : tps) {
        auto s = make_unique<sf::RectangleShape>();
        s->setPosition(t.p);
        s->setSize(t.s);
        s->setFillColor(t.c);
        _sprites.push_back(move(s));
    }

    cout << "Level with " << (_width * _height) << " Tiles, with "
         << " compressed into " << _sprites.size() << " Sprites\n";
}

void LevelSystem::render(RenderWindow& window) {
    for (auto& t : _sprites) {
        window.draw(*t);
    }
}

LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
    if (p.x >= _width || p.y >= _height) {
        throw string("Tile out of range: ") + to_string(p.x) + "," + to_string(p.y);
    }
    return _tiles[(p.y * _width) + p.x];
}

size_t LevelSystem::getWidth() { return _width; }

size_t LevelSystem::getHeight() { return _height; }

sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
    return (Vector2f(p.x, p.y) * _tileSize) + _offset;
}

std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) {
    vector<sf::Vector2ul> positions;
    for (size_t i = 0; i < _width * _height; ++i) {
        if (_tiles[i] == type) {
            positions.push_back({i % _width, i / _width});
        }
    }
    return positions;
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
    auto adjusted = v - _offset;
    if (adjusted.x < 0 || adjusted.y < 0) {
        throw string("Tile out of range");
    }
    return getTile(Vector2ul((adjusted) / _tileSize));
}

bool LevelSystem::isOnGrid(sf::Vector2f v) {
    auto adjusted = v - _offset;
    if (adjusted.x < 0 || adjusted.y < 0) return false;
    auto p = Vector2ul(adjusted / _tileSize);
    return p.x < _width && p.y < _height;
}

void LevelSystem::setOffset(const Vector2f& offset) {
    _offset = offset;
    buildSprites();
}

void LevelSystem::unload() {
    cout << "LevelSystem unloading\n";
    _sprites.clear();
    _tiles.reset();
    _width = 0;
    _height = 0;
    _offset = {0, 0};
}

const Vector2f& LevelSystem::getOffset() { return _offset; }

float LevelSystem::getTileSize() { return _tileSize; }
