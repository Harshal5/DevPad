Project Title: Text Editor
Name: Harshal Navneet Patil
MIS : 111803160

Designing a Text Editor using the ncurses library

How to install ncurses:
-> sudo apt-get install libncurses5-dev libncursesw5-dev
 
Work done on Project:
1) Designing of data structure for text editor
	The Data Structure used is a 2-Dimensional Doubly Linked List.
	It contains a vertical list whose each node points to horizontal list.
	Each new line in the editor corresponds to a new vertical node.
	And each letter in the editor corresponds to new horizontal node. 
	
2) Basic input output functions of Ncurses
	All arrow keys are operable and special characters/buttons are also implemented.
	
3) Screen manupulation using Ncurses
	move(y, x) function is used for screen manupulation.
	
4) Prompt Designing
	Inbuilt functions of ncurses are used
	
5) Implemented following Functions in text editor
		a) Read, write and save in existing file
		b) Create, write and save in new file given by user
		c) Insertion of charcters like new_line and showing them on screen
		d) Cursur movement like VI
		e) Some basic functions along with their animation on screen like
			i)   To Save
			ii)  To Quit
			iii) To Copy
			iv)  To Cut
			V)  To Paste
