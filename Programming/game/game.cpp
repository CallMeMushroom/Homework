#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <cmath>
#include <string>
#include <set>
#include <utility>
#include "game.h"

constexpr int PieceRadius = (int)(1.414 * SymbolSize / 2) + 2;
constexpr int GridSize = (int)(PieceRadius * 2.22) + 1;
constexpr int GridWeight = (int)(GridSize / 50);

#define P(pos)              (pos) * GridSize

LOGFONT chessfont;
std::string board[12] = {
    "..........",
    ".rnbakabnr",
    ".         ",
    ". c     c ",
    ".p p p p p",
    ".         ",
    ".         ",
    ".P P P P P",
    ". C     C ",
    ".         ",
    ".RNBAKABNR"
};
std::set<std::pair<int, int>> choosings;                                    // store one or two "choosing" piece(s)
struct Hold {
    bool isHolding = false;
    int xPos = 0, yPos = 0;
    char symbolchar = ' ';
} hold;                                                                     // the chess you are holding (if you are holding one)

// sqaure of distance from (x0, y0) to (x, y), pure math
inline int distsquare(int x, int y, int x0, int y0) {
    return (x - x0) * (x - x0) + (y - y0) * (y - y0);
}

// are chess A and chess B the same color
inline bool isfriend(char A, char B) {
    return (std::islower(A) && std::islower(B) || std::isupper(A) && std::isupper(B));
}

// a better version of solidcircle()
void bettersolidcircle(int x, int y, int radius)
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

