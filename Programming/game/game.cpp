#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <cmath>
#include <string>

#define WindowWidth         960
#define WindowHeight        720

#define SymbolSize          36                  // SymbolSize *MUST* be an even integer!
constexpr int PieceRadius = (int)(1.414 * SymbolSize / 2) + 2;
constexpr int GridSize = (int)(PieceRadius * 2.22) + 1;
constexpr int GridWeight = (int)(GridSize / 50);

#define P(pos)              (pos) * GridSize

#define _King               _T("帥")
#define _Advisor            _T("士")
#define _Bishop             _T("相")
#define _Knight             _T("馬")
#define _Roof               _T("車")
#define _Canon              _T("炮")
#define _Pawn               _T("兵")
#define _king               _T("將")
#define _advisor            _T("仕")
#define _bishop             _T("象")
#define _knight             _T("馬")
#define _roof               _T("車")
#define _canon              _T("砲")
#define _pawn               _T("卒")

#define GRAY                0x7F7F7F
#define OFFWHITE            0xD6EEF7
#define CRIMSON             0x462FBF
#define NAVAJO              0xADDEFF
#define MOCCASIN            0xB5E4FF
#define BRULYWOOD           0x87B8DE

#define BKGCOLOR            MOCCASIN
#define CHESSCOLOR          BRULYWOOD
#define REDCOLOR            CRIMSON
#define BALCKCOLOR          BLACK
#define GRIDCOLOR           BLACK
#define TEXTCOLOR           WHITE

inline void bettersolidcircle(int x, int y, int radius)
// a better version of solidcircle()
{
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();          // copy origin line style and color

    setlinestyle(PS_SOLID, 1);
    setlinecolor(getfillcolor());

    circle(x, y, radius);
    floodfill(x, y, getfillcolor());

    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);                      // reset to orgin
}

LOGFONT chessfont;
void drawpiece(int x, int y, COLORREF color, const wchar_t* symbol, LOGFONT font = chessfont)
// draw a chess piece at position (x, y)
{
    assert(x > PieceRadius && y > PieceRadius && x + PieceRadius < WindowWidth&& y + PieceRadius < WindowHeight);

    COLORREF __fillcolor = getfillcolor();                      // copy origin fill color
    LOGFONT __textstyle;
    gettextstyle(&__textstyle);
    COLORREF __textcolor = gettextcolor();                      // copy origin text style & color
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();                      // copy origin line style & color

    setfillcolor(CHESSCOLOR);
    settextstyle(&font);
    settextcolor(color);
    setlinestyle(PS_SOLID, std::round(PieceRadius * 0.075));
    setlinecolor(color);

    bettersolidcircle(x, y, PieceRadius);
    outtextxy(x - SymbolSize / 2, y - SymbolSize / 2, symbol);
    circle(x, y, std::round(PieceRadius * 0.9));

    setfillcolor(__fillcolor);
    settextstyle(&__textstyle);
    settextcolor(__textcolor);
    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);                                  // reset to origin
}

const int Seam = 4, Dsti = 12;
void drawboard() {
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();                          // copy origin line style & color
    COLORREF __fillcolor = getfillcolor();                          // copy origin fill color

    setlinestyle(PS_SOLID, GridWeight);
    setlinecolor(GRIDCOLOR);
    setfillcolor(BKGCOLOR);

    for (int j = 1; j <= 10; j++) {
        line(P(1), P(j), P(9), P(j));
    }
    for (int i = 1; i <= 9; i++) {
        line(P(i), P(1), P(i), P(10));
    }                                                               // draw basic board grid
    solidrectangle(P(1) + 1, P(5) + 1, P(9) - 1, P(6) - 1);         // erase lines that are over Chuhe Hanjie
    line(P(4), P(1), P(6), P(3));
    line(P(6), P(1), P(4), P(3));
    line(P(4), P(10), P(6), P(8));
    line(P(6), P(10), P(4), P(8));                                  // draw diagnals of the Palace

    int targets[][2] = {                                            // positions of each "target" on Canon and Pawn
        {2, 3}, {8, 3}, {1, 4}, {3, 4}, {5, 4}, {7, 4}, {9, 4},
        {2, 8}, {8, 8}, {1, 7}, {3, 7}, {5, 7}, {7, 7}, {9, 7}
    };
    int sgnlist[][2] = {                                            // sgnlist controls offsets of each of 4 corners of one target
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    for (int* target : targets) {                                   // for target in targets: for pos in corners: line(vrtc); line(hrzn)
        int x = P(target[0]), y = P(target[1]);
        for (int* sgn : sgnlist) {
            line(x + sgn[0] * Seam, y + sgn[1] * Seam, x + sgn[0] * Dsti, y + sgn[1] * Seam);       // draw a target that weight is 1
            line(x + sgn[0] * Seam, y + sgn[1] * Seam, x + sgn[0] * Seam, y + sgn[1] * Dsti);
            x += sgn[0]; y += sgn[1];
            line(x + sgn[0] * Seam, y + sgn[1] * Seam, x + sgn[0] * (Dsti - 1), y + sgn[1] * Seam); // draw another further to target center
            line(x + sgn[0] * Seam, y + sgn[1] * Seam, x + sgn[0] * Seam, y + sgn[1] * (Dsti - 1));
            x -= sgn[0]; y -= sgn[1];
        }
    }
    setlinestyle(PS_SOLID, 18);
    setlinecolor(BKGCOLOR);
    rectangle(P(1) - 9, P(1) - 9, P(9) + 10, P(10) + 10);           // erase out-of-range target halves

    setlinestyle(PS_SOLID, 3);
    setlinecolor(GRIDCOLOR);
    rectangle(P(1) - 5, P(1) - 5, P(9) + 6, P(10) + 6);             // draw bold border

    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);
    setfillcolor(__fillcolor);                                      // reset to origin
}

void render() {
    drawboard();
}

void init() {
    gettextstyle(&chessfont);
    chessfont.lfHeight = SymbolSize;
    chessfont.lfWeight = FW_BOLD;
    _tcscpy_s(chessfont.lfFaceName, _T("HanWangWeBe"));
    chessfont.lfQuality = ANTIALIASED_QUALITY;
    setbkmode(TRANSPARENT);
}

std::string ChessBoard = \
"rnbakabnr" \
"........." \
".c.....c." \
"p.p.p.p.p" \
"........." \
"........." \
"P.P.P.P.P" \
".C.....C." \
"........." \
"RNBAKABNR";
int main()
{
    initgraph(WindowWidth, WindowHeight);
    setbkcolor(BKGCOLOR);
    BeginBatchDraw();
    cleardevice();
    init();
    render();
    FlushBatchDraw();

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

    drawpiece(WindowWidth / 2, WindowHeight / 2, REDCOLOR, _T("蘑"));
    FlushBatchDraw();
    std::cerr << GRIDCOLOR;
    _getch();
    closegraph();
    return 0;
}