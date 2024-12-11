#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_player_turret.h" 
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
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

// Declare sound buffers and sound objects for jump and spacebar sounds
static sf::SoundBuffer jumpSoundBuffer;
static sf::SoundBuffer spacebarSoundBuffer;
static sf::Sound jumpSound;
static sf::Sound spacebarSound;

void Level2Scene::Load() {
    cout << "Scene 2 Load" << endl;
    ls::loadLevelFile("res/level_2.txt", 40.0f);
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

    // Load sound effects
    if (!jumpSoundBuffer.loadFromFile("res/music/jump.wav")) {
        cerr << "Failed to load jump sound!" << endl;
    }
    else {
        jumpSound.setBuffer(jumpSoundBuffer); // Set jump sound buffer
    }

    if (!spacebarSoundBuffer.loadFromFile("res/music/knife.wav")) {
        cerr << "Failed to load spacebar sound!" << endl;
    }
    else {
        spacebarSound.setBuffer(spacebarSoundBuffer); // Set spacebar sound buffer
    }

    // Load sound effects
    if (!jumpSoundBuffer.loadFromFile("res/music/jump.wav")) {
        cerr << "Failed to load jump sound!" << endl;
    }
    else {
        jumpSound.setBuffer(jumpSoundBuffer); // Set jump sound buffer
    }

    if (!spacebarSoundBuffer.loadFromFile("res/music/knife.wav")) {
        cerr << "Failed to load spacebar sound!" << endl;
    }
    else {
        spacebarSound.setBuffer(spacebarSoundBuffer); // Set spacebar sound buffer
    }

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
        player->addComponent<PlayerTurretComponent>();
    }

    // Load texture for interactive tiles ('i' or WINDOW)
    auto windowTexture = make_shared<sf::Texture>();
    if (!windowTexture->loadFromFile("res/img/window.png")) {
        cerr << "Failed to load window texture!" << endl;
    }

    // Add sprites for interactive tiles ('i' or WINDOW)
    {
        auto interactiveTiles = ls::findTiles(ls::WINDOW); // Find all tiles marked as 'i' or WINDOW
        for (auto t : interactiveTiles) {
            auto pos = ls::getTilePosition(t);
            pos += Vector2f(20.f, 20.f); // Offset to center
            auto e = makeEntity();
            e->setPosition(pos);

            // Add sprite component with window texture
            auto spriteComp = e->addComponent<SpriteComponent>();
            spriteComp->setTexture(windowTexture);  // Set window texture

            // Scale the sprite to match tile size (40x40)
            sf::Vector2f targetSize(40.f, 40.f);
            sf::Vector2u textureSize = windowTexture->getSize();
            spriteComp->getSprite().setScale(
                targetSize.x / textureSize.x,
                targetSize.y / textureSize.y
            );

            // Set the origin of the sprite to its center
            spriteComp->getSprite().setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);

            // Add physics if interaction is required
            e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
        }
    }
    // Create Enemies
    {
        auto enemyPositions = ls::findTiles(ls::ENEMY); // Find all 'n' tiles

        // Iterate over each enemy position
        for (const auto& pos : enemyPositions) {
            auto enemy = makeEntity();
            enemy->setPosition(ls::getTilePosition(pos) + Vector2f(0, 24)); // Set position with offset
            enemy->addTag("enemy");

            // Load the sprite texture
            std::shared_ptr<sf::Texture> enemyTexture = std::make_shared<sf::Texture>();
            if (!enemyTexture->loadFromFile("res/img/enemy.png")) { // Replace "enemy.png" with your sprite filename
                std::cerr << "Error loading enemy sprite!" << std::endl;
                continue; // Skip this enemy if texture loading fails
            }

            // Add a SpriteComponent and set the texture
            auto sprite = enemy->addComponent<SpriteComponent>();
            sprite->setTexture(enemyTexture);

            // Scale the sprite to fit the desired size of 20x30
            sf::Vector2f targetSize(30.f, 40.f); // Desired size
            sf::Vector2u textureSize = enemyTexture->getSize(); // Original texture size
            sprite->getSprite().setScale(
                targetSize.x / textureSize.x,
                targetSize.y / textureSize.y
            );

            // Adjust the origin of the sprite to the center
            sprite->getSprite().setOrigin(
                textureSize.x / 2.f,
                textureSize.y / 2.f
            );

            // Add HurtComponent for interaction with the player
            enemy->addComponent<HurtComponent>();

            // Add EnemyAIComponent for movement/behavior
            auto ai = enemy->addComponent<EnemyAIComponent>();
            ai->setFlipSpriteOnDirection(true); // Enable flipping the sprite on direction change
        }
    }

    // Create Turret
    {
        auto turret = makeEntity();

        // Position the turret based on the tile position
        turret->setPosition(ls::getTilePosition(ls::findTiles('t')[0]) + Vector2f(20, 0));

        // Make the turret invisible by not adding any visual components

        // Add the turret's logic component
        turret->addComponent<EnemyTurretComponent>();
    }

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

    cout << " Scene 2 Load Done" << endl;
    setLoaded(true);
}


void Level2Scene::UnLoad() {
    cout << "Scene 2 UnLoad" << endl;
    player.reset();
    backgroundMusic.stop();
    ls::unload();
    Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {
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

    // Play jump sound when the Up arrow key is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        jumpSound.play();
    }

    // Play spacebar sound when the Spacebar is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        spacebarSound.play();
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
