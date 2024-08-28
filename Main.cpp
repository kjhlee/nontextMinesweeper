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


int hidden_board[map_size_y][map_size_x];
int real_board[map_size_y][map_size_x];

sf::RenderWindow app(sf::VideoMode(windowX, windowY), "MineSweeper", sf::Style::Close);
sf::Color Grey(128, 128, 128);
sf::Texture mine;
sf::Texture flag;
sf::Font font;


bool checkGame = false;
enum GameState  {Playing, Won, Lost};
GameState gs = Playing;

bool isValid(int row, int col) {
    return (row >= 0 && row < map_size_y && col >= 0 && col < map_size_x);
}


bool isMine(int row, int col) {
    return hidden_board[row][col] == -1;
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
            cout << hidden_board[i][j] << " ";
        }
        cout << endl;
    }
}
void placeMineCount(){
    for(int i = 0; i < map_size_y; i++){
        for(int j = 0; j < map_size_x; j++){
            if(!isMine(i,j)) {
                int numMines = countAdjMines(i, j);
                hidden_board[i][j] = numMines;
            }
        }
    }
}
void generateMap(){
    for(int i = 0; i < map_size_y; i++) {
        for (int j = 0; j < map_size_x; j++) {
            hidden_board[i][j] = 0;
            real_board[i][j] = 10;  // All tiles start hidden
        }
    }
    srand(time(0));
    int placedMines = 0;
    while(placedMines < numMines){
        int row = rand() % map_size_y;
        int col = rand() % map_size_x;
        if(hidden_board[row][col] == 0){
            hidden_board[row][col] = -1;
            placedMines++;
        }
    }
    placeMineCount();
    // cout << placedMines << endl;
}
int countMines() {
    int mineCount = 0;
    for (int i = 0; i < map_size_y; i++) {
        for (int j = 0; j < map_size_x; j++) {
            if (hidden_board[i][j] == -1) {
                mineCount++;
            }
        }
    }
    return mineCount;
}

bool checkWin(){
    for(int i = 0; i < map_size_y; i++){
        for(int j = 0; j < map_size_x; j++){
            if(real_board[i][j] == 10 && hidden_board[i][j] != -1){
                return false;
            }
        }
    }
    return true;
}
void openAllPos(int row, int col){
    if(!isValid(row, col) || real_board[row][col] != 10){
        return;
    }
    int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    real_board[row][col] = hidden_board[row][col];

    if(hidden_board[row][col] > 0){
        return;
    }
    for(int x = 0; x < 8; x++){
        openAllPos(row + dx[x], col + dy[x]);
    }
}
bool gameOver(){
    return checkGame;
}
void loop(){
    sf::Event e;
    while(app.isOpen()){
        while(app.pollEvent(e)){
            if(e.type == sf::Event::Closed){
                app.close();
            }
            if(gs == Playing){
                if(e.type == sf::Event::MouseButtonPressed){
                    int x = (int)((e.mouseButton.x - offsetX) / squareSize);
                    int y = (int)((e.mouseButton.y - offsetY) / squareSize);

                    if(x < map_size_x && x >= 0 && y < map_size_y && y >= 0){
                        if(e.mouseButton.button == sf::Mouse::Right){
                            // Toggle flag
                            if(real_board[y][x] == 10){  // If the tile is hidden
                                real_board[y][x] = 9;    // Place a flag
                            } else if(real_board[y][x] == 9){  // If already flagged
                                real_board[y][x] = 10;   // Remove the flag
                            }
                        }
                        if(e.mouseButton.button == sf::Mouse::Left){
                            if(real_board[y][x] != 9){
                                if(hidden_board[y][x] == -1){
                                    real_board[y][x] = -1;
                                    gs = Lost;
                                } else {
                                    // real_board[y][x] = hidden_board[y][x];
                                    openAllPos(y, x);
                                    if(checkWin()){
                                        gs = Won;
                                    }
                                }   
                            } 
                        }
                    }
                }
            }
            if(e.type == sf::Event::KeyPressed && gs == Playing){
                sf::Vector2i mousePos = sf::Mouse::getPosition(app);
                int x = (int)((mousePos.x - offsetX) / squareSize);
                int y = (int)((mousePos.y - offsetY) / squareSize);
                if(x < map_size_x && x >= 0 && y < map_size_y && y >= 0){
                    if(e.key.code == sf::Keyboard::Space){
                        if(real_board[y][x] == 10){  // If the tile is hidden
                            real_board[y][x] = 9;    // Place a flag
                        } else if(real_board[y][x] == 9){  // If already flagged
                            real_board[y][x] = 10;   // Remove the flag
                        }
                    }
                }
            }
        }

        // Redraw the board
        app.clear(sf::Color::Black);

        for(int i = 0; i < map_size_y; i++){
            for(int j = 0; j < map_size_x; j++){
                sf::RectangleShape rectangle(sf::Vector2f(squareSize, squareSize));
                rectangle.setFillColor(Grey);
                rectangle.setPosition(sf::Vector2f(offsetX + j * squareSize, offsetY + i * squareSize));
                rectangle.setOutlineColor(sf::Color::Black);
                rectangle.setOutlineThickness(1);
                app.draw(rectangle);


                if(real_board[i][j] == -1){
                    sf::Sprite sprite;
                    rectangle.setFillColor(sf::Color::Red);
                    sprite.setTexture(mine);
                    sprite.scale(sf::Vector2f((float)squareSize / (float)mine.getSize().x, (float)squareSize / (float)mine.getSize().y));
                    sprite.setPosition(sf::Vector2f(offsetX + j * squareSize, offsetY + i * squareSize));
                    app.draw(rectangle);
                    app.draw(sprite);
                } else if(real_board[i][j] == 9){
                    sf::Sprite sprite;
                    sprite.setTexture(flag);
                    sprite.scale(sf::Vector2f((float)squareSize / (float)flag.getSize().x, (float)squareSize / (float)flag.getSize().y));
                    sprite.setPosition(sf::Vector2f(offsetX + j * squareSize, offsetY + i * squareSize));
                    app.draw(sprite);
                } else if(real_board[i][j] > 0 && real_board[i][j] < 9) {
                    // Draw number for adjacent mines
                    sf::Color colors[] = {
                        sf::Color::Blue,
                        sf::Color::Green,
                        sf::Color::Red,
                        sf::Color::Magenta,
                        sf::Color::Cyan,
                        sf::Color::Yellow,
                        sf::Color::Black
                    };
                    rectangle.setFillColor(sf::Color::White);
                    sf::Text text;
                    text.setFont(font);
                    text.setString(to_string(hidden_board[i][j]));
                    text.setCharacterSize(squareSize / 2);
                    text.setFillColor(colors[hidden_board[i][j] - 1]);
                    sf::FloatRect textRect = text.getLocalBounds();
                    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    text.setPosition(sf::Vector2f(offsetX + j * squareSize + squareSize / 2, offsetY + i * squareSize + squareSize / 2));
                    app.draw(rectangle);
                    app.draw(text);
                } else if(real_board[i][j] == 0){
                    sf::RectangleShape rectangle(sf::Vector2f(squareSize, squareSize));
                    rectangle.setFillColor(sf::Color::White);
                    rectangle.setPosition(sf::Vector2f(offsetX + j * squareSize, offsetY + i * squareSize));
                    rectangle.setOutlineColor(sf::Color::Black);
                    rectangle.setOutlineThickness(1);
                    app.draw(rectangle);
                }
            }
        }
        if (gs == Won || gs == Lost) {
            sf::Text endText;
            endText.setFont(font);
            endText.setCharacterSize(50);
            endText.setFillColor(sf::Color::Red);
            endText.setString(gs == Won ? "You Win!" : "Game Over!");
            sf::FloatRect textRect = endText.getLocalBounds();
            endText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            endText.setPosition(sf::Vector2f(windowX / 2.0f, windowY / 2.0f));
            app.draw(endText);
        }

        app.display();
    }
}



int main(){
    // cout << flag.loadFromFile("icons/flag.png") << endl;
    flag.loadFromFile("icons/flag.png");
    mine.loadFromFile("icons/mine.png");
    font.loadFromFile("arial.ttf");
    generateMap();
    // placeMineCount();
    // printBoard();
    // cout << countMines() << endl;
    loop();
    // loop();
    return 0;
}
