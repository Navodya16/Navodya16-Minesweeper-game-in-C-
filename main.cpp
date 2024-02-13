#include <iostream>
#include <SFML/Graphics.hpp>
#include "image_manager.h"
#include "board.h"
#include "gameover.h"
#include "icons.h"
#include <cctype>
#include <fstream>
#include <string> 
#include <time.h>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip> // for setw and setfill
#include <SFML/Audio.hpp>

#define CELL_SIZE 32

using std::cin;
using std::cout;
using std::endl;
using namespace std;

std::string playerName;

void setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

void readFile(int &width, int &height, int &mines)
{
    std::ifstream configFile("board_config.cfg");
    if (configFile.is_open()) 
    {
        std::string line;
        if (std::getline(configFile, line)) {
            width = std::stoi(line);
        } else {
            std::cerr << "Error reading width from the file." << std::endl;
        }

        if (std::getline(configFile, line)) {
            height = std::stoi(line);
        } else {
            std::cerr << "Error reading height from the file." << std::endl;
        }

        if (std::getline(configFile, line)) {
            mines = std::stoi(line);
        } else {
            std::cerr << "Error reading mines from the file." << std::endl;
        }

        configFile.close();
    } 
    else 
    {
        std::cerr << "Error opening the file." << std::endl;
    }
}

void writeSortFile(float time, string name)
{
    // Read the existing leaderboard entries from the file into a vector of pairs (time, name)
    std::vector<std::pair<std::string, std::string>> leaderboardEntries;
    std::ifstream leaderboardFile("leaderboard.txt");
    if (leaderboardFile.is_open()) {
        std::string line;
        while (std::getline(leaderboardFile, line)) {
            std::istringstream iss(line);
            std::string time, name;
            std::getline(iss, time, ',');
            std::getline(iss, name);
            leaderboardEntries.push_back({time, name});
        }
        leaderboardFile.close();
    } else {
        std::cerr << "Error opening leaderboard file." << std::endl;
        //return;
    }

    int winningTimeInSeconds = int(time); // Total elapsed time in seconds
    int winningMinutes = winningTimeInSeconds / 60;
    int winningSeconds = winningTimeInSeconds % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << winningMinutes << ":" << std::setw(2) << std::setfill('0') << winningSeconds;
    std::string winningTimeStr = oss.str();

    // Check if the player's name already exists in the top five ranks
    bool playerExists = false;
    for (int i = 0; i < std::min(5, static_cast<int>(leaderboardEntries.size())); ++i) {
        if (leaderboardEntries[i].second == name) {
            playerExists = true;
            // Compare the times and update if the new time is better
            std::string& existingTimeStr = leaderboardEntries[i].first;
            int existingTimeInSeconds = std::stoi(existingTimeStr.substr(0, 2)) * 60 + std::stoi(existingTimeStr.substr(3, 2));
            int newTimeInSeconds = int(time);
            if (newTimeInSeconds < existingTimeInSeconds) {
                int newMinutes = newTimeInSeconds / 60;
                int newSeconds = newTimeInSeconds % 60;
                std::ostringstream oss;
                oss << std::setw(2) << std::setfill('0') << newMinutes << ":" << std::setw(2) << std::setfill('0') << newSeconds;
                existingTimeStr = oss.str();
            }
            break;
        }
    }

    // If the player's name is not found in the top five ranks, insert the winning time at the correct rank
    if (!playerExists) {
        leaderboardEntries.push_back({winningTimeStr, name}); // Replace "PlayerName" with the actual name of the player
        std::sort(leaderboardEntries.begin(), leaderboardEntries.end());
    }

    // Write the sorted leaderboard entries back to the file (only the top five ranks)
    std::ofstream leaderboardFileOut("leaderboard.txt");
    if (leaderboardFileOut.is_open()) {
        int rank = 1;
        for (const auto& entry : leaderboardEntries) {
            leaderboardFileOut << entry.first << "," << entry.second << std::endl;
            rank++;
            if (rank > 5) // Break the loop after the first 5 entries are written
                break;
        }
        leaderboardFileOut.close();
    } else {
        std::cerr << "Error opening leaderboard file for writing." << std::endl;
    }
}

