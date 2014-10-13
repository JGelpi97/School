/*
 Joey Gelpi - jbg29
 CS1501 - Assignment 5
 
 Uses Nodes for quick moving, instead of an array where we would have to shift
*/

public class MoveToFront 
{
	static Node head;
	
	// apply move-to-front encoding, reading from standard input and writing to standard output - 20 points
	public static void encode()
	{
		String s = StdIn.readAll();
		char[] input = s.toCharArray();		
				
		//For each char in input
		for (int i = 0; i < input.length; i++)
		{
			Node prev = null;
			Node cur = head;
			int ascii = (int)input[i];	//The ascii value of the character from input
			char index = 0;				//Which number node we are at
			while (cur != null)			//Go through each node
			{
				if (cur.val == ascii)	//Found it, MTF
				{
					if (cur == head)
					{
						BinaryStdOut.write(index);
					}
					else if (cur.next == null)	//Last node
					{
						prev.next = null;
						cur.next = head;
						head = cur;
						BinaryStdOut.write(index);
					}
					else						//Some node between
					{
						prev.next = cur.next;
						cur.next = head;
						head = cur;
						BinaryStdOut.write(index);
					}
					break;
				}
				prev = cur;
				cur = cur.next;
				index++;
			}
		}
		BinaryStdOut.flush();					//Out binary
	}

	// apply move-to-front decoding, reading from standard input and writing to standard output - 20 points
	public static void decode()
	{			
		while (!BinaryStdIn.isEmpty()) 
		{
			char indexOfChar = BinaryStdIn.readChar();	//Place where we are getting the character to print (the index)
			int index = 0;								//The number of the node we are at
			Node prev = null;
			Node cur = head;
			
			while (cur != null)							//Go through all nodes
			{
				if (index == (int)indexOfChar)			//If in proper position
				{
					System.out.print((char)cur.val);	//Print out the char
					
					//MTF
					if (cur != head)
					{
						if (cur.next == null)	//Last node
						{
							prev.next = null;
							cur.next = head;
							head = cur;
						}
						else					//Some node between
						{
							prev.next = cur.next;
							cur.next = head;
							head = cur;
						}										
					}
					break;								//Get out of searching for correct node loop					
				}
				
				prev = cur;
				cur = cur.next;
				index++;
			}
		}
		//System.out.println();
	}

	// if args[0] is '-', apply move-to-front encoding - 5 points
	// if args[0] is '+', apply move-to-front decoding - 5 points
	public static void main(String[] args)
	{
		//Create 256 nodes with the numbers 0 - 255 as their values
		//This is the alphabet, faster to MTF than an array
		head = new Node(0, null);
		Node cur = head;
		for (int i = 1; i < 256; i++)
		{
			cur.next = new Node(i, null);
			cur = cur.next;
		}
		
		if (args[0].equals("-"))
		{
			encode();
		}
		else if (args[0].equals("+"))
		{
			decode();
		}
		else
		{
			System.out.println("Need '-' or '+' as first argument. Program exiting.");
			System.exit(0);
		}
	}
	
	private static class Node
	{
		int val;
		Node next;
		
		public Node(int x, Node n)
		{
			val = x;
			next = n;
		}
	}
} 