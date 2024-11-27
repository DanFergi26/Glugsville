#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level2Scene::Load() {
    cout << "Scene 2 Load" << endl;
    ls::loadLevelFile("res/level_2.txt", 40.0f);
    auto ho = Engine::getWindowSize().y - ((ls::getHeight() + 5) * 40.f);
    ls::setOffset(Vector2f(0, ho));

    // Create player
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

        // Add physics to the player
        player->addComponent<PlayerPhysicsComponent>(Vector2f(30.f, 40.f));
    }

    // Create Enemy
    {
        auto enemy = makeEntity();
        enemy->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]) +
            Vector2f(0, 24));
        enemy->addTag("enemy");
        enemy->addComponent<HurtComponent>();

        auto s = enemy->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(16.f);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(16.f, 16.f));

        enemy->addComponent<EnemyAIComponent>();
    }

    // Create Turret
    {
        auto turret = makeEntity();
        turret->setPosition(ls::getTilePosition(ls::findTiles('t')[0]) +
            Vector2f(20, 0));
        auto s = turret->addComponent<ShapeComponent>();
        s->setShape<sf::CircleShape>(16.f, 3);
        s->getShape().setFillColor(Color::Red);
        s->getShape().setOrigin(Vector2f(16.f, 16.f));
        turret->addComponent<EnemyTurretComponent>();
    }

    // Add physics colliders to level tiles.
    {
        auto walls = ls::findTiles(ls::WALL);
        for (auto w : walls) {
            auto pos = ls::getTilePosition(w);
            pos += Vector2f(20.f, 20.f);
            auto e = makeEntity();
            e->setPosition(pos);
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
    }

    cout << " Scene 2 Load Done" << endl;
    setLoaded(true);
}


void Level2Scene::UnLoad() {
    cout << "Scene 2 UnLoad" << endl;
    player.reset();
    ls::unload();
    Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {
    // Center the view on the player, with constraints
    auto windowSize = Engine::getWindowSize();
    auto levelWidth = ls::getWidth() * 40.f;

    Vector2f viewCenter = player->getPosition();
    viewCenter.y = windowSize.y / 2.0f; // Center vertically

    // Ensure the view is within the level bounds (left and right)
    viewCenter.x = std::max(windowSize.x / 2.0f, viewCenter.x); // Prevent scrolling left out of bounds
    viewCenter.x = std::min(levelWidth - windowSize.x / 2.0f, viewCenter.x); // Prevent scrolling past right boundary

    // Set the new view (only modify the view size and center)
    sf::View view;
    view.setSize(Vector2f(windowSize.x, windowSize.y));
    view.setCenter(viewCenter);
    Engine::GetWindow().setView(view);

    // Check for level completion or player death
    const auto pp = player->getPosition();
    if (ls::getTileAt(pp) == ls::END) {
        Engine::ChangeScene((Scene*)&level3);
    }
    else if (!player->isAlive()) {
        Engine::ChangeScene((Scene*)&level2);
    }

    Scene::Update(dt);
}

void Level2Scene::Render() {
    ls::render(Engine::GetWindow());
    Scene::Render();
}
