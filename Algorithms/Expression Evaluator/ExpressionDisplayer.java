/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 2
*/

import java.io.*;
import java.util.*;


public class ExpressionDisplayer
{
	public static void main(String[] args)
	{
		try
		{
			BufferedReader file = new BufferedReader(new FileReader(args[0]));
			String ex;	
			int count = 1;
			char cont = 'y';
			Scanner scanner = new Scanner(System.in);
			//This is the main loop, get one expression, do everything with it
			while ((ex = file.readLine())!= null && cont == 'y')
			{
				Expression expression = new Expression(ex);
				if (expression.valid())
				{
					System.out.print("  " + count + ".  " + expression + "\tContinue [y/n]: ");
					expression.displayTree();
					expression.normalize();
					expression.displayNormalized();
					count++;
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
			System.out.println("EOF reached.");
		}
		catch (FileNotFoundException e) 
		{			
			System.out.println("File not found.\nCorrect usage: java ExpressionDisplayer 'file'");
		}
		catch (IOException e)
		{
			System.out.println("IOException: " + e);
		}

	}
}