#include <SFML/Graphics.hpp>
#include "utils.hpp"
#include <iostream>
#include <ctime>
#include <vector>
#include <math.h>

const int WIDTH = 1200;
const int HEIGHT = 900;
const int maxPongAngle = 75;
enum Difficulty { EASY, MEDIUM, HARD };
std::map<Difficulty, std::string> diffMap;
Difficulty gameDiff = EASY;



int boundRand(int low, int high) {
  return std::rand() % (high - low) + high;
}

int diffSpeed() {
  switch(gameDiff){
    case EASY: return boundRand(1, 4);
    case MEDIUM: return boundRand(2, 6);
    case HARD: return boundRand(4, 8);
  }
}


int pongSpeed = 12;

class Button {
  float x, y, width, height;
  std::string text;
  Difficulty diff;
  public:
    sf::RectangleShape shape;
    Button(Difficulty diff, float x, float y, float width, float height, std::string text): diff(diff), x(x), y(y), width(width), height(height), text(text), shape(sf::Vector2f(width, height)) {
      shape.setPosition(x, y);
      shape.setFillColor(sf::Color(55, 55, 55));
    }
    float get_x() { return x; }
    float get_y() { return y; }
    float get_width() { return width; }
    float get_height() { return height; }
    std::string get_text() { return text; }
    Difficulty get_difficulty() { return diff; }
};

std::vector<Button> buttons;
float lastButtonY = (WIDTH / 2) - 200;
void add_button(Difficulty diff, float x, std::string text) {
  buttons.push_back(Button(diff, x, lastButtonY, 200, 40, text));
  lastButtonY += 60;
}

class Pong {
  protected:
    float x, y;
  public:
    int score;
    sf::RectangleShape shape;
    Pong(float x, float y, float w, float h) { shape = sf::RectangleShape(sf::Vector2f(w, h)); shape.setPosition(x, y); }
} player(20, 20, 20, 220), robot(WIDTH - 40, 20, 20, 220);


std::pair<float, float> og_ball_vel;

class Ball : public Pong {
  public:
    Ball(float x, float y, float w, float h, bool iball): Pong(x, y, w, h) {
      if(iball) {
        reset();
        shape.setFillColor(sf::Color::Blue);
      } else {
        dx = (std::rand() % 2 == 1 ? std::rand() % 10 : -(std::rand() % 15));
        dy = -(std::rand() % 2 == 1 ? std::rand() % 10 : -(std::rand() % 15));
        og_ball_vel = std::make_pair(dx, dy);
      }
    };
    void reset() {
      dx = og_ball_vel.first * 1.05;
      dy = og_ball_vel.second * 1.05;
    }
    float dx, dy;
    bool left = true;
} ball(575, (900 / 2) - 25, 25, 25, false), iball(575, (900 / 2) - 25, 25, 25, true);

bool running = false;

struct Bound {
  int xMin, xMax, yMin, yMax;
  bool isWithin(sf::Vector2i point) {
    return point.x >= xMin && point.x <= xMax && point.y >= yMin && point.y <= yMax;
  }
} pongBound{0, 1200, 0, 900};

bool rect_intersect(sf::RectangleShape a, sf::RectangleShape b) {
  return sf::FloatRect(a.getPosition(), a.getSize()).intersects(sf::FloatRect(b.getPosition(), b.getSize()));
}

void input() {
}

void reset() {
  iball.shape.setPosition(WIDTH / 2, HEIGHT / 2);
  ball.shape.setPosition(WIDTH / 2, HEIGHT / 2);
  og_ball_vel = std::make_pair(ball.dx, ball.dy);
  iball.reset();
}
  
void logic(sf::RenderWindow& wind) {
  if(!running) {
    for(std::vector<Button>::iterator b = buttons.begin(); b != buttons.end(); b++) {
          auto m = sf::Mouse::getPosition(wind);
          if(ut::irect_from_shape(b->shape).contains(m)) {
            b->shape.setFillColor(sf::Color(110, 110, 110));
          } else
            b->shape.setFillColor(sf::Color(55, 55, 55));
    }
  } else {
    auto bx = ball.shape.getPosition().x;
    auto by = ball.shape.getPosition().y;

    int mov = diffSpeed();

    auto mpr = robot.shape.getPosition().y + robot.shape.getSize().y / 2;
    robot.shape.move(0, iball.shape.getPosition().y > mpr ? mov : -mov);

    if(iball.shape.getPosition().y + iball.dy <= 0 || by + iball.dy > HEIGHT)
      iball.dy = -iball.dy;

    if(iball.shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
      iball.dx = -ball.dx;
      iball.shape.move(ball.dx, 0);
    } else if(iball.shape.getGlobalBounds().intersects(robot.shape.getGlobalBounds())) {
      iball.dx = -iball.dx;
      iball.shape.move(iball.dx, 0);
    } else {
      iball.shape.move(iball.dx, iball.dy);
    }

    if(iball.shape.getPosition().y + iball.dy <= 0 || iball.shape.getPosition().y + iball.dy > HEIGHT) {
      iball.dy = -iball.dy;
    } else if(iball.shape.getPosition().x + iball.dx <= 0) {
      iball.dx = -iball.dx;
    }

    if(by + ball.dy <= 0 || by + ball.dy > HEIGHT) {
      ball.dy = -ball.dy;
    } else if(bx + ball.dx >= WIDTH - ball.shape.getSize().x) {
      player.score++;
      reset();
    } else if(bx + ball.dx <= 0) {
      robot.score++;
      reset();
    }


    float bmp = ball.shape.getPosition().y;
    sf::FloatRect pSlice(player.shape.getSize().x - 2, player.shape.getPosition().y, player.shape.getSize().x,  player.shape.getSize().y);
    sf::FloatRect rSlice(robot.shape.getPosition().x + 2, robot.shape.getPosition().y, robot.shape.getSize().x,  robot.shape.getSize().y);
    if(ball.shape.getGlobalBounds().intersects(pSlice)) {
      float sectY = (player.shape.getPosition().y + (player.shape.getSize().y / 2) - bmp);
      float normSectY = (sectY / (player.shape.getSize().y / 2));
      float angle = normSectY * maxPongAngle;
      ball.dx = -ball.dx;
      ball.shape.move(ball.dx, 0);
      ball.dy = pongSpeed * sin(angle);
      ball.dx = pongSpeed * -cos(angle);
      iball.reset();
    } else if(ball.shape.getGlobalBounds().intersects(rSlice)) {
      float sectY = (robot.shape.getPosition().y + (robot.shape.getSize().y / 2) - bmp);
      float normSectY = (sectY / (robot.shape.getSize().y / 2));
      float angle = normSectY * maxPongAngle;
      ball.dx = -ball.dx;
      ball.shape.move(ball.dx, 0);
      ball.dy = pongSpeed * sin(angle);
      ball.dx = pongSpeed * -cos(angle);
      iball.reset();
    } else {
      ball.shape.move(ball.dx, ball.dy);
    }
  }
}

