#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class SolidSelector {
public:
  SolidSelector(float x, float y, float width, float height,
                const std::vector<std::string> &items)
      : selectedIndex(0), isOpen(false) {
    // Setup the dropdown button (top part)
    button.setSize({width, height});
    button.setPosition(x, y);
    button.setFillColor(sf::Color::White);

    // TODO is this really going to be here? or is it better to
    // pass the font file as creation argument?
    font.loadFromFile("assets/fonts/712_serif.ttf");

    // Setup each option
    for (size_t i = 0; i < items.size(); ++i) {
      sf::RectangleShape optionRect({width, height});
      optionRect.setPosition(
          x,
          y + height * (i + 1)); // Each option is positioned below the button
      optionRect.setFillColor(sf::Color(200, 200, 200));

      sf::Text optionText;
      optionText.setFont(font); // Load a font
      optionText.setString(items[i]);
      optionText.setCharacterSize(20);
      optionText.setFillColor(sf::Color::Black);
      optionText.setPosition(x + 5, y + height * (i + 1) + 5);

      options.push_back({optionRect, optionText});
      std::cout << "button created at " << x << ", " << y << std::endl;
    }

    // Set up the text on the button
    selectedText.setFont(font); // Load a font
    selectedText.setString(items[0]);
    selectedText.setCharacterSize(20);
    selectedText.setFillColor(sf::Color::Black);
    selectedText.setPosition(x + 5, y + 5);
  }

  void handleEvent(const sf::Event &event, const sf::Window &window) {
    if (event.type == sf::Event::MouseButtonPressed) {
      sf::Vector2i mousePos2i = sf::Mouse::getPosition(window);
      sf::Vector2f mousePos2f(static_cast<float>(mousePos2i.x),
                              static_cast<float>(mousePos2i.y));

      if (button.getGlobalBounds().contains(mousePos2f)) {
        isOpen = !isOpen; // Toggle dropdown list
      } else if (isOpen) {
        for (size_t i = 0; i < options.size(); ++i) {
          if (options[i].first.getGlobalBounds().contains(mousePos2f)) {
            selectedIndex = i;
            selectedText.setString(options[i].second.getString());
            std::cout << selectedText.getString().toAnsiString() << std::endl;
            isOpen = false;
            break;
          }
        }
      }
    }
  }

  void draw(sf::RenderWindow &window) {
    window.draw(button);
    window.draw(selectedText);

    if (isOpen) {
      for (const auto &option : options) {
        window.draw(option.first);
        window.draw(option.second);
      }
    }
  }

private:
  sf::RectangleShape button;
  sf::Text selectedText;
  std::vector<std::pair<sf::RectangleShape, sf::Text>> options;
  size_t selectedIndex;
  bool isOpen;
  sf::Font font; // Load your font in the constructor
};

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600),
//                             "SFML SolidSelector Example");

//     // Load font (adjust path as needed)
//     sf::Font font;
//     if (!font.loadFromFile("path/to/your/font.ttf")) {
//         return -1; // Handle error
//     }

//     std::vector<std::string> items = {"Option 1", "Option 2", "Option 3"};
//     SolidSelector dropdown(100, 50, 150, 30, items);

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();

//             dropdown.handleEvent(event, window);
//         }

//         window.clear();
//         dropdown.draw(window);
//         window.display();
//     }

//     return 0;
// }
