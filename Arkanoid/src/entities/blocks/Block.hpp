#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Pos.hpp"
#include "../../EnumTypes.hpp"
#include "../Ball.hpp"




class Block {
public:
    static constexpr float SPEEDUP_MULTIPLIER = 1.5f;

    // Конструктор: задаём позицию, тип, HP
    Block(const Pos& logicalPosition,
        const sf::Vector2f& pxPosition,
        float blockWidth,
        float blockHeight);

    virtual ~Block() = default;

    // Отрисовать блок (вызывается из Game::render)
    virtual void draw(sf::RenderWindow& window) const;

    // Вызывается при попадании. Возвращает опциональный бонус, если надо его породить.
    // Отмечает блок разрушенным при необходимости
    virtual std::optional<BonusType> onHit(Ball& ball);

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

    virtual bool isIndestructible() const { return false; }

protected:
    float blockWidth_;
    float blockHeight_;

    sf::RectangleShape shape_;
    Pos logicalPos_;
    BlockType type_;
    int hp_ = 1;
    bool isDestroyed_ = false;


    // Устанавливает цвет для стандартных блоков 
    // (в соотвествии с картой уровня)
    virtual int defaultHp() const { return 1; }
};
