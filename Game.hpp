#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Cell.hpp"

class Game {
public:
  Game();
  ~Game();
  void run();
  void fill_rand();
  void show_text();
private:
  const int WINDOW_SIZE = 840;
  const int CELL_SIZE = 12;
  const int GRID_SIZE = WINDOW_SIZE / 12;
  const int ROWS = WINDOW_SIZE / CELL_SIZE;
  const int FPS = 60;
  const int delay1 = 5.0;
  int generation = 0;
  int delay = 40.0;
  bool isRunning;

  sf::RenderWindow window;
  std::vector<Cell> grid, gridCopy;
  sf::Clock clockActivating;
  sf::Time elapsedActivated;
  sf::Clock clockStart;
  sf::Time elapsedStart;
  sf::Font font;
  sf::Text text;
  sf::Text gen;

  int getLivingNeighbors(int x, int y);
};
