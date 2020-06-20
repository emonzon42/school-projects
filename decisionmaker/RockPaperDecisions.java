/**
 * RockPaperDecisions
 * Created by Elijah Monzon on 12/11/19.
 * Copyright © 2019 Elijah Monzon. All rights reserved.
 */


import java.util.*;

public class RockPaperDecisions {

    private boolean again = false;  //the flag to see if this is the second time the user is playing w same choices
    private Scanner scan;           //user input
    private LinkedList<String> memory; //a memory bank of all the users previous choices for rock/paper/scissors
    private String[] decision;      //the user's decisions

    public RockPaperDecisions() {//constructor
        decision = new String[2];
        memory = new LinkedList<String>();
        scan = new Scanner(System.in);
    }

    public boolean run() { // the beginning and end of the game
        boolean invalid1 = true;
        boolean invalid2 = true;
        String userInput = "";

        // user is given opportunity to enter their choices
        if (!again) {
            System.out.print("Decision 1: ");
            decision[0] = scan.nextLine();
            System.out.print("Decision 2: ");
            decision[1] = scan.nextLine();
        }

        play(); //rock paper scissors

        while (invalid1) { // check if user wants to play again;
            System.out.println("Play Again? (Y/N)");
            userInput = scan.nextLine();
            if ("y".equalsIgnoreCase(userInput)) {
                System.out.println("Would you like to redo with the same choices or make new ones? (REDO/NEW)");
                while (invalid2) {
                    userInput = scan.nextLine();
                    if ("redo".equalsIgnoreCase(userInput)) {
                        again = true;
                        return true;
                    } else if ("new".equalsIgnoreCase(userInput)){
                        return true;
                    }
                    System.out.println("Invalid input. Only enter REDO/NEW");
                }
                return true;
            } else if ("n".equalsIgnoreCase(userInput)) {

                System.out.println("Thanks for playing boss.");
                return false;
            } else if ("y/n".equalsIgnoreCase(userInput)) {
                System.out.println("u think ur funny");
            }
            System.out.println("Invalid input. Only enter Y/N");
        }
        return true;
    }

    private int rand(int max) { //random number generator between 0 to max value indicated
        Random rand = new Random();
        int value = rand.nextInt(max);
        return value;
    }

    private String rps() { //the cpu's move
        String choices[] = {"","",""}; //choices
        String choice = "";
        int num = rand(3000)/1000;
        int numRock = 0; //num of times "rock" was chosen
        int numPape = 0; //num of times "paper" was chosen
        int numScis = 0; //num of times "scissors" was chosen
        
        switch (num) { //depending on the random value returned the users first option will be 
            case 0:
                choices[0] = "rock";
                break;
            case 1:
                choices[0] = "paper";
                break;
            case 2:
                choices[0] = "scissors";
                break;
            default:
                break;
        }

        /** How the choices 2 & 3 are made: (choices[1] & choices[2])
         * 
         * 2: The cpu will check how many times the user has picked the same value since opening
         * the program and then depending on the one that the user picks the most often the
         * cpu will pick the one that beats it
         * &
         * 3: the cpu will check what the last move the user made is and consider choosing the value that beats it
         */

        if (!memory.isEmpty()) {
            //tallies how many times the user chose each move
            for (int i = 0; i < memory.size(); i++) {
                if (memory.get(i).equals("rock"))
                    numRock++;
                else if (memory.get(i).equals("paper"))
                    numPape++;
                else if (memory.get(i).equals("scissors"))
                    numScis++;
            }

            //the users second option is based on how many times the user has chosen an option, assumes it will choose it again and counters it
            if (numRock > numPape && numRock > numScis) { //if rock is the highest then go with paper
                choices[1] = "paper";
            } else if (numPape > numRock && numPape > numScis) { // if paper is the highest then go with scissors
                choices[1] = "scissors";
            } else if (numScis > numRock && numScis > numPape) {//if scissors is the highest then go with rock
                choices[1] = "rock";
            }else{ //if there is an equal amount of all the moves it'll just go random
                choices[1] = choices[0];
            }

            //the users third option is to assume the user will use the same move as last round and counter it
            String last = memory.getFirst().toString();
            if (last.equalsIgnoreCase("rock")) {
                choices[2] = "paper";
            } else if (last.equalsIgnoreCase("paper")) {
                choices[2] = "scissors";
            }else if (last.equalsIgnoreCase("scissors")) {
                choices[2] = "rock";
            }
        }
        
        //if all choices are the same or choice 2/3 are empty, then choice is automatically set as choice 1
        if ((choices[0].equals(choices[1]) && choices[1].equals(choices[2])) || choices[1].equals("") || choices[2].equals("")) {
            ////System.out.println("damn all my choices are the same? " + choices[0]+" "+choices[1]+" "+choices[2]);
            choice = choices[0];
        }else{ //randomly pick a number between 0 to 3000 and if between 0-1000 it will choose choices[1], 1000-2000: choices[0], 2000-3000: choices[2];
            num = rand(3000)/1000;
            switch (num) {
                case 0:
                    ////System.out.println("imma be smart w it");
                    choice = choices[1];
                    break;
                case 1:
                    ////System.out.println("imma just take a chance");
                    choice = choices[0];
                    break;
                case 2:
                    ////System.out.println("imma take a educated chance");
                    choice = choices[2];
                    break;
                default:
                    ////System.out.println("idk");
                    choice = choices[0];
                    break;
            }
        }
        return choice;
    }

