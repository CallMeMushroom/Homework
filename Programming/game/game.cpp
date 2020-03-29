#include <iostream>
#include <assert.h>
#include <conio.h>
#include <graphics.h>
#include <cmath>
#include <string>
#include <set>
#include <map>
#include <utility>
#include "game.h"

constexpr int PieceRadius = (int)(1.414 * SymbolSize / 2) + 2;
constexpr int GridSize = (int)(PieceRadius * 2.22) + 1;
constexpr int GridWeight = (int)(GridSize / 50);

#define P(pos)              (pos) * GridSize

// sqaure of distance from (x0, y0) to (x, y), pure math
inline int distsquare(int x, int y, int x0, int y0) {
    return (x - x0) * (x - x0) + (y - y0) * (y - y0);
}

// are chess A and chess B the same color
inline bool isfriend(char A, char B) {
    return (std::islower(A) and std::islower(B) or std::isupper(A) and std::isupper(B));
}

LOGFONT chessfont;

struct BoardStatus {
    std::pair<int, int> kingpos[2] = { std::make_pair(10, 5), std::make_pair(1, 5) };
    std::string chrboard[12] = {
    "..........",
    ".rhbagabhr",
    ".         ",
    ". c     c ",
    ".p p p p p",
    ".         ",
    ".         ",
    ".P P P P P",
    ". C     C ",
    ".         ",
    ".RHBAGABHR"
    };
    int numboard[11][10] = {
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0, 10,  0,  0,  0,  0,  0, 11,  0 },
        {  0, 12,  0, 13,  0, 14,  0, 15,  0, 16 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0, 17,  0, 18,  0, 19,  0, 20,  0, 21 },
        {  0,  0, 22,  0,  0,  0,  0,  0, 23,  0 },
        {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
        {  0, 24, 25, 26, 27, 28, 29, 30, 31, 32 },
    };                                                                      // remember that 5 and 28 are governer and general
    std::set<std::pair<int, int>> choosings;                                // store one or two "choosing" piece(s)
} board;

std::set<std::pair<int, int>> palace;

class Piece {
public:
    int xPos = 0, yPos = 0;
    char name = ' ';
    int id = 0;
    bool region = true;
    std::set<std::pair<int, int>> legalmoves;

    inline std::pair<int, int> getpos() {
        return std::make_pair(this->xPos, this->yPos);
    }

    inline void setpos(int xPos, int yPos) {
        this->xPos = xPos;
        this->yPos = yPos;
    }

    inline void setpos(std::pair<int, int> pos) {
        this->xPos = pos.first;
        this->yPos = pos.second;
    }

    bool islegalmove(int xPos, int yPos) {
        int xDist = xPos - this->xPos, yDist = yPos - this->yPos;
        bool crossriver = ((xPos / 6) == !!std::islower(this->name));
        int step = (xDist ^ yDist) > 0 ? 1 : -1;
        std::set<char> path;

        switch (std::toupper(this->name)) {
        case 'G':
            if (abs(xDist) + abs(yDist) != 1) return false;
            return (palace.count(std::make_pair(xPos, yPos)));

        case 'A':
            if (abs(xDist) != 1 or abs(yDist) != 1) return false;
            return (palace.count(std::make_pair(xPos, yPos)));

        case 'B':
            if (crossriver) return false;
            if (abs(xDist) != 2 or abs(yDist) != 2) return false;
            return (board.chrboard[this->xPos + xDist / 2][this->yPos + yDist / 2] == ' ');

        case 'H':
            if (abs(xDist * yDist) != 2) return false;
            return (board.chrboard[this->xPos + xDist / 2][this->yPos + yDist / 2] == ' ');
            // comment: I'm really surprised that Bishops and Knights have formula of blockage exactly THE SAME!!!

        case 'R':
            if (xDist * yDist != 0) return false;
            if (xDist) {
                for (int row = this->xPos + step; row != xPos; row += step)
                    if (board.chrboard[row][yPos] != ' ') return false;
            }
            else if (yDist) {
                for (int clm = this->yPos + step; clm != yPos; clm += step)
                    if (board.chrboard[xPos][clm] != ' ') return false;
            }
            return true;

        case 'C':
            if (xDist * yDist != 0) return false;

            if (xDist) {
                for (int row = this->xPos + step; row != xPos + step; row += step)
                    path.insert(board.chrboard[row][yPos]);
            }
            else if (yDist) {
                for (int clm = this->yPos + step; clm != yPos + step; clm += step)
                    path.insert(board.chrboard[xPos][clm]);
            }
            path.insert(' ');
            if (path.size() != 1 and path.size() != 3) return false;
            return (path.size() == 1) == (board.chrboard[xPos][yPos] == ' ');
            // comment: rule code for Rook and Canon is super ugly, looking forward to simplification.

        case 'P':
            if (crossriver and abs(yDist) == 1 and xDist == 0) return true;
            return (xDist == (std::islower(this->name) ? 1 : -1) and yDist == 0);
        }
    }

