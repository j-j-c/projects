#include <iostream>
#include <string>
#include "AVLTree.h"
#include "parser.h"

/*
My program implements the binary search tree data structure to store nodes according to their unique 8-digit key.
Nodes are associated with a non-unique name.

There are commands for insertion of nodes, removal of nodes by key (or alternatively, inorder traversal position),
searching of nodes by either key or name, inorder traversal printing, preorder traversal printing, postorder traversal printing, 
and printing of current tree height.

Insertions,removals, searches have worst case time complexity of O(log n)
Traversals have worst case time complexity of O(n)

For first input: type in # of lines the program should read before the program exits (if unsure, enter arbitrary high number)

The available commands are:

insert:
inserts a node at the appropriate location within the binary search tree according to its unique 8-digit key. 
nodes are associated with a name.
insert "John Smith" 1827342

remove:
removes a node of the given key
remove 1827342

search:
searches for nodes either by key or by name. If searched by key, returns the associated name. 
If searched by name, returns the key(s) associated with the name. 
search "John Smith" (returns 1827342)
search 1827342 (returns "John Smith")

printInorder:
prints the names from the preorder traversal of the binary search tree

printPreorder
prints the names from the preorder traversal of the binary search tree

printPostorder
prints the names from the postorder traversal of the binary search tree

printLevelCount
prints the # of levels of the binary search tree

removeInorder n:
remove the n-th element of the inorder traversal
(if inorder traversal is "Sara","Jane","John")
removeInorder 3 removes the "John" node
*/

int main()
{
	AVLTree tree;
	int numCommands = 0;

	string input;

	//counts the # of commands to interpret before program terminates.
	getline(cin, input);
	numCommands = stoi(input);
	
	//Creates command objects that hold the input parameters
	//Runs the command objects which calls the respective functions of the AVLTree
	while (numCommands >0)
	{
		numCommands--;
		getline(cin, input);
		Command cmd(input);
		cmd.runCommand(tree);
	}
	return 0;
}
