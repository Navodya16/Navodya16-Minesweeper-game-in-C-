#include "image_manager.h"

ImageManager::ImageManager() = default;

sf::Texture ImageManager::retrieve(const char key)
{
    if (data.find(key) != data.end())
    {
        return data[key];
    }
    return sf::Texture();
}

bool ImageManager::loadImage(const string& path, const char key)
{
    if (data.find(key) != data.end())
        return true;
    else
    {
        sf::Texture texture; 
        if (texture.loadFromFile(path))
        {
            data[key] = texture;
            return true;
        }
        return false;
    }
}

void ImageManager::loadAllImages()
{
    loadImage("images/tile_revealed.png", '0');
    loadImage("images/number_1.png", '1');
    loadImage("images/number_2.png", '2');
    loadImage("images/number_3.png", '3');
    loadImage("images/number_4.png", '4');
    loadImage("images/number_5.png", '5');
    loadImage("images/number_6.png", '6');
    loadImage("images/number_7.png", '7');
    loadImage("images/number_8.png", '8');
    loadImage("images/mine.png", 'b');
    loadImage("images/flag.png", 'f');
    loadImage("images/tile_hidden.png", 'n');
    loadImage("images/debug.png", 'd');
    loadImage("images/digits.png", 'g');
    loadImage("images/face_happy.png", 'h');
    loadImage("images/face_lose.png", 'l');
    loadImage("images/face_win.png", 'w');
    loadImage("images/leaderboard.png", 'r');
    loadImage("images/pause.png", 'u');
    loadImage("images/play.png", 'a');
}

ImageManager::~ImageManager() = default;
