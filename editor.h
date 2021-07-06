#ifndef EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>

#define EDITOR_H
#define CTRL_KEY(x) ((x) & 0x1f)
#define TABS 4

typedef struct {
	int size;
	int linecount;
	char **lines; // will store all the new lines in and will be what is edited and visualized
}filebuf;

typedef struct {
	int maxx;
	int maxy;
	int curx;
	int cury;
	int ctrlc;
	int linelen;
	int hoffset;
	int voffset;
}visualbuf;

void update_screen(filebuf *fbuf, visualbuf *vbuf);
int getlinesize(char **buf, int line);
char **readlines(FILE *f, int *count);
void process_input(char input, filebuf *fbuf, visualbuf *vbuf);

#endif
