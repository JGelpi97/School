/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 3

*/

import java.util.*;

public class Solver
{
	MinPQ<Node> pq = new MinPQ<Node>();
	Queue<Board> pathQ = new Queue<Board>();
	HashMap hm = new HashMap();
	int moves = 0;
	Board initial;
	public Solver(Board init)
	{
		initial = init;
		if (isSolvable())
		{
			//Create final board
			int[][] ans = new int[initial.dimension()][initial.dimension()];
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
			Board answer = new Board(ans, 1);
			
			pq.insert(new Node(initial, 0, null));
			hm.put(initial, null);
			while (!pq.isEmpty())
			{
				Node rem = pq.delMin();								//Take of top node
				Board b = rem.board;				
				if (!b.equals(answer))								//Add neighbors of top node to pq
				{
					Iterable<Board> nei = b.neighbors();
					for (Board bo : nei)
					{
						if (rem.prev == null)
						{
							if (!hm.containsKey(bo))
							{
								pq.insert(new Node(bo, moves, rem));
								hm.put(bo, null);
							}
						}
						else if (rem.prev != null && !bo.equals(rem.prev.board))	//Dont add if its the prev board
						{
							if (!hm.containsKey(bo))
							{
								pq.insert(new Node(bo, moves, rem));
								hm.put(bo, null);
							}
						}
					}
				}
				else	//Found final board
				{
					//Follow the chain backwards to get the answer
					ArrayDeque<Board> s = new ArrayDeque<Board>();	//Store it in a stack to reverse it
					Node n = rem;
					while (n != null)								//Traverse through the nodes backwards and add to stack
					{
						moves++;					
						s.push(n.board);					
						n = n.prev;
					}
					while (!s.isEmpty())							//Put the boards into a queue in the right order
					{
						pathQ.enqueue(s.pop());
					}
					moves--;
					break;
				}
			}
		}		
		
	}
	
	public boolean isSolvable()
	{
		return initial.isSolvable();
	}
	
	//Min number of moves to solve the board
	public int moves()
	{
		return moves;
	}
	
	//Sequence of boards in a shortest solution
	public Iterable<Board> solution()
	{
		//System.out.println("pqsize=" + pq.size());
		return pathQ;	//Return pathQ, the queue that contains the solution that I create in Solver()
	}	
	
	//-------------------------------------------------------------------------------------------------------------//
	
	private class Node implements Comparable<Node>
	{
		Board board;
		int movesTo;
		Node prev;
		
		private Node(Board b, int moves, Node p)
		{
			board = b;
			movesTo = moves;
			prev = p;
		}
		
		public int compareTo(Node other)
		{
			return this.board.compareTo(other.board);
		}
		
		
		
	}	
}







