/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 2
*/

public class ParseError	
{
	public boolean isValid(String expression, int n)
	{
		boolean valid = true;
		String s = new String(expression.replaceAll("\\s+",""));
		for (int i = 0; i < n; i++)
		{
			char x = s.charAt(i);
			if (((x < 65) || (x > 90)) && (x != '(') && (x != ')') && (x != 'v') && (x != '^') && (x != '!'))	//if some random character
			{
				System.out.println("\tError:\n\t" + expression + " - Illegal character " + x);
				valid = false;
			}
			if (x == 'v' || x == '^' || x == '!') 							//if an op
			{					
				if (((i + 1) < n) && (s.charAt(i + 1)  ==  ')'))			//if the next char is a ')' after an op thats no good
				{
					System.out.println("\tError:\n\t" + expression + " - Operator(v, ^, !) cannot operate on ')'.");
					valid = false;
				}
				if (x == '!' && (s.charAt(i -1) != '('))
				{
					System.out.println("\tError:\n\t" + expression + " - Missing parenthesis.");
					valid = false;
				}
			}
			if (x >= 65 && x <= 90 && ((i + 1) < n))						//Atom followed by '('
			{
				if (s.charAt(i + 1) == '(')
				{
					System.out.println("\tError:\n\t" + expression + " - Illegal Operator.");
					valid = false;
				}
			}
		}
		return valid;
	}
}