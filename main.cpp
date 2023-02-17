//
// Snake Game Cleaned Up Version
// Garrett Heisey
//
// changelog:
//          23jan23 - overall code cleanup and organization, adding of comments,
//                    and changing of function and variable names
//          25jan23 - renaming of variables, adding function-preceding comments,
//                    and adding comments describing purpose of blocks of code
//          28jan23 - added comment to describe why the ncurses library is included,
//                    and added parameter list/function return to comments
//          2Feb23  - added pause, increase game speed, and decrease game speed functionalities
//                    to the keyboard input function
//
// notes:
//
//          case 'p':
//              while(!_kbhit));
//              break;
//          case '+':
//              sleepAmt = sleepAmt-5;
//              break;
//          case '-':
//              sleepAmt = sleepAmt+5;
//              break;
//          -------------------------
//          switch(tolower(_getch()))
//
// Original code by N. Vitanovic
// see his YouTube video here: https://bit.ly/29WZ5Ml

#include <iostream>
#include <ncurses.h> // mac library to detect keyboard input for movement
using namespace std;

bool gameOver;
const int boardWidth = 20;
const int boardHeight = 20;
int snakeHeadX, snakeHeadY, fruitX, fruitY, score;
int tailXCoords[100], tailYCoords[100];
int tailLength;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton direction;
int sleepAmt = 100;

// The setup function gets the game ready to play for the user by initializing the variables necessary for runtime.
//
// Params: N/A
// Returns: N/A

void Setup()
{
    gameOver = false;
    direction = STOP;
    snakeHeadX = boardWidth / 2;
    snakeHeadY = boardHeight / 2;
    fruitX = rand() % boardWidth;
    fruitY = rand() % boardHeight;
    score = 0;
    initscr();
    noecho();
    timeout(sleepAmt);
} // Setup



// The drawGameBoard function draws the board on the terminal with the respective symbols including the border,
// the snake head and tail, and the fruits.
//
// Params: N/A
// Returns: N/A

void DrawGameBoard()
{
    system("clear");
    
    // Draw the top border
    
    for (int i = 0; i < boardWidth+2; i++)
    {
        cout << "#";
    }
    cout << "\r" << endl;
 
    for (int i = 0; i < boardHeight; i++)
    {
        for (int j = 0; j < boardWidth; j++)
        {
            // Draw a # on the left side border
            
            if (j == 0)
            {
                cout << "#";
            }
            
            // Draw a O where the snake head is located
            
            if (i == snakeHeadY && j == snakeHeadX)
            {
                cout << "O";
            }
            
            // Draw a F where the fruit is located
            
            else if (i == fruitY && j == fruitX)
            {
                cout << "F";
            }
            else
            {
                bool print = false;
                
                // Draw snake tail where it is located
                
                for (int k = 0; k < tailLength; k++)
                {
                    if (tailXCoords[k] == j && tailYCoords[k] == i)
                    {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                {
                    cout << " ";
                }
            }
                 
            // Draw a # on the right side border
            
            if (j == boardWidth - 1)
            {
                cout << "#";
            }
        }
        cout << "\r" << endl;
    }
    
    // Draw the bottom border
    
    for (int i = 0; i < boardWidth+2; i++)
    {
        cout << "#";
    }
    cout << "\r" << endl;
    
    cout << "Score:" << score << "\r" << endl;
} // DrawGameBoard



// The GetKeyboardInput function uses the getch() command from the ncurses library to detect keyboard input from the user,
// and updates direction in the other functions.
//
// Params: N/A
// Returns: N/A

void GetKeyboardInput()
{
    switch (getch())
    {
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        case 'p':
            cout << "Game Paused" << endl;
            break;
        case '+':
            sleepAmt = sleepAmt - 5;
            break;
        case '-':
            sleepAmt = sleepAmt + 5;
            break;
        default:
            break;
    }
} // GetKeyboardInput



// The UpdateGameBoard function updates the game board for each second the game is running. This includes moving the snake,
// generating fruit, and detecting if the snake hits itself to end the game.
//
// Params: N/A
// Returns: N/A

void UpdateGameBoard()
{
    // remember current location of snake head
    
    int firstTailPieceX = tailXCoords[0];
    int firstTailPieceY = tailYCoords[0];
    
    // set first tail piece to current head location
    
    int secondTailPieceX, secondTailPieceY;
    tailXCoords[0] = snakeHeadX;
    tailYCoords[0] = snakeHeadY;
    
    // move the rest of the tail
    
    for (int i = 1; i < tailLength; i++)
    {
        secondTailPieceX = tailXCoords[i];
        secondTailPieceY = tailYCoords[i];
        tailXCoords[i] = firstTailPieceX;
        tailYCoords[i] = firstTailPieceY;
        firstTailPieceX = secondTailPieceX;
        firstTailPieceY = secondTailPieceY;
    }
    
    // Move the snake's head
    
    switch (direction)
    {
    case LEFT:
        snakeHeadX--;
        break;
    case RIGHT:
        snakeHeadX++;
        break;
    case UP:
        snakeHeadY--;
        break;
    case DOWN:
        snakeHeadY++;
        break;
    default:
        break;
    }
    
    // If snake hits wall, wrap around to other side of game board
    
    if (snakeHeadX >= boardWidth)
    {
        snakeHeadX = 0;
    }
    else if (snakeHeadX < 0)
    {
        snakeHeadX = boardWidth - 1;
    }
    if (snakeHeadY >= boardHeight)
    {
        snakeHeadY = 0;
    }
    else if (snakeHeadY < 0)
    {
        snakeHeadY = boardHeight - 1;
    }
    
    // Check if snake hits own tail
 
    for (int i = 0; i < tailLength; i++)
    {
        if (tailXCoords[i] == snakeHeadX && tailYCoords[i] == snakeHeadY)
        {
            gameOver = true;
        }
    }
    
    // Check if fruit has been eaten
 
    if (snakeHeadX == fruitX && snakeHeadY == fruitY)
    {
        score += 10;
        
        // Generate new fruit
        
        fruitX = rand() % boardWidth;
        fruitY = rand() % boardHeight;
        tailLength++;
    }
} // UpdateGameBoard



int main()
{
    Setup();
    
    while (!gameOver)
    {
        DrawGameBoard();
        GetKeyboardInput();
        UpdateGameBoard();
    }
    
    endwin();
    return 0;
}
