#include "scene_menu1.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <chrono> // For timing

using namespace std;
using namespace sf;

// Variable to keep track of the elapsed time
static float elapsedTime = 0.f;

void Menu1Scene::Load() {
    cout << "Menu 1 Load \n";

    // Create and display the text
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Glugsville\n Level 1:\n Buchanan Street");

    setLoaded(true);
    elapsedTime = 0.f; // Reset elapsed time
}

void Menu1Scene::Update(const double& dt) {
    // Increment elapsed time
    elapsedTime += static_cast<float>(dt);

    // Transition to the next scene after 10 seconds
    if (elapsedTime >= 2.f) {
        Engine::ChangeScene(&level1);
        return;
    }

    Scene::Update(dt);
}