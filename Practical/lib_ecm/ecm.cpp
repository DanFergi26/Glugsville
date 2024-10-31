#include "ecm.h"
#include <unordered_map>
#include <vector>

using namespace std;

Entity::Entity(Scene* const s)
    : _position({0, 0}), _rotation(0), _alive(true), _visible(true),
      scene(s), _fordeletion(false) {}

void Entity::addTag(const std::string& t) {
    _tags.insert(t);
    if (scene) {
        scene->tagEntity(t, shared_from_this());  // Ensure scene is updated
    }
}

void Entity::removeTag(const std::string& t) {
    _tags.erase(t);
    if (scene) {
        scene->untagEntity(t, shared_from_this());  // Ensure scene is updated
    }
}

const std::set<std::string>& Entity::getTags() const { return _tags; }

// Other Entity methods remain unchanged

Component::Component(Entity* const p) : _parent(p), _fordeletion(false) {}

Entity::~Entity() {
    // Clear tags from scene manager before deletion
    for (const auto& tag : _tags) {
        if (scene) {
            scene->untagEntity(tag, shared_from_this());
        }
    }

    // Existing cleanup logic
    int deli = 0;
    while (deli != _components.size()) {
        deli = _components.size();
        _components.erase(
            remove_if(_components.begin(), _components.end(),
                      [](auto& sp) { return (sp.use_count() <= 1); }),
            _components.end());
    }

    if (_components.size() > 0) {
        throw std::runtime_error(
            "Can't delete entity, someone is grabbing a component!");
    }

    _components.clear();
}

// EntityManager with tagging support
class EntityManager {
public:
    // New map to track tags to entities
    std::unordered_map<std::string, std::vector<std::weak_ptr<Entity>>> tag_map;

    void tagEntity(const std::string& tag, std::shared_ptr<Entity> entity) {
        tag_map[tag].push_back(entity);
    }

    void untagEntity(const std::string& tag, std::shared_ptr<Entity> entity) {
        auto& entities = tag_map[tag];
        entities.erase(
            remove_if(entities.begin(), entities.end(),
                      [&entity](const weak_ptr<Entity>& e) { return e.lock() == entity; }),
            entities.end());
        if (entities.empty()) {
            tag_map.erase(tag); // Remove empty entries
        }
    }

    vector<shared_ptr<Entity>> find(const std::string& tag) const {
        vector<shared_ptr<Entity>> ret;
        auto it = tag_map.find(tag);
        if (it != tag_map.end()) {
            for (const auto& e : it->second) {
                if (auto entity = e.lock()) {
                    ret.push_back(entity);
                }
            }
        }
        return ret;
    }

    vector<shared_ptr<Entity>> find(const vector<std::string>& tags) const {
        vector<shared_ptr<Entity>> ret;
        for (const auto& tag : tags) {
            auto entities = find(tag);
            ret.insert(ret.end(), entities.begin(), entities.end());
        }
        return ret;
    }

    // Other EntityManager methods remain unchanged
};
