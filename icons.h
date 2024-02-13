#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class FaceExpression {
    Happy,
    Lose,
    Win
};

class Face {
public:
    Face(int width, int height);
    void setExpression(FaceExpression expression);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow* window);
    //bool handleMouseClick(int mouseX, int mouseY);
    bool isClicked(int mouseX, int mouseY) const;

private:
    sf::Sprite sprite;
    sf::Texture happyTexture;
    sf::Texture loseTexture;
    sf::Texture winTexture;
    FaceExpression currentExpression;
    sf::FloatRect bounds;
};

class Debug{
public:
    Debug(int width, int height);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow* window);
    bool isClicked(int mouseX, int mouseY) const;

private:
    sf::Sprite sprite;
    sf::Texture debugTexture;
    sf::FloatRect bounds;
};

enum class PausePlay {
    Pause,
    Play
};

class PnP {
public:
    PnP(int width, int height);
    void setOption(PausePlay option);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow* window);
    //bool handleMouseClick(int mouseX, int mouseY);
    bool isClicked(int mouseX, int mouseY) const;

private:
    sf::Sprite sprite;
    sf::Texture pauseTexture;
    sf::Texture playTexture;
    PausePlay currentOption;
    sf::FloatRect bounds;
};

class Leaderboard{
public:
    Leaderboard(int width, int height);
    sf::FloatRect getBounds() const;
    void draw(sf::RenderWindow* window);
    bool isClicked(int mouseX, int mouseY) const;

private:
    sf::Sprite sprite;
    sf::Texture leaderboardTexture;
    sf::FloatRect bounds;
};

