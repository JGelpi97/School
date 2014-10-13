//Joey Gelpi - jbg29
//CS1501 - Assignment 1

import java.io.*;
import java.util.*;
import java.lang.Math;

public class Assignment1
{
	public static void main(String args[])
	{
		Stopwatch watch = new Stopwatch();
		
		int lastChar = 0;
		int numLines1 = 0, numWords1 = 0, numDisWords1 = 0;
		HashMap<String, Integer> words1 = new HashMap<String, Integer>();
		int numLines2 = 0, numWords2 = 0, numDisWords2 = 0;
		HashMap<String, Integer> words2 = new HashMap<String, Integer>();
		StringBuilder word = new StringBuilder();	//faaster than using a regular string
		int _char;		//the character we read in as an int for comparing to see if its alphanumeric
		
		//Program run wrong
		if (args.length < 2)
		{
			System.out.println("Usage: java Program filename_1 filename_2\nProgram Exiting.");
			System.exit(0);
		}
		
		try
		{		

				

			while ((_char = file1.read()) != -1)						//whule not eof
			{
				lastChar = _char;
				if ((_char >=97 && _char <=122) || (_char >=65 && _char <= 90) || (_char >= 48 && _char <= 57))	//it is alphanumeric
				{
					word.append((char)_char);							//build the word
				}
				else		//not alphanumeric
				{
					String s = new String(word);						//convert to a string
					s = s.toLowerCase();					
					if (_char == 10)										//end of a line
					{
						numLines1++;			
					}
					if (s.length() > 0)										//if its an actual word
					{
						numWords1++;
						if (words1.containsKey(s))						//it contains the word i just got
						{							
							int freq = words1.get(s) + 1;				//add one to the frequency of it
							words1.put(s, freq);							//insert it
						}
						else
						{
							words1.put(s, 1); 								//add the word if it wasnt already in there
						}
						
					}
					word.delete(0, word.length());					//'reset' the StringBuilder
				}
			
			}
			//if the last character wasnt a newline, add one to line count
			if (lastChar != 13 && lastChar != 10)
			{
				numLines1++;
			}
			file1.close();
			numDisWords1 = words1.size();
			System.out.println("File " + args[0] + ": " + numLines1 + " lines, " + numWords1 + " words, " + numDisWords1 + " distinct words");		
		}
		catch (Exception e)
		{
			System.out.println(args[0] + " is not found!");
			numDisWords1 = words1.size();
			System.out.println("File " + args[0] + ": " + numLines1 + " lines, " + numWords1 + " words, " + numDisWords1 + " distinct words");		
		}
		

		/**Same thing for file 2 **/
		try
		{	
			BufferedReader file2 = new BufferedReader(new FileReader(args[1]));			
			word.delete(0, word.length());
			while ((_char = file2.read()) != -1)
			{
				lastChar = _char;
				if ((_char >=97 && _char <=122) || (_char >=65 && _char <= 90) || (_char >= 48 && _char <= 57))	//it is alphanumeric
				{
					word.append((char)_char);
				}
				else		//not alphanumeric
				{
					String s = new String(word);
					s = s.toLowerCase();					
					if (_char == 10)
					{
						numLines2++;
					}
					if (s.length() > 0)										//if its an actual word
					{
						//System.out.println(s);
						numWords2++;
						if (words2.containsKey(s))						//it contains the word i just got
						{							
							int freq = words2.get(s) + 1;				//add one to the frequency of it
							words2.put(s, freq);
						}
						else
						{
							words2.put(s, 1); 
						}
						
					}
					word.delete(0, word.length());
				}
			
			}
			//if the last character wasnt a newline, add one to line count
			if (lastChar != 13 && lastChar != 10)
			{
				numLines2++;
			}
			file2.close();
			numDisWords2 = words2.size();
			System.out.println("File " + args[1] + ": " + numLines2 + " lines, " + numWords2 + " words, " + numDisWords2 + " distinct words");
		}
		catch (Exception e)
		{
			System.out.println(args[1] + " is not found!");
			numDisWords2 = words2.size();
			System.out.println("File " + args[1] + ": " + numLines2 + " lines, " + numWords2 + " words, " + numDisWords2 + " distinct words");
		}
		
		
		//Compute angle
		double norm1 = 0;
		for (Map.Entry<String, Integer> entry : words1.entrySet())
		{
			norm1 = norm1 + (entry.getValue() * entry.getValue());
		}
		
		double norm2 = 0;
		for (Map.Entry<String, Integer> entry : words2.entrySet())
		{
			norm2 = norm2 + (entry.getValue() * entry.getValue());
		}
		
		norm1 = Math.sqrt(norm1);
		norm2 = Math.sqrt(norm2);
		
		double dist = 0;
		if (norm1 != norm2)												//if theyre equal no need to calculate angle because the angle is 0
		{
			double inPro = 0;
			if (words1.size() < words2.size())						//use the smaller list so you have less to go through
			{
				for (Map.Entry<String, Integer> entry : words1.entrySet())
				{
					if (words2.containsKey(entry.getKey()))		//if 2nd file has the first ones word
					{
						inPro = inPro + (entry.getValue() * words2.get(entry.getKey()));
					}
				}
			}
			else
			{
				for (Map.Entry<String, Integer> entry : words2.entrySet())
				{
					if (words1.containsKey(entry.getKey()))		//if 1st file has the first ones word
					{
						inPro = inPro + (entry.getValue() * words1.get(entry.getKey()));
					}
				}
			}
			dist = Math.acos(inPro / (norm1 * norm2));			//compute distance
		}
		
		System.out.println("The distance between the documents is: " + dist + " radians");		
		System.out.println("Time elapsed: " + watch.elapsedTime() + " seconds\n");
	}
}