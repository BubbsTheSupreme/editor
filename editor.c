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
	int len;
	char *data;
};

struct visualbuf {
	int maxx;
	int maxy;
	int curx;
	int cury;
	int linelen;
	int tabsinline;
	char *line; // original lines with tabs
	char *newline; // new line with tabs replaced with spaces
};

void write_buf_to_file(int len, char *filename, char *buf);
void write_file_rows();
void read_file_to_buf(struct filebuf *fbuf, char *filename);
void update_screen();
void process_input(char input);
int getsize(char *filename);


// struct filebuf _fbuf;
struct visualbuf _vbuf;
struct visualbuf *vbuf = &_vbuf;

int main(int argc, char *argv[]) {
	char input; // input buffer
	struct filebuf _fbuf;
	struct filebuf *fbuf = &_fbuf;
	fbuf->len = getsize("test.txt");
	vbuf->curx = 0;
	vbuf->cury = 0;
	initscr();
	getmaxyx(stdscr, vbuf->maxy,vbuf->maxx);
	noecho();
	cbreak();
	read_file_to_buf(fbuf, "test.txt");
	update_screen(fbuf);
	move(0,0);
	while (1) {
		input = getch();
		process_input(input);
		refresh();
	}
	endwin();
	return 0;
}

void read_file_to_buf(struct filebuf *fbuf, char *filename) {
	FILE *file = fopen(filename, "r");
	fbuf->data = malloc(sizeof(char)*fbuf->len);
	fread(fbuf->data, fbuf->len, 1, file);
	fclose(file);
}

void update_screen(struct filebuf *fbuf) {
	printw(fbuf->data);
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

int getsize(char *filename){ // counts each byte of the file it opens
	FILE *file = fopen(filename, "r");
	if (file == NULL){
		printf("ERROR: file failed to open..\n");
		exit(1);
	}
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
	fclose(file);
	return count; // return count
}