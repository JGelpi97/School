Joey Gelpi - jbg29
CS1501 Assignment 3

*****************DO NOT USE NOVACKY'S CHECKER.JAVA*******************

My Board.java constructor is different than the one in Checker.java.
	I modified his Checker.java and included it as EightChecker.java. It is the 
	exact same as his but requires the command line argument of -h or -m before
	the file name. It behaves in the exact same way as his but chooses the
	priority function. It tells the boards which priority function to use via the
	constructor.

EightPuzzleDriver.java runs one puzzle and prints out the steps with the specified
	output on courseweb.

All 46 puzzles complete with manhattan priority. Some require extra heap space.
With hamming it gets to puzzle 31 before it goes very slow and will eventually 
	run out of heap space.