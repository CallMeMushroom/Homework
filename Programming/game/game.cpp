#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <string>

#define FONTSIZE			36						// FONTSIZE *MUST* be an even integer!
#define WIDTH				800
#define HEIGHT				600
#define SEAM				3

#define GRAY				0x7F7F7F
#define OFFWHITE			0xD6EEF7
#define CRIMSON				0x462FBF
#define NAVAJO				0xADDEFF
#define MOCCASIN			0xB5E4FF
#define BRULYWOOD			0x87B8DE

#define BKGCOLOR			MOCCASIN
#define CHESSCOLOR			BRULYWOOD
#define REDCOLOR			CRIMSON
#define BALCKCOLOR			BLACK
#define GRIDCOLOR			WHITE
#define TEXTCOLOR			WHITE

inline void bettersolidcircle(int x, int y, int radius)
// a better version of solidcircle()
{
	LINESTYLE __linestyle;
	getlinestyle(&__linestyle);
	COLORREF __linecolor = getlinecolor();			// copy origin line style and color

	setlinestyle(PS_SOLID, 1);
	setlinecolor(getfillcolor());

	circle(x, y, radius);
	floodfill(x, y, getfillcolor());

	setlinestyle(&__linestyle);
	setlinecolor(__linecolor);						// reset to orgin
}

void drawpieceat(int x, int y, COLORREF color, const wchar_t* str, LOGFONT font = chessfont)
// draw a chess piece at
{
	int r = (int)(1.414 * FONTSIZE / 2);
	assert(x > r && y > r && x + r < WIDTH&& y + r < HEIGHT);

	COLORREF __fillcolor = getfillcolor();			// copy origin fill color
	LOGFONT __textstyle;
	gettextstyle(&__textstyle);
	COLORREF __textcolor = gettextcolor();			// copy origin text style & color
	LINESTYLE __linestyle;
	getlinestyle(&__linestyle);
	COLORREF __linecolor = getlinecolor();			// copy origin line style & color

	setfillcolor(CHESSCOLOR);
	settextstyle(&font);
	settextcolor(color);
	setlinestyle(PS_SOLID, 2);
	setlinecolor(color);

	bettersolidcircle(x, y, r + 2);
	outtextxy(x - FONTSIZE / 2, y - FONTSIZE / 2, str);
	circle(x, y, r - 1);

	setfillcolor(__fillcolor);
	settextstyle(&__textstyle);
	settextcolor(__textcolor);
	setlinestyle(&__linestyle);
	setlinecolor(__linecolor);						// reset to origin
}

void render() {
}

LOGFONT chessfont;
void init() {
	gettextstyle(&chessfont);
	chessfont.lfHeight = FONTSIZE;
	_tcscpy_s(chessfont.lfFaceName, _T("华康简魏碑"));
	chessfont.lfQuality = ANTIALIASED_QUALITY;
	setbkmode(TRANSPARENT);
}

std::string ChessBoard = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";
int main()
{
	initgraph(WIDTH, HEIGHT);
	setbkcolor(BKGCOLOR);
	cleardevice();
	init();
	render();

	MOUSEMSG msg;
	while (true) { // for each "frame"
		if (MouseHit()) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) {
			}
			else continue;
		}
	}

	drawpieceat(WIDTH / 2, HEIGHT / 2, REDCOLOR, _T("蘑"));
	_getch();
	closegraph();
	return 0;
}