
Joey Gelpi - jbg29
CS1501 - Assignment 2

The Expression class is passed an expression string. It first gets passed to a ParseError which determines if it is valid.
 If it is valid the expression is converted to postfix using Deques to easily create a tree from it. A binary tree is then
 created from it using another Deque.
Evaluate first creates a copy of the tree. It then finds the deepest node in the copyed tree, which would be the furthest atom 
 into the parenthesis of the actual expression string. It then evaluates that part of the expression based on its parent 
 which is either a ^, v, or !. And deletes nodes depending on the operator. So I am effectivly deleting the tree as I go along.
 The loop stops going when the final parent of an atom is the root of the tree, meaning that the rest of the nodes have been 
 deleted and everything else has been evaluated.
The atoms and values for each expression are just 2 parallel arrays, and need to be sent into the Expression by the main program.

Normalizing does not fully work, only removing double negatives.