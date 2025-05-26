#include "OneTimeFloorBonus.hpp"
#include "../../Game.hpp"

OneTimeFloorBonus::OneTimeFloorBonus(Game& game,
    const sf::Vector2f& pos)
    : Bonus(pos), game_(game) {

    const auto& tex = ResourceHolder::getBonusTexture(
        BonusType::OneTimeFloor);
    sprite_.setTexture(tex);
    sprite_.setScale(SIZE / tex.getSize().x,
        SIZE / tex.getSize().y);
}

void OneTimeFloorBonus::apply() {
    game_.activateOneTimeFloor();
}