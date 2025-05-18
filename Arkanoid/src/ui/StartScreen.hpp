#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.hpp"
#include "Button.hpp"

class StartScreen {
public:
    StartScreen(const sf::Vector2u& winSize);

    // Рисует окно старта игры
    void draw(sf::RenderWindow& w);

    // Возвращает true, если кликнули по кнопке «Play»
    bool handleEvent(const sf::Event& ev);

private:
    sf::RectangleShape backdrop_;
    sf::Text title_;
    Button  playBtn_;
};