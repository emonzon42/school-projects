/* 
Eli Monzon 
2.6.20
MineSweeper
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct xyPairs{//(x,y) coordinate pair
    int x;
    int y;
}Coordinate;

typedef struct inputData{ //holds the structure for the user's input data
    char command;
    Coordinate cor; //inputted x,y coordinates
    bool quit; 
}InputData;

typedef struct boardSquare{//the data behind each game square
    Coordinate cor; //x,y coordinates
    char nearbyBombs; //# of bombs it's next to
    char *tile; //the actual square as a string [ ]
    int indexNum; //to refer to the actual square in gameBoard array
    bool bombHere;                              
    bool revealed;
    bool isFlagged;
}GameSquare;

int width; //x max
int height;//y max
GameSquare *gameBoard; //gameboard is an array of gamesquares
char *board; //the gameboard as a string grid of tiles

void printBoard(bool gameEnded){ //prints board (if game ended then prints all bombs)
    printf("\n");
    for (int i = 0; i < (width * height); i++){
        if (gameEnded && !gameBoard[i].isFlagged && gameBoard[i].bombHere){//game ended reveals all bombs that arent flagged
                gameBoard[i].tile = "[X]";
        }else if (gameBoard[i].revealed) {
            char tile[4];
            sprintf(tile, "[%d]", gameBoard[i].nearbyBombs);
            gameBoard[i].tile = tile;
        }
        else if (gameBoard[i].isFlagged){
            gameBoard[i].tile = "[F]";
        }
        else{
            gameBoard[i].tile = "[ ]";
        }
        if ((i+1)/width <= width && ((i+1)/width)*width == (i+1)){//i is in the (width)s place (if width = 10, 10,20,30...) - makes board symmetrical
            sprintf(&board[i],"%s\n",gameBoard[i].tile);
            printf("%s\n",&board[i]);
        }else{
            sprintf(&board[i],"%s",gameBoard[i].tile);
            printf("%s",&board[i]);
        }   
    }
}

GameSquare createSquare(bool override, bool bomb, Coordinate usedPairs[], int boardIndex){ //the creation of a game square
    GameSquare square;
    if (bomb){square.bombHere = true;} //square is a bomb
    else{square.bombHere = false;}

    square.isFlagged = false;
    square.revealed = false;
    square.nearbyBombs = 0; //default value until figured out later on
    square.tile = "[ ]";
    square.indexNum = boardIndex;

    if (!override){ //assigns (x,y) to (1,1) then increases with every creation
            square.cor.x = 1;
            square.cor.y = height;
            //printf("\narrsize: %d, x: %d, y: %d\n",boardIndex, usedPairs[boardIndex].x,usedPairs[boardIndex].y);
            
        if (boardIndex == 0){
           // printf("yer first time gang\n");
            square.cor.x = 1;
            square.cor.y = height;
        }else{
            if (usedPairs[boardIndex-1].x == width){
              //  printf("end of line jump to next\n");
                square.cor.x = 1; 
                square.cor.y = usedPairs[boardIndex-1].y-1;
            }else{
            //    printf("movin thru line\n");
                square.cor.x = usedPairs[boardIndex-1].x+1;
                square.cor.y = usedPairs[boardIndex-1].y;
            }
        }
        ////if (square.bombHere){printf("\n(%d,%d) : B0MB?(%d)", square.cor.x, square.cor.y, square.bombHere);}
    }else{ //square is outside gameboard used for initialization/debugging (not part of game)
        square.cor.x = -500;
        square.cor.y = -500;
        square.bombHere = false; //fixes error if outside square has a bomb associated
    }
    
    return square;
}

GameSquare getSquare(Coordinate coor){ //the search for a game square using coordinates
    if ((coor.x > width || coor.x < 1) || (coor.y > height || coor.y < 1)){//x,y is out of range
        Coordinate temp = coor;
        return createSquare(true,false, &temp, 0); //returns a square that doesn't exist within the gameboard (used for the bombdetector otherwise useless)
    }
    int boardSize = width*height;
    GameSquare currentSquare;
    for (int i = 0; i < boardSize; i++){
        if (coor.x == gameBoard[i].cor.x && coor.y == gameBoard[i].cor.y){
            currentSquare = gameBoard[i];
            break;
        }    
    }
    return currentSquare;
}

void bombDetector(int boardSize){ //for every gamesquare checks if surrounding squares have bombs then tallys it up
    Coordinate above, right, below, left, topRight, topLeft, botRight, botLeft; //coordinate surrounding square

    //sets the coordinates for the surrounding squares of gameBoard[i] and if they contain a bomb increase nearbyBombs by one
    for (int i = 0; i < boardSize; i++){
        //printf("im detecting bombs boss (iteration #%d) \n", i);
        above.x = gameBoard[i].cor.x;
        above.y = gameBoard[i].cor.y+1;

        right.x = gameBoard[i].cor.x+1;
        right.y = gameBoard[i].cor.y;

        below.x = gameBoard[i].cor.x;
        below.y = gameBoard[i].cor.y-1;

        left.x = gameBoard[i].cor.x-1;
        left.y = gameBoard[i].cor.y;

        topRight.x = gameBoard[i].cor.x+1;
        topRight.y = gameBoard[i].cor.y+1;

        topLeft.x = gameBoard[i].cor.x-1;
        topLeft.y = gameBoard[i].cor.y+1;

        botRight.x = gameBoard[i].cor.x+1;
        botRight.y = gameBoard[i].cor.y-1;

        botLeft.x = gameBoard[i].cor.x-1;
        botLeft.y = gameBoard[i].cor.y-1;

        //the checks for the surrounding squares to see if there is a bomb
        if (getSquare(above).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(right).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(below).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(left).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(topRight).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(topLeft).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(botRight).bombHere)
            gameBoard[i].nearbyBombs++;
        if (getSquare(botLeft).bombHere)
            gameBoard[i].nearbyBombs++;
    }
}

void initialize(){ //allocates memory for the gameboard and creates the board square by square
    int difficulty;
    printf("(1: beginner, 2: novice, 3: hard, 4: expert)\nEnter Difficulty (pick a valid num from above): ");
    scanf(" %d", &difficulty);
    if(difficulty < 1 || difficulty > 4)
        printf("Shouldve listened...\n");
    else
        printf("Okay!\nSetting up the game...\n");
    
    //gameboard creation
    int boardSize = width*height;
    int totalBombs = 0;
    gameBoard = malloc(boardSize * sizeof(GameSquare)); 
    board = malloc(boardSize+(boardSize/4) * sizeof("[ ]"));

    Coordinate usedPairs[boardSize];
    srand(time(NULL)); //uses current time as seed for random num gen
    for (int i = 0; i < boardSize; i++){
        bool bomb = false;
        int temp = rand()%boardSize+1;
        if (difficulty == 1 || difficulty == 2){
            if (temp <= (difficulty*boardSize/8)){
               // printf(" %d | ", (difficulty*boardSize/8));
                bomb = true;
                totalBombs++;
            }
        }else if(difficulty == 3 || difficulty == 4){
            
            if (temp <= (difficulty*boardSize/6)){
               // printf(" %d | ", (difficulty*boardSize/6));
                bomb = true;
                totalBombs++;
            }
        }else{//if user selected anything else thats what they get
            bomb = true;
        }
        
        gameBoard[i] = createSquare(false, bomb, usedPairs, i);
        usedPairs[i].x = gameBoard[i].cor.x;
        usedPairs[i].y = gameBoard[i].cor.y;
    }

    bombDetector(boardSize); //figures out what tiles are next to bombs and how many
    printBoard(false);
    printf("There are a total of %d bombs.\n",totalBombs);
}

void teardown(){ //frees memory held by gameboard
    printf("Destroying the game...\n");

    free(gameBoard);
    free(board);
}

int reveal(GameSquare chosenTile, bool firstMove){ //reveals the chosen tile
    if (chosenTile.bombHere){return -1;} //if a bomb dont reveal and just return unsucess
    
    int boardIndex = chosenTile.indexNum;
    gameBoard[boardIndex].revealed = true; //reveals tile
   //// printf("%d:",chosenTile.nearbyBombs);

    if (chosenTile.nearbyBombs == 0 || firstMove){ //reveal other tiles not near bombs (if first move and is near a bomb reveals surrounding 8 squares)
        char bombNum = 0;
        if (firstMove){
            bombNum = chosenTile.nearbyBombs;
        }
        firstMove = false; //when recursively called doesn't assume issa first move
       //// printf("%d:%s\n",boardIndex,gameBoard[boardIndex + (width - 1)].tile);

        //first condition(s) in the logics are math statements to check if tile is along the edge of board to avoid stepping out of array
        //second is if the tile has already been revealed
        if ((boardIndex+1)%width != 0 && !gameBoard[boardIndex + 1].revealed){
            reveal(gameBoard[boardIndex + 1], firstMove); // tile to right
        }
        if (((boardIndex+1)%width != 0 && boardIndex+width < (width*height)) && !gameBoard[boardIndex + (width + 1)].revealed){
            reveal(gameBoard[boardIndex + (width + 1)], firstMove); //tile bottm right
        }
        if (boardIndex+width < (width*height) && !gameBoard[boardIndex + width].revealed){ 
            reveal(gameBoard[boardIndex + width],firstMove); //tile below
        }
        if ((boardIndex+width < (width*height)&& boardIndex%width != 0 && (boardIndex-1)%width != 0) && !gameBoard[boardIndex + (width - 1)].revealed){ 
            reveal(gameBoard[boardIndex + (width - 1)],firstMove); //tile bottom left
        }
        if (boardIndex%width != 0 && (boardIndex-1)%width != 0 && !gameBoard[boardIndex - 1].revealed){ 
            reveal(gameBoard[boardIndex - 1],firstMove); //tile to left
        }
        if ((boardIndex%width != 0 &&(boardIndex-1)%width != 0 && boardIndex-width > 0) && !gameBoard[boardIndex - (width + 1)].revealed){ 
            reveal(gameBoard[boardIndex - (width + 1)],firstMove); //tile top left
        }
        if (boardIndex-width > 0 && !gameBoard[boardIndex - width].revealed){ 
            reveal(gameBoard[boardIndex - width],firstMove); //tile above
        }
        if ((boardIndex-width > 0 && (boardIndex+1)%width != 0) && !gameBoard[boardIndex - (width - 1)].revealed){ 
            reveal(gameBoard[boardIndex - (width - 1)],firstMove); //tile top right
        }
    }
    
    return 0;//sucess
}

InputData input(int movesMade){ //collects input data from user and stores in struct inputData
    const char LIST_OF_INPUTS[] = "\n F - Flag a square \n U - Unflag a square \n A - Assert that square is bomb-free\n O - Open up board (only works first turn)\n S - Show the board \n Q - Quit Game\n H - How to play \n T - List of Inputs\n";
    const char HOW_TO_PLAY[] = "\nAssert if you think a tile has no bombs, Flag tiles with bombs and unflag \nif you feel that you made an error. Goal is to flag all bombs. Once every \nbomb has been appropriately flagged and nothing else, you win the game.\nCoordinates for the tiles are as shown: \n\n|(1,height)||(2,height)|...|(width,height)|\n|  .   .   .   .   .   .   .   .   .   .  |\n|  .   .   .   .   .   .   .   .   .   .  |\n|(1,1)||(2,1)| . . . . . . . . .|(width,1)|\n\n";
    InputData user;
    user.quit = false;
    
    printf("\n(hint: enter T for a list of all inputs!)                 moves made: %d\n", movesMade);
    printf("Enter an input: ");
    scanf("%1s", &user.command);

    if (user.command == 'T' || user.command == 't'){ //Command List
        printf("%s", LIST_OF_INPUTS);
        user = input(movesMade);

    }else if(user.command == 'H' || user.command == 'h'){//How to play
        printf("%s", HOW_TO_PLAY);
        user = input(movesMade);

    }else if(user.command == 'S' || user.command == 's'){//Show board
        printBoard(false);
        user = input(movesMade);

    }else if ((user.command == 'F' || user.command == 'f')     //Flag
             || (user.command == 'U' || user.command == 'u')   //Unflag
             || (user.command == 'A' || user.command == 'a')){ //Assert

        char choice;
        bool unconfirmed = true;

        while (unconfirmed){
            char tempx[8];
            char tempy[8];

            printf("Enter your x coordinate (x,y): ");
            scanf(" %s", tempx);
            user.cor.x = atoi(tempx);
            printf("Enter your y coordinate (%d,y): ", user.cor.x);
            scanf(" %s", tempy);
            user.cor.y = atoi(tempy);
            if ((user.cor.x > width || user.cor.x <= 0) || (user.cor.y > height || user.cor.y <= 0)){ //user inputted out of bound coordinates
                printf("Invalid coordinates, not within game's board (%dx%d)\n", width, height);
                continue;
                }
            printf("Your coordinates are at (%d,%d), Correct? (Y/N): ", user.cor.x, user.cor.y);
            scanf(" %c", &choice);

            if (choice == 'Y' || choice == 'y')
                unconfirmed = false;
        }
    }else if (user.command == 'O' || user.command == 'o'){ //Opens up free spaces in board to start the game
        if (movesMade == 0){
            for (int i = 0; i < (width*height); i++){//opens first instance of 0s or a set of tiles in the middle of board
                if (gameBoard[i].nearbyBombs == 0){
                    if (reveal(gameBoard[i],true) == 0){
                        movesMade++;
                        break;
                    }
                }else if(i > (width*height)/2 && !gameBoard[i].bombHere){//1/2-1/3 of the board has been searched no open 0s so start checking for any bombfree tile
                    if (reveal(gameBoard[i],true) == 0){
                        movesMade++;
                        break;
                    }
                }
            }
            printBoard(false);
            if (movesMade == 0){
                printf("Sorry, nothing useful can be opened up.\n");
            }
            user = input(movesMade);
        }
        else{
            printf("Sorry boss you can only do that first turn.\n");
            user = input(movesMade);
        }
        
    }else if (user.command == 'Q' || user.command == 'q'){ //Quit 
        user.quit = true;
    }else{ //unknown input
        printf("Invalid input try again.\n");
        user = input(movesMade);
    }
    return user;
}

bool* update(InputData user, int movesMade){ //updates the state of the world (returns (gameIsRunning,winner) )
    GameSquare chosenTile = getSquare(user.cor);
    bool bomb = (chosenTile.bombHere);
    bool firstMove = (movesMade == 0);
    ////printf("B0MB?(%d)\n", bomb);

    //adjusts gamesquare according to users command
    if (bomb && (user.command == 'a' || user.command == 'A')){//user is asserting and square is a bomb
        static bool returnVals[] = {false, false};//(!gameIsRunning,!winner)
        return returnVals;
    }
    else if(user.command == 'a' || user.command == 'A'){ //user is asserting and square isnt a bomb
        reveal(chosenTile, firstMove);
        chosenTile.revealed = true;
    }else if (user.command == 'f' || user.command == 'F') //user is flagging the square
        chosenTile.isFlagged = true;
    else if(user.command == 'u' || user.command == 'U') //user is unflagging the square
        chosenTile.isFlagged = false;
    
    gameBoard[chosenTile.indexNum] = chosenTile; //updates current tile
    //todo: figure out how winning is going to work (once every non bomb is revealed / bomb has been flagged)
    int boardSize = width*height;
    bool won = false;
    for (int i = 0, numBombs = 0, numFlags = 0; i < boardSize; i++){ //checks for a win
        if(gameBoard[i].bombHere && gameBoard[i].isFlagged)
            numFlags++;
        if (gameBoard[i].bombHere)
            numBombs++;

        if (numFlags == numBombs && i == boardSize-1)//all bombs have been flagged
            won = true;
    }
    if (won){
        static bool returnVals[] = {true, true};//gameIsRunning,winner
        return returnVals;
    }
    
    static bool returnVals[] = {true, false};//gameIsRunning,!winner
    return returnVals;
}

void display(bool gameIsRunning, InputData user){ //displays the state of the world
    GameSquare chosenTile = getSquare(user.cor);
    printBoard(!gameIsRunning);//prints an updated board to the screen

    if (gameIsRunning){
        printf("OK!\n");
        if(user.command == 'a' || user.command == 'A') //user is asserting and square isnt a bomb
            printf("Your chosen tile has %d bomb(s) around it\n", chosenTile.nearbyBombs);
        else if (user.command == 'f' || user.command == 'F') //user is flagging the square
            printf("Your chosen tile has been flagged\n");
        else if(user.command == 'u' || user.command == 'U') //user is unflagging the square
            printf("Your chosen tile has been unflagged\n");
    }else
        printf("BOOM!\n");

}

int main(int argc, char *argv[]){ //entry
    
    //the check to see if the user started the program correctly
    if (argc < 3 || argc > 3){
        printf("\nYou entered a invalid # of args.\n");
        printf("Start the program like this: \n\"MineSweeper (width) (height)\"\n");
        return 1;
    }else{//user inputted correct amount of args
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        if ((height <= 0 || width <= 0) || (height > 50 || width > 50)){

            printf("\nInvalid format. Height & Width must be numerical / between 0-50. Try again.\n");
            return 1;
        }
        printf("\nThe width of your board is %d and the height is %d\n", width, height);
    }

    bool gameIsRunning = true; //game loop condition
    bool winner = false;
    int movesMade = 0;
    initialize();
    while (gameIsRunning) {
        InputData user = input(movesMade); //stores all of users input in struct
        if (user.quit){//if user quit end game
            gameIsRunning = false;
            continue;
        }   
        bool *updateVals = update(user, movesMade); //returns whether user is still playing or not (asserted a bomb or not) and if the user won
        gameIsRunning = updateVals[0];
        winner = updateVals[1];
        movesMade++;
        display(gameIsRunning, user); //displays result from update()
        if (winner)
            gameIsRunning = false;
    }
    if (winner)
        printf("You have won!\n");
    else
        printf("You have lost!\n");

    teardown();
    return 0;
}