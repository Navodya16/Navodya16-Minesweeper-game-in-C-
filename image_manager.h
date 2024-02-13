#include <map>
#include <SFML/Graphics.hpp>

using std::string;

#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

class ImageManager
{
private:
    typedef std::map<char, sf::Texture> ImageMap;
    ImageMap data;

public:
    ImageManager();
    sf::Texture retrieve(char key);
    bool loadImage(const string& path, char key);
    void loadAllImages();
    ~ImageManager();
};

#endif //IMAGE_MANAGER_H