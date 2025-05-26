#pragma once
#include "Screen.hpp"
#include "ResourceHolder.hpp"
#include "Button.hpp"

class StartScreen: public Screen {
public:
    StartScreen(const sf::Vector2u& winSize);

    // Рисует окно старта игры
    void draw(sf::RenderWindow& w) override;

    // Возвращает true, если кликнули по кнопке «Play»
    bool handleEvent(const sf::Event& ev) override;

private:
    sf::RectangleShape backdrop_;
    sf::Text title_;
    Button  playBtn_;
};