#include "raylib.h"
#include "Board.h"
#include "SideMenu.h"
#include <map>
#include <string>
#include "InputHandler.h"
#include <iostream>

std::map<std::string, Texture2D> pieceTextures;


void loadAllTextures() {
    pieceTextures["w_pawn"]   = LoadTexture("assets/w_pawn.png");
    pieceTextures["w_rook"]   = LoadTexture("assets/w_rook.png");
    pieceTextures["w_knight"] = LoadTexture("assets/w_knight.png");
    pieceTextures["w_bishop"] = LoadTexture("assets/w_bishop.png");
    pieceTextures["w_queen"]  = LoadTexture("assets/w_queen.png");
    pieceTextures["w_king"]   = LoadTexture("assets/w_king.png");

    pieceTextures["b_pawn"]   = LoadTexture("assets/b_pawn.png");
    pieceTextures["b_rook"]   = LoadTexture("assets/b_rook.png");
    pieceTextures["b_knight"] = LoadTexture("assets/b_knight.png");
    pieceTextures["b_bishop"] = LoadTexture("assets/b_bishop.png");
    pieceTextures["b_queen"]  = LoadTexture("assets/b_queen.png");
    pieceTextures["b_king"]   = LoadTexture("assets/b_king.png");
}
void unloadAllTextures() {
    for (auto const& [key, texture] : pieceTextures) {
        UnloadTexture(texture);
    }
    pieceTextures.clear();
}

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Szachownica");
    SetTargetFPS(60);


    loadAllTextures();
    Board chessboard;
    Menu sidemenu;
    chessboard.setupPieces();
    InputHandler input;
    while (!WindowShouldClose()) {
        input.update(100);
        static int startCol = -1, startRow = -1;
        static bool pieceSelected = false;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int col = GetMouseX() / 100;
            int row = GetMouseY() / 100;
            Piece* clickedPiece = chessboard.getPieceAt(col, row);

            if (!pieceSelected) {
                if (clickedPiece != nullptr && chessboard.getCurrentColorTurn() == clickedPiece->color) {
                    startCol = col;
                    startRow = row;
                    pieceSelected = true;
                }
            } else if (chessboard.isMoveValid(startCol, startRow, col, row)) {
                chessboard.movePiece(startCol, startRow, col, row);
                pieceSelected = false;
            } else {
                pieceSelected = false;
            }
        }

        BeginDrawing();
            ClearBackground(DARKBROWN);
            chessboard.drawBoard(); 
            if(pieceSelected){
            DrawRectangle(startCol * 100, startRow * 100, 100, 100, Fade(YELLOW, 0.4f));
            }
            if(GetMouseX() < 800){
                if(input.selected.active){
                    Rectangle rec = {input.selected.col * 100, input.selected.row * 100, 100, 100};
                    DrawRectangleLinesEx(rec, 3, DARKGREEN);
                }
            }
            sidemenu.Notation();
            sidemenu.drawMenu();
        EndDrawing();
    }
    unloadAllTextures();
    CloseWindow();
    return 0;
}