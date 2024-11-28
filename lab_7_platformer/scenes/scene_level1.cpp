#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_player_turret.h" 
#include "../game.h"
#include <LevelSystem.h>
#include <SFML/Graphics.hpp>   // For SFML components like Vector2f, Color, etc.
#include <SFML/Window.hpp>     // For SFML window and events
#include <SFML/System.hpp> 
#include <SFML/Audio.hpp>       // For SFML audio
#include <iostream>             // For std::cout and std::endl
#include <thread>               // For std::this_thread::sleep_for

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Load() {
    cout << " Scene 1 Load" << endl;
    ls::loadLevelFile("res/level_1.txt", 40.0f);
    auto ho = max(0.f, Engine::getWindowSize().y - ((ls::getHeight() + 5) * 40.f));
    ls::setOffset(Vector2f(0, ho));

    // code for music class etc
    music_.openFromFile("res/music/level1.wav");
    music_.setLoop(true);
    music_.play();

    // Create player
    {
        player = makeEntity();
        player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));


        // Load texture for the player
        auto texture = make_shared<sf::Texture>();
        if (!texture->loadFromFile("res/img/mcafferty.png")) {
            cerr << "Failed to load player sprite!" << endl;
        }
        else {
            auto spriteComp = player->addComponent<SpriteComponent>();
            spriteComp->setTexture(texture); // Assign texture to the SpriteComponent

            // Scale the sprite to fit the player size (20x30)
            sf::Vector2f targetSize(30.f, 40.f);
            sf::Vector2u textureSize = texture->getSize();
            spriteComp->getSprite().setScale(
                targetSize.x / textureSize.x,
                targetSize.y / textureSize.y
            );

            // Set the origin of the sprite to its center
            spriteComp->getSprite().setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        }

        // Add physics to the player
        player->addComponent<PlayerPhysicsComponent>(Vector2f(30.f, 40.f));
        player->addComponent<PlayerTurretComponent>();

    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f); // Offset to center
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
    }

    // Simulate long loading times
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << " Scene 1 Load Done" << endl;

    setLoaded(true);
}

void Level1Scene::UnLoad() {
    cout << "Scene 1 Unload" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
    music_.stop();
}

void Level1Scene::Update(const double& dt) {
    // Get the SpriteComponent of the player
    bool isLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    auto spriteComp = player->getComponent<SpriteComponent>();

    //Flip sprite horizontally if the left arrow key is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        spriteComp->getSprite().setScale(-abs(spriteComp->getSprite().getScale().x), spriteComp->getSprite().getScale().y);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        spriteComp->getSprite().setScale(abs(spriteComp->getSprite().getScale().x), spriteComp->getSprite().getScale().y);
    }

    // Center the view on the player, with constraints
    auto windowSize = Engine::getWindowSize();
    auto levelWidth = ls::getWidth() * 40.f;

    Vector2f viewCenter = player->getPosition();
    viewCenter.y = windowSize.y / 2.0f;  // Center vertically
    viewCenter.x = std::max(windowSize.x / 2.0f, viewCenter.x);  // Prevent scrolling left out of bounds
    viewCenter.x = std::min(levelWidth - windowSize.x / 2.0f, viewCenter.x);  // Prevent scrolling past right boundary

    // Set the new view
    sf::View view;
    view.setSize(Vector2f(windowSize.x, windowSize.y));
    view.setCenter(viewCenter);
    Engine::GetWindow().setView(view);

    if (ls::getTileAt(player->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&level2);
    }
    Scene::Update(dt);
}


void Level1Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
