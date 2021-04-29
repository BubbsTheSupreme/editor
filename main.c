#include "editor.h"


int main(int argc, char *argv[]) {
	visualbuf _vbuf;
	filebuf _fbuf;
	visualbuf *vbuf = &_vbuf;
	filebuf *fbuf = &_fbuf;
	char *filename = "test.txt";
	char input; // input buffer
	int count;
	vbuf->curx = 0;
	vbuf->cury = 0;
	
	initscr();
	getmaxyx(stdscr, vbuf->maxy,vbuf->maxx);
	noecho();
	cbreak();
	WINDOW *win; // look into how to implement this

	FILE *file = fopen(filename, "r+");
	if (file == NULL){
		printf("ERROR: file failed to open..\n");
		exit(1);
	}
	
	fbuf->lines = readlines(file, &count);
	fbuf->linecount = count;
	
	move(0,0);
	update_screen(fbuf, win);
	while (1) {
		input = getch();
		process_input(input, vbuf);
		refresh();
	}
	fclose(file);
	endwin();
	return 0;
}
