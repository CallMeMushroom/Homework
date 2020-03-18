#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <cmath>
#include <string>


typedef std::pair<int, int> pair;
#define mkpr(a, b)			std::make_pair(a, b);

#define WINDOWWIDTH			960
#define WINDOWHEIGHT		720

#define SYMBOLSIZE			36								// SYMBOLSIZE *MUST* be an even integer!
constexpr int PIECERADIUS = (int)(1.414 * SYMBOLSIZE / 2) + 2;
constexpr int GRIDSIZE = (int)(PIECERADIUS * 2.22) + 1;

#define P(pos)				pos * GRIDSIZE

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
#define GRIDCOLOR			BLACK
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

LOGFONT chessfont;
void drawpiece(int x, int y, COLORREF color, const wchar_t* symbol, LOGFONT font = chessfont)
// draw a chess piece at the position (x, y)
{
	assert(x > PIECERADIUS && y > PIECERADIUS && x + PIECERADIUS < WINDOWWIDTH&& y + PIECERADIUS < WINDOWHEIGHT);

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
	setlinestyle(PS_SOLID, std::round(PIECERADIUS * 0.075));
	setlinecolor(color);

	bettersolidcircle(x, y, PIECERADIUS);
	outtextxy(x - SYMBOLSIZE / 2, y - SYMBOLSIZE / 2, symbol);
	circle(x, y, std::round(PIECERADIUS * 0.9));

	setfillcolor(__fillcolor);
	settextstyle(&__textstyle);
	settextcolor(__textcolor);
	setlinestyle(&__linestyle);
	setlinecolor(__linecolor);						// reset to origin
}

void drawboard() {
	LINESTYLE __linestyle;
	getlinestyle(&__linestyle);
	COLORREF __linecolor = getlinecolor();			// copy origin line style & color
	COLORREF __fillcolor = getfillcolor();			// copy origin fill color

	setlinestyle(PS_SOLID, 1);
	setlinecolor(GRIDCOLOR);
	setfillcolor(BKGCOLOR);

	for (int j = 1; j <= 10; j++)
		line(P(1), P(j), P(9), P(j));
	for (int i = 1; i <= 9; i++)
		line(P(i), P(1), P(i), P(10));							// draw basic board grid
	solidrectangle(P(1) + 1, P(5) + 1, P(9) - 1, P(6) - 1);		// erase lines on Chuhe Hanjie
	line(P(4), P(1), P(6), P(3));
	line(P(6), P(1), P(4), P(3));
	line(P(4), P(10), P(6), P(8));
	line(P(6), P(10), P(4), P(8));								// draw palace diagnals

	int targets[][2] = {										// "target"s on Canon and Pawn positions
		{2, 3}, {8, 3}, {1, 4}, {3, 4}, {5, 4}, {7, 4}, {9, 4},
		{2, 8}, {8, 8}, {1, 7}, {3, 7}, {5, 7}, {7, 7}, {9, 7}
	};
	for (auto target : targets) {
		int x = P(target[0]), y = P(target[1]);
		// TODO: drawtarget(x, y);
	}

	setlinestyle(PS_SOLID, 2);
	rectangle(P(1) - 4, P(1) - 4, P(9) + 5, P(10) + 5);

	setlinestyle(&__linestyle);
	setlinecolor(__linecolor);
	setfillcolor(__fillcolor);						// reset to origin
}

void render() {
	drawboard();
}

void init() {
	gettextstyle(&chessfont);
	chessfont.lfHeight = SYMBOLSIZE;
	_tcscpy_s(chessfont.lfFaceName, _T("华康简魏碑"));
	chessfont.lfQuality = ANTIALIASED_QUALITY;
	setbkmode(TRANSPARENT);
}

std::string ChessBoard = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";
int main()
{
	initgraph(WINDOWWIDTH, WINDOWHEIGHT);
	setbkcolor(BKGCOLOR);
	cleardevice();
	init();
	render();

	MOUSEMSG msg;
	while (true) { // for each "frame"
		if (MouseHit()) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN) {
				break;
			}
			else continue;
		}
	}

	drawpiece(WINDOWWIDTH / 2, WINDOWHEIGHT / 2, REDCOLOR, _T("蘑"));
	std::cout << GRIDCOLOR;
	_getch();
	closegraph();
	return 0;
}