void counter(int number, sf::RenderWindow* window, int &rows)
{
    bool negative = false;
    // Load the image containing digits from 0 to 9
    sf::Texture digitTexture;
    if (!digitTexture.loadFromFile("images/digits.png"))
    {
        std::cout << "Failed to load the digit image!" << std::endl;
    }

    // Create a sprite to display the digits
    sf::Sprite digitSprite(digitTexture);

    // Define the width and height of each digit in the image (assuming they are equal)
    int digitWidth = 21; // Adjust this value according to the actual width of each digit
    int digitHeight = 32; // Adjust this value according to the actual height of each digit

    // Loop to extract and display each digit
    int x = 0; // The x-coordinate of the first digit (rightmost) in the image
    int y = 0; // The y-coordinate of the first digit (rightmost) in the image
    digitSprite.setPosition(33, 32 * ((rows)+0.5f)+16);

    std::vector<int> digits;
    if( number > 0)
    {
        while (number > 0)
        {
            digits.push_back(number % 10);
            number /= 10;
        }
    }
    else if(number == 0)
    {
        digits.push_back(0);
    }
    else
    {
        int pNumber = -1 * number;
        while (pNumber > 0)
        {
            digits.push_back(pNumber % 10);
            pNumber /= 10;
        }
        negative = true;
    }
    
    // Draw the digits by moving rightwards
    if(negative)
    {
        digitSprite.setTextureRect(sf::IntRect( (x + 10 * digitWidth), y, digitWidth, digitHeight));
        window->draw(digitSprite);
        digitSprite.move(digitWidth, 0);
    }
    for (int i = digits.size() - 1; i >= 0; --i)
    {
        int digit = digits[i];
        // Set the position of the sprite to display the current digit
        int digitX = x + digit * digitWidth;
        int digitY = y;
        digitSprite.setTextureRect(sf::IntRect(digitX, digitY, digitWidth, digitHeight));

        window->draw(digitSprite);
        digitSprite.move(digitWidth, 0);
    }
}

void timer(int number, sf::RenderWindow* window, int &rows, int &cols)
{
    int minutes = number / 60; // Get the whole minutes
    int seconds = number % 60; // Get the remaining seconds

    // Load the image containing digits from 0 to 9
    sf::Texture digitTexture;
    if (!digitTexture.loadFromFile("images/digits.png"))
    {
        std::cout << "Failed to load the digit image!" << std::endl;
    }

    // Create a sprite to display the digits
    sf::Sprite digitSprite(digitTexture);

    // Define the width and height of each digit in the image (assuming they are equal)
    int digitWidth = 21; // Adjust this value according to the actual width of each digit
    int digitHeight = 32; // Adjust this value according to the actual height of each digit

    // Loop to extract and display each digit
    int x = 0; // The x-coordinate of the first digit (rightmost) in the image
    int y = 0; // The y-coordinate of the first digit (rightmost) in the image

    std::vector<int> digitsM;
    int minutesCopy = minutes; // Create a copy of minutes
    if (minutesCopy > 0) {
        while (minutesCopy > 0) {
            digitsM.push_back(minutesCopy % 10);
            minutesCopy /= 10;
        }
    } else {
        digitsM.push_back(0);
    }

    std::vector<int> digitsS;
    int secondsCopy = seconds; // Create a copy of seconds
    if (secondsCopy > 0) {
        while (secondsCopy > 0) {
            digitsS.push_back(secondsCopy % 10);
            secondsCopy /= 10;
        }
    } else {
        digitsS.push_back(0);
    }

    digitSprite.setPosition( ( (cols)*32)-97,  32*((rows)+0.5f)+16 );
    for (int i = digitsM.size() - 1; i >= 0; --i)
    {
        int digit = digitsM[i];
        int digitX = x + digit * digitWidth;
        int digitY = y;
        digitSprite.setTextureRect(sf::IntRect(digitX, digitY, digitWidth, digitHeight));

        window->draw(digitSprite);
        digitSprite.move(digitWidth, 0);
    }

    digitSprite.setPosition( ((cols)*32)-54, 32*((rows)+0.5f)+16 );
    for (int i = digitsS.size() - 1; i >= 0; --i)
    {
        int digit = digitsS[i];
        int digitX = x + digit * digitWidth;
        int digitY = y;
        digitSprite.setTextureRect(sf::IntRect(digitX, digitY, digitWidth, digitHeight));

        window->draw(digitSprite);
        digitSprite.move(digitWidth, 0);
    }
}

