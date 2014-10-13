/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 3

	This driver(Checker.java) is used to go through all of the puzzles
	EightPuzzleDriver.java is for 1 puzzle at a time
	Example Usage: Checker -h puzzle*.txt
	Enter either -h or -m for hamming or manhattan
	
	On manhattan, if doing puzzles 00-45, it will make it through in less than 1 min on my machine with -Xmx1350m
	On hamming, it makes it to 31 before it starts to go very slow then runs out of space. I cannot give it more heap space on my machine.
*/
import java.util.*;
import java.io.*;

public class EightChecker 
{

    public static void main(String[] args) throws IOException{
        Scanner in;
		
		if (args.length < 2 || (args[0].compareTo("-h") != 0 && args[0].compareTo("-m") != 0))
		{
			System.out.println("Correct usage: Checker (Priority function(-h/-m) puzzle*.txt");
			System.out.println("Example: Checker -h puzzle*.txt");
			System.exit(0);
		}
		int prtyType = 1;
		if (args[0].compareTo("-h") == 0)
		{
			prtyType = 1;
		}
		else if (args[0].compareTo("-m") == 0)
		{
			prtyType = 2;
		}		
        // for each command-line argument
        for (String filename : args) {

            // read in the board specified in the filename
			if (filename.compareTo("-h") != 0 && filename.compareTo("-m") != 0)
			{
				in = new Scanner(new File(filename));
				int N = in.nextInt();
				int[][] tiles = new int[N][N];
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						tiles[i][j] = in.nextInt();
					}
				}

				// solve the slider puzzle
				Board initial = new Board(tiles, prtyType);
				Solver solver = new Solver(initial);
				System.out.println(filename + ": " + solver.moves());
			}
        }
    }
}
