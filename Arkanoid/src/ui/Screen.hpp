#pragma once
#include <SFML/Graphics.hpp>

/// Абстрактный интерфейс для любого экрана
class Screen {
public:
	virtual ~Screen() = default;

	/// Нарисовать экран
	virtual void draw(sf::RenderWindow& window) = 0;

	/// Обработать SFML-событие.
	/// Возвращает true, если экран обработал событие
	virtual bool handleEvent(const sf::Event& ev) = 0;
};