#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Pos.hpp"
#include "../EnumTypes.hpp"
#include "Ball.hpp"




class Block {
public:
    static constexpr float SPEEDUP_MULTIPLIER = 1.5f;

    // Конструктор: задаём позицию, тип, HP, цвет, опциональный бонус, мультипликатор скорости
    Block(const Pos& logicalPosition,
        const sf::Vector2f& pxPosition,
        BlockType type,
        BlockColor color,
        float blockWidth,
        float blockHeight,
        int hp = 1,
        std::optional<BonusType> bonus = std::nullopt);

    // Отрисовать блок (вызывается из Game::render)
    void draw(sf::RenderWindow& window) const;

    // Вызывается при попадании шарика; возвращает true если блок уничтожен
    // Отмечает блок разрушенным при необходимости
    bool hit(Ball& ball);

    // Геттер прямоугольника для коллизий
    sf::FloatRect getBounds() const;

    // Проверить, уничтожен ли блок
    bool isDestroyed() const;

    // (Опционально) сбросить цвет/HP на исходные (для рестарта уровня)
    void reset();

    // Возвращает логическую позицию на поле
    Pos getLogicalPos() const;

    // Возвращает позицию плитки в окне
    sf::Vector2f getPxPosition() const;

    // Возращает true если блок неразрушимый
    bool isIndestructible() const;

    //Геттер бонуса
    std::optional<BonusType> getBonus() const;

private:
    float blockWidth_;
    float blockHeight_;
    BlockColor color_;

    sf::RectangleShape shape_;
    Pos logicalPos_;
    BlockType type_;
    int hp_;
    bool isIndestructible_;
    float speedMultiplier_;
    std::optional<BonusType> bonusType_;
    bool isDestroyed_ = false;

    // Внутренний метод: обновить цвет/текстуру по hp
    void updateAppearance();

    // Устанавливает цвет для стандартных блоков 
    // (в соотвествии с картой уровня)
    void setColorForStandartBlocks();
};
