#pragma once
#include <SFML/Graphics.hpp>
#include "Ball.hpp"
#include "functional"
#include <optional>

class Paddle {
public:
	static constexpr float WIDTH = 100.f;
	static constexpr float HEIGHT = 20.f;

	// Конструктор: инициализация стартовой позиция
	Paddle(const sf::Vector2f& pos);

	// Обработка ввода игрока
	void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

	//void update(float dt);

	// Рисует платформу
	void draw(sf::RenderWindow& window) const;

	// Восстанавливает изначальное состояние платформы
	void reset();

	// Увеличивает ширину в multiplier раз
	void multiplyWidth(float multiplier);

	// Геттер границ платформы
	sf::FloatRect getBounds() const;

	// Геттер позиции платформы
	sf::Vector2f getPosition() const;

	// Включает липкий режим
	void enableSticky();
	
	// Приклеивае шар в липком режиме
	void stickBall(Ball& ball);

	// Есть ли приклееный шар
	bool hasStuckBall() const;

	// "Липкая" ли платформа
	bool IsStickyEnabled() const;

	// Отключает логику липкой платформы
	void disableSticky();

	// Делает цвет платформы - базовым
	void setBaseColor();

	// Каждый кадр пересчитываем скорость
	void updateVelocity(float dt);

	// Геттер горизонтальной скорости платформы
	float getVelocityX() const;

private:
	sf::RectangleShape shape_;
	sf::Vector2f initialPosition_;
	sf::Vector2f position_;
	sf::Color baseColor_;
	// Обёртка для хранения приклееного шара
	std::optional<std::reference_wrapper<Ball>> stuckBall_;

	// двигается ли сейчас платформа или нет
	bool dragging_ = false;

	// применён ли бонус липучести
	bool isSticky_ = false;

	// Смещение шара (по x) относительно центра платформы 
	// нужно для реализации липкого бонуса
	float  stickOffsetX_ = 0.f;

	// Переменные для учёта скорости платформы в момент коллизии с шаром
	float lastX_ = initialPosition_.x, velocityX_ = 0;

	// Для корректного предвижения платформы
	float lastMouseX_ = 0.f;

	// Текущий скейл ширины платформы
	float widthScale_ = 1.f;

	// Синхронизирует shape_ с position_
	void synchronizeShapePos();
};