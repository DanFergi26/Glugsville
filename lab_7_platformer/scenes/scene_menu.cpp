#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void MenuScene::Load() {
    cout << "Menu Load \n";
    {
        auto txt = makeEntity();
        auto t = txt->addComponent<TextComponent>(
            "Glugsville\nPress:\nSpace to Start\nQ to quit");
    }
    setLoaded(true);
}

void MenuScene::Update(const double& dt) {
    // cout << "Menu Update "<<dt<<"\n";

    // Change scene if Space is pressed
    if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        Engine::ChangeScene(&menu1);
    }

    // Exit the game if Q is pressed
    if (sf::Keyboard::isKeyPressed(Keyboard::Q)) {
        std::cout << "Exiting the game..." << std::endl;
        exit(0); // Correct way to exit the program
    }

    Scene::Update(dt);
}
