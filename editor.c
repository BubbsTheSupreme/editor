#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define gotoxy(x, y) printf("\033[%d;%df", y, x)
#define update() printf("\033[H\033[J") 
#define CTRL_KEY(x) ((x) & 0x1f)
#define CURSOR "\033[D"
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
void read_file_to_buf(struct filebuf buf);
void update_screen(struct visualbuf buf);
void rawmode(struct termios *ts);
char read_input(char input);

int main(int argc, char *argv[]) {
	struct termios oldts, ts;
	struct visualbuf *vbuf;
	vbuf->curx, vbuf->cury = 0;
	tcgetattr(STDIN_FILENO, &ts);
	oldts = ts;
	rawmode(&ts);
	char input; // input buffer
	while(1) {
		read(STDIN_FILENO, &input, 1);
		if (input == '\033') {
			read(STDIN_FILENO, &input, 1);
			read(STDIN_FILENO, &input, 1);
			switch(input) {
				case 65:
					vbuf->cury--;
					gotoxy(vbuf->curx,vbuf->cury);
					printf(CURSOR);
					break;
				case 66:
					vbuf->cury++;
					gotoxy(vbuf->curx,vbuf->cury);
					printf(CURSOR);
					break;
				case 67:
					vbuf->curx++;
					gotoxy(vbuf->curx,vbuf->cury);
					printf(CURSOR);
					printf("%c", 219);
					update();
					break;
				case 68:
					vbuf->curx--;
					gotoxy(vbuf->curx,vbuf->cury);
					printf(CURSOR);
					printf("%c", 219);
					update();
					break;
			}
		}
		else if (CTRL_KEY('c') == input) {
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldts);
			return 0;
		}
		else{
			// printf("%d \n", input);
		}
	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldts);
	return 0;
}

void rawmode(struct termios *ts) {
	ts->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	ts->c_oflag |= ~OPOST;
	ts->c_lflag &= ~(ECHO | ICANON | ECHONL | ISIG | IEXTEN);
	ts->c_cflag &= ~(CSIZE | PARENB);
	ts->c_cflag |= CS8;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, ts);
}