#include "Button.hpp"

Button::Button(const sf::Vector2f& buttonSize,
    const std::string& buttonTitle,
    const sf::Vector2f& buttonPosition,
    const sf::Font& font) 
{
    // Устанавливаем позицию кнопки через Transformable
    setPosition(buttonPosition);

    // Основная форма (локальные координаты)
    mainShape_.setSize(buttonSize);
    mainShape_.setPosition(0, 0); // Относительно позиции кнопки
    mainShape_.setFillColor(sf::Color( 36, 9, 53 ));
    mainShape_.setOutlineThickness(1);
    mainShape_.setOutlineColor(sf::Color(255, 255, 102));

    // Настройка текста
    text_.setFont(font);
    text_.setString(buttonTitle);
    text_.setCharacterSize(50);
    text_.setFillColor(sf::Color(255, 255, 102));
    centerText(buttonSize);

    // Инициализация рельефа
    reliefSize_ = 1.5f;
    initReliefs(buttonSize);
}

void Button::initReliefs(const sf::Vector2f& size) {

    // Позиционирование относительно кнопки (0,0)
    topRelief_.setSize({ size.x, reliefSize_ });
    leftRelief_.setSize({ reliefSize_, size.y });
    bottomRelief_.setSize({ size.x, reliefSize_ });
    rightRelief_.setSize({ reliefSize_, size.y });

    topRelief_.setPosition(reliefSize_, reliefSize_);
    leftRelief_.setPosition(reliefSize_, reliefSize_);
    bottomRelief_.setPosition(reliefSize_, size.y - reliefSize_);
    rightRelief_.setPosition(size.x - reliefSize_, reliefSize_);

    // Цвета границ
    topRelief_.setFillColor(sf::Color(255, 255, 255, 80));
    leftRelief_.setFillColor(sf::Color(255, 255, 255, 80));
    bottomRelief_.setFillColor(sf::Color(0, 0, 0, 80));
    rightRelief_.setFillColor(sf::Color(0, 0, 0, 80));
}

void Button::centerText(const sf::Vector2f& size) {
    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    text_.setPosition(size.x / 2.0f, size.y / 2.0f);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); // Применяем трансформации кнопки
    target.draw(mainShape_, states);
    target.draw(topRelief_, states);
    target.draw(leftRelief_, states);
    target.draw(bottomRelief_, states);
    target.draw(rightRelief_, states);
    target.draw(text_, states);
}

sf::FloatRect Button::getGlobalBounds() const {
    return getTransform().transformRect(mainShape_.getLocalBounds());
}
