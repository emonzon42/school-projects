/**
 * RockPaperDecisions
 * Created by Elijah Monzon on 12/11/19.
 * Copyright © 2019 Elijah Monzon. All rights reserved.
 */

public class Driver {

    public static void main(String[] args) {
        RockPaperDecisions game = new RockPaperDecisions();
        boolean running = true;
        System.out.println("Be sure to check out some of my other designs at https://github.com/emonzon42");

        while (running) {
            System.out.println(); //make game cleaner
            running = game.run();
        }
    }
}