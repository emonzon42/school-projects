/* 
Eli Monzon 
2.6.20
Mine Sweeper
*/
//TODO: FINSIH fixing arr size and allocation for the usedpairs arr
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h>

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
    bool bombHere;
    bool revealed;
    bool isFlagged;
    struct boardSquare *p; //pointer to the square
}GameSquare;

int width; //x max
int height;//y max
GameSquare *gameBoard; //gameboard is an array of gamesquares

GameSquare createSquare(bool override, bool bomb, Coordinate usedPairs[], int arrSize){ //the creation of a game square
   //printf("hey boss a square is being made\n");
    GameSquare square;
    square.p = &square;
    
    if (bomb) {square.bombHere = true;} //square is a bomb
    square.isFlagged = false;
    square.revealed = false;
    square.nearbyBombs = 0; //default value until figured out later on

    if (!override){ //randomly assigns the square a (x,y) coordinate based on whether it was used or not
        bool unsureIfUsed = true; //an error chrcking flag that prevents (x,y) pairs being created twice
        srandom(time(NULL)); //uses current time as seed

        while (unsureIfUsed){ //decides the (x,y) coordinate for the square
            square.cor.x = random() % width+1; //x coordinate
            square.cor.y = random() % height+1; //y coordinate
            for (int i = 0; i < arrSize; i++){
                //printf("%d:%d\n", i, arrSize);
                if (square.cor.x == usedPairs[i].x && square.cor.y == usedPairs[i].y){ //if assigned x and y combo is used already try again
                    unsureIfUsed = true;
                    break;
                }
                unsureIfUsed = false;
                
            }
            //printf("\n%d\n",unsureIfUsed);
            //sleep(1);
        }

    }else{//square is outside gameboard used for initialization/debugging (not part of game)
        square.cor.x = usedPairs[0].x;
        square.cor.y = usedPairs[0].y;
        square.bombHere = false; //fixes error if outside square has a bomb associated
    }
    return square;
}

GameSquare getSquare(Coordinate coor){ //the search for a game square using coordinates
    if ((coor.x > width || coor.x < 0) || (coor.y > height || coor.y < 0)){//x,y is out of range
        Coordinate temp = coor;
        return createSquare(true, false, &temp, 0); //returns a square that doesn't exist within the gameboard (used for the bombdetector otherwise useless)
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
    printf("\nSetting up the game...");
    
    //gameboard creation
    int boardSize = width*height;
    gameBoard = malloc(boardSize * sizeof(GameSquare)); 
    
    int usedPairSize = 0;
    Coordinate *usedPairs = malloc(usedPairSize * sizeof(Coordinate)); //array of used x,y pair values used for the createSquare() funct
    for (int i = 0; i < boardSize; i++){
        usedPairSize = i+1; //kept for faster creation loops & reallocation
        usedPairs = realloc(usedPairs, usedPairSize * sizeof(Coordinate));
        
        //printf("%d\n ", i);
        if(i == (i/4)*4){ //if i is divisble by 4
            gameBoard[i] = createSquare(false, true, usedPairs, usedPairSize); //this square will be a bomb
            usedPairs[i].x = gameBoard[i].cor.x;
            usedPairs[i].y = gameBoard[i].cor.y;
            //printf("\n(%d,%d) : B0MB?(%d)", gameBoard[i].cor.x, gameBoard[i].cor.y, gameBoard[i].bombHere);
            continue;
        }
        gameBoard[i] = createSquare(false, false, usedPairs, usedPairSize); //this square wont be a bomb
        usedPairs[i].x = gameBoard[i].cor.x;
        usedPairs[i].y = gameBoard[i].cor.y;
        //printf("\n(%d,%d) : B0MB?(%d)", gameBoard[i].cor.x, gameBoard[i].cor.y, gameBoard[i].bombHere);
    }
    free(usedPairs);
    bombDetector(boardSize); //figures out whether or not the squares are next to bombs

}


void teardown(){ //frees memory held by gameboard
    printf("Destroying the game...\n");

    free(gameBoard);
}

InputData input(){ //collects input data from user and stores in struct inputData
    const char LIST_OF_INPUTS[] = " I - Flag a square \n O - Unflag a square \n A - Assert that square is bomb-free \n Q - Quit Game \n T - List of Inputs\n";
    InputData user;
    user.quit = false;
    
    printf("\n(hint: enter T for a list of all inputs!)\n");
    printf("Enter an input: ");
    scanf("%1s", &user.command);

    if (user.command == 'T' || user.command == 't'){ //Command List
        printf("%s", LIST_OF_INPUTS);
        user = input();

    }else if ((user.command == 'I' || user.command == 'i')     //Flag
             || (user.command == 'O' || user.command == 'o')   //Unflag
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
    }else if (user.command == 'Q' || user.command == 'q'){ //Quit 
        user.quit = true;
    }else{ //unknown input
        printf("Invalid input try again.\n");
        user = input();
    }
    return user;
}

bool update(InputData user){ //updates the state of the world
    GameSquare currentSquare = getSquare(user.cor);
    bool bomb = (currentSquare.bombHere);
    //printf("B0MB?(%d)\n", bomb);

    //adjusts gamesquare according to users command
    if (bomb && (user.command == 'a' || user.command == 'A'))//user is asserting and square is a bomb
        return false;
    else if(user.command == 'a' || user.command == 'A') //user is asserting and square isnt a bomb
        currentSquare.revealed = true;
    else if (user.command == 'i' || user.command == 'I') //user is flagging the square
        currentSquare.isFlagged = true;
    else if(user.command == 'o' || user.command == 'O') //user is unflagging the square
        currentSquare.isFlagged = false;
    
    return true;
}

void display(bool gameIsRunning, InputData user){ //displays the state of the world
    GameSquare currentSquare = getSquare(user.cor);

    if (gameIsRunning){
        printf("OK!\n");
        if(user.command == 'a' || user.command == 'A') //user is asserting and square isnt a bomb
            printf("Your chosen tile has %d bomb(s) around it\n", currentSquare.nearbyBombs);
        else if (user.command == 'i' || user.command == 'I') //user is flagging the square
            printf("Your chosen tile has been flagged\n");
        else if(user.command == 'o' || user.command == 'O') //user is unflagging the square
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

    bool gameIsRunning = true;
    initialize();
    while (gameIsRunning) {
        InputData user = input(); //stores all of users input in struct
        if (user.quit){//if user quit end game
            gameIsRunning = false;
            continue;
            }
        gameIsRunning = update(user); //returns whether user is still playing or not (asserted a bomb or not)
        display(gameIsRunning, user); //displays result from update()
        }
    teardown();
    return 0;
}