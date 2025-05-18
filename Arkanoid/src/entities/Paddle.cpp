#include "Paddle.hpp"

Paddle::Paddle(const sf::Vector2f& pos) {
	shape_.setSize({ WIDTH, HEIGHT });
	shape_.setOrigin({ WIDTH / 2, HEIGHT / 2 });
	initialPosition_ = pos;
	position_ = initialPosition_;
	shape_.setPosition(initialPosition_);
	shape_.setFillColor(sf::Color(128, 128, 128));
	baseColor_ = shape_.getFillColor();
}

void Paddle::handleEvent(const sf::Event& event, 
	const sf::RenderWindow& window) 
{
	if (event.type == sf::Event::MouseButtonPressed 
		&& event.mouseButton.button == sf::Mouse::Left) 
	{
		dragging_ = true;
		lastMouseX_ = static_cast<float>(event.mouseButton.x);
	}
	else if (event.type == sf::Event::MouseButtonReleased 
		&& event.mouseButton.button == sf::Mouse::Left)
	{
		dragging_ = false;
		if (stuckBall_.has_value()) {
			Ball& ball = stuckBall_->get();
			ball.initVelocity();
			stuckBall_ = std::nullopt;
			disableSticky();
		}
	}
	else if (dragging_ && event.type == sf::Event::MouseMoved) {
		float mouseX = static_cast<float>(event.mouseMove.x);

		float half = WIDTH / 2.f;
		float dx = mouseX - lastMouseX_;
		lastMouseX_ = mouseX;
		position_.x += dx;

		// проверяем на переход границ окна
		if (position_.x - half < 0.f) {
			position_.x = half;
		}
		else if (position_.x + half > window.getSize().x) {
			position_.x = window.getSize().x - half;
		}

		synchronizeShapePos();

		if (stuckBall_.has_value()) {
			Ball& ball = stuckBall_->get();
			auto ballPos = ball.getPosition();
			ball.setPosition({ position_.x + stickOffsetX_, position_.y - HEIGHT / 2 - Ball::RADIUS});
		}
	}
}

void Paddle::draw(sf::RenderWindow& window) const {
	window.draw(shape_);
}

void Paddle::reset() {
	position_ = initialPosition_;
	stuckBall_ = std::nullopt;
	disableSticky();
	velocityX_ = 0.f;
	lastMouseX_ = position_.x;
	widthScale_ = 1.f;
	shape_.setScale(1.f, 1.f);
	synchronizeShapePos();
}

void Paddle::synchronizeShapePos() {
	shape_.setPosition(position_);
}

sf::FloatRect Paddle::getBounds() const {
	return shape_.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const{
	return position_;
}

void Paddle::multiplyWidth(float multiplier) {
	multiplier = std::clamp(widthScale_ * multiplier, 0.3f, 3.f);
	widthScale_ = multiplier;
	shape_.setScale(widthScale_, 1.f);
	if (isSticky_) {
		shape_.setFillColor(sf::Color::Green);
	}
	else {
		shape_.setFillColor(baseColor_);
	}

	//    пересчитываем offset и позицию шарика
	if (stuckBall_) {
		Ball& ball = stuckBall_->get();
		// новый отступ по X от центра платформы
		stickOffsetX_ = ball.getPosition().x - position_.x;
		// и сразу возвращаем шар на правильную позицию
		ball.setPosition({ position_.x + stickOffsetX_,
						   ball.getPosition().y });
	}
}

void Paddle::enableSticky() {
	isSticky_ = true;
	shape_.setFillColor(sf::Color::Green);
}

bool Paddle::hasStuckBall() const {
	return stuckBall_.has_value();
}

void Paddle::stickBall(Ball& ball) {
	if (!isSticky_) { return;}
	stuckBall_ = ball;
	stickOffsetX_ = ball.getPosition().x - position_.x;
	ball.setVelocity({ 0.f, 0.f });
}

bool Paddle::IsStickyEnabled() const {
	return isSticky_;
}

void Paddle::disableSticky() {
	isSticky_ = false;
	stuckBall_.reset();
	shape_.setFillColor(baseColor_);
}

void Paddle::setBaseColor() {
	shape_.setFillColor(baseColor_);
}

void Paddle::updateVelocity(float dt) {
	velocityX_ = (position_.x - lastX_) / dt;
	lastX_ = position_.x;
}

float Paddle::getVelocityX() const {
	return velocityX_;
}
