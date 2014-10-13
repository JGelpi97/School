/*
	Joey Gelpi - jbg29
	CS1501 - Assignment 2
*/

public class Node
{	
	public String symbol;
	public Node left = null;
	public Node right = null;
	public Node parent = null;
	boolean used = false;	
	
	public Node(String dataToEnter)
	{
		symbol = dataToEnter;
	}
	public Node(String dataToEnter, Node leftNode, Node rightNode)
	{
		symbol = dataToEnter;
		left = leftNode;
		right = rightNode;
	}
}