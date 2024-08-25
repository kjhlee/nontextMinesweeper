#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <string>


using namespace std;


const int map_size_x = 30;
const int map_size_y = 16;
const int windowX = 1024;
const int windowY = 768;
const int squareSize = (int)(min(windowX/map_size_x, windowY/map_size_y));
const int offsetX = (windowX - squareSize * map_size_x) / 2;
const int offsetY = (windowY - squareSize * map_size_y) / 2;
const int numMines = 99;


int board[map_size_y][map_size_x] = {0};
sf::RenderWindow app(sf::VideoMode(windowX, windowY), "MineSweeper", sf::Style::Close);
sf::Color Grey(128, 128, 128);
sf::Texture mine;
sf::Texture flag;
sf::Font font;



bool isValid(int row, int col) {
    return (row >= 0 && row < map_size_y && col >= 0 && col < map_size_x);
}


bool isMine(int row, int col) {
    return board[row][col] == -1;
}


int countAdjMines(int row, int col){
    int count = 0;
    int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    for(int pos = 0; pos < 8; pos++){
        int updateRow = row + dx[pos];
        int updateCol = col + dy[pos];
        if(isValid(updateRow, updateCol)){
            if(isMine(updateRow, updateCol)){
                count++;
            }
        }
    }
    return count;
}
void printBoard() {
    for (int i = 0; i < map_size_y; i++) {
        for (int j = 0; j < map_size_x; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void generateMap(){
    srand(time(0));
    int placedMines = 0;
    while(placedMines < numMines){
        int row = rand() % map_size_y;
        int col = rand() % map_size_x;
        if(board[row][col] == 0){
            board[row][col] = -1;
            placedMines++;
        }
    }
    // cout << placedMines << endl;
}
int countMines() {
    int mineCount = 0;
    for (int i = 0; i < map_size_y; i++) {
        for (int j = 0; j < map_size_x; j++) {
            if (board[i][j] == -1) {
                mineCount++;
            }
        }
    }
    return mineCount;
}
void loop(){
    sf::Event e;
    while(app.isOpen()){
        while(app.pollEvent(e)){
            if(e.type == sf::Event::Closed){
                app.close();
            }
        }
        app.clear(sf::Color::Black);

        for(int i = 0; i < map_size_y; i++){
            for(int j = 0; j < map_size_x; j++){
                sf::RectangleShape rectangle(sf::Vector2f(squareSize, squareSize));
                rectangle.setFillColor(Grey);
                rectangle.setPosition(sf::Vector2f(offsetX + j * squareSize, offsetY + i * squareSize));
                rectangle.setOutlineColor(sf::Color::Black);
                rectangle.setOutlineThickness(1);
                app.draw(rectangle);

                if(board[i][j] == -1){
                    sf::Sprite sprite;
                    sprite.setTexture(mine);
                    sprite.scale(sf::Vector2f((float)squareSize / (float)mine.getSize().x, (float)squareSize / (float)mine.getSize().y));
                    sprite.setPosition(sf::Vector2f(offsetX + j * squareSize, offsetY + i * squareSize));
                    app.draw(sprite);
                } else if(board[i][j] > 0) { // Only draw text if there are adjacent mines
                    sf::Color colors[] = {
                        sf::Color::Blue,
                        sf::Color::Green,
                        sf::Color::Red,
                        sf::Color::Magenta,
                        sf::Color::Cyan,
                        sf::Color::Yellow,
                        sf::Color::White,
                        sf::Color::Black
                    };
                    sf::Text text;
                    text.setFont(font);
                    text.setString(to_string(board[i][j]));  // Use the correct number of adjacent mines
                    text.setCharacterSize(squareSize / 2);  // Adjust character size
                    text.setFillColor(colors[board[i][j] - 1]);
                    // Center the text within the square
                    sf::FloatRect textRect = text.getLocalBounds();
                    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    text.setPosition(sf::Vector2f(offsetX + j * squareSize + squareSize / 2, offsetY + i * squareSize + squareSize / 2));
                    app.draw(text);
                }
            }
        }
        app.display();
    }
}


int main(){
    flag.loadFromFile("icons/flag.png");
    mine.loadFromFile("icons/mine.png");
    font.loadFromFile("arial.ttf");
    generateMap();
    // printBoard();
    // cout << countMines() << endl;
    loop();
    return 0;
}
