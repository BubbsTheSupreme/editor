#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>
 
#define CTRL_KEY(x) ((x) & 0x1f)
#define CURSOR "\u2588"
#define TABS 4

struct filebuf {
	int size;
	int linecount;
	char **lines; // will store all the new lines in and will be what is edited and visualized
};

struct visualbuf {
	int maxx;
	int maxy;
	int curx;
	int cury;
	int linelen;
	int tabsinline;
	char **newlines; // \t replaced with spaces
};

void write_buf_to_file(int len, char *filename, char *buf);
void write_file_rows();
void update_screen(struct filebuf *fbuf);
void process_input(char input);
int getsize(FILE *file, char *filename);
char **readlines(FILE *f, int *count); // credit for this function goes to this SO link
// https://stackoverflow.com/questions/22863977/dynamically-allocated-2d-array-of-strings-from-file

// struct filebuf _fbuf;
struct visualbuf _vbuf;
struct visualbuf *vbuf = &_vbuf;

int main(int argc, char *argv[]) {
	char *filename = "test2.txt";
	char input; // input buffer
	int count;
	struct filebuf _fbuf;
	struct filebuf *fbuf = &_fbuf;
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

	// move(0,0);
	// update_screen(fbuf);
	// while (1) {
	// 	input = getch();
	// 	process_input(input);
	// 	update_screen(fbuf);
	//  refresh();
	// }
	fclose(file);
	endwin();
	return 0;
}

void update_screen(struct filebuf *fbuf) {
	for (int i = 0; i < fbuf->linecount; i++) {
		wprintw("%s", fbuf->lines[i]);
	}
}

void process_input(char input) {
	switch(input) {
		case CTRL_KEY('c'):
			exit(0);
		case '\033':
			getch();
			switch(getch()){
				case 65:
					if(vbuf->cury < 0) {
						vbuf->cury = 0;
					}
					else{
						vbuf->cury--;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 66:
					if(vbuf->cury > vbuf->maxy){
						vbuf->cury = vbuf->maxy;
					}
					else {
						vbuf->cury++;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 67:
					if(vbuf->curx > vbuf->maxx){
						vbuf->curx = vbuf->maxx;
					}
					else {
						vbuf->curx++;
					}
					move(vbuf->cury, vbuf->curx);
					break;
				case 68:
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

int getsize(FILE *file, char *filename){ // counts each byte of the file it opens
	int count = 0;
	char c;
	if (file) {
		while(1) {
			c = fgetc(file); // read byte and store it in c
			if (c == EOF){ // if the byte is equal to the end of file byte 
				break; // then exit the loop and continue through the function
			}
			else{
				count++; // increment the count 
			}
		}
	}
	return count; // return count
}

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
