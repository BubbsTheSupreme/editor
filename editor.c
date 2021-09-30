#include "editor.h"


void update_screen(filebuf *fbuf, visualbuf *vbuf) {
	int i;
	int j;

	endwin();
	initscr();
	clear();

	for (i = 0 + vbuf->voffset; i - vbuf->voffset < vbuf->maxy; i++) {
		if (i > fbuf->linecount) {
			continue;
		}
		else {
			for (j = 0 + vbuf->hoffset; j - vbuf->hoffset < vbuf->maxx; j++) {
				// newlines were removed in the process of storing values so we can replace the \0 with \n
				if (j > fbuf->linesize[i] && vbuf->hoffset > 0) {
					break;
				}
				else if (j > fbuf->linesize[i]) {
					continue;
				}
				else {
					if (fbuf->lines[i][j] == '\0') {
						printw("\n");
					}
					else {
						mvwprintw(stdscr, i - vbuf->voffset, j - vbuf->hoffset, "%c", fbuf->lines[i][j]);
					}
				}
			}
		}
	}
}

void process_input(char input, filebuf *fbuf, visualbuf *vbuf) {
	switch(input) {
		case CTRL_KEY('c'):
			vbuf->ctrlc = 1;
			break;
		case '\033':
			getch();
			switch(getch()) {
				case 65: // up
					if(vbuf->cury == 0) {
						vbuf->cury = 0;
						if (vbuf->voffset > 0){
							vbuf->voffset--;
							update_screen(fbuf, vbuf);
						}
					}
					else {
						vbuf->cury--;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 66: // down
					if(vbuf->cury == vbuf->maxy - 1 && (vbuf->maxy + vbuf->voffset) != fbuf->linecount) {
						vbuf->cury = vbuf->maxy - 1;
						if ((vbuf->maxy + vbuf->voffset) < fbuf->linecount) {
							vbuf->voffset++;
							update_screen(fbuf, vbuf);
						}
					}
					else if ((vbuf->maxy + vbuf->voffset) <= fbuf->linecount) {
						vbuf->cury++;
					}
					if (vbuf->maxx < fbuf->linesize[vbuf->cury + vbuf->voffset]) {
						vbuf->hoffset = fbuf->linesize[vbuf->cury + vbuf->voffset] - vbuf->maxx;
						vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset] - vbuf->hoffset;
						update_screen(fbuf, vbuf);
					}

					move(vbuf->cury, vbuf->curx);
					break;
				case 67: // right
					if(vbuf->curx == vbuf->maxx - 1) {
						vbuf->curx = vbuf->maxx - 1;
						if (fbuf->lines[vbuf->cury + vbuf->voffset][vbuf->curx + vbuf->hoffset] != '\0') {
							vbuf->hoffset++;
						}
						update_screen(fbuf, vbuf);
					}
					else {
						if (fbuf->lines[vbuf->cury + vbuf->voffset][vbuf->curx + vbuf->hoffset] != '\0') {
							vbuf->curx++;
						}
					} // was working on the bug that lets the cursor move past the end of the line
					move(vbuf->cury, vbuf->curx);
					break;
				case 68: // left
					if(vbuf->curx <= 0 && vbuf->cury == 0) {
						vbuf->curx = 0;
					}
					else if (vbuf->curx == 0 && vbuf->hoffset > 0) {
						vbuf->hoffset--;
						update_screen(fbuf, vbuf);
					}
					else{
						if (vbuf->curx == 0) vbuf->curx = 0;
						else vbuf->curx--;
					}
					move(vbuf->cury, vbuf->curx);
					break;
			}
			break;
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

// puts size of each line in an array of ints
int getlinesize(filebuf *fbuf) {
	int i;
	int j;
	for (i = 0; i < fbuf->linecount; i++) {
		for (j = 0; fbuf->lines[i][j] != '\0'; j++) {}
		fbuf->linesize[i] = j;
	}
}
