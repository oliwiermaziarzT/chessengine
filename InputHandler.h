#include "raylib.h"

struct SelectedPiece {
    int col = -1;
    int row = -1;
    bool active = false;
};

class InputHandler {
    public:
    SelectedPiece selected;

    void update(int cell_size){
        int mX = GetMouseX();
        int mY = GetMouseY();

        selected.col = mX / cell_size;
        selected.row = mY / cell_size;
        selected.active = true;
    }
};