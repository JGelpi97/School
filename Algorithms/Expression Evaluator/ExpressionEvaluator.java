/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 2
*/

import java.io.*;
import java.util.*;
public class ExpressionEvaluator
{
	public static void main(String[] args)
	{
		try
		{
			BufferedReader file = new BufferedReader(new FileReader(args[0]));
			char[] atoms = new char[26];
			boolean[] values = new boolean[26];
			String r;
	
			//Store all the atoms and values
			int atomCount = 0;
			while (atomCount < 26)
			{				
				r = file.readLine();
				atoms[atomCount] = r.charAt(0);
				
				String val = r.substring(2);				
				if (val.compareTo("true") == 0)
				{
					values[atomCount] = true;
				}
				else
				{
					values[atomCount] = false;
				}					
				atomCount++;
			}
			for (int i = 0; i < 26; i++)
			{
				System.out.print(atoms[i] + " = " + values[i] + "\t");
			}
			System.out.println();
			
			String ex;	
			int count = 1;
			char cont = 'y';
			Scanner scanner = new Scanner(System.in);
			//This is the main loop, get one expression, do everything with it
			while ((ex = file.readLine())!= null && cont == 'y')
			{
				Expression expression = new Expression(ex);
				//Set all atoms and values
				for (int i = 0; i < 26; i++)
				{
					expression.setAtom(atoms[i], values[i]);
				}
				if (expression.valid())
				{
					System.out.println("  " + count + ".  " + expression + " = " + expression.evaluate());
					count++;
					System.out.print("\tContinue? [y/n]: ");
					cont = scanner.nextLine().charAt(0);
					if (cont != 'y')
					{
						System.out.println("Program exiting.");
						System.exit(0);
					}
				}
				else				
				{
					System.out.println("\tGetting next expression.");
				}
			}
			System.out.println("EOF reached.\nProgram exiting.");
			System.exit(0);
		}
		catch (FileNotFoundException e) 
		{			
			System.out.println("File not found.\nCorrect usage: java ExpressionEvaluator 'file'");
		}
		catch (IOException e)
		{
			System.out.println("IOException: " + e);
		}
	}
}