    private int roundWinner(String user, String cpu) {  //the check to see who won the round
        /** 
         * return 1: user wins
         * return 2: cpu wins
         * return 3: draw
        */
        String rock = "rock";
        String paper = "paper";
        String scissors = "scissors";

        if (user.equalsIgnoreCase(rock) && cpu.equalsIgnoreCase(rock)) { //user chose rock, cpu chose rock
            return 3;
        } else if (user.equalsIgnoreCase(rock) && cpu.equalsIgnoreCase(paper)) { //user chose rock, cpu chose paper
            return 2;
        } else if (user.equalsIgnoreCase(rock) && cpu.equalsIgnoreCase(scissors)) {//user chose rock, cpu chose scissors
            return 1;
        } else if (user.equalsIgnoreCase(paper) && cpu.equalsIgnoreCase(rock)){ //user chose paper, cpu chose rock
            return 1;
        } else if (user.equalsIgnoreCase(paper) && cpu.equalsIgnoreCase(paper)) {//user chose paper, cpu chose paper
            return 3;
        } else if (user.equalsIgnoreCase(paper) && cpu.equalsIgnoreCase(scissors)) {//user chose paper cpu chose scissors
            return 2;
        } else if (user.equalsIgnoreCase(scissors) && cpu.equalsIgnoreCase(rock)) {//user chose scissors, cpu chose rock
            return 2;
        } else if (user.equalsIgnoreCase(scissors) && cpu.equalsIgnoreCase(paper)) {//user chose scissors, cpu chose paper
            return 1;
        } else if (user.equalsIgnoreCase(scissors) && cpu.equalsIgnoreCase(scissors)) {//user chose scissors, cpu chose scissors
            return 3;
        }
        return 0;
    }


    private void play() { //Rock/Paper/Scissors game
    
        final String INSTRUCTIONS = "So now we will play Rock Paper Scissors to help you decide what to do. \n"
                                    + "I will randomly pick one of the options you gave me and if i win then you \n"
                                    + "then thats the option you go with but if i lose then you go with the \n"
                                    + "other option. Best 2 out of 3 wins. \n\n";

        final String SHOOT = "Rock, Paper, Scissors, say SHOOT: ";
                                    
        String usersMove;           //the move that the user makes
        String cpusMove;            //the move the cpu makes
        String userDecision;        //to store the decision value associated with the user
        String cpuDecision;         //to store the decision value associated with the cpu
        int cpuWin = 0;             // # of times the cpu won
        int userWin = 0;            // # of times the user won
        boolean playerWon = false;  //flag if the user won or not
        boolean noWinner = true;    //flag to allow another round of the game to happen

        /**if the user starts the application over from the beginning of the rock/paper/scissors game then the instructions
        will not be presented*/
        System.out.println();
        if (!again)
            System.out.print(INSTRUCTIONS);

        System.out.print(SHOOT);

        //how the cpu picks what decision the user should go with
        if (rand(2000) < 1000) {
            cpuDecision = decision[0];
        } else {
            cpuDecision = decision[1];
        }

        //the decision the user has to go with if the cpu loses
        if (cpuDecision.equals(decision[0])) {
            userDecision = decision[1];
        } else {
            userDecision = decision[0];
        }

        while (noWinner) { //will loop until somebody has won 2 games
            if (cpuWin == 2 || userWin == 2) {
                noWinner = false;
                if (userWin > cpuWin)
                    playerWon = true;
                break;
            }

            cpusMove = rps();
            usersMove = scan.nextLine();

            if (!(usersMove.equalsIgnoreCase("rock") || usersMove.equalsIgnoreCase("paper") || usersMove.equalsIgnoreCase("scissors"))) {
                System.out.print("\ninvalid input try again: ");
                continue;
            }  
            System.out.println(cpusMove);
            if (memory.size() == 100){
                memory.removeLast();
            } //to avoid storing more than what's necessary in memory
                
            memory.addFirst(usersMove);

            switch (roundWinner(usersMove, cpusMove)) {
            case 1:
                ++userWin;
                if (userWin != 2) {
                    System.out.println("Alright you beat me this round.");
                    System.out.println("I chose " + cpusMove + ". Lets go again.");
                    System.out.print(SHOOT);
                }
                break;
            case 2:
                ++cpuWin;
                if (cpuWin != 2) {
                    System.out.println("I won this round!");
                    System.out.println("I chose " + cpusMove + ". Lets go again.");
                    System.out.print(SHOOT);
                }
                break;
            case 3:
                System.out.println("Draw!");
                System.out.println("I chose " + cpusMove + ". Lets go again.");
                System.out.print(SHOOT);
                break;
            default:
                break;
            }
        }
        
        if (playerWon){
            System.out.println("You won!");
            System.out.println("You now have to go with '" + userDecision + "' as your decision");
            return;
        }
        System.out.println("I won!");
        System.out.println("You now have to go with '" + cpuDecision + "' as your decision");
    }
}
