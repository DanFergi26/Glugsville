#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Render() {
    ls::render(Engine::GetWindow()); // Render the level
    Scene::Render();                 // Call base class render
}

void Level1Scene::Load() {
    cout << "Scene 1 Load" << endl;
    ls::loadLevelFile("res/level_1.txt", 40.0f);

    auto startPos = ls::getTilePosition(ls::findTiles(ls::START)[0]);

    // Set camera offset
    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Load player sprite texture
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("res/img/mcafferty.png")) {
        cerr << "Failed to load player sprite!" << std::endl;
    }

    // Create player entity
    player = makeEntity();
    player->setPosition(startPos);
    cout << "Player spawned at: " << player->getPosition().x << ", " << player->getPosition().y << endl;

    // Create and set the player sprite using SpriteComponent
    auto spriteComponent = player->addComponent<SpriteComponent>(player);  // Add SpriteComponent to the player
    spriteComponent->setTexture(std::make_shared<sf::Texture>(playerTexture));  // Set the sprite's texture

    sf::Sprite playerSprite(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));  // Assuming your sprite sheet uses 32x32 tiles
    playerSprite.setOrigin(16.f, 24.f);  // Adjust the origin to center the sprite (if needed)
    spriteComponent->getSprite() = playerSprite;  // Set the player sprite

    // Add physics and other components
    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

    // Add physics colliders to level tiles.
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
        auto pos = ls::getTilePosition(w);
        pos += Vector2f(20.f, 20.f);  // offset to center
        auto e = makeEntity();
        e->setPosition(pos);
        e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    cout << "Scene 1 Load Done" << endl;

    setLoaded(true);
}

void set_camera(sf::View& camera, const sf::Vector2f& playerPosition, const sf::Vector2u& levelSize, const sf::Vector2u& windowSize) {
    // Center the camera on the player's position
    sf::Vector2f newCameraCenter = playerPosition;

    // Calculate half of the window size for bounds checking
    sf::Vector2f halfWindowSize(windowSize.x / 2.0f, windowSize.y / 2.0f);

    // Keep the camera within the bounds of the level
    if (newCameraCenter.x - halfWindowSize.x < 0) {
        newCameraCenter.x = halfWindowSize.x;
    }
    if (newCameraCenter.y - halfWindowSize.y < 0) {
        newCameraCenter.y = halfWindowSize.y;
    }
    if (newCameraCenter.x + halfWindowSize.x > levelSize.x) {
        newCameraCenter.x = levelSize.x - halfWindowSize.x;
    }
    if (newCameraCenter.y + halfWindowSize.y > levelSize.y) {
        newCameraCenter.y = levelSize.y - halfWindowSize.y;
    }

    // Update the camera's center
    camera.setCenter(newCameraCenter);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {
    // Check if player reaches the end
    if (ls::getTileAt(player->getPosition()) == ls::END) {
        Engine::ChangeScene((Scene*)&level2);
    }

    // Update the camera to follow the player
    auto& camera = Engine::GetView();
    camera.setCenter(player->getPosition());
    Engine::SetView(camera);

    // Call the parent update
    Scene::Update(dt);
}
