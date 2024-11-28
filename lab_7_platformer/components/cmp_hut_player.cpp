#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {
      lives_--; // take away a life 
      if (lives_ <= 0) {
       pl->setForDelete();
      _parent->setForDelete();
      } 
      }
    }
  }


HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]), lives_(3) {} // sets default lives as 3