void LeaderboardScreen(int &width, int &height) {
    sf::RenderWindow window(sf::VideoMode(width, height), "leaderboard screen", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error loading font file." << std::endl;
        return;
    }

    sf::Text welcomeText("LEADERBOARD", font, 20);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width/2.0f, height/2.0f - 120);

    sf::Text Text("", font, 18);
    Text.setFillColor(sf::Color::White);
    Text.setStyle(sf::Text::Bold);
    setText(Text, width/4.0f, height/4.0f +20);
    
    std::ifstream file("leaderboard.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string line;
    string textEntry = "";
    int i = 1;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string time, name;
        std::getline(iss, time, ',');
        std::getline(iss, name);
        textEntry += to_string(i);
        textEntry += "\t";
        textEntry += time;
        textEntry += "\t";
        textEntry += name;
        textEntry += "\n\n";
        ++i;
    }

    file.close();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Blue);
        Text.setString(textEntry);
        window.draw(welcomeText);
        window.draw(Text);

        window.display();
    }
}

void gameWindow(int &width, int &height, int &MINES, board_t &board, bool &isGameOver, int &cols, int &rows)
{
    
    int noMines = MINES;
    bool isDebug = false;
    bool isPlay = true;
    bool leaderboardDisplayed = false;
    auto style = sf::Style::Titlebar | sf::Style::Close;
    sf::RenderWindow window(sf::VideoMode(width, height), "MineSweeper Game", style);
    ImageManager ImageManager;
    ImageManager.loadAllImages();
    
    Face face(cols, rows);
    Debug debug(cols, rows);
    PnP pnp(cols, rows);
    Leaderboard leaderboard(cols, rows);
    board.draw(&window, &ImageManager, CELL_SIZE);
    face.draw(&window);
    debug.draw(&window);
    pnp.draw(&window);
    leaderboard.draw(&window);
    counter(noMines, &window, rows);

    sf::Clock clock; //starts the clock
    float totalElapsedTime = 0.0f; // Accumulated elapsed time

    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                int i = event.mouseButton.x / CELL_SIZE;
                int j = event.mouseButton.y / CELL_SIZE;

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (face.isClicked(event.mouseButton.x, event.mouseButton.y)) 
                    {
                        
                        totalElapsedTime = 0.0f;
                        isGameOver = false;
                        board.reset();
                        board.place_mines(MINES);
                        face.setExpression(FaceExpression::Happy);
                        leaderboardDisplayed = false;
                        noMines = MINES;
                    }
                    else if (debug.isClicked(event.mouseButton.x, event.mouseButton.y) && !isDebug && !isGameOver) 
                    {
                        board.reveal_all_mines();
                        isDebug = true;
                    }
                    else if (debug.isClicked(event.mouseButton.x, event.mouseButton.y) && isDebug && !isGameOver) 
                    {
                        board.unreveal_all_mines();
                        isDebug = false;
                    }
                    else if (pnp.isClicked(event.mouseButton.x, event.mouseButton.y) && isPlay && !isGameOver) 
                    {
                        pnp.setOption(PausePlay::Pause);
                        isPlay = false;
                    }
                    else if (pnp.isClicked(event.mouseButton.x, event.mouseButton.y) && !isPlay && !isGameOver) 
                    {
                        pnp.setOption(PausePlay::Play);
                        isPlay = true;
                    }
                    else if (leaderboard.isClicked(event.mouseButton.x, event.mouseButton.y) && !isGameOver) 
                    {
                        int wn = width / 2;
                        int hn = height / 2;
                        LeaderboardScreen(wn, hn);
                    }
                    else
                    {
                        try
                        {
                            board.reveal_cell(i, j);
                        }
                        catch(GameOver& e)
                        {
                            isGameOver = true;
                            board.reveal_all_cells();
                            face.setExpression(FaceExpression::Lose);
                        }
                    }
                }
                else
                {
                    board.toggle_flag_cell(i, j, noMines);
                }
            }
            else if(board.countNonMines() == ((cols * rows)-MINES) && !isGameOver && !leaderboardDisplayed)//removrd !isgameovr
            {
                writeSortFile(totalElapsedTime, playerName);
                int wn = width / 2;
                int hn = height / 2;
                LeaderboardScreen(wn, hn);
                leaderboardDisplayed = true;
                board.reveal_all_cells();
                face.setExpression(FaceExpression::Win);
            }

            sf::Time frameTime = clock.restart();
            if(isPlay)
            { 
                totalElapsedTime += frameTime.asSeconds();
            }

            board.draw(&window, &ImageManager, CELL_SIZE);
            face.draw(&window);
            debug.draw(&window);
            pnp.draw(&window);
            leaderboard.draw(&window);
            counter(noMines, &window, rows);
            timer(int(totalElapsedTime), &window, rows, cols);

            window.display();
        }
    }
}

