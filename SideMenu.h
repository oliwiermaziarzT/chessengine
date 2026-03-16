#include "raylib.h"
#include <vector>
#include <string>
class Menu {
    private:
    Rectangle rec = {800, 0, 200, 800};
    std::vector<std::string> logs;
    public:
    void Notation(){
        DrawText("Notation soon", 850, 400, 15, BLACK);
    }
    void drawMenu(){
        DrawRectangleLinesEx(rec, 5, RED);
    }
    void addLog(std::string message){
        logs.push_back(message);
    }
};