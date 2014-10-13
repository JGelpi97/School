/*
*	Joey Gelpi - jbg29
*	CS1501 - Assignment 4
*/

import java.util.*;

public class hw4_jbg29
{
	static EdgeWeightedGraph graph;
	static Graph nwGraph;			//the non weighted graph version

	public static void main(String[] args)
	{
		In in = new In(args[0]);
		graph = new EdgeWeightedGraph(in);	//New edge weighted graph based on file

		in = new In(args[0]);
		nwGraph = new Graph(in, 1);			//New non edge weighted graph based on file

		//Start the main loop
		char input = 'A';
		while (true)
		{
			System.out.println("1. R (eport)");
			System.out.println("2. M (inimum Spanning Tree)");
			System.out.println("3. S (hortest Path from) i j");
			System.out.println("4. D (own edge) i j");
			System.out.println("5. U (p edge) i j");
			System.out.println("6. C (hange weight of edge) i j x");
			System.out.println("7. E (ulerian)");
			System.out.println("8. Q (uit)");

			System.out.print("\nEnter your choice: ");

			Scanner s = new Scanner(System.in);
			input = s.next().charAt(0);

			if (input == '1')
			{
				report();
			}
			else if (input == '2')
			{
				findMST();
			}
			else if (input == '3')	//Shortest path
			{
				System.out.print("Enter from vertex: ");
				int v1 = s.nextInt();
				System.out.print("Enter to vertex: ");
				int v2 = s.nextInt();
				shortest(v1, v2);
			}
			else if (input == '4')	//Down edge
			{
				System.out.print("Enter from vertex: ");
				int v1 = s.nextInt();
				System.out.print("Enter to vertex: ");
				int v2 = s.nextInt();
				downEdge(v1, v2);
			}
			else if (input == '5')	//Up edge
			{
				System.out.print("Enter from vertex: ");
				int v1 = s.nextInt();
				System.out.print("Enter to vertex: ");
				int v2 = s.nextInt();
				System.out.print("Enter weight: ");
				double w = s.nextDouble();
				upEdge(v1, v2, w);
			}
			else if (input == '6')	//Change weight
			{
				System.out.print("Enter from vertex: ");
				int v1 = s.nextInt();
				System.out.print("Enter to vertex: ");
				int v2 = s.nextInt();
				System.out.print("Enter to weight: ");
				double w = s.nextDouble();
				if (w >= 0)
				{
					changeWeight(v1, v2, w);
				}
				else
				{
					downEdge(v1, v2);
				}
			}
			else if (input == '7')	//Eulerian
			{
				eulerian();
			}
			else if (input == '8')
			{
				System.exit(0);
			}
		}
	}

	public static void report()
	{
		System.out.println(graph);
		CC conComp = new CC(nwGraph);
		if (conComp.count() == 1)
		{
			System.out.println("Network is connected");
		}
		else
		{
			System.out.println("Network is not connected");
		}
		System.out.println("Connected components");

        //Number of connected components
        int M = conComp.count();

        //Compute list of vertices in each connected component
        Queue<Integer>[] components = (Queue<Integer>[]) new Queue[M];
        for (int i = 0; i < M; i++)
		{
            components[i] = new Queue<Integer>();
        }
        for (int v = 0; v < nwGraph.V(); v++)
		{
            components[conComp.id(v)].enqueue(v);
        }

        //Print results
		int k = 1;
        for (int i = 0; i < M; i++)
		{
			System.out.print(" " + k + ". ");
            for (int v : components[i])
			{
                StdOut.print(v + " ");
            }
            StdOut.println();
        }
		System.out.println();
	}
	
	public static void shortest(int v1, int v2)
	{
		EdgeWeightedDigraph dg = new EdgeWeightedDigraph(graph);
		DijkstraSP dsp = new DijkstraSP(dg, v1);
		System.out.println("Shortest Path from vertex " + v1 + " to vertex " + v2 + " is:");
		if (dsp.hasPathTo(v2))
		{
			System.out.print("(" + dsp.distTo(v2) + ")" + "  ");
			for (DirectedEdge e : dsp.pathTo(v2))
			{
				System.out.print(e + "  ");
			}
		}
		else
		{
			System.out.println("No Path.");
		}
		System.out.println();

	}

