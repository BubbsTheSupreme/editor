#include "editor.h"

int main(int argc, char *argv[]) {
	visualbuf _vbuf;
	filebuf _fbuf;
	ctrlbuf _cbuf;
	visualbuf *vbuf = &_vbuf;
	filebuf *fbuf = &_fbuf;
	ctrlbuf *cbuf = &_cbuf;
	char *filename = argv[1];
	char input; // input buffer
	int count;
	cbuf->ctrlc = 0;
	vbuf->curx = 0;
	vbuf->cury = 0;
	vbuf->hoffset = 0;
	vbuf->voffset = 0;

	initscr();
	raw();
	getmaxyx(stdscr, vbuf->maxy, vbuf->maxx);
	noecho();

	FILE *file = fopen(filename, "r+");
	if (file == NULL) {
		printf("Failed to open file..\n");
		endwin();
		return 1;
	}
	
	fbuf->lines = readlines(file, &count);
	fbuf->linecount = count;
	fbuf->linesize = (int*)malloc(count * sizeof(int));
	fbuf->toffset = (int*)malloc(count * sizeof(int));

	getlinesize(fbuf);
	update_screen(fbuf, vbuf);
	move(0,0);
	while (1) {
		input = getch();
		process_input(input, fbuf, vbuf, cbuf);
		refresh();
		if (cbuf->ctrlc == 1) break;
	}
	endwin();
	fclose(file);
	return 0;
}
