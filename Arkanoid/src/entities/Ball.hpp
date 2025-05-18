#pragma once
#include <SFML/Graphics.hpp>
#include <random>


class Ball {
public:
	// Радиус и стартовая скорость
	static constexpr float RADIUS = 8.f;
	static constexpr float INITIAL_SPEED = 300.f;
	static constexpr float PI = 3.14159265358979323846;
	static constexpr float MIN_VY = INITIAL_SPEED * 0.25f;

	// Конструктор: задаёт начальную позицию и рандомный вектор скорости
	// pos — в пикселях 
	Ball(const sf::Vector2f& pos);

	// Перемещение шара и отсток от стен окна
	void update(float dt, const sf::RenderWindow& window);

	// Применяет теорию абсолютно упругого удара к движению шара
	void reflect(const sf::Vector2f& normal);

	// Сбрасывает шар в начальное состояние
	void reset();

	// Рисует шар
	void draw(sf::RenderWindow& window) const;

	// Геттер позиции центра шара на экране
	sf::Vector2f getPosition() const;

	// Геттер прямоугольника для коллизий
	sf::FloatRect getBounds() const;

	// Возвращает вектор скорости шара
	sf::Vector2f getVelocity() const;

	// Умножает скорость шара на multiplier
	void multiplyVelocity(float multiplier);

	// Устанавливает скорость шара
	void setVelocity(const sf::Vector2f& newVelocity);

	// Устанавливает velocity_ в случайном направлении с модулем INITIAL_SPEED
	void initVelocity();

	// Устанавливает позицию шара
	void setPosition(const sf::Vector2f& pos);

	// Применяет бонус RandomBounce
	void scheduleRandBounce(float delay);

private:
	sf::CircleShape shape_;
	sf::Vector2f initialPosition_;
	// Вектор скорости шара
	sf::Vector2f velocity_;

	// генератор рандома
	std::mt19937 rng_;

	float randomBounceTimer_ = -1.f;

	// Выполняет смену направления
	void performRandomBounce();
};