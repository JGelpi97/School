/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 3
*/

import java.lang.Math.*;

public class Board implements Comparable<Board>
{
	private int board[][];
	private int prty = 0;
	private int prtyType = 1;	//1 = H, 2 = M
	private int n;
	int moves = 0;

	
	//blocks[i][j] = row i, col j
	public Board(int[][] blocks, int pType)
	{		
		n = blocks.length;
		prtyType = pType;
		//Set up the board
		board = new int[n][n];
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				board[r][c] = blocks[r][c];
			}
		}
		if (!isSolvable())
		{
			System.out.println("Unsolvable Puzzle");
		}
	}
	
	//It makes more sense to have moves in the board(the number of moves it took to get to the board)
	//So I use moves in here instead of in the nodes. But i am not allowed to change the constructor so i have to individually call this method for every new board...
	//So basically it sucks, now in addition to creating every board, I also have to call setMoves();
	public void setMoves(int m)
	{
		moves = m;
		if (prtyType == 1)
		{
			prty = hamming() + moves;
		}
		else
		{
			prty = manhattan() + moves;
		}
	}
	
	public int dimension()
	{
		return n;
	}
	
	//Number of blocks out of place
	public int hamming()
	{
		int h = 0;
		int arr[] = new int[n*n];
		int i = 0;
		//Create a 1d array of the blocks
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				arr[i] = board[r][c];
				i++;
			}
		}

		//Compare each slot in array to what it should be
		for (int k = 1; k <= n * n; k++)
		{
			if (k != arr[k - 1])
			{
				h++;
			}
		}
		return h;
	}
	
	//Sum of manhattan distances between blocks and goal
	public int manhattan()
	{	
		int m = 0;
		//Create goal board for comparing
		int[][] ans = new int[n][n];
		int k = 1;
	    for (int i = 0; i < ans.length; i++)
		{
            for (int j = 0; j < ans.length; j++)
			{
				if ((i == ans.length - 1) && (j == ans.length - 1))
				{
					ans[i][j] = 0;
				}
				else
				{
					ans[i][j] = k;
					k++;
				}
			}
        }
		//Go through solved board, then find the number in the current board
		for (int i = 0; i < ans.length; i++)
		{
            for (int j = 0; j < ans.length; j++)
			{
				for (int a = 0; a < ans.length; a++)
				{
					for (int b = 0; b < ans.length; b++)
					{
						if (ans[i][j] == board[a][b] && ans[i][j] != 0)
						{
							m += (Math.abs(i - a) + Math.abs(j - b));		//calculate the offset between the current board and the goal							
							a = ans.length;
							b = ans.length;
						}
					}
				}
			}
		}
		return m;
	}
	
	//Is this the goal board?
	public boolean isGoal()
	{
		//Create goal board
		int[][] ans = new int[n][n];
		int k = 1;
	    for (int i = 0; i < ans.length; i++)
		{
            for (int j = 0; j < ans.length; j++)
			{
				if ((i == ans.length - 1) && (j == ans.length - 1))
				{
					ans[i][j] = 0;
				}
				else
				{
					ans[i][j] = k;
					k++;
				}
			}
        }
		//Does goal board equal this?
		return equals(ans);
	}
	
	//Is this board solvable?
	public boolean isSolvable()
	{
		int inv = 0;
		
		int arr[] = new int[n*n];
		int i = 0;
		//Create a 1d array of the blocks
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				arr[i] = board[r][c];
				i++;
			}
		}
		
		if (n % 2 == 1)				//odd board size
		{
			for (int r = 0; r < arr.length; r++)		//Count inversions
			{
				if (arr[r] == 0)						//Dont use blank space
				{
					r++;
				}
				for (int k = r; k < arr.length; k++)	
				{					
					if (arr[k] != 0)
					{
						if (arr[k] < arr[r])
						{
							inv++;
						}
					}
				}
			}
			if (inv % 2 == 1)							//If odd and odd board then unsolvable
			{
				return false;
			}
			return true;
		}
		else 						//even board size
		{

			for (int r = 0; r < arr.length; r++)		//Count inversions
			{
				if (arr[r] == 0)
				{
					r++;
				}
				for (int k = r; k < arr.length; k++)
				{					
					if (arr[k] != 0)
					{
						if (arr[k] < arr[r])
						{
							inv++;
						}
					}
				}
			}
			for (int r = 0; r < arr.length; r++)
			{
				if (arr[r] == 0)
				{
					inv += r / n;						//Add the row that blank space is in 
					break;
				}
			}
			if (inv % 2 == 0)							//If even and board even then unsolvable
			{
				return false;
			}
			return true;
		}
	}
	
	//Does this board equal y?
	public boolean equals(Object y)
	{
		Board bo = (Board)y;
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				if (board[r][c] != bo.board[r][c])		//If any index doesnt equal the other return false
				{
					return false;
				}
			}
			
		}
		return true;
	}
	
	public int priority()
	{
		return prty;
	}
	
	//Place all neighboring boards into your iterable Queue
	public Iterable<Board> neighbors()
	{
		Queue<Board> nei = new Queue<Board>();		//All the neighbors
		
		//Get where 0 is store in row, col
		int row = 0;
		int col = 0;
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				if (board[r][c] == 0)
				{
					row = r;
					col = c;
					break;
				}
			}
		}
		if (row != 0 && row != (n - 1)) 	//Not on top of bottom row, get the 2 boards by switching 0 up and down
		{
			int newB1[][] = new int[n][n];
			int newB2[][] = new int[n][n];
			for (int r = 0; r < n; r++)
			{
				for (int c = 0; c < n; c++)
				{
					newB1[r][c] = board[r][c];
					newB2[r][c] = board[r][c];					
				}
			}

			newB1[row][col] = newB1[row - 1][col];
			newB1[row - 1][col] = 0;
			Board b1 = new Board(newB1, prtyType);
			b1.setMoves(moves + 1);
			newB2[row][col] = newB1[row + 1][col];
			newB2[row + 1][col] = 0;
			Board b2 = new Board(newB2, prtyType);
			b2.setMoves(moves + 1);
			nei.enqueue(b1);
			nei.enqueue(b2);
		}
		if (row == 0)						//In far left row, get 1 neighbor
		{
			int newB1[][] = new int[n][n];
			for (int r = 0; r < n; r++)
			{
				for (int c = 0; c < n; c++)
				{
					newB1[r][c] = board[r][c];				
				}
			}
			newB1[row][col] = newB1[row + 1][col];
			newB1[row + 1][col] = 0;
			Board b1 = new Board(newB1, prtyType);
			b1.setMoves(moves + 1);
			nei.enqueue(b1);
		}
		if (row == (n - 1))					//In right tow, get 1 neghbor
		{
			int newB1[][] = new int[n][n];
			for (int r = 0; r < n; r++)
			{
				for (int c = 0; c < n; c++)
				{
					newB1[r][c] = board[r][c];				
				}
			}
			newB1[row][col] = newB1[row - 1][col];
			newB1[row - 1][col] = 0;
			Board b1 = new Board(newB1, prtyType);
			b1.setMoves(moves + 1);
			nei.enqueue(b1);
		}
		
		if (col != 0 && col != (n - 1)) 	//Not in either column end, get 2 boards
		{
			int newB1[][] = new int[n][n];
			int newB2[][] = new int[n][n];
			for (int r = 0; r < n; r++)
			{
				for (int c = 0; c < n; c++)
				{
					newB1[r][c] = board[r][c];
					newB2[r][c] = board[r][c];					
				}
			}

			newB1[row][col] = newB1[row][col - 1];
			newB1[row][col - 1] = 0;
			Board b1 = new Board(newB1, prtyType);
			b1.setMoves(moves + 1);
			newB2[row][col] = newB1[row][col +  1];
			newB2[row][col + 1] = 0;
			Board b2 = new Board(newB2, prtyType);
			b2.setMoves(moves + 1);
			nei.enqueue(b1);
			nei.enqueue(b2);
		}	
		if (col == 0)						//Far left col
		{
			int newB1[][] = new int[n][n];
			for (int r = 0; r < n; r++)
			{
				for (int c = 0; c < n; c++)
				{
					newB1[r][c] = board[r][c];				
				}
			}
			newB1[row][col] = newB1[row][col + 1];
			newB1[row][col + 1] = 0;
			Board b1 = new Board(newB1, prtyType);
			b1.setMoves(moves + 1);
			nei.enqueue(b1);
		}
		if (col == (n - 1))					//Far right col
		{
			int newB1[][] = new int[n][n];
			for (int r = 0; r < n; r++)
			{
				for (int c = 0; c < n; c++)
				{
					newB1[r][c] = board[r][c];				
				}
			}
			newB1[row][col] = newB1[row][col - 1];
			newB1[row][col - 1] = 0;
			Board b1 = new Board(newB1, prtyType);
			b1.setMoves(moves + 1);
			nei.enqueue(b1);
		}
		//Return queue of neighbors
		return nei;
	}
	
	public String toString()
	{
		StringBuilder x = new StringBuilder();
		x.append(n + "\n");
		for (int r = 0; r < n; r++)
		{
			for (int c = 0; c < n; c++)
			{
				x.append(" " + board[r][c]);
			}
			x.append("\n");
		}
		return x.toString();
	}
	
	//Compare priotities
	public int compareTo(Board b)
	{
		if (b.prty == this.prty)
			return 0;
		else if (this.prty < b.prty)
			return -1;
		else if (this.prty > b.prty)
			return 1;
		return 0;
	}
}
