#include "EndScreen.hpp"
#include "ResourceHolder.hpp"
#include <sstream>

EndScreen::EndScreen(const sf::Vector2u& winSize)
    : retryBtn_(sf::Vector2f(200, 50),
        "RETRY",
        sf::Vector2f(winSize.x / 2 - 100, winSize.y * 2 / 3.f),
        ResourceHolder::getFont())
{
    backdrop_.setSize({ static_cast<float>(winSize.x),
        static_cast<float>(winSize.y) });
    backdrop_.setFillColor({ 36, 9, 53 });

    overText_.setFont(ResourceHolder::getFont());
    overText_.setString("GAME OVER");
    overText_.setCharacterSize(64);
    overText_.setFillColor(sf::Color(139, 0, 0));
    auto ob = overText_.getLocalBounds();
    overText_.setPosition(winSize.x / 2 - ob.width / 2, winSize.y / 3.f);

    scoreText_.setFont(ResourceHolder::getFont());
    scoreText_.setCharacterSize(32);
    scoreText_.setFillColor(sf::Color(255, 255, 102));
    // позиция позже, в setScore()

}

void EndScreen::setScore(int score) {
    std::ostringstream ss;
    ss << "Score: " << score;
    scoreText_.setString(ss.str());
    auto sb = scoreText_.getLocalBounds();
    scoreText_.setPosition(
        backdrop_.getSize().x / 2 - sb.width / 2,
        backdrop_.getSize().y / 2 - sb.height / 2
    );
}

void EndScreen::draw(sf::RenderWindow& w) {
    w.draw(backdrop_);
    w.draw(overText_);
    w.draw(scoreText_);
    w.draw(retryBtn_);
}

bool EndScreen::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::MouseButtonReleased
        && ev.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f m{ float(ev.mouseButton.x), float(ev.mouseButton.y) };
        return retryBtn_.getGlobalBounds().contains(m);
    }
    return false;
}
