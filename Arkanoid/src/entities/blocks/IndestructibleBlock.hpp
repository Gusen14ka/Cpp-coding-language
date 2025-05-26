#pragma once
#include "Block.hpp"

class IndestructibleBlock : public Block {
public:
    IndestructibleBlock(const Pos& pos,
        const sf::Vector2f& pxPos,
        float width, float height)
        : Block(pos, pxPos, width, height)
    {
        shape_.setFillColor(sf::Color(100, 100, 100));  // серый
    }

    std::optional<BonusType> onHit(Ball& ball) override {
        return std::nullopt;
    }

    bool isIndestructible() const override { return true; }
};