	public static void findMST()
	{
		System.out.println("Minimum Spanning Tree");
		KruskalMST mst = new KruskalMST(graph);
		double total = 0;
		for (Edge e : mst.edges())
		{
            System.out.println(e);
			total += e.weight();
        }
		System.out.println(total + "\n");

	}

	public static void changeWeight(int v1, int v2, double w)
	{
		boolean edgeExists = false;
		Bag<Edge>[] al = graph.adjList();		//Ref to adj list
		if (v1 < al.length && v2 < al.length)	//v1 and v2 have to be valid indeces
		{
			for (Edge e : graph.adj(v1))	//all edges from v1
			{
				if (e.other(v1) == v2)		//if the other end of v1 is v2
				{
					System.out.print("Change edge " + v1 + "->" + v2 + " from ");
					edgeExists = true;
					System.out.print(e.weight() + " to " + w);
					e.setWeight(w);
					break;
				}
			}
			for (Edge e : graph.adj(v2))
			{
				if (e.other(v2) == v1)
				{
					edgeExists = true;
					e.setWeight(w);
					break;
				}
			}
			if (!edgeExists)
			{
				System.out.println("No edge exists between the 2 vertices.");
			}
		}
		else
		{
			System.out.println("1 or both vertices do not exist.");
		}
		System.out.println();
	}

	public static void downEdge(int v1, int v2)
	{
		Bag<Edge>[] al = graph.adjList();		//Ref to adj list
		if (v1 < al.length && v2 < al.length)	//v1 and v2 have to be valid indeces
		{
			for (Edge e : al[v1])					//For all edges from/to v1
			{
				if (e.other(v1) == v2)				//if the other endpoint of v1 is v2, you found the edge you want to remove
				{
					al[v1].remove(e);				//Remove it
					graph.E -= 1;
					nwGraph.E -= 1;
					break;
				}
			}
			
			for (Edge e : al[v2])					//Removed from v1->v2, now remove v2->v1
			{
				if (e.other(v2) == v1)				
				{
					al[v2].remove(e);				
					break;
				}
			}
			
			//Do the same with the non wieghted version
			Bag<Integer>[] al2 = nwGraph.adjList();
			for (int e : al2[v1])
			{
				if (e == v2)				
				{
					al2[v1].remove(e);				
					break;
				}
			}
			for (int e : al2[v2])					
			{
				if (e == v1)				
				{
					al2[v2].remove(e);				
					break;
				}
			}
			System.out.println("Remove edge " + v1 + "->" + v2 + "\n");
		}
		else
		{
			System.out.println("1 or both vertices do not exist.\n");
		}
	}
	
	public static void upEdge(int v1, int v2, double w)
	{		
		Bag<Edge>[] al = graph.adjList();
		if (v1 < al.length && v2 < al.length)	//v1 and v2 have to be valid indeces
		{
			graph.addEdge(new Edge(v1, v2, w));
			nwGraph.addEdge(v1, v2);
			System.out.println("Insert edge " + v1 + "->" + v2 + " (" + w + ")\n");		
		}
		else
		{
			System.out.println("1 or both vertices do not exist.\n");
		}		
	}
	
	public static void eulerian()
	{
		int oddCount = 0;
		Bag<Edge>[] al = graph.adjList();
		for (int i = 0; i < al.length; i++)
		{
			if (al[i].size() % 2 != 0)	//if odd
			{
				oddCount++;
			}
		}
		if (oddCount == 2)
		{
			System.out.println("Graph has a Eulerian path.\n");
		}
		else if (oddCount == 0)
		{
			System.out.println("Graph has a Eulerian tour.\n");
		}
		else
		{
			System.out.println("Graph does not have a Eulerian path or tour.\n");
		}
		Digraph dg = new Digraph(graph);
		DirectedEulerianCycle duc = new DirectedEulerianCycle(dg);
		
		
		if (oddCount == 0 || oddCount == 2)
		{
			System.out.println("Eulerian path: ");
			for (Integer v : duc.tour())
			{
				System.out.print(v + " - ");
			}
			System.out.println();
		}
		
		
	}

}
