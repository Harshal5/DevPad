#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <curses.h>

int check_existance(char *file) {
	FILE *fp;
	fp = fopen(file, "r");
	if(fp != NULL) 
		return 1;
	else
		return 0;
}

int main(int argc, char *argv[]) {
	
	int cols, rows;
	FILE *fp;
	char ch;
	if(argc == 2) {
		if(check_existance(argv[1])) {
			
			fp = fopen(argv[1], "r");
			//printf("open");
			initscr();
			raw();
			noecho();
			getmaxyx(stdscr, rows, cols);
			keypad(stdscr, TRUE);			
			while((ch = fgetc(fp)) != EOF) {
				 printw("%c", ch);			
			}
			ch = getch();
			endwin(); 	
		//fclose(fp);

		}
	
		else {
			initscr();
			raw();
			noecho();
			getmaxyx(stdscr, rows, cols); 				//initscr();
			keypad(stdscr, TRUE);
			printw("%d  %d", rows, cols);
			while((ch = getch()) != (char)KEY_UP) {
				printw("%c",ch);				
				
			}
			//getch();		
			refresh();	
			endwin();			

		}
	}
	else {
		errno = EINVAL;
		perror("usage");
		return errno;
	}	
	return 0;
}
