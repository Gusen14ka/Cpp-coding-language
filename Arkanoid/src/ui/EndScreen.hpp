#pragma once
#include <SFML/Graphics.hpp>
#include "Button.hpp"

class EndScreen {
public:
    EndScreen(const sf::Vector2u& win);

    // Установить финальный счёт
    void setScore(int score);

    void draw(sf::RenderWindow& w);
    bool handleEvent(const sf::Event& ev);

private:
    sf::RectangleShape backdrop_;
    sf::Text overText_;
    sf::Text scoreText_;
    Button retryBtn_;
};