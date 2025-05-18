#include "StartScreen.hpp"

StartScreen::StartScreen(const sf::Vector2u& winSize) 
	: playBtn_(sf::Vector2f(200, 50),
	"Play",
	sf::Vector2f( winSize.x / 2 - 100, winSize.y / 2.f ),
	ResourceHolder::getFont())
{
	backdrop_.setSize({ static_cast<float>(winSize.x),
		static_cast<float>(winSize.y) });
	backdrop_.setFillColor({ 36, 9, 53 });

	title_.setFont(ResourceHolder::getFont());
	title_.setString("ARKANOID");
	title_.setCharacterSize(64);
	title_.setFillColor({ 0, 0, 230 });

	auto table = title_.getLocalBounds();
	title_.setPosition(winSize.x / 2.f - table.width / 2.f, winSize.y / 3.f);


}

void StartScreen::draw(sf::RenderWindow& w) {
	w.draw(backdrop_);
	w.draw(title_);
	w.draw(playBtn_);
}

bool StartScreen::handleEvent(const sf::Event& ev) {
	if (ev.type == sf::Event::MouseButtonReleased
		&& ev.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f m{ float(ev.mouseButton.x), float(ev.mouseButton.y) };
		if (playBtn_.getGlobalBounds().contains(m))
			return true;
	}
	return false;
}