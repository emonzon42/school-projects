/* 
Eli Monzon 
2.6.20
Mine Sweeper
*/

#include <stdio.h>
#include <stdbool.h>

typedef struct xyPairs{//(x,y) coordinate pair
    int x;
    int y;
}Coordinate;

typedef struct inputData{ //holds the structure for the user's input data
    char command;
    Coordinate cor; //inputted x,y coordinates
    bool quit; 
}InputData;

int width; //x max
int height;//y max

void initialize(){
    printf("\nSetting up the game...\n");
}

void teardown(){
    printf("Destroying the game...\n");
}

InputData input(){
    const char LIST_OF_INPUTS[] = " I - Flag a square \n O - Unflag a square \n A - Assert that square is bomb-free \n Q - Quit Game \n T - List of Inputs\n";
    InputData user;
    
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
            printf("Enter your x coordinate (x,y): ");
            scanf(" %d", &user.cor.x);
            printf("Enter your y coordinate (%d,y): ", user.cor.x);
            scanf(" %d", &user.cor.y);
            printf("Your coordinates are at (%d,%d), Correct? (Y/N): ", user.cor.x, user.cor.y);
            scanf(" %c", &choice);

            if (choice == 'Y' || choice == 'y')
                unconfirmed = false;
        }
    }else if (user.command == 'Q' || user.command == 'q'){ //Quit 
        user.quit = true;
    }else{ //unknown input
        printf("missed\n");
        user = input();
    }
    return user;
}

bool update(InputData user){
    bool bomb = (user.cor.x == 0 && user.cor.y == 0);

    if (bomb && (user.command == 'a' || user.command == 'A')) //user is asserting bombfree and square is a bomb
        return false;
    return true;
}

void display(bool gameIsRunning, InputData user){

    if (gameIsRunning){
        printf("OK!\n");
    }else
        printf("BOOM!\n");
}

int main(){
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