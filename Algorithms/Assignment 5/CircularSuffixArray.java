/*
 Joey Gelpi - jbg29
 CS1501 - Assignment 5
*/

import java.util.*;

public class CircularSuffixArray 
{
	private ArrayList<Suffix> sorted;
	int len;
	int originalIndex;

	// circular suffix array of s - 10 points
	public CircularSuffixArray(String s)
	{
		len = s.length();
		sorted = new ArrayList<Suffix>();
		sorted.add(new Suffix(s, 0));			//Add first which is just the original string
		
		for (int i = 1; i < s.length(); i++)	//Add the rest of the strings and their original index
		{
			String x = sorted.get(i - 1).text.substring(1, s.length());
			x = x.concat(sorted.get(i - 1).text.substring(0, 1));
			sorted.add(new Suffix(new String(x), i));			
		}
		
		Collections.sort(sorted);				//Sort the suffixes
		
		/*
		for (int i = 0; i < sorted.size(); i++)
		{
			System.out.println(sorted.get(i).text + " - " + sorted.get(i).index);
		}
		*/		
		
		//Find the original string's new index
		for (int i = 0; i < sorted.size(); i++)
		{
			if (sorted.get(i).index == 0)
			{
				originalIndex = i;
				break;
			}
		}
	}
	
	//Outputs the Burrows Wheeler encoding
	//Easy to do in here because we have the Suffix class
	//Prints straight to Binary Standard Out
	public void writeEncoding()
	{
		BinaryStdOut.write(originalIndex);
		for (int i = 0; i < sorted.size(); i++)
		{
			String y = sorted.get(i).text;
			char z = (y.substring(y.length() - 1, y.length())).charAt(0);
			BinaryStdOut.write(z);
		}	
		BinaryStdOut.flush();
	}
	
	// length of s
	public int length()
	{
		return len;
	}
	
	// returns index of ith sorted suffix - 10 points
	//No idea what this is actually for, but it does what it should
	public int index(int i)
	{
		return sorted.get(i).index;
	}
	
	//A simple class with a String and index to where it was in the original, unsorted array
   private class Suffix implements Comparable<Suffix>
   {
        public String text;
        public int index;

        private Suffix(String text, int index) 
		{
            this.text  = text;
            this.index = index;
        }

        public int compareTo(Suffix that) 
		{
			return text.compareTo(that.text);
        }

        public String toString() 
		{
            return text;
        }
    }
	
	public static void main(String a[])
	{
		CircularSuffixArray s = new CircularSuffixArray("ABRACADABRA!");
	}
} 