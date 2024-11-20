#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_hurt_player.h"
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

    auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
    auto startTiles = ls::findTiles(ls::START);
    if (startTiles.empty()) {
        cerr << "Error: No START tile found in level!" << endl;
        return;
    }

    player = makeEntity();
    player->setPosition(ls::getTilePosition(startTiles[0]));
    auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    s->getShape().setFillColor(Color::Magenta);
    s->getShape().setOrigin(Vector2f(10.f, 15.f));

    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));

    // Add physics colliders to level tiles
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
        auto pos = ls::getTilePosition(w);
        pos += Vector2f(20.f, 20.f); // offset to center
        auto e = makeEntity();
        e->setPosition(pos);
        e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
    }

    // Center camera on the player
    auto& camera = Engine::GetView();
    camera.setCenter(player->getPosition());
    Engine::SetView(camera);

    // Simulate long loading times
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

// WHEELS ENEMY CODE
  {
    auto wheelsEnemy = makeEntity();
    wheelsEnemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) +
                             Vector2f(100.f, 24.f)); // Offset position slightly
    
    auto s = wheelsEnemy->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(16.f);
    s->getShape().setFillColor(Color::Green); // testing colour for wheels, sprites to be added later 
    s->getShape().setOrigin(Vector2f(16.f, 16.f));

    // hurt component 
    // wheelsEnemy->addComponent<HurtComponent>();

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
