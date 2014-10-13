/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 2
*/

import java.util.*;
import java.io.*;

public class Expression
{
	Node root;
	Node rootNorm;
	String expression;
	String expressionNorm;
	String postfixEx;
	static char[] atoms = new char[26];
	static boolean[] values = new boolean[26];
	boolean valid = true;	
	
	//Creates a new expression object and its tree and displays the tree
	public Expression(String s)
	{
		root = null;
		rootNorm = null;
		expression = s;
		
		//remove spaces
		s = s.replaceAll("\\s+","");
		int n = s.length();
		
		ParseError pe = new ParseError();
		valid = pe.isValid(expression, n);
		
		//Check for equal number of parenthesis
		int leftPs = 0;
		int rightPs = 0;		
		for (int i = 0; i < n; i++)
		{
			char x = s.charAt(i);
			if (x == '(')
				leftPs++;
			else if (x == ')')
				rightPs++;				
		}
		if (leftPs != rightPs)
		{
			System.out.println("\tError:\n\t" + expression + " - Missing parenthesis.");
			valid = false;
		}
		
		//If its a good expression, build a tree and eval it
		if (valid)
		{
			root = null;
			
			//To postifix
			Deque<Character> stack = new ArrayDeque<Character>();
			StringBuilder postfix = new StringBuilder();
			for (int i = 0; i < n; i++)
			{
				char x = s.charAt(i);
				if (x >= 65 && x <= 90)
				{
					postfix.append(x);
				}
				else if (x == '(')
				{
					stack.push(x);
				}
				if (x == '!' || x == 'v' || x == '^')
				{
					stack.push(x);
				}
				else if (x == ')')
				{
					while ((!stack.isEmpty()) && ((char)stack.peek() != '('))
					{
						postfix.append(stack.pop());					
					}
					stack.pop();		//left brace
				}
			}

			while (!stack.isEmpty())
			{		
				postfix.append(stack.pop());
			}
			//We are now in postfix
			postfixEx = postfix.toString();
			//System.out.println(postfixEx);
			
			//Create tree
			Deque<Node> treeStack = new ArrayDeque<Node>();
			for (int i = 0; i < postfix.length(); i++)
			{
				char x = postfix.charAt(i);
				if (x >= 65 && x <= 90)							//Push in atoms
				{
					treeStack.push(new Node(x + ""));				
				}
				if (x == '!')									//Push in a new '!' node with an atom as child
				{
					Node temp = treeStack.pop();
					Node nw = new Node("!", null, temp);
					temp.parent = nw;
					treeStack.push(nw);
				}
				if (x == '^' || x == 'v')						//Push in a new '^' or 'v' node with 2 atoms as children
				{
					Node right = treeStack.pop();
					Node left = treeStack.pop();
					Node nw = new Node(x + "", left, right);
					right.parent = nw;
					left.parent = nw;
					treeStack.push(nw);
				}
			}			
			root = treeStack.pop();			
		}
	}
	
	//Is the expression valid
	public boolean valid()
	{
		return valid;
	}
	
	//Sets the value of an atom.
	public static void setAtom(char atom, boolean value) 
	{
		atoms[atom - 65] = atom;
		values[atom - 65] = value;
	}
	
	//Evaluates this expression.
	public boolean evaluate() 
	{		
		Expression treeToEval = this.copy();								//Create a copy of the tree because i destroy it to evaluate
		Node newRoot = treeToEval.root;
		while (newRoot != null)
		{						
			Node deepNode = deepestNode(newRoot);							//Get current deepest node becaues that is where we want to start
			if (deepNode.parent.symbol.charAt(0) == '!')					//If deepests parent is '!'
			{
				int index = deepNode.symbol.charAt(0) - 65;					//Get atom index
				treeToEval.values[index] = !treeToEval.values[index];		//Switch atom value
				if (deepNode.parent == newRoot)								//If its the last node return the value
				{
					return treeToEval.values[index];
				}
				deepNode.parent.symbol = deepNode.symbol;					//make the '!' the atom
				deepNode.parent.right = null;								//delete the node with the atom
			}
			else if (deepNode.parent.symbol.charAt(0) == '^')
			{
				int indexLChild = deepNode.parent.left.symbol.charAt(0) - 65;	//Index of each child for value of the atom
				int indexRChild = deepNode.parent.right.symbol.charAt(0) - 65;
				treeToEval.values[indexLChild] = (treeToEval.values[indexLChild] && treeToEval.values[indexRChild]);	//Store new value in left child arom, so keep using that one
				if (deepNode.parent == newRoot)								//If its last node
				{											
					return treeToEval.values[indexLChild];
				}
				Node parent = deepNode.parent;								
				parent.symbol = parent.left.symbol;							//Set '^' node to the atom that is the left child
				parent.left = null;											//Delete child nodes
				parent.right = null;				
			}
			else if (deepNode.parent.symbol.charAt(0) == 'v')
			{				
				int indexLChild = deepNode.parent.left.symbol.charAt(0) - 65;
				int indexRChild = deepNode.parent.right.symbol.charAt(0) - 65;
				treeToEval.values[indexLChild] = (treeToEval.values[indexLChild] || treeToEval.values[indexRChild]);		//Store value in left child atom, so keep using that one
				if (deepNode.parent == newRoot)
				{
				
					return treeToEval.values[indexLChild];
				}
				Node parent = deepNode.parent;
				parent.symbol = parent.left.symbol;		//set 'v' node to the atom that is the left child
				parent.left = null;
				parent.right = null;
			}
			/*
			TreeDisplay display = new TreeDisplay(r.toString());
			display.setRoot(newRoot);			
			try {
			Thread.sleep(4000);
			} catch(InterruptedException e) {
			} 
			*/
		}		
		return false;		
	}
	
