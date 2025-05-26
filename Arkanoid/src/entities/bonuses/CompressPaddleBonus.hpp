#pragma once

#include "Bonus.hpp"
#include "../../EnumTypes.hpp"
#include "../../ResourceHolder.hpp"
#include "../Paddle.hpp"

class CompressPaddleBonus : public Bonus {
public:
	CompressPaddleBonus(Paddle& paddle, const sf::Vector2f& pos);

	void apply() override;

private:
	Paddle& paddle_;
};