// draw chess board, seam and dest is about how those "target"s perform.
void drawboard(const int seam = 4, const int dest = 12) {
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();                          // copy origin line style & color
    COLORREF __fillcolor = getfillcolor();                          // copy origin fill color

    setlinestyle(PS_SOLID, GridWeight);
    setlinecolor(GRIDCOLOR);
    setfillcolor(BKGCOLOR);

    solidrectangle(P(0), P(0), P(10), P(11));                       // clear board

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
    int sgnlist[][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (auto target : targets) {                                   // for target in targets: for pos in corners: line(vrtc); line(hrzn)
        int x = P(target[0]), y = P(target[1]);
        for (auto sgn : sgnlist) {
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * dest, y + sgn[1] * seam);       // draw a target that weight is 1
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * seam, y + sgn[1] * dest);
            x += sgn[0]; y += sgn[1];
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * (dest - 1), y + sgn[1] * seam); // draw another further to target center
            line(x + sgn[0] * seam, y + sgn[1] * seam, x + sgn[0] * seam, y + sgn[1] * (dest - 1));
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

// draw a chess piece at position (x, y)
void drawpiece(int x, int y, COLORREF color, const wchar_t* symbol, LOGFONT font = chessfont)
{
    assert(x > PieceRadius && y > PieceRadius && x + PieceRadius < WindowWidth&& y + PieceRadius < WindowHeight);

    COLORREF __fillcolor = getfillcolor();                      // copy origin fill color
    COLORREF __bkcolor = getbkcolor();                          // copy origin background color
    LOGFONT __textstyle;
    gettextstyle(&__textstyle);
    COLORREF __textcolor = gettextcolor();                      // copy origin text style & color
    LINESTYLE __linestyle;
    getlinestyle(&__linestyle);
    COLORREF __linecolor = getlinecolor();                      // copy origin line style & color

    setfillcolor(CHESSCOLOR);
    setbkcolor(CHESSCOLOR);
    settextstyle(&font);
    settextcolor(color);
    setlinestyle(PS_SOLID, std::round(PieceRadius * 0.075));
    setlinecolor(color);

    bettersolidcircle(x, y, PieceRadius);
    outtextxy(x - SymbolSize / 2, y - SymbolSize / 2, symbol);
    circle(x, y, std::round(PieceRadius * 0.9));

    setfillcolor(__fillcolor);
    setbkcolor(__bkcolor);
    settextstyle(&__textstyle);
    settextcolor(__textcolor);
    setlinestyle(&__linestyle);
    setlinecolor(__linecolor);                                  // reset to origin
}

// draw a chess piece at board grid (xPos, yPos)
void drawpiece(int xPos, int yPos, char symbolchar) {
    COLORREF color = std::islower(symbolchar) ? BLACKCOLOR : REDCOLOR;

    if (symbolchar == 'M') drawpiece(P(yPos), P(xPos), LIGHTBLUE, _MUSHROOM);

    else if (symbolchar == 'K') drawpiece(P(yPos), P(xPos), color, _King);
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

// show board and all chess pieces and chosen frames
void render() {
    drawboard();

    int size = PieceRadius / 2;
    int sgnlist[][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (auto choose : choosings) {                                         // draw focuses
        int centre_x = P(choose.second), centre_y = P(choose.first);
        for (auto sgn : sgnlist) {
            int x = centre_x + sgn[0] * size;
            int y = centre_y + sgn[1] * size;
            for (int i = x + sgn[0] * size; i != x; i -= sgn[0])
                for (int j = y + sgn[1] * size; j != y; j -= sgn[1])
                    if (distsquare(x, y, i, j) > size * size + 4)
                        putpixel(i, j, FOCUSCOLOR);
                    else break;
        }
    }

    for (int row = 1; row <= 10; row++) {                                   // draw pieces
        for (int column = 1; column <= 9; column++) {
            drawpiece(row, column, board[row][column]);
        }
    }

    // DEBUG ONLY
    for (int row = 1; row <= 10; row++) {
        for (int column = 1; column <= 9; column++)
            std::cout << ' ' << board[row][column];
        std::cout << std::endl;
    }
}

bool ischoosing(MOUSEMSG mouse, int xPos, int yPos) {
    if (xPos < 1 || xPos > 10 || yPos < 1 || yPos > 9) return false;

    int x = mouse.x, x0 = P(yPos);
    int y = mouse.y, y0 = P(xPos);
    if (distsquare(x, y, x0, y0) > PieceRadius * PieceRadius) return false;

    return true;
}

std::set<std::pair<int, int>> palace;
bool islegalmove(Hold* hold, int xPos, int yPos) {
    int HxPos = hold->xPos, HyPos = hold->yPos;
    int xDist = xPos - HxPos, yDist = yPos - HyPos;
    int towards = std::islower(hold->symbolchar) ? 1 : -1;
    bool crossriver = ((xPos / 6) == !!std::islower(hold->symbolchar));

    switch (std::toupper(hold->symbolchar)) {
    case 'K':
        if (abs(xDist) + abs(yDist) != 1) return false;
        return (palace.count(std::make_pair(xPos, yPos)));
    case 'A':
        if (abs(xDist) != 1 or abs(yDist) != 1) return false;
        return (palace.count(std::make_pair(xPos, yPos)));
    case 'B':
        if (crossriver) return false;
        if (abs(xDist) != 2 or abs(yDist) != 2) return false;
        return (board[HxPos + xDist / 2][HyPos + yDist / 2] == ' ');
    case 'N':
        if (abs(xDist * yDist) != 2) return false;
        return (board[HxPos + xDist / 2][HyPos + yDist / 2] == ' ');
        // comment: I'm really surprised that Bishops and Knights have formula of blockage exactly THE SAME!!!
        // Rook and Canon: WIP
    case 'R':
        if (xDist * yDist != 0) return false;
        return true;
    case 'C':
        if (xDist * yDist != 0) return false;
        return true;
    case 'P':
        if (xPos == HxPos + towards and yPos == HyPos) return true;
        return (crossriver and yDist == 1 and xPos == HxPos);
    }
}

// everything you shall do before run anything
void init() {
    gettextstyle(&chessfont);
    chessfont.lfHeight = SymbolSize;
    chessfont.lfWeight = FW_BOLD;
    _tcscpy_s(chessfont.lfFaceName, CHESSFONT);
    chessfont.lfQuality = ANTIALIASED_QUALITY;
    //setbkmode(TRANSPARENT);
    for (int row : {1, 2, 3, 8, 9, 10})
        for (int column : {4, 5, 6})
            palace.insert(std::make_pair(row, column));
}

int main()
{
    assert(WindowWidth > P(10) && WindowHeight > P(11));
    initgraph(WindowWidth, WindowHeight, SHOWCONSOLE);
    setbkcolor(BKGCOLOR);
    BeginBatchDraw();
    cleardevice();
    init();

    MOUSEMSG mouse;
    bool shall_render = true;
    bool turnflag = false;                                                  // false<->red; true<->black

    while (true) { // for each "frame"
        if (shall_render) {
            render();
            shall_render = false;
            FlushBatchDraw();
        }
        if (MouseHit()) {
            mouse = GetMouseMsg();

            if (mouse.uMsg == WM_MBUTTONDOWN) {
                break;
            }

            else if (mouse.uMsg == WM_RBUTTONDOWN) {                        // cancel all choosings
                if (choosings.size() > 0) shall_render = true;
                hold = { false };
                choosings.clear();
            }

            else if (mouse.uMsg == WM_LBUTTONDOWN) {                        // when left-click
                int xPos = std::round(mouse.y / (double)GridSize);
                int yPos = std::round(mouse.x / (double)GridSize);          // get where you are hovering on
                char piecech = board[xPos][yPos];

                if (!ischoosing(mouse, xPos, yPos)) continue;

                if (hold.isHolding) {                                       // try to put down a piece
                    if (isfriend(hold.symbolchar, board[xPos][yPos])) {
                        choosings.clear();
                        choosings.insert(std::make_pair(xPos, yPos));
                        hold = { true, xPos, yPos, board[xPos][yPos] };
                        shall_render = true;
                    }
                    else if (islegalmove(&hold, xPos, yPos)) {     // is a legal move
                        assert(choosings.size() == 1);
                        shall_render = true;
                        turnflag = !turnflag;

                        board[xPos][yPos] = hold.symbolchar;
                        board[hold.xPos][hold.yPos] = ' ';
                        hold = { false };
                        choosings.insert(std::make_pair(xPos, yPos));
                    }
                }

                else {                                                      // try to pick up a piece
                    if (piecech != ' ' && !!std::islower(piecech) == turnflag) {  // is not empty
                        shall_render = true;

                        hold = { true, xPos, yPos, piecech };
                        choosings.clear();
                        choosings.insert(std::make_pair(xPos, yPos));
                    }
                }
            }

            else continue;
        }
        FlushBatchDraw();
    }

    drawpiece(P(5), P(10), LIGHTBLUE, _MUSHROOM);
    drawpiece(P(5), P(1), LIGHTBLUE, _MUSHROOM);
    FlushBatchDraw();
    _getch();
    closegraph();
    return 0;
}