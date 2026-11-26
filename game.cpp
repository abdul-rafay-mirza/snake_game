#include <iostream>
#include <raylib.h>
#include <raygui.h>

class Button {
private:
    Rectangle bounds;
    const char* text;
    int fontSize;

public:
    Button(Rectangle bounds, const char* text, int fontSize = 24)
        : bounds(bounds), text(text), fontSize(fontSize) {}

    bool IsClicked() const {
        return CheckCollisionPointRec(GetMousePosition(), bounds) &&
               IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

    void Draw() const {
        bool hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
        DrawRectangleRec(bounds, hovered ? LIGHTGRAY : RAYWHITE);
        DrawRectangleLinesEx(bounds, 2.0f, BLACK);

        int textWidth = MeasureText(text, fontSize);
        int textX = static_cast<int>(bounds.x + (bounds.width - textWidth) / 2.0f);
        int textY = static_cast<int>(bounds.y + (bounds.height - fontSize) / 2.0f);
        DrawText(text, textX, textY, fontSize, BLACK);
    }
};

// Game States
enum GAME_STATE {
    MENU,
    PLAYING,
    GAME_OVER,
    EXIT
};

GAME_STATE DrawMenu() {
    DrawText("Snake Game", 100, 100, 50, BLACK);

    // Buttons
    const Vector2 BUTTON_COORDS = {120, 200};       
    const int BUTTON_WIDTH = 250;   // The width of each button
    const int BUTTON_HEIGHT = 60;   // The height of each button

    const int BUTTON_GAP = 30;      // The gap between the start button and the exit button in pixels

    Button play_btn = Button({BUTTON_COORDS.x, BUTTON_COORDS.y, BUTTON_WIDTH, BUTTON_HEIGHT}, "Play", 20);
    play_btn.Draw();

    Button exit_btn = Button({BUTTON_COORDS.x, BUTTON_COORDS.y + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, "Exit", 20);
    exit_btn.Draw();

    if (exit_btn.IsClicked()) {
        return EXIT;
    } else if (play_btn.IsClicked()) {
        return PLAYING;
    } else {
        return MENU;
    }
}

GAME_STATE DrawPlayScreen() {
    int fontSize = 20;
    const char* text = "TODO: Make the actual Playing Grid";
    int textWidth = MeasureText(text, fontSize);
    float posX = (GetScreenWidth() - textWidth) / 2.0f;
    float posY = (GetScreenHeight() - fontSize) / 2.0f;

    DrawText(text, posX, posY, fontSize, BLACK);
    return PLAYING;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    GAME_STATE gameState = MENU;

    InitWindow(500, 500, "Snake Game");

    while(!WindowShouldClose()) {

        if (gameState == MENU) {
            gameState = DrawMenu();
        } else if (gameState == EXIT) {
            break;
        } else if (gameState == PLAYING) {
            gameState = DrawPlayScreen();
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}