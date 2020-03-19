#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <cmath>
#include <string>
#include "game.h"

constexpr int PieceRadius = (int)(1.414 * SymbolSize / 2) + 2;
constexpr int GridSize = (int)(PieceRadius * 2.22) + 1;
constexpr int GridWeight = (int)(GridSize / 50);

#define P(pos)              (pos) * GridSize

// a better version of solidcircle()
inline void bettersolidcircle(int x, int y, int radius)

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
// draw a chess piece at position (x, y)
void drawpiece(int x, int y, COLORREF color, const wchar_t* symbol, LOGFONT font = chessfont)
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

std::string ChessBoard;
// draw a chess piece at board grid (xPos, yPos)
void drawpiece(int xPos, int yPos, char symbolchar) {
    COLORREF color = std::islower(symbolchar) ? BLACKCOLOR : REDCOLOR;

    if (symbolchar == 'K') drawpiece(P(yPos), P(xPos), color, _King);
    else if (symbolchar == 'A') drawpiece(P(yPos), P(xPos), color, _Advisor);
    else if (symbolchar == 'B') drawpiece(P(yPos), P(xPos), color, _Bishop);
    else if (symbolchar == 'N') drawpiece(P(yPos), P(xPos), color, _Knight);
    else if (symbolchar == 'R') drawpiece(P(yPos), P(xPos), color, _Rook);
    else if (symbolchar == 'C') drawpiece(P(yPos), P(xPos), color, _Cannon);
    else if (symbolchar == 'P') drawpiece(P(yPos), P(xPos), color, _Pawn);
    else if (symbolchar == 'k') drawpiece(P(yPos), P(xPos), color, _king);
    else if (symbolchar == 'a') drawpiece(P(yPos), P(xPos), color, _advisor);
    else if (symbolchar == 'b') drawpiece(P(yPos), P(xPos), color, _bishop);
    else if (symbolchar == 'n') drawpiece(P(yPos), P(xPos), color, _knight);
    else if (symbolchar == 'r') drawpiece(P(yPos), P(xPos), color, _rook);
    else if (symbolchar == 'c') drawpiece(P(yPos), P(xPos), color, _cannon);
    else if (symbolchar == 'p') drawpiece(P(yPos), P(xPos), color, _pawn);
}

const int Seam = 4, Dsti = 12;
// draw chess board, Seam and Dsti is about how those "target"s perform.
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

// render board AND all chess pieces
void render() {
    drawboard();
    for (int row = 1; row <= 10; row++) {
        for (int column = 1; column <= 9; column++) {
            drawpiece(row, column, ChessBoard[(row - 1) * 9 + (column - 1)]);
        }
    }
}

// everything you shall do before run anything
void init() {
    gettextstyle(&chessfont);
    chessfont.lfHeight = SymbolSize;
    chessfont.lfWeight = FW_BOLD;
    _tcscpy_s(chessfont.lfFaceName, _T("HanWangWeBe"));
    chessfont.lfQuality = ANTIALIASED_QUALITY;
    setbkmode(TRANSPARENT);

    /*std::string*/ ChessBoard = \
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
}

int main()
{
    initgraph(WindowWidth, WindowHeight);
    setbkcolor(BKGCOLOR);
    BeginBatchDraw();
    cleardevice();
    init();

    MOUSEMSG msg;
    bool doHoldPiece = false;
    while (true) { // for each "frame"
        render();
        FlushBatchDraw();
        if (MouseHit()) {
            msg = GetMouseMsg();
            if (msg.uMsg == WM_MBUTTONDOWN) {
                break;
            }
            else if (msg.uMsg == WM_LBUTTONDBLCLK) {

            }
            else continue;
        }
        FlushBatchDraw();
    }

    drawpiece(WindowWidth / 2, WindowHeight / 2, REDCOLOR, _T("蘑"));
    FlushBatchDraw();
    std::cerr << GRIDCOLOR;
    _getch();
    closegraph();
    return 0;
}