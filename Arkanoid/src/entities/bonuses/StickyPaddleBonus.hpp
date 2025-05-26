#pragma once

#include "Bonus.hpp"
#include "../../EnumTypes.hpp"
#include "../../ResourceHolder.hpp"
#include "../Paddle.hpp"

class StickyPaddleBonus : public Bonus {
public:
	StickyPaddleBonus(Paddle& paddle, const sf::Vector2f& pos);

	void apply() override;

private:
	Paddle& paddle_;
};
