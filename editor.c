#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
 
#define CTRL_KEY(x) ((x) & 0x1f)
#define CURSOR "\u2588"
#define TABS 4

struct filebuf {
	int len;
	char *data;
};

struct visualbuf {
	int curx;
	int cury;
	int linelen;
	int tabsinline;
	char *line; // original lines with tabs
	char *newline; // new line with tabs replaced with spaces
};

void write_buf_to_file(struct filebuf buf);
void write_file_rows(struct filebuf buf);
void read_file_to_buf(struct filebuf buf);
void update_screen(struct visualbuf buf);
char read_input(char input);

int main(int argc, char *argv[]) {
	struct visualbuf _vbuf;
	struct visualbuf *vbuf = &_vbuf;
	vbuf->curx = 0;
	vbuf->cury = 0;
	int row, col;
	char input; // input buffer
	initscr();
	noecho();
	getmaxyx(stdscr, row, col);
	cbreak();
	move(0,0);
	while (1) {
		input = getch();
		switch(input) {
			case CTRL_KEY('c'):
				exit(0);
			case '\033':
				getch();
				switch(getch()){
					case 65:
						vbuf->cury--;
						move(vbuf->cury, vbuf->curx);
						break;
					case 66:
						vbuf->cury++;
						move(vbuf->cury, vbuf->curx);
						break;
					case 67:
						vbuf->curx++;
						move(vbuf->cury, vbuf->curx);
						break;
					case 68:
						vbuf->curx--;
						move(vbuf->cury, vbuf->curx);
						break;
				}
				break;
		}
		refresh();
	}
	endwin();
	return 0;
}



// https://en.cppreference.com/w/c/io/setvbuf