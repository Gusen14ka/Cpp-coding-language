#include "Bonus.hpp"

Bonus::Bonus(const sf::Vector2f& pos) {
	sprite_.setOrigin(SIZE / 2, SIZE / 2);
	sprite_.setPosition(pos);
}

void Bonus::update(float dt) {
	sprite_.move(0.f, FALL_SPEED * dt);
}

void Bonus::draw(sf::RenderWindow& window) const {
	window.draw(sprite_);
}

sf::FloatRect Bonus::getBounds() const {
	return sprite_.getGlobalBounds();
}

sf::Vector2f Bonus::getPosition() const {
	return sprite_.getPosition();
}
