#include "RandomBounceBonus.hpp"

RandomBounceBonus::RandomBounceBonus(Ball& ball,
	const sf::Vector2f& pos)
	: Bonus(pos), ball_(ball)
{
	auto& tex = ResourceHolder::getBonusTexture(
		BonusType::RandomBounce);
	sprite_.setTexture(tex);
	sprite_.setScale(SIZE / tex.getSize().x,
		SIZE / tex.getSize().y);
}

void RandomBounceBonus::apply() {
	ball_.scheduleRandBounce(1.f);
}