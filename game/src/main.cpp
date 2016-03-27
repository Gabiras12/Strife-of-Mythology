#include <iostream>

namespace Game {
  bool init()
  {
    return true;
  }

  void quit()
  {
    return;
  }

  void get_input()
  {
    return;
  }

  void update()
  {
    return;
  }

  void draw()
  {
    return;
  }
};


int main(int argc, char const *argv[])
{
  if (Game::init()) {
    bool quit = false;

    quit = true;
    while (not quit) {
      Game::get_input();
      Game::update();
      Game::draw();
    }

    Game::quit();

  } else {
    std::cout << "Error at init!" << std::endl;
  }
  return 0;
}
