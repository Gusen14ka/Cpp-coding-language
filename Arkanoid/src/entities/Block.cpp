#include "Block.hpp"

Block::Block(const Pos& logicalPosition,
    const sf::Vector2f& pxPosition,
    BlockType type,
    BlockColor color,
    float blockWidth,
    float blockHeight,
    int hp,
    std::optional<BonusType> bonus) 
    : logicalPos_(logicalPosition), type_(type),
    color_(color), blockHeight_(blockHeight), 
    blockWidth_(blockWidth), hp_(hp),
    bonusType_(bonus)
{
    shape_.setSize({ blockWidth_, blockHeight_ });
    shape_.setPosition(pxPosition);
    shape_.setOutlineThickness(-1.f);
    shape_.setOutlineColor(sf::Color::Black);

    speedMultiplier_ = (type_ == BlockType::SpeedUp ? SPEEDUP_MULTIPLIER : 1.f);

    isIndestructible_ = (type_ == BlockType::Indestructible);

    updateAppearance();
}

void Block::updateAppearance() {
    switch (type_) {
    case BlockType::Standard:
        setColorForStandartBlocks();
        break;
    case BlockType::MultiHP:
        // цвет по оставшимся жизням
        if (hp_ >= 3)      shape_.setFillColor(sf::Color::Red);
        else if (hp_ == 2) shape_.setFillColor(sf::Color::Yellow);
        else               shape_.setFillColor(sf::Color::Green);
        break;
    case BlockType::SpeedUp:
        shape_.setFillColor(sf::Color(128, 0, 128)); // фиолетовый
        break;
    case BlockType::Indestructible:
        shape_.setFillColor(sf::Color(100, 100, 100)); // серый
        break;
    }
}

void Block::setColorForStandartBlocks() {
    switch (color_) {
    case BlockColor::Green:  shape_.setFillColor(sf::Color::Green); break;
    //case BlockColor::Red:    shape_.setFillColor(sf::Color::Red); break;
    case BlockColor::Blue:   shape_.setFillColor(sf::Color::Blue); break;
    //case BlockColor::Yellow: shape_.setFillColor(sf::Color::Yellow); break;
    case BlockColor::Orange: shape_.setFillColor(sf::Color(255, 165, 0)); break;
    //case BlockColor::Gray: shape_.setFillColor(sf::Color(100, 100, 100)); break;
    //case BlockColor::Purple: shape_.setFillColor(sf::Color(128, 0, 128)); break;
    case BlockColor::Cyan:   shape_.setFillColor(sf::Color::Cyan); break;
    case BlockColor::Magenta: shape_.setFillColor(sf::Color::Magenta); break;
    }
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

bool Block::isIndestructible() const {
    return isIndestructible_;
}

void Block::reset() {
    isDestroyed_ = false;
    hp_ = (type_ == BlockType::MultiHP ? 3 : 1);
    updateAppearance();
}

bool Block::hit(Ball& ball) {
    if (type_ == BlockType::SpeedUp) {
        ball.multiplyVelocity(speedMultiplier_);
    }
    if (isIndestructible_)
        return false;
    hp_--;
    if (hp_ <= 0) {
        isDestroyed_ = true;
    }
    updateAppearance();
    return isDestroyed_;
}

sf::FloatRect Block::getBounds() const {
    return shape_.getGlobalBounds();
}

std::optional<BonusType> Block::getBonus() const {
    return bonusType_;
}