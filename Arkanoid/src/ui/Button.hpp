#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable, public sf::Transformable {
public:
    Button(const sf::Vector2f& buttonSize,
        const std::string& buttonTitle,
        const sf::Vector2f& buttonPosition,
        const sf::Font& font);

    sf::FloatRect getGlobalBounds() const;

private:
    sf::RectangleShape mainShape_;
    sf::Text text_;

    // Ёлементы рельефа
    sf::RectangleShape topRelief_;
    sf::RectangleShape leftRelief_;
    sf::RectangleShape bottomRelief_;
    sf::RectangleShape rightRelief_;
    float reliefSize_;

    // »нициализаци€ границ рельефа
    void initReliefs(const sf::Vector2f& size);


    // ÷ентрирование текста
    void centerText(const sf::Vector2f& size);

    // ќтрисовка конпки
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};