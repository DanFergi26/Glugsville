#include "engine.h"
#include "game.h" 
#include "scenes/scene_menu.h"

using namespace std;


MenuScene menu;
Menu1Scene menu1;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
Menu2Scene menu2;
Level4Scene level4;
Level5Scene level5;
Level6Scene level6;
Level7Scene level7;
Menu3Scene menu3;

int main() {
  Engine::Start(1920, 1080, "Platformer",&menu);

}