
#ifndef COLOR_PUTS_H
#define COLOR_PUTS_H

#include <stdio.h>


class ColorPuts {

public:
	enum{RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35, CYAN = 36, WHITE = 37};

	void static puts(char *s, bool bright, int colFG, int colBG) {
		printf("%c[%d;%d;%dm%s%c[%dm", 0x1B, (int)bright, colFG, colBG + 10, s, 0x1B, 0);
	}
	void static putsnobg(char *s, bool bright, int colFG) {
		printf("%c[%d;%dm%s%c[%dm", 0x1B, (int)bright, colFG, s, 0x1B, 0);
	}
};

#endif
