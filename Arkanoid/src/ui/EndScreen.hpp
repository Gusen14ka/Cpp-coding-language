#pragma once
#include "Screen.hpp"
#include "Button.hpp"

class EndScreen : public Screen {
public:
    EndScreen(const sf::Vector2u& win);

    // Установить финальный счёт
    void setScore(int score);

    // Рисует окно завершения игры
    void draw(sf::RenderWindow& w) override;

    // Возвращает true, если кликнули по кнопке "Retry"
    bool handleEvent(const sf::Event& ev) override;

private:
    sf::RectangleShape backdrop_;
    sf::Text overText_;
    sf::Text scoreText_;
    Button retryBtn_;
};