void draw(sf::RenderWindow &wind) {
  sf::RectangleShape guide(sf::Vector2f(5, 10));
  guide.setPosition(20, sf::Mouse::getPosition(wind).y);
  guide.setSize(sf::Vector2f(sf::Mouse::getPosition(wind).x - player.shape.getPosition().x, 10));
  guide.setFillColor(sf::Color(200, 200, 200, 100));
  // wind.draw(ut::centRectangle(robot.shape.getPosition().x, robot.shape.getPosition().y + robot.shape.getSize().y / 2, 20, 20));
  // wind.draw(ut::centRectangle(robot.shape.getPosition().x, robot.shape.getPosition().y + robot.shape.getSize().y, 20, 20));
  wind.draw(guide);
   //wind.draw(ut::rectangle(sf::Mouse::getPosition(wind).x, sf::Mouse::getPosition(wind).y, 20, 20));
   if(!running) {
    std::for_each(buttons.begin(), buttons.end(), [&wind](Button b){ 
        wind.draw(b.shape);
        wind.draw(
            ut::createCenteredText(
              b.shape.getPosition().x + b.shape.getSize().x / 2,
              b.shape.getPosition().y + b.shape.getSize().y / 2,
              b.get_text(),
              35
            ));
        });
   } else {
      //wind.draw(ut::createText(10, 10, std::to_string(player.score), 45));
      //wind.draw(ut::createText(WIDTH - 40, 10, std::to_string(robot.score), 45));

      wind.draw(ut::rectangle(WIDTH / 2, 0, 1, HEIGHT));
      wind.draw(ut::createCenteredText((WIDTH / 4) * 3, 35, std::to_string(robot.score), 50));
      wind.draw(ut::createCenteredText((WIDTH / 4), 35, std::to_string(player.score), 50));
      wind.draw(ut::createText(10, HEIGHT - 60, diffMap[gameDiff], 50));
   }
  // wind.draw(ut::createText(2, 2, std::to_string(pongSpeed), 50));
  // wind.draw(ut::createText(2, 54, std::to_string(ball.dx), 50));
  // wind.draw(ut::createText(2, 108, std::to_string(ball.dy), 50));
  if( pongBound.isWithin(sf::Mouse::getPosition(wind)) && sf::Mouse::getPosition(wind).y - player.shape.getSize().y / 2 >= 20 && sf::Mouse::getPosition(wind).y + player.shape.getSize().y / 2 <= 880) {
    player.shape.setPosition(player.shape.getPosition().x, sf::Mouse::getPosition(wind).y - player.shape.getSize().y / 2);
  }
  wind.draw(player.shape);
  wind.draw(robot.shape);
  if(running) {
    wind.draw(ball.shape);
  } else {
    wind.draw(ut::createCenteredText(WIDTH / 2, HEIGHT / 3, "Pong", 150));
  }
}

int main(void) {
  std::srand(std::time(NULL));

  float btnX = (WIDTH / 2) - 100;
  diffMap[EASY] = "Easy";
  diffMap[MEDIUM] = "Medium";
  diffMap[HARD] = "Hard";

  add_button(EASY, btnX, "Easy");
  add_button(MEDIUM, btnX, "Medium");
  add_button(HARD, btnX, "Hard");

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pong", sf::Style::Default, settings);
  window.setVerticalSyncEnabled(true);
  if(!ut::dfont.loadFromFile("res/lato.ttf")) {
    return 1;
  }

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed){
        window.close();
      } else if(event.type == sf::Event::MouseButtonReleased) {
        auto pos = event.mouseButton;
        for(auto b = buttons.begin(); b != buttons.end(); b++) {
          if(b->shape.getGlobalBounds().contains(pos.x, pos.y)) {
            gameDiff = b->get_difficulty();
            running = true;
          }
        }
      } else if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Left) {
          pongSpeed--;
        } else if(event.key.code == sf::Keyboard::Right) {
          pongSpeed++;
        }
      }
    }

    input();
    logic(window);
    window.clear(sf::Color(45, 45, 45));
    draw(window);
    window.display();
  }
  return 0;
}

