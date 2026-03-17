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
    if (from_x == to_x && from_y == to_y) return;
    if (from_x < 0 || from_x >= COLS || from_y < 0 || from_y >= ROWS ||
        to_x < 0 || to_x >= COLS || to_y < 0 || to_y >= ROWS) {
        return;
    }

    Piece* pieceToMove = grid[from_x][from_y];

    if (pieceToMove == nullptr) return;
    if (grid[to_x][to_y] != nullptr) {
        delete grid[to_x][to_y];
    }

    grid[to_x][to_y] = pieceToMove;
    grid[from_x][from_y] = nullptr;

    // en passant - 
    if (pieceToMove->type == PAWN && to_x != from_x && 
        lastMove.toX == to_x && lastMove.toY == from_y) {
        delete grid[to_x][from_y];
        grid[to_x][from_y] = nullptr;
    }

    // roszada
    if (pieceToMove->type == KING && abs(to_x - from_x) == 2) {
        int rookFromX, rookToX;
        
        if (to_x > from_x) { 
            rookFromX = 7;
            rookToX = to_x - 1;
        } else { 
            rookFromX = 0;
            rookToX = to_x + 1;
        }
        Piece* rook = grid[rookFromX][to_y];
        if (rook != nullptr) {
            grid[rookToX][to_y] = rook;
            grid[rookFromX][to_y] = nullptr;
            rook->hasMoved = true;
        }
    }

    lastMove = { from_x, from_y, to_x, to_y, pieceToMove->type };

    if(!pieceToMove->hasMoved) { pieceToMove->hasMoved = true; };
    switchTurn();
    increaseMoves();
    
}

bool Board::isMoveValid(int from_x, int from_y, int to_x, int to_y) {
    Piece* p = grid[from_x][from_y];
    if (!p) return false;
    bool movesLegally = false;
    int dx = abs(to_x - from_x);
    int dy = abs(to_y - from_y);

    switch(p->type) {
        case PAWN:   movesLegally = validatePawn(from_x, from_y, to_x, to_y, p->color, p->type); break;
        case ROOK:   movesLegally = validateRook(from_x, from_y, to_x, to_y, p->color); break;
        case KNIGHT: movesLegally = ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)); break;
        case BISHOP: movesLegally = validateBishop(from_x, from_y, to_x, to_y, p->color); break;
        case QUEEN:  movesLegally = validateQueen(from_x, from_y, to_x, to_y, p->color); break;
        case KING:   movesLegally = validateKing(from_x, from_y, to_x, to_y, p->color); break;
    }

    if (!movesLegally) return false;

    Piece* tempTarget = grid[to_x][to_y];
    grid[to_x][to_y] = p;
    grid[from_x][from_y] = nullptr;

    int kingX = -1, kingY = -1;
    for(int x = 0; x < 8; x++) {
        for(int y = 0; y < 8; y++) {
            if(grid[x][y] != nullptr && grid[x][y]->type == KING && grid[x][y]->color == p->color) {
                kingX = x; kingY = y;
                break;
            }
        }
    }
    bool leadsToCheck = isUnderCheck(kingX, kingY);
    grid[from_x][from_y] = p;
    grid[to_x][to_y] = tempTarget;

    return !leadsToCheck;
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

bool Board::isUnderCheck(int currX, int currY){
    Piece* king = grid[currX][currY];
    if(king == nullptr) return false;
    PieceColor opponentColor = (king->color == WHITE_PIECE) ? BLACK_PIECE : WHITE_PIECE;

    for(int i = 0; i < COLS; i++){
        for(int j = 0; j < ROWS; j++){
            Piece* p = grid[i][j];
            if(p != nullptr && p->color == opponentColor){
                if(isMoveValid(i, j, currX, currY)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::validatePawn(int from_x, int from_y, int to_x, int to_y, PieceColor color, PieceType type){
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
    // bicie 
   if (abs(dx) == 1 && dy == direction) {
        //na skos
        if (grid[to_x][to_y] != nullptr && grid[to_x][to_y]->color != color) {
            return true;
        }
        // en pasą
        if (grid[to_x][to_y] == nullptr) {
            Piece* sasiad = grid[to_x][from_y];
            if (sasiad != nullptr &&
                sasiad->type == PAWN &&
                sasiad->color != color &&
                lastMove.toX == to_x &&       
                lastMove.toY == from_y &&     
                lastMove.fromY == from_y + (color == WHITE_PIECE ? -2 : 2)) 
            {
                return true;
            }
        }
    }
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

    if(dx != dy) return false;                        
    if(!isPathClear(from_x, from_y, to_x, to_y)) return false;

    Piece* target = grid[to_x][to_y];
    if(target != nullptr && target->color == color) return false;

    return true;
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
        if (target != nullptr && target->color == color) return false;

        Piece* originalFrom = grid[from_x][from_y];
        Piece* originalTo = grid[to_x][to_y];

        grid[from_x][from_y] = nullptr;
        grid[to_x][to_y] = originalFrom;

        bool safe = !isUnderCheck(to_x, to_y);

        grid[from_x][from_y] = originalFrom;
        grid[to_x][to_y] = originalTo;

        return safe;
    }

    //  logika roszady 
    if (!grid[from_x][from_y]->hasMoved && dy == 0 && dx == 2) {

        if (isUnderCheck(from_x, from_y)) return false;

        int rookX = (to_x > from_x) ? 7 : 0;
        int step = (to_x > from_x) ? 1 : -1;

        if (isUnderCheck(from_x + step, from_y)) return false;
        if (isUnderCheck(to_x, to_y)) return false;

        return isPathClear(from_x, from_y, rookX, from_y);
    }

    return false;
}

