#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
//
namespace ut {
  sf::Font dfont;
  sf::Text createText(float x, float y, std::string str, sf::Font &inFont, float size) {
    sf::Text text(str, inFont);
    text.setCharacterSize(size);
    text.setPosition(sf::Vector2f(x, y));
    return text;
  }

  sf::Text createCenteredText(float x, float y, std::string str, sf::Font &inFont, float size) {
    sf::Text text(str, inFont);
    text.setCharacterSize(size);
    sf::FloatRect bd = text.getLocalBounds();;
    text.setOrigin(bd.left + bd.width / 2, bd.top + bd.height / 2);
    text.setPosition(sf::Vector2f(x, y));
    return text;
  }

  sf::Text createCenteredText(float x, float y, std::string str, float size) {
    return createCenteredText(x, y, str, dfont, size);
  }

  sf::Text createCenteredText(float x, float y, std::string str) {
    return createCenteredText(x, y, str, dfont, 30);
  }

  sf::Text createText(float x, float y, std::string text, sf::Font &inFont) {
    return createText(x, y, text, inFont, 18.f);
  }

  sf::Text createText(float x, float y, std::string text, float size) { return createText(x, y, text, dfont, size); }

  sf::Text createText(float x, float y, std::string text) { return createText(x, y, text, dfont); }


  // rects
  sf::RectangleShape rectangle(float x, float y, float width, float height) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    return rect;
  }

  sf::RectangleShape centRectangle(float x, float y, float width, float height) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x - width / 2, y - height / 2);
    return rect;
  }

  sf::FloatRect frect_from_shape(sf::RectangleShape shape) {
    return sf::FloatRect(shape.getPosition().x, shape.getPosition().y, shape.getSize().x, shape.getSize().y);
  }
  sf::IntRect irect_from_shape(sf::RectangleShape shape) {
    return sf::IntRect(shape.getPosition().x, shape.getPosition().y, shape.getSize().x, shape.getSize().y);
  }
}

