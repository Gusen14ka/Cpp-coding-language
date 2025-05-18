#include "Bonus.hpp"

Bonus::Bonus(BonusType type, const sf::Vector2f& startPos)
	: type_(type)
{
	const sf::Texture& tex = ResourceHolder::getBonusTexture(type);
	sprite_.setTexture(tex);
	sprite_.setScale(
		SIZE / tex.getSize().x,
		SIZE / tex.getSize().y
	);
	sprite_.setOrigin({
		SIZE / 2,
		SIZE / 2
	});
	sprite_.setPosition(startPos);
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

BonusType Bonus::getBonusType() const {
	return type_;
}