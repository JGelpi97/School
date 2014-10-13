/*
 Joey Gelpi - jbg29
 CS1501 - Assignment 5
*/

import java.util.*;

public class BurrowsWheeler 
{
	// apply Burrows-Wheeler encoding, reading from standard input and writing to standard output - 20 points
	public static void encode()
	{
		CircularSuffixArray csa = new CircularSuffixArray(StdIn.readAll());
		
		//Easier to get the encoding in the CSA because it contains the Suffix class.
		//Writes binary of encoding to Binary Standard Out.
		csa.writeEncoding();
	}

	// apply Burrows-Wheeler decoding, reading from standard input and writing to standard output - 20 points
	public static void decode()
	{
		int originalIndex = BinaryStdIn.readInt();					//Get the index of the original string to start with
		String code = "";
		
		while (!BinaryStdIn.isEmpty())								//Create string
		{
			code = code.concat(BinaryStdIn.readChar() + "");			
		}
		
		ArrayList<Character> end = new ArrayList<Character>();		//The last chars in the CSA
		ArrayList<Character> start = new ArrayList<Character>();	//The first chars in the CSA
		
		for (int i = 0; i < code.length(); i++)						//Add the chars to each ArrayList
		{
			end.add(code.charAt(i));
			start.add(code.charAt(i));
		}
		Collections.sort(start);									//Sort the start list
		
		boolean used[] = new boolean[code.length()];				//If the current index was already used		
		for (int i = 0; i < code.length(); i++)
		{
			used[i] = false;
		}
		
		int[] next = new int[code.length()];						//Next array
		
		for (int i = 0; i < code.length(); i++)						//Go through each char in start
		{
			for (int j = 0; j < code.length(); j++)
			{
				if (start.get(i) == end.get(j) && !used[j])			//If matching chars on start and end, and the index wasnt already used
				{
					next[i] = j;
					used[j] = true;		//Dont use an index you already used
					break;
				}
			}
		}
		//Next array is completeed
		
		int index = originalIndex;
		//Create string using next array
		for (int i = 0; i < next.length; i++)
		{
			index = next[index];
			System.out.print(code.charAt(index));
		}
		
		//System.out.println();
	}

	// if args[0] is '-', apply Burrows-Wheeler encoding   - 5 points
	// if args[0] is '+', apply Burrows-Wheeler decoding   - 5 points
	public static void main(String[] args)
	{	
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
} 