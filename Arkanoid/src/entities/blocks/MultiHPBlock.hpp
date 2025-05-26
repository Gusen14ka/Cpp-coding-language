#pragma once
#include "Block.hpp"

class MultiHPBlock : public Block {
public:
    MultiHPBlock(const Pos& pos,
                 const sf::Vector2f& pxPos,
                 float width, float height)
      : Block(pos, pxPos, width, height)
    {
        hp_ = 3;
        refreshColor();
    }

    std::optional<BonusType> onHit(Ball& ball) override {
        auto b = Block::onHit(ball);
        refreshColor();
        return b;
    }

private:
    void refreshColor() {
        switch (hp_) {
        case 3: shape_.setFillColor(sf::Color::Red);    break;
        case 2: shape_.setFillColor(sf::Color::Yellow); break;
        case 1: shape_.setFillColor(sf::Color::Green);  break;
        default: shape_.setFillColor(sf::Color::Transparent);
        }
    }
};