#include "scene_menu3.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <chrono> // For timing

using namespace std;
using namespace sf;

// Variable to keep track of the elapsed time
static float elapsedTime = 0.f;

void Menu3Scene::Load() {
    cout << "Menu 3 Load \n";

    // Create and display the text
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Glugsville\n Level 3:\n Kelvin Grove");

    setLoaded(true);
    elapsedTime = 0.f; // Reset elapsed time
}

void Menu3Scene::Update(const double& dt) {
    // Increment elapsed time
    elapsedTime += static_cast<float>(dt);

    // Transition to the next scene after 10 seconds
    if (elapsedTime >= 2.f) {
        Engine::ChangeScene(&level1);
        return;
    }

    Scene::Update(dt);
}