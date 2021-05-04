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

	FILE *file = fopen(filename, "r+");
	if (file == NULL){
		printf("ERROR: file failed to open..\n");
		exit(1);
	}
	
	fbuf->lines = readlines(file, &count);
	fbuf->linecount = count;

	update_screen(fbuf);
	move(0,0);
	while (1) {
		input = getch();
		process_input(input, fbuf, vbuf);
		refresh();
	}
	fclose(file);
	endwin();
	return 0;
}