	//Makes a deep copy of this expression.
	public Expression copy() 
	{
		Expression ex = new Expression(expression);
		for (int i = 0; i < 26; i++)
		{
			ex.atoms[i] = atoms[i];
			ex.values[i] = values[i];
		}
		return ex;
	}
	//Converts this expression's tree to normal form.
	public void normalize() 
	{
		String expressionNormPost = new String(postfixEx);
		
		
		//double negs
		if (expressionNormPost.length() > 2)
		{
			for (int i = 0; i < expressionNormPost.length() - 2;)
			{
				i++;
				if (expressionNormPost.charAt(i) == '!' && expressionNormPost.charAt(i + 1) == '!')
				{
					expressionNormPost = expressionNormPost.substring(0, i) + expressionNormPost.substring(i + 2);					
					i = 0;					
				}
			}
		}
		
		//Create tree
		Deque<Node> treeStack = new ArrayDeque<Node>();
		for (int i = 0; i < expressionNormPost.length(); i++)
		{
			char x = expressionNormPost.charAt(i);
			if (x >= 65 && x <= 90)							//Push in atoms
			{
				treeStack.push(new Node(x + ""));				
			}
			if (x == '!')									//Push in a new '!' node with an atom as child
			{
				Node temp = treeStack.pop();
				Node nw = new Node("!", null, temp);
				temp.parent = nw;
				treeStack.push(nw);
			}
			if (x == '^' || x == 'v')						//Push in a new '^' or 'v' node with 2 atoms as children
			{
				Node right = treeStack.pop();
				Node left = treeStack.pop();
				Node nw = new Node(x + "", left, right);
				right.parent = nw;
				left.parent = nw;
				treeStack.push(nw);
			}
		}			
		rootNorm = treeStack.pop();	
	}
	
	//Displays the normalized tree.
	public void displayNormalized()
	{
		TreeDisplay display = new TreeDisplay("Normalized: " + expression);
		display.setRoot(rootNorm);
	}
	
	//Displays regular tree
	public void displayTree()
	{
		TreeDisplay display = new TreeDisplay(expression);
		display.setRoot(root);
	}
	
	//Gets the height of a tree starting at some node
	private int treeHeight(Node n)
	{
		if (n == null)
		{
			return 0;
		}
		return 1 + Math.max(treeHeight(n.left), treeHeight(n.right));
	}
	
	//Get the deepest node in the tree with the left one taking precedence
	private Node deepestNode(Node n)
	{
		if (n == null)
		{
			//System.out.println("node was null");
			return null;
		}
		if (n.left == null && n.right == null)
		{
			//System.out.println("both children null");
			return n;
		}
		if (n.left == null)
		{
			//System.out.println("n.left = null");
			return deepestNode(n.right);
		}
		//otherwise has 2 children
		int lHeight = treeHeight(n.left);
		int rHeight = treeHeight(n.right);
		if (lHeight == rHeight)
		{
			//System.out.println("heights are same");
			return deepestNode(n.left);
		}
		else if (lHeight > rHeight)
		{
			//System.out.println("lh > rh");
			return deepestNode(n.left);
		}
		else if (rHeight > lHeight)
		{
			//System.out.println("rh > lh");
			return deepestNode(n.right);
		}		
		//System.out.println("Returning null");
		return null;
	}
	
	//returns the print form of an expression.
	public String toString() 
	{
		return expression;
	}
}
