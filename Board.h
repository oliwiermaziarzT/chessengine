#include "raylib.h"
#include <cmath>

enum PieceType {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum PieceColor {WHITE_PIECE, BLACK_PIECE};



class Piece {
    private:
    public:
        PieceType type;
        PieceColor color;
        bool hasMoved = false;

        void draw(int col, int row, int cellSize, Texture2D pieceTexture) {
        float targetSize = cellSize * 0.8f; 
        float scale = targetSize / (float)pieceTexture.width;
        float posX = col * cellSize + (cellSize - (pieceTexture.width * scale)) / 2.0f;
        float posY = row * cellSize + (cellSize - (pieceTexture.height * scale)) / 2.0f;
        DrawTextureEx(pieceTexture, {posX, posY}, 0.0f, scale, WHITE);
        }


};

class Board {
    struct LastMove {
        int fromX, fromY, toX, toY;
        PieceType type;
    };

    private:
    const static int COLS = 8;
    const static int ROWS = 8;
    const static int CELL_SIZE = 100;
    int moves = 0;
    int tura = ceil(moves / 2);

    PieceColor currentTurn = WHITE_PIECE; 
    Piece* grid[COLS][ROWS];
    LastMove lastMove = {-1, -1, -1, -1, PAWN};

    public:
    
    Board();
    void drawBoard();   
    void setupPieces();
    Piece* getPieceAt(int col, int row);
    //logika tur
    void increaseMoves();
    void switchTurn();
    int increaseTura();
    // logika ruchu
    void movePiece(int from_x, int from_y, int to_x, int to_y);
    bool isMoveValid(int from_x, int from_y, int to_x, int to_y);
    bool isPathClear(int from_x, int from_y, int to_x, int to_y);
    bool validatePawn(int a, int b, int c, int d, PieceColor color, PieceType type);
    bool validateRook(int a, int b, int c, int d, PieceColor color);
    bool validateBishop(int a, int b, int c, int d, PieceColor color);
    bool validateQueen(int a, int b, int c, int d, PieceColor color);
    bool validateKnight(int a, int b, int c, int d, PieceColor color);
    bool validateKing(int a, int b, int c, int d, PieceColor color);
    bool isUnderCheck(int a, int b);
    PieceColor getCurrentColorTurn() { return currentTurn;} 
};