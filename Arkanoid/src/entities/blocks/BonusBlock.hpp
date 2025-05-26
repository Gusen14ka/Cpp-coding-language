#pragma once
#include "Block.hpp"

class BonusBlock : public Block {
public:
    BonusBlock(BonusType bonus,
        const Pos& pos,
        const sf::Vector2f& pxPos,
        float width, float height)
        : Block(pos, pxPos, width, height), bonus_(bonus)
    {
        shape_.setFillColor(sf::Color::Cyan);
    }

    std::optional<BonusType> onHit(Ball& ball) override {
        Block::onHit(ball);              // ломаемся
        return isDestroyed_ ? std::optional{ bonus_ } : std::nullopt;
    }

private:
    BonusType bonus_;
};