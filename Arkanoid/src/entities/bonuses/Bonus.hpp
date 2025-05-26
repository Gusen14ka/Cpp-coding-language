#pragma once
#include <SFML/Graphics.hpp>
//#include "ResourceHolder.hpp"



class Bonus {
public:
	// Скорость падения бонуса
	static constexpr float FALL_SPEED = 150.f;

	// Размер бонусов
	static constexpr float SIZE = 50.f;

	// Конструктор: стартовая позиция центра 
	explicit Bonus(const sf::Vector2f& pos);

	virtual ~Bonus() = default;

	// Логика падения
	void update(float dt);

	// Отрисовывает бонус
	void draw(sf::RenderWindow& window) const;

	// Геттер границ для коллизии
	sf::FloatRect getBounds() const;

	// Геттер текущей позиции
	sf::Vector2f getPosition() const;

	// Вызывается при ловле, определение в наследниках
	virtual void apply() = 0;

protected:
	sf::Sprite sprite_;

};