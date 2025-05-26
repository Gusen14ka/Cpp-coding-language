#pragma once
#include "Block.hpp"

class SpeedUpBlock : public Block {
public:
    SpeedUpBlock(const Pos& pos,
        const sf::Vector2f& pxPos,
        float width, float height)
        : Block(pos, pxPos, width, height)
    {
        shape_.setFillColor(sf::Color(128, 0, 128)); // фиолет
    }

    std::optional<BonusType> onHit(Ball& ball) override {
        ball.multiplyVelocity(1.5f);    // ускоряем
        return Block::onHit(ball);       // дальше как обычный одноударный
    }
};