#include "editor.h"

void write_buf_to_file(int len, char *filename, char *buf);


int getlinesize(char **buf, int line) {
	int i;
	for (i = 0; buf[line][i] != '\0'; i++);
	return i;
}

void update_screen(filebuf *fbuf, WINDOW *win) {
	int i;
	int j;
	int linesize;
	
	for (i = 0; i < fbuf->linecount; i++) {
		linesize = getlinesize(fbuf->lines, i);
		for (j = 0; j < linesize; j++) {
			// if (fbuf->lines[i][j] == '\n') {
			// 	printw("\n", fbuf->lines[i][j]);
			// }
			// if (fbuf->lines[i][j] == '\t') {
			// 	for (int k = 0; k < TABS; k++)
			// 		printw(" ", fbuf->lines[i][j]);
			// }
			// else {
			// 	printw("%s", fbuf->lines[i][j]);
			// }
			// printw("%s", fbuf->lines[i][j]);
			wprintw(win, "%d %d\n", i, j);
		}
		
	}
}

// credit for this function goes to this SO link
// https://stackoverflow.com/questions/22863977/dynamically-allocated-2d-array-of-strings-from-file
char **readlines(FILE *f, int *count) {
	char **array = NULL;
	int i;
	char line[150];
	int linecount;
	int linelen;

	*count = 0;

	linecount = 0;
	while(fgets(line, sizeof(line), f) != NULL) {
		linecount++;
	}

	rewind(f);

	array = malloc(linecount * sizeof(char *));
	if(array == NULL) {
		return NULL; // ERROR
	}

	for(i = 0; i < linecount; i++) {
		fgets(line, sizeof(line), f);

		linelen = strlen(line);
		line[linelen - 1] = '\0';
		linelen--;

		array[i] = malloc(linelen + 1);

		strcpy(array[i], line);
	}

	*count = linecount;

	return array;
}

void process_input(char input, visualbuf *vbuf) {
	switch(input) {
		case CTRL_KEY('c'):
			exit(0);
		case '\033':
			getch();
			switch(getch()){
				case 65: // up
					if(vbuf->cury < 0) {
						vbuf->cury = 0;
					}
					else{
						vbuf->cury--;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 66: // down
					if(vbuf->cury > vbuf->maxy){
						vbuf->cury = vbuf->maxy;
					}
					else {
						vbuf->cury++;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 67: // right
					if(vbuf->curx > vbuf->maxx){
						vbuf->curx = vbuf->maxx;
					}
					else {
						vbuf->curx++;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 68: // left
					if(vbuf->curx < 0) {
						vbuf->curx = 0;
					}
					else{
						vbuf->curx--;
					}
					move(vbuf->cury, vbuf->curx);
					break;
			}
			break;
	}
}

