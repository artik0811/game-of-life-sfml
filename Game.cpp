#include "Game.hpp"
#include <iostream>

void copyVector(std::vector<Cell> &v1, std::vector<Cell> &v2) 
{
  v2.clear();
  for (int i = 0; i < v1.size(); i++) {
    v2.push_back(v1[i]);
  }
}

Game::Game() 
{ 
  window.create(sf::VideoMode(this->WINDOW_SIZE + 300, this->WINDOW_SIZE), "Game of life");
  window.setFramerateLimit(FPS);
  font.loadFromFile("./resources/sansation.ttf");
  for (int i = 0; i < this->GRID_SIZE; i++) {
    for (int j = 0; j < this->GRID_SIZE; j++) {
      Cell newCell(i, j, this->CELL_SIZE);
      this->grid.push_back(newCell);
      this->gridCopy.push_back(newCell);
    }
  }
  isRunning = false;
}

Game::~Game() { }

int Game::getLivingNeighbors(int x, int y) 
{
  int livingNeighbors = 0;
  if (x < GRID_SIZE - 1                                  &&  grid[y * GRID_SIZE + x + 1].isAlive()) livingNeighbors++;
  if (x > 0                                              &&  grid[y * GRID_SIZE + x - 1].isAlive()) livingNeighbors++;
  if (y < GRID_SIZE - 1                                  &&  grid[(y + 1) * GRID_SIZE + x].isAlive()) livingNeighbors++;
  if (y > 0                                              &&  grid[(y - 1) * GRID_SIZE + x].isAlive()) livingNeighbors++;
  if (y < GRID_SIZE - 1 && x < GRID_SIZE - 1             &&  grid[(y + 1) * GRID_SIZE + x + 1].isAlive()) livingNeighbors++;
  if (y < GRID_SIZE - 1 &&  x > 0                        &&  grid[(y + 1) * GRID_SIZE + x - 1].isAlive()) livingNeighbors++;
  if (y > 0 &&  x < GRID_SIZE - 1                        &&  grid[(y - 1) * GRID_SIZE + x + 1].isAlive()) livingNeighbors++;
  if (y > 0 &&  x > 0                                    &&  grid[(y - 1) * GRID_SIZE + x - 1].isAlive()) livingNeighbors++;

  return livingNeighbors;
}

void Game::run() 
{ // ---------------------------------------------------------------------
    sf::RectangleShape line(sf::Vector2f(840, 1));
    line.setPosition(840, 0);
    line.rotate(90);
    text.setCharacterSize(20);
    text.setFont(font);
    text.setString("              Game of Life\n\nControl:\nP - start/pause\nC - clear grid\nR - random filling grid\nEsc - exit\nLeft arrow - speed--\nRight arrow - speed++\nFill rand working only when\ngame on pause!");
    text.setPosition(sf::Vector2f(860, 20));
    gen.setCharacterSize(20);
    gen.setFont(font);
    gen.setPosition(860, 500);
    std::string g = "Generation: ";
    
    int sp = delay/40;
 //------------------------------------------------------------------------- gui
  fill_rand();
  this->elapsedActivated = this->clockActivating.getElapsedTime();
  while(window.isOpen()) 
  {
      window.clear();
      g = "Generation: ";
      window.draw(line);
      window.draw(text);
      window.draw(gen);
      gen.setString(g + std::to_string(generation));
    sf::Event event;
    this->elapsedActivated = this->clockActivating.getElapsedTime();
    elapsedStart = clockStart.getElapsedTime();
    while (window.pollEvent(event)) 
        {
      if (event.type == sf::Event::Closed) 
      {
        window.close();
      } 
      else if (event.mouseButton.button == sf::Mouse::Left && !isRunning && event.mouseButton.x <= WINDOW_SIZE) 
      {
        int yCell = event.mouseButton.x / CELL_SIZE;
        int xCell = event.mouseButton.y / CELL_SIZE;
        grid[yCell * ROWS + xCell].update(this->window, this->clockActivating, this->elapsedActivated);
        // getLivingNeighbors(xCell, yCell);
      } 
      else if (event.type = sf::Event::KeyPressed) 
      {
        if (event.key.code == sf::Keyboard::P) // P - пауза
        { 
          if (elapsedStart.asSeconds() >= 0.2) {
            isRunning = !isRunning;
            clockStart.restart();
          }
        }
        if (event.key.code == sf::Keyboard::C && !isRunning) // C - очистка поля
        { 
          for (int i = 0; i < grid.size(); i++) 
            grid[i].kill();
          generation = 0;
        }
        if (event.key.code == sf::Keyboard::Right)
        {
            delay = std::max(delay - delay1, 0);
        }
        if (event.key.code == sf::Keyboard::Left)
        {
            delay += delay1;
        }
        if (event.key.code == sf::Keyboard::R && !isRunning)
            fill_rand();
        if (event.key.code == sf::Keyboard::Escape)
            window.close();
      }
    }
    if (isRunning) 
    {
      copyVector(grid, gridCopy);
      for (int i = 0; i < grid.size(); i++) {
        int nb = getLivingNeighbors(grid[i].getY(), grid[i].getX());
        bool lives = gridCopy[i].isAlive();
        if (nb < 2) {
          gridCopy[i].kill();
        } else if (nb > 3) {
          gridCopy[i].kill();
        } else if (nb == 3) {
          gridCopy[i].born();
        }
      }
      copyVector(gridCopy, grid);
      generation++;
    }
    for (int i = 0; i < grid.size(); i++) {
      grid[i].draw(window);
        }
    window.display();
    sf::sleep(sf::milliseconds(delay));
  }
}

void Game::fill_rand()
{
    srand(time(NULL));
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].setlive((double(rand()) / RAND_MAX < 0.1) ? 1 : 0);
    }
    generation = 0;
}

void Game::show_text()
{
}
