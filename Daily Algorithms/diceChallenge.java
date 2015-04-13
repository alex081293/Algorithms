import java.util.*;
import java.lang.*;
import java.io.*;

class diceChallenge {

	public static void main(String args[]) {

		int NUM_OF_DIE = 5;
		int GOAL_DIE_MAX = 20;

		Random randomGenerator = new Random();
		int goalNum = randomGenerator.nextInt(GOAL_DIE_MAX)+1;

		System.out.println("You're target number is " + goalNum);

		int diceArray[] = new int[NUM_OF_DIE];

		int safetyCheckTotal = 0;
		for(int i=0; i<NUM_OF_DIE; i++) {
			diceArray[i] = randomGenerator.nextInt(6)+1;
			safetyCheckTotal += diceArray[i];
		}

	    Boolean incompleteFlag = false;

	    if ((safetyCheckTotal%2) == (goalNum%2)) {
			for(int i=0; i<(Math.pow(2, (NUM_OF_DIE))); i++) {	
				int sumOfDice = 0;
				String binaryOfI = Integer.toBinaryString(i);
				
				while (binaryOfI.length() < (NUM_OF_DIE)) {
					binaryOfI = "0" + binaryOfI;
				}

				for(int k=0; k<(NUM_OF_DIE); k++) {
					char decider = binaryOfI.charAt((k));
					int multipler = ((decider == '1') ? -1 : 1);
					sumOfDice += (diceArray[k] * multipler);
				}

				if (sumOfDice == goalNum) {
					incompleteFlag = true;

					for (int j=0; j<NUM_OF_DIE; j++) {
						char symbol = (binaryOfI.charAt(j) == '0') ? '+' : '-'; 
						System.out.print(" " + symbol + " " + Math.abs(diceArray[j]));
					}

					System.out.print(" = " + goalNum);
					break;
				}
			}
		}

		if (!incompleteFlag) {
			System.out.println("Target of " + goalNum + " can not be made with dices:");
			for(int i=0; i<NUM_OF_DIE; i++) {
				System.out.print(" " + Math.abs(diceArray[i]));
			}
		}
	}
}