#include "icons.h"

Face::Face(int width, int height) {
    happyTexture.loadFromFile("images/face_happy.png");
    loseTexture.loadFromFile("images/face_lose.png");
    winTexture.loadFromFile("images/face_win.png");
    sprite.setTexture(happyTexture); // Start with the happy face texture
    sprite.setPosition((((width) / 2.0) * 32) - 32, 32 * ((height) + 0.5f));
    bounds = sprite.getGlobalBounds();
    currentExpression = FaceExpression::Happy;
}

void Face::setExpression(FaceExpression expression) {
    // Change the texture based on the expression
    if (expression == FaceExpression::Happy) {
        sprite.setTexture(happyTexture);
    } else if (expression == FaceExpression::Lose) {
        sprite.setTexture(loseTexture);
    } else if (expression == FaceExpression::Win) {
        sprite.setTexture(winTexture);
    }

    currentExpression = expression;
    bounds = sprite.getGlobalBounds();
}

sf::FloatRect Face::getBounds() const {
    return bounds;
}

void Face::draw(sf::RenderWindow* window) {
    window->draw(sprite);
}

bool Face::isClicked(int mouseX, int mouseY) const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(mouseX, mouseY);
}

Debug::Debug(int width, int height) {
    debugTexture.loadFromFile("images/debug.png");
    sprite.setTexture(debugTexture); 
    sprite.setPosition( ((width) * 32) - 304, 32 * ((height)+0.5f) );
    bounds = sprite.getGlobalBounds();
}

sf::FloatRect Debug::getBounds() const {
    return bounds;
}

void Debug::draw(sf::RenderWindow* window) {
    window->draw(sprite);
}

bool Debug::isClicked(int mouseX, int mouseY) const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(mouseX, mouseY);
}

PnP::PnP(int width, int height) {
    pauseTexture.loadFromFile("images/pause.png");
    playTexture.loadFromFile("images/play.png");
    sprite.setTexture(playTexture); // Start with the play texture
    sprite.setPosition( ((width) * 32) - 240, 32 * ((height)+0.5f) );
    bounds = sprite.getGlobalBounds();
    currentOption = PausePlay::Play;
}

void PnP::setOption(PausePlay expression) {
    // Change the texture based on the expression
    if (expression == PausePlay::Play) {
        sprite.setTexture(playTexture);
    } else if (expression == PausePlay::Pause) {
        sprite.setTexture(pauseTexture);
    }

    currentOption = expression;
    bounds = sprite.getGlobalBounds();
}

sf::FloatRect PnP::getBounds() const {
    return bounds;
}

void PnP::draw(sf::RenderWindow* window) {
    window->draw(sprite);
}

bool PnP::isClicked(int mouseX, int mouseY) const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(mouseX, mouseY);
}

Leaderboard::Leaderboard(int width, int height) {
    leaderboardTexture.loadFromFile("images/leaderboard.png");
    sprite.setTexture(leaderboardTexture); 
    sprite.setPosition( ((width) * 32) - 176, 32 * ((height)+0.5f) );
    bounds = sprite.getGlobalBounds();
}

sf::FloatRect Leaderboard::getBounds() const {
    return bounds;
}

void Leaderboard::draw(sf::RenderWindow* window) {
    window->draw(sprite);
}

bool Leaderboard::isClicked(int mouseX, int mouseY) const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(mouseX, mouseY);
}
