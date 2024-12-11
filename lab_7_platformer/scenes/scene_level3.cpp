#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_player_turret.h"
#include "../game.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_bullet.h"
#include <LevelSystem.h>
#include <SFML/Graphics.hpp>   // For SFML components like Vector2f, Color, etc.
#include <SFML/Audio.hpp>      // For SFML audio components
#include <SFML/Window.hpp>     // For SFML window and events
#include <SFML/System.hpp> 
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static sf::Music backgroundMusic;

void Level3Scene::Load() {
    cout << "Scene 3 Load" << endl;
    ls::loadLevelFile("res/level_3.txt", 40.0f);
    auto ho = Engine::getWindowSize().y - ((ls::getHeight() + 8) * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Load and play background music
    if (!backgroundMusic.openFromFile("res/music/scotland-the-brave-8-bit-chiptune.wav")) {
        cerr << "Failed to load background music!" << endl;
    }
    else {
        backgroundMusic.setLoop(true); // Enable looping
        backgroundMusic.play();       // Start playing music
    }

    // Create player
    {
        {
            player = makeEntity();
            player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
            player->addTag("player");

            // Add SpriteComponent to player
            auto texture = make_shared<sf::Texture>();
            if (!texture->loadFromFile("res/img/mcafferty.png")) {
                cerr << "Failed to load player sprite!" << endl;
            }
            else {
                auto spriteComp = player->addComponent<SpriteComponent>();
                spriteComp->setTexture(texture); // Assign texture to the SpriteComponent

                // Scale the sprite to fit the player size (30x40)
                sf::Vector2f targetSize(30.f, 40.f);
                sf::Vector2u textureSize = texture->getSize();
                spriteComp->getSprite().setScale(
                    targetSize.x / textureSize.x,
                    targetSize.y / textureSize.y
                );

                // Set the origin of the sprite to its center
                spriteComp->getSprite().setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
            }

            player->addComponent<PlayerPhysicsComponent>(Vector2f(30.f, 40.f));
            player->addComponent<PlayerTurretComponent>();

            // Load texture for the wall tiles ('w')
            auto wallTexture = make_shared<sf::Texture>();
            if (!wallTexture->loadFromFile("res/img/groundTile1.png")) {
                cerr << "Failed to load groundTile1 texture!" << endl;
            }

            // Add physics colliders and sprites for wall tiles ('w')
            {
                auto walls = ls::findTiles(ls::WALL);
                for (auto w : walls) {
                    auto pos = ls::getTilePosition(w);
                    pos += Vector2f(20.f, 20.f); // Offset to center
                    auto e = makeEntity();
                    e->setPosition(pos);

                    // Add sprite component with wall texture
                    auto spriteComp = e->addComponent<SpriteComponent>();
                    spriteComp->setTexture(wallTexture);  // Set wall texture

                    // Scale the sprite to match tile size (40x40)
                    sf::Vector2f targetSize(40.f, 40.f);
                    sf::Vector2u textureSize = wallTexture->getSize();
                    spriteComp->getSprite().setScale(
                        targetSize.x / textureSize.x,
                        targetSize.y / textureSize.y
                    );

                    // Set the origin of the sprite to its center
                    spriteComp->getSprite().setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

                    // Add physics component to the wall entity
                    e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
                }
            }

            cout << " Scene 3 Load Done" << endl;
            setLoaded(true);
        }
    }
}

void Level3Scene::UnLoad() {
    cout << "Scene 3 UnLoad" << endl;
    player.reset();
    backgroundMusic.stop();
    ls::unload();
    Scene::UnLoad();
}



void Level3Scene::Update(const double& dt) {
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

    
    const auto pp = player->getPosition();
    if (ls::getTileAt(pp) == ls::END) {
        Engine::ChangeScene((Scene*)&menu2);
    }
    else if (!player->isAlive()) {
        Engine::ChangeScene((Scene*)&level3);
    }
    Scene::Update(dt);
}

    

void Level3Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