    void maintain() {
        for (int row = 1; row <= 10; row++)
            for (int column = 1; column <= 9; column++) {
                if (!isfriend(this->name, board.chrboard[row][column]) and this->islegalmove(row, column))
                    this->legalmoves.insert(std::make_pair(row, column));
            }
    }
};
std::map<int, Piece> pieces;

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

    else if (symbolchar == 'G') drawpiece(P(yPos), P(xPos), color, _Governer);
    else if (symbolchar == 'A') drawpiece(P(yPos), P(xPos), color, _Advisor);
    else if (symbolchar == 'B') drawpiece(P(yPos), P(xPos), color, _Bishop);
    else if (symbolchar == 'H') drawpiece(P(yPos), P(xPos), color, _Horse);
    else if (symbolchar == 'R') drawpiece(P(yPos), P(xPos), color, _Rook);
    else if (symbolchar == 'C') drawpiece(P(yPos), P(xPos), color, _Cannon);
    else if (symbolchar == 'P') drawpiece(P(yPos), P(xPos), color, _Pawn);
    else if (symbolchar == 'g') drawpiece(P(yPos), P(xPos), color, _general);
    else if (symbolchar == 'a') drawpiece(P(yPos), P(xPos), color, _advisor);
    else if (symbolchar == 'b') drawpiece(P(yPos), P(xPos), color, _bishop);
    else if (symbolchar == 'h') drawpiece(P(yPos), P(xPos), color, _horse);
    else if (symbolchar == 'r') drawpiece(P(yPos), P(xPos), color, _rook);
    else if (symbolchar == 'c') drawpiece(P(yPos), P(xPos), color, _catapult);
    else if (symbolchar == 'p') drawpiece(P(yPos), P(xPos), color, _pawn);
}

// show board and all chess pieces and chosen frames
void render() {
    drawboard();

    int size = PieceRadius / 2;
    int sgnlist[][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (auto choose : board.choosings) {                                         // draw focuses
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
            drawpiece(row, column, board.chrboard[row][column]);
        }
    }
}

bool ischoosing(MOUSEMSG mouse, int xPos, int yPos) {
    if (xPos < 1 or xPos > 10 or yPos < 1 or yPos > 9) return false;

    int x = mouse.x, x0 = P(yPos);
    int y = mouse.y, y0 = P(xPos);
    return (distsquare(x, y, x0, y0) <= (PieceRadius + 1) * (PieceRadius + 1));
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
    for (int row = 1; row <= 10; row++) {
        for (int column = 1; column <= 9; column++) {
            if (board.numboard[row][column] == 0) continue;
            Piece piece;
            piece.setpos(row, column);
            piece.name = board.chrboard[row][column];
            piece.id = board.numboard[row][column];
            piece.region = !!std::islower(piece.name);
            piece.maintain();
            pieces[piece.id] = piece;
        }
    }
}

int main()
{
    assert(WindowWidth > P(10) and WindowHeight > P(11));
    initgraph(WindowWidth, WindowHeight, SHOWCONSOLE);
    setbkcolor(BKGCOLOR);
    BeginBatchDraw();
    cleardevice();
    init();

    MOUSEMSG mouse;
    bool shall_render = true;
    bool shall_judge = false;
    bool turnflag = REDTURN;
    Piece hold;                                                             // the chess you are holding (if you are holding one)

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
                if (board.choosings.size() > 0) shall_render = true;
                hold = { 0, 0, ' ' };
                board.choosings.clear();
            }

            else if (mouse.uMsg == WM_LBUTTONDOWN) {                        // when left-click
                int xPos = std::round(mouse.y / (double)GridSize);
                int yPos = std::round(mouse.x / (double)GridSize);          // get where you are hovering on
                if (!ischoosing(mouse, xPos, yPos)) continue;

                int id = board.numboard[xPos][yPos];                        // name of piece at target intersection

                if (pieces[id].name != ' ' and pieces[id].region == turnflag) { // pick up a piece
                    board.choosings.clear();
                    board.choosings.insert(std::make_pair(xPos, yPos));     // update choosings
                    hold = pieces[id];                                      // update hold
                    shall_render = true;

                    hold.maintain();
                }

                else if (hold.name != ' ') {                                // put down a piece
                    if (!hold.legalmoves.count(std::make_pair(xPos, yPos))) continue;

                    BoardStatus boardcopy = board;
                    auto piecescopy = pieces;

                    if (board.chrboard[xPos][yPos] != ' ')
                        pieces.erase(board.numboard[xPos][yPos]);
                    pieces[hold.id].setpos(xPos, yPos);
                    board.chrboard[hold.xPos][hold.yPos] = ' ';
                    board.numboard[hold.xPos][hold.yPos] = 0;
                    board.chrboard[xPos][yPos] = hold.name;
                    board.numboard[xPos][yPos] = hold.id;
                    if (std::toupper(hold.name) == 'G')
                        board.kingpos[turnflag] = std::make_pair(xPos, yPos);
                    board.choosings.insert(std::make_pair(xPos, yPos));

                    bool in_check = false;
                    bool face_each = (pieces[5].yPos == pieces[28].yPos);
                    for (auto map : pieces) {
                        Piece piece = map.second;
                        if (face_each and (piece.yPos == pieces[5].yPos) and (pieces[5].xPos < piece.xPos and piece.xPos < pieces[28].xPos))
                            face_each = false;
                        if (piece.region != turnflag) {
                            piece.maintain();
                            std::cout << piece.xPos << ' ' << piece.yPos << ' ' << piece.name << std::endl;
                            if (piece.legalmoves.count(board.kingpos[turnflag])) {
                                in_check = true;
                                continue;
                            }
                        }
                    }

                    std::cout << std::endl;
                    if (in_check or face_each) {
                        board = boardcopy;
                        pieces = piecescopy;
                        continue;
                        std::cout << in_check << face_each << std::endl;
                    }

                    hold = { 0, 0, ' ' };
                    turnflag = !turnflag;
                    shall_render = true;
                    shall_judge = true;
                }
            }
            else continue;
        }
    }

    drawpiece(P(5), P(10), LIGHTBLUE, _MUSHROOM);
    drawpiece(P(5), P(1), LIGHTBLUE, _MUSHROOM);
    FlushBatchDraw();
    _getch();
    closegraph();
    return 0;
}
//end