#include <SFML/Graphics.hpp>
#include <string>
namespace ut {
  extern sf::Font dfont;
  // text
  sf::Text createText(float x, float y, std::string str, sf::Font &inFont, float size);
  sf::Text createText(float x, float y, std::string str, sf::Font &inFont);
  sf::Text createText(float x, float y, std::string str, float size);
  sf::Text createText(float x, float y, std::string str);
  sf::Text createCenteredText(float x, float y, std::string str, sf::Font &inFont, float size);
  sf::Text createCenteredText(float x, float y, std::string str, float size);
  sf::Text createCenteredText(float x, float y, std::string str, sf::Font &inFont);
  sf::Text createCenteredText(float x, float y, std::string str);
  // rects
  sf::RectangleShape rectangle(float x, float y, float width, float height);
  sf::RectangleShape centRectangle(float x, float y, float width, float height);
  sf::FloatRect frect_from_shape(sf::RectangleShape shape);
  sf::IntRect irect_from_shape(sf::RectangleShape shape);
}