bool welcomeScreen(int &width, int &height)
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome to Minesweeper", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) 
    {
        std::cerr << "Error loading font file." << std::endl;
    }

    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 150);

    sf::Text inputPromptText("Enter your name:", font, 20);
    inputPromptText.setFillColor(sf::Color::White);
    inputPromptText.setStyle(sf::Text::Bold);
    setText(inputPromptText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 75);

    sf::Text inputText("", font, 18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    setText(inputText, window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45);

    //std::string playerName;
    int cursorPosition = 0; // Initialize cursor position at the beginning

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                    // Check if the input is an alphabetic character (a letter)
                    if (std::isalpha(static_cast<unsigned char>(event.text.unicode))) {
                        char typedChar = static_cast<char>(event.text.unicode);
                        if (playerName.size() < 10) {
                            playerName.insert(cursorPosition, 1, typedChar);
                            cursorPosition++; // Move the cursor after the inserted character
                        }
                    }
                } else if (event.text.unicode == 8 && !playerName.empty() && cursorPosition > 0) {
                    // Handle backspace to remove the last character
                    playerName.erase(cursorPosition - 1, 1);
                    cursorPosition--; // Move the cursor backward
                } else if (event.text.unicode == 13 && !playerName.empty()) {
                    // Handle Enter key press to proceed to the game window
                    return true;
                }
            }
        }

        // Capitalize the first letter of the name, while others in lowercase
        if (!playerName.empty()) 
        {
            playerName[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(playerName[0])));
            for (size_t i = 1; i < playerName.size(); ++i) {
                playerName[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(playerName[i])));
            }
        }

        inputText.setString(playerName.substr(0, cursorPosition) + "|" + playerName.substr(cursorPosition));

        window.clear(sf::Color::Blue);
        window.draw(welcomeText);
        window.draw(inputPromptText);
        window.draw(inputText);

        window.display();
    }
    return false;
}

int main() {
    int BOARD_WIDTH = 0;
    int BOARD_HEIGHT = 0;
    int MINES = 0;

    readFile(BOARD_WIDTH, BOARD_HEIGHT, MINES);
    board_t board(BOARD_HEIGHT, BOARD_WIDTH);
    board.place_mines(MINES);

    bool isGameOver = false;
    int width = BOARD_WIDTH * CELL_SIZE;
    int height = (BOARD_HEIGHT * CELL_SIZE) +100;

    if(welcomeScreen(width, height))
        gameWindow(width, height, MINES, board, isGameOver, BOARD_WIDTH, BOARD_HEIGHT);

    return 0;
}
