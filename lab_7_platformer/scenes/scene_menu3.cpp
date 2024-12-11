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

    auto& text = t->getText(); // Get the sf::Text object

    // Center the text on the screen
    auto windowSize = Engine::getWindowSize();
    auto textSize = text.getLocalBounds(); // Get the bounding box of the text
    text.setOrigin(textSize.width / 2.f, textSize.height / 2.f); // Set the origin to the center of the text
    text.setPosition(Vector2f(windowSize.x / 2.f, windowSize.y / 2.f)); // Position the text at the center of the window

    setLoaded(true);
    elapsedTime = 0.f; // Reset elapsed time
}

void Menu3Scene::Update(const double& dt) {
    // Increment elapsed time
    elapsedTime += static_cast<float>(dt);

    // Transition to the next scene after 10 seconds
    if (elapsedTime >= 2.f) { // Change to 10 seconds instead of 2
        Engine::ChangeScene(&level7);
        return;
    }

    Scene::Update(dt);
}