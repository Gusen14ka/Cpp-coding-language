#pragma once
#include <SFML/Graphics.hpp>
#include "EnumTypes.hpp"
#include "ResourceHolder.hpp"

class Bonus {
public:
	// Скорость падения бонуса
	static constexpr float FALL_SPEED = 150.f;

	// Размер бонусов
	static constexpr float SIZE = 50.f;

	// Конструктор: тип бонуса и стартовая позиция центра 
	Bonus(BonusType type, const sf::Vector2f& startPos);

	// Логика падения
	void update(float dt);

	// Отрисовывает бонус
	void draw(sf::RenderWindow& window) const;

	// Геттер границ для коллизии
	sf::FloatRect getBounds() const;

	// Геттер текущей позиции
	sf::Vector2f getPosition() const;

	// Геттер типа бонуса
	BonusType getBonusType() const;

private:
	BonusType type_;
	sf::Sprite sprite_;

};