#include "Board.h"
#include "raylib.h"
#include "SideMenu.h"
#include <string>
#include <map>
#include <format>

extern std::map<std::string, Texture2D> pieceTextures;

Board::Board(){
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            grid[i][j] = nullptr;
        }
    }
}

void Board::setupPieces() {
    for(int i = 0; i < 8; i++) grid[i][6] = new Piece{PAWN, WHITE_PIECE};
    grid[0][7] = new Piece{ROOK, WHITE_PIECE};
    grid[1][7] = new Piece{KNIGHT, WHITE_PIECE};
    grid[2][7] = new Piece{BISHOP, WHITE_PIECE};
    grid[3][7] = new Piece{QUEEN, WHITE_PIECE};
    grid[4][7] = new Piece{KING, WHITE_PIECE};
    grid[5][7] = new Piece{BISHOP, WHITE_PIECE};
    grid[6][7] = new Piece{KNIGHT, WHITE_PIECE};
    grid[7][7] = new Piece{ROOK, WHITE_PIECE};

    for(int i = 0; i < 8; i++) grid[i][1] = new Piece{PAWN, BLACK_PIECE};
    grid[0][0] = new Piece{ROOK, BLACK_PIECE};
    grid[1][0] = new Piece{KNIGHT, BLACK_PIECE};
    grid[2][0] = new Piece{BISHOP, BLACK_PIECE};
    grid[3][0] = new Piece{QUEEN, BLACK_PIECE};
    grid[4][0] = new Piece{KING, BLACK_PIECE};
    grid[5][0] = new Piece{BISHOP, BLACK_PIECE};
    grid[6][0] = new Piece{KNIGHT, BLACK_PIECE};
    grid[7][0] = new Piece{ROOK, BLACK_PIECE};
}

void Board::drawBoard() {
    for(int i = 0; i < COLS; i++) {
        for(int j = 0; j < ROWS; j++) {
            Color tileColor = ((i + j) % 2 == 0) ? LIGHTGRAY : DARKGRAY;
            DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, tileColor);

            if(grid[i][j] != nullptr) {
                std::string key = (grid[i][j]->color == WHITE_PIECE ? "w_" : "b_");
                switch(grid[i][j]->type) {
                    case PAWN:   key += "pawn"; break;
                    case ROOK:   key += "rook"; break;
                    case KNIGHT: key += "knight"; break;
                    case BISHOP: key += "bishop"; break;
                    case QUEEN:  key += "queen"; break;
                    case KING:   key += "king"; break;
                }
                grid[i][j]->draw(i, j, CELL_SIZE, pieceTextures[key]);
            }
        }
    }
}

Piece* Board::getPieceAt(int col, int row){
    if(col >= 0 && col < COLS && row >= 0 && row < ROWS){
        return grid[col][row];
    }
    return nullptr;
}

void Board::movePiece(int from_x, int from_y, int to_x, int to_y){
    Menu menu;
    // jezeli sam na siebie najedzie return
    if (from_x == to_x && from_y == to_y) return;
    // jezeli jest out of bounds return
    if (from_x < 0 || from_x >= COLS || from_y < 0 || from_y >= ROWS ||
        to_x < 0 || to_x >= COLS || to_y < 0 || to_y >= ROWS) {
        return;
    }
    Piece* pieceToMove = grid[from_x][from_y];
    // jezeli pole zaznaczone jest puste return
    if (pieceToMove == nullptr) return;
    if (grid[to_x][to_y] != nullptr ) {
        // jezeli sa takiego samego koloru return
        if (pieceToMove->color == grid[to_x][to_y]->color) return;
        delete grid[to_x][to_y];
    }
    grid[to_x][to_y] = pieceToMove;
    grid[from_x][from_y] = nullptr;
    switchTurn();
    increaseMoves();
   // std::string s = std::format("Move {}\n", moves);
    //menu.addLog("Move");
}

bool Board::isMoveValid(int from_x, int from_y, int to_x, int to_y){
    Piece* p = grid[from_x][from_y];
    if (!p) return false;
    if (from_x == to_x && from_y == to_y) return false;
    int dx = from_x - to_x;
    int dy = from_y - to_y;

    switch(p->type){
        case PAWN:
            return validatePawn(from_x, from_y, to_x, to_y, p->color);
        case ROOK:
            return validateRook(from_x, from_y, to_x, to_y, p->color);
        case KNIGHT:
            return validateKnight(from_x, from_y, to_x, to_y, p->color);
        case BISHOP:  
            return validateBishop(from_x, from_y, to_x, to_y, p->color);
        case QUEEN:
            return validateQueen(from_x, from_y, to_x, to_y, p->color);
        case KING: 
            return validateKing(from_x, from_y, to_x, to_y, p->color);
        default:
            return false;
    }
}

void Board::increaseMoves(){
    moves++;
}

void Board::switchTurn(){
    if (currentTurn == WHITE_PIECE){
        currentTurn = BLACK_PIECE;
        return;
    } else if (currentTurn == BLACK_PIECE){
        currentTurn = WHITE_PIECE;
        return;
    }
}

// --------------- sekcja walidacjiiiiii ------------ //

bool Board::isPathClear(int from_x, int from_y, int to_x, int to_y){
    int dx = (to_x > from_x) ? 1 : ((to_x < from_x) ? -1 : 0);
    int dy = (to_y > from_y) ? 1 : ((to_y < from_y) ? -1 : 0);

    int currentX = from_x + dx;
    int currentY = from_y + dy;

    while(currentX != to_x || currentY != to_y){
        if(getPieceAt(currentX, currentY) != nullptr){
            return false;
        }
        currentX += dx;
        currentY += dy;
    }
    return true;

}

bool Board::validatePawn(int from_x, int from_y, int to_x, int to_y, PieceColor color){
    int direction = (color == WHITE_PIECE) ? -1 : 1;
    int startRow = (color == WHITE_PIECE) ? 6 : 1;

    int dx = to_x - from_x;
    int dy = to_y - from_y;

    Piece* target = grid[to_x][to_y];
    if(target != nullptr && target->color == color){
        return false;
    }

    // 1 pole do przodu
    if(dx == 0 && dy == direction){
        return (grid[to_x][to_y] == nullptr);
    }
    // 2 pola do przodu
    if(dx == 0 && dy == 2 * direction && from_y == startRow){
        return (grid[to_x][to_y] == nullptr && grid[from_x][from_y + direction] == nullptr);
    }
    // bicie na skos
    if(abs(dx) == 1 && dy == direction){
        return (grid[to_x][to_y] != nullptr && grid[to_x][to_y]->color != color);
    }
    // tutaj bedzie en pasą

    return false;
}

bool Board::validateRook(int from_x, int from_y, int to_x, int to_y, PieceColor color){
    if(from_x != to_x && from_y != to_y){
        return false;
    }
    if(!isPathClear(from_x, from_y, to_x, to_y)){
        return false;
    }
    Piece* target = grid[to_x][to_y];
    if(target != nullptr && target->color == color){
        return false;
    }
    return true;
}

bool Board::validateBishop(int from_x, int from_y, int to_x, int to_y, PieceColor color){
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);

    if(!isPathClear(from_x, from_y, to_x, to_y)){
        return false;
    }
    Piece* target = grid[to_x][to_y];
    if(target != nullptr && target->color == color){
        return false;
    }
    if(dx == dy){
        return true;
    }
    return false;
}

bool Board::validateQueen(int from_x, int from_y, int to_x, int to_y, PieceColor color){
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);

    bool isStraight = (from_x == to_x || from_y == to_y);
    bool isDiagonal = (dx == dy);

    if (!isStraight && !isDiagonal) return false;
    if (!isPathClear(from_x, from_y, to_x, to_y)) return false;

    Piece* target = grid[to_x][to_y];
    if (target != nullptr && target->color == color) return false;

    return true;
}

bool Board::validateKnight(int from_x, int from_y, int to_x, int to_y, PieceColor color){
    int dx = to_x - from_x;
    int dy = to_y - from_y;

    Piece* target = grid[to_x][to_y];
    if(target != nullptr && target->color == color){
        return false;
    }
    if( (abs(dx) == 1 && abs(dy) == 2) || (abs(dx) == 2 && abs(dy) == 1) ) {
        return true;
    }
    return false;
}
bool Board::validateKing(int from_x, int from_y, int to_x, int to_y, PieceColor color) {
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);
    if (dx <= 1 && dy <= 1) {
        Piece* target = grid[to_x][to_y];
        if (target != nullptr && target->color == color) {
            return false;
        }
        return true;
    }

    return false;
}