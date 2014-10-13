/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 3
	
	Example Usage: EightPuzzleDriver -h puzzle04.txt
	Enter either -h or -m for hamming or manhattan
	
	All puzzles work on manhattan
	Most puzzles work on hamming, the others run out of space or go very slow
*/

public class EightPuzzleDriver
{
	public static void main(String[] args)
	{
	    // create initial board from file
		if (args.length < 2)
		{
			System.out.println("Correct usage: EightPuzzleDriver (Priority function(-h/-m) file.txt");
			System.out.println("Example: EightPuzzleDriver -h puzzle04.txt");
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
		
        In in = new In(args[1]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
		{
            for (int j = 0; j < N; j++)
			{
                blocks[i][j] = in.readInt();
			}
        }    
		
        Board initial = new Board(blocks, prtyType);      // solve the puzzle
        Solver solver = new Solver(initial);    // print solution to standard output
        if (!initial.isSolvable())
		{
            System.out.println("No solution possible");
		}
        else 
		{
			System.out.println("Minimum number of moves = " + solver.moves());
            for (Board board : solver.solution())
			{
                System.out.println(board);				
			}
        }
	}
}