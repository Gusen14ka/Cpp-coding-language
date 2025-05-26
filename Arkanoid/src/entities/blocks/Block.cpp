#include "Block.hpp"

Block::Block(const Pos& logicalPosition,
    const sf::Vector2f& pxPosition,
    float blockWidth,
    float blockHeight) 
    : logicalPos_(logicalPosition),
    blockHeight_(blockHeight), 
    blockWidth_(blockWidth)
{
    shape_.setSize({ blockWidth_, blockHeight_ });
    shape_.setPosition(pxPosition);
    shape_.setOutlineThickness(-1.f);
    shape_.setOutlineColor(sf::Color::Black);

}


void Block::draw(sf::RenderWindow& window) const {
    if (!isDestroyed_) {
        window.draw(shape_);
    }
}

bool Block::isDestroyed() const {
    return isDestroyed_;
}

Pos Block::getLogicalPos() const {
    return logicalPos_;
}

sf::Vector2f Block::getPxPosition() const {
    return shape_.getPosition();
}

std::optional<BonusType> Block::onHit(Ball& ball) {
    hp_ -= 1;
    if (hp_ <= 0) isDestroyed_ = true;
    return std::nullopt;
}

void Block::reset() {
    isDestroyed_ = false;
    hp_ = defaultHp();
}

sf::FloatRect Block::getBounds() const {
    return shape_.getGlobalBounds();
}
