#pragma once
#include "Block.hpp"

class StandardBlock : public Block {
public:
    StandardBlock(const Pos& pos,
        const sf::Vector2f& pxPos,
        float width, float height)
        : Block(pos, pxPos, width, height)
    {
        shape_.setFillColor(sf::Color::Green);
    }

};