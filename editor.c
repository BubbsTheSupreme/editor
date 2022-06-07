#include "editor.h"


void update_screen(filebuf *fbuf, visualbuf *vbuf) {
	int i;
	int j;
	int y = fbuf->linecount;

	if (y > vbuf->maxy) y = vbuf->maxy;

	endwin();
	initscr();
	clear();

	for (i = 0 + vbuf->voffset; i - vbuf->voffset < y; i++) {
		if (i > fbuf->linecount) {
			continue;
		}
		else {
			for (j = 0 + vbuf->hoffset; j - vbuf->hoffset < vbuf->maxx; j++) {
			// 	// newlines were removed in the process of storing values so we can replace the \0 with \n
				if (j > fbuf->linesize[i] && vbuf->hoffset > 0) {
					break;
				}
				else if (j > fbuf->linesize[i]) {
					continue;
				}
				else {
					mvwprintw(stdscr, i - vbuf->voffset, j - vbuf->hoffset, "%c", fbuf->lines[i][j]);
				}
			}
		}
	}
}

void process_input(char input, filebuf *fbuf, visualbuf *vbuf, ctrlbuf *cbuf) {
	switch(input) {
		case CTRL_KEY('c'):
			cbuf->ctrlc = 1;
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
					if (vbuf->curx > fbuf->linesize[vbuf->cury + vbuf->voffset] && vbuf->hoffset == 0) {	
						vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset];
					} 
					//dear god where do I start with this if, its 3:05 AM as Im writing this Im tired
					else if (vbuf->curx > fbuf->linesize[vbuf->cury + vbuf->voffset] || // checks if its past the end of the line 
						(vbuf->curx <= fbuf->linesize[vbuf->cury + vbuf->voffset] && (fbuf->linesize[vbuf->cury + vbuf->voffset] - vbuf->hoffset) < vbuf->maxx) 
						// ^^ ensures it moves correctly when aligned with the original line len without moving the cursor too far on longer lines
						&& vbuf->hoffset > 0){ //checks for offset1
						vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset] - vbuf->hoffset;
						if (fbuf->linesize[vbuf->cury + vbuf->voffset] < vbuf->hoffset){
							vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset];
							vbuf->hoffset = 0;
							update_screen(fbuf, vbuf);
						}
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 66: // down
					// handles current y position
					if (vbuf->cury == vbuf->maxy - 1 && (vbuf->maxy + vbuf->voffset) != fbuf->linecount) {
						vbuf->cury == vbuf->maxy - 1;
						if ((vbuf->maxy + vbuf->voffset) < fbuf->linecount) {
							vbuf->voffset++;
							update_screen(fbuf, vbuf);
						}
					}
					else if ((vbuf->maxy + vbuf->voffset) <= fbuf->linecount) vbuf->cury++;
					//handles the current x position
					if (vbuf->curx > fbuf->linesize[vbuf->cury + vbuf->voffset] && vbuf->hoffset == 0) {	
						vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset];
					} 
					//dear god where do I start with this if, its 3:05 AM as Im writing this Im tired
					else if (vbuf->curx > fbuf->linesize[vbuf->cury + vbuf->voffset] || // checks if its past the end of the line 
						(vbuf->curx <= fbuf->linesize[vbuf->cury + vbuf->voffset] && (fbuf->linesize[vbuf->cury + vbuf->voffset] - vbuf->hoffset) < vbuf->maxx) 
						// ^^ ensures it moves correctly when aligned with the original line len without moving the cursor too far on longer lines
						&& vbuf->hoffset > 0){ //checks for offset1
						vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset] - vbuf->hoffset;
						if (fbuf->linesize[vbuf->cury + vbuf->voffset] < vbuf->hoffset){
							vbuf->curx = fbuf->linesize[vbuf->cury + vbuf->voffset];
							vbuf->hoffset = 0;
							update_screen(fbuf, vbuf);
						}
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 67: // right
					if (vbuf->curx == vbuf->maxx - 1) {
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
					}
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

// stores size of each line in an array of ints
int total_tabs(filebuf *fbuf) {
	int i;
	int j;
	int count;
	for (i = 0; i < fbuf->linecount; i++) {
		for (j = 0; fbuf->lines[i][j] != '\0'; j++) {
			if (fbuf->lines[i][j] == '\t') count++;
		}
		fbuf->toffset[i] = count;
		count = 0; 
	}	
}

// stores size of each line in an array of ints
int getlinesize(filebuf *fbuf) {
	int i;
	int j;
	for (i = 0; i < fbuf->linecount; i++) {
		for (j = 0; fbuf->lines[i][j] != '\0'; j++) {}
		fbuf->linesize[i] = j;
	}
}

