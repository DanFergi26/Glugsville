#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp> // Include for sf::View
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>

class Scene {
public:
    Scene() = default;
    virtual ~Scene();
    virtual void Load() = 0;
    virtual void LoadAsync();
    virtual void UnLoad();
    virtual void Update(const double& dt);
    virtual void Render();
    bool isLoaded() const;
    std::shared_ptr<Entity> makeEntity();

    EntityManager ents;

protected:
    void setLoaded(bool);
private:
    mutable bool _loaded;
    mutable std::future<void> _loaded_future;
    mutable std::mutex _loaded_mtx;
};

class Engine {
public:
    Engine() = delete;

    // Core functions
    static void Start(unsigned int width, unsigned int height,
        const std::string& gameName, Scene* scn);
    static void ChangeScene(Scene*);
    static sf::RenderWindow& GetWindow();
    static sf::Vector2u getWindowSize();
    static void setVsync(bool b);

    // Camera-related functions
    static sf::View& GetView();             // Getter for the current camera view
    static void SetView(const sf::View& v); // Setter to update the camera view

private:
    static Scene* _activeScene;
    static std::string _gameName;
    static void Update();
    static void Render(sf::RenderWindow& window);

    static sf::View _view;                   // Camera view
};

namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing