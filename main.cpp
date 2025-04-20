#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

const int gameSpeed = 300;
int nScreenWidth = 120;			// Console Screen Size X (columns)
int nScreenHeight = 40;			// Console Screen Size Y (rows)

// Create Screen Buffer
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    DWORD dwBytesWritten = 0;

void hidecursor(){
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void createScreenBuffer(){

    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
    SetConsoleActiveScreenBuffer(hConsole);
}

struct SnakePart{
    int positionX;
    int positionY;
};

struct Food{
    int positionX;
    int positionY;
    bool foodGenerated;
};

int board[11][11] = {
        {1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,2,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1}
    };

int playerPositionX = 5;    //from 0 to 10, from left to right
int playerPositionY = 5;    //from 0 to 10, from up to down

bool isGameInProgress = true;

Food food;

int playerPoints = 0; //length of the snake as well. 0 length means it's only head

SnakePart snake[1000];

int maxPlayerPoints = 81 - 1;

int lastDirection; //w - 119, a - 97, s - 115, d - 100

void showWinMenu(){
    system("CLS");
    cout << "Congratulations!" << endl;
    cout << "You win!" << endl;
    cout << endl;
    cout << "Your points: " << playerPoints << endl;

    getch();
}

bool isFree(int newPlayerPostionX, int newPlayerPostionY){
    //Checking snake collision
    for(int i = 1; i < playerPoints; i++){
        if(snake[i].positionX == newPlayerPostionX && snake[i].positionY == newPlayerPostionY){
            return false;
        }
    }
    //Checking exceptional 2-size snake collision
    if(playerPoints == 1){
        if(snake[1].positionX == newPlayerPostionX && snake[1].positionY == newPlayerPostionY){
            return false;
        }
    }
    //Checking wall collision
    if(board[newPlayerPostionY][newPlayerPostionX] == 1){
        return false;
    }
    return true;
}

void clearBoard(){
    for(int y = 1; y < 10; y++){
        for(int x = 1; x < 10; x++){
            board[y][x] = 0;
        }
    }
}

void generateSnake(){
    for(int i = 0; i <= playerPoints; i++){
        board[snake[i].positionY][snake[i].positionX] = 3;
    }
    board[snake[0].positionY][snake[0].positionX] = 2;
}

void movingWholeSnake(){
    for(int i = playerPoints-1; i >= 0; i--){
        snake[i+1].positionX = snake[i].positionX;
        snake[i+1].positionY = snake[i].positionY;
    }
    snake[0].positionX = playerPositionX;
    snake[0].positionY = playerPositionY;
}

void eatOrMove(){
    if(board[playerPositionY][playerPositionX] == 4){
        food.foodGenerated = false;
        playerPoints++;

        movingWholeSnake();
    }
    else{
        for(int i = playerPoints; i > 0; i--) {
            snake[i].positionX = snake[i - 1].positionX;
            snake[i].positionY = snake[i - 1].positionY;
        }
        snake[0].positionX = playerPositionX;
        snake[0].positionY = playerPositionY;
    }
}

void automateTurn(){
    switch(lastDirection){
    case 97:    //a
        if(!isFree(playerPositionX - 1, playerPositionY)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionX--;
        eatOrMove();
        break;
    case 100:   //d
        if(!isFree(playerPositionX + 1, playerPositionY)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionX++;
        eatOrMove();
        break;
    case 115:   //s
        if(!isFree(playerPositionX, playerPositionY + 1)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionY++;
        eatOrMove();
        break;
    case 119:   //w
        if(!isFree(playerPositionX, playerPositionY - 1)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionY--;
        eatOrMove();
        break;
    }
}

void playerTurn(int playerMove){
    switch(playerMove){
    case 97:    //a
        if(!isFree(playerPositionX - 1, playerPositionY)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionX--;
        lastDirection = 97;
        eatOrMove();
        break;
    case 100:   //d
        if(!isFree(playerPositionX + 1, playerPositionY)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionX++;
        lastDirection = 100;
        eatOrMove();
        break;
    case 115:   //s
        if(!isFree(playerPositionX, playerPositionY + 1)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionY++;
        lastDirection = 115;
        eatOrMove();
        break;
    case 119:   //w
        if(!isFree(playerPositionX, playerPositionY - 1)){
            isGameInProgress = false;
            playerPoints++;
        }
        playerPositionY--;
        lastDirection = 119;
        eatOrMove();
        break;
    }
}

void drawBoard(){
    //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//HANDLE hstdin  = GetStdHandle( STD_INPUT_HANDLE  );
	//HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );


    for(int y = 0; y < 11; y++){
        for(int x = 0; x < 11; x++){
            switch(board[y][x]){
            case 0:
                screen[y * nScreenWidth + x*2] = ' ';
                //cout << "  ";
                break;
            case 1:
                //SetConsoleTextAttribute(hConsole, 128);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                screen[y * nScreenWidth + x*2] = '#';
                cout << "  ";
                //SetConsoleTextAttribute(hConsole, 7);   //I think it's a default color
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                //cout << "# ";
                break;
            case 2:
                //SetConsoleTextAttribute(hConsole, 160);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
                screen[y * nScreenWidth + x*2] = 'O';
                cout << "  ";
                //SetConsoleTextAttribute(hConsole, 7);   //I think it's a default color
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                //cout << "O ";
                //FlushConsoleInputBuffer( hstdin );
                break;
            case 3:
                //SetConsoleTextAttribute(hConsole, 32);
                screen[y * nScreenWidth + x*2] = 'o';
                //std::vector<WORD> attributes(nScreenWidth * nScreenHeight, FOREGROUND_GREEN);
                //WriteConsoleOutputAttribute(hConsole, &attributes[0], nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
                cout << "  ";
                //SetConsoleTextAttribute(hConsole, 7);   //I think it's a default color
                //cout << "o ";
                break;
            case 4:
                //SetConsoleTextAttribute(hConsole, 192);
                screen[y * nScreenWidth + x*2] = 'F';
                cout << "  ";
                //SetConsoleTextAttribute(hConsole, 7);   //I think it's a default color
                //cout << "F ";
                break;
            case 5:
                //cout << "X ";
                break;
            }
        }
        //cout << endl;
    }
    //cout << "Points: " << playerPoints << endl;
    //cout << "X: " << playerPositionX << endl;
    //cout << "Y: " << playerPositionY << endl;

    // Display Frame
    screen[nScreenWidth * nScreenHeight - 1] = '\0';
    WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    std::vector<WORD> attributes(nScreenWidth * nScreenHeight, FOREGROUND_GREEN);
    WriteConsoleOutputAttribute(hConsole, &attributes[0], nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}

void generateFood(){
    srand(time(NULL));

    while(!food.foodGenerated){
        int foodSpawnX = rand() % 9 + 1;
        int foodSpawnY = rand() % 9 + 1;

        if(board[foodSpawnY][foodSpawnX] == 0){
            food.positionX = foodSpawnX;
            food.positionY = foodSpawnY;
            food.foodGenerated = true;
        }
    }

    board[food.positionY][food.positionX] = 4;
}

void makeFirstThings(){
    //Game stats
    playerPoints = 0;
    isGameInProgress = true;
    playerPositionX = 5;    //from 0 to 10, from left to right
    playerPositionY = 5;    //from 0 to 10, from up to down

    //Position of the head
    snake[0].positionX = playerPositionX;
    snake[0].positionY = playerPositionY;

    //Things about food
    food.foodGenerated = false;

    snake[1].positionX = playerPositionX + 1;
    snake[1].positionY = playerPositionY;
    playerPoints++;

    lastDirection = 97;
    hidecursor();
    createScreenBuffer();
}

void startGame(){
    makeFirstThings();
    while(isGameInProgress){
        //system("CLS");
        clearBoard();
        generateSnake();
        generateFood();
        drawBoard();
        Sleep(gameSpeed);
        if(kbhit()){
            playerTurn(_getwch()); //without double buffor
        }
        else{
            automateTurn();
        }
        if(playerPoints >= maxPlayerPoints){
            isGameInProgress = false;
        }
    }
    if(playerPoints >= maxPlayerPoints){
        showWinMenu();
    }
}

void showMenu(){
    while(true){
        system("CLS");

        cout << "Play" << endl;
        cout << "Options" << endl;
        cout << "Exit" << endl;

        int userChoice;
        cin >> userChoice;

        switch(userChoice){
        case 1:
            startGame();
            break;
        case 2:
            cout << "Options" << endl;
            break;
        case 3:
            cout << "Exit" << endl;
            break;
        }
    }
}

int main(){
    startGame();
    //showMenu();

    return 0;
}
