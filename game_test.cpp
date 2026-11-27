#include <iostream>
#include <raylib.h>
#include <raygui.h>
#include <vector>

float elapsedTime = 0.0f;
std::vector<Vector2> cellsOccupiedBySnake;

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

enum SNAKE_DIRECTION {
    RIGHT,
    LEFT,
    UP,
    DOWN
};

class Snake {
private:
    constexpr static Vector2 head = {2, 2};
    Vector2 tail;
    SNAKE_DIRECTION direction;
    float cellSize;
    float gap;

public:
    Snake(Vector2 tail, SNAKE_DIRECTION direction, float cellSize, float gap)
        : tail(tail), direction(direction), cellSize(cellSize), gap(gap) {}

    // Takes in a logical coordinate (Vector2) of a cell and fills its corresponding actual cell on the screen
    void fillCell(Vector2 coord, Color cellColor, float cellSize, float gap) {
        Rectangle actualCell = {coord.x * cellSize, coord.y * cellSize, cellSize - gap, cellSize - gap};
        DrawRectangleRec(actualCell, cellColor);
    }

    void DrawCells() {
        for (int i = 0; i < cellsOccupiedBySnake.size(); i++) {
            fillCell(cellsOccupiedBySnake[i], BLACK, cellSize, gap);
        }
    }

    void Move() {
        float movementSpeed = 1.0f;
        float dt = GetFrameTime();

        elapsedTime = elapsedTime + dt;
        // std::cout << elapsedTime << std::endl;

        if (elapsedTime > movementSpeed) {
            cellsOccupiedBySnake.push_back(head);
            std::cout << "Before: " << head.x << std::endl;
            head.x += 1;
            std::cout << "After: " << head.x << std::endl;
            elapsedTime = 0.0f;
        }
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
    int fontSize = 50;
    const char* menuHeadingText = "Snake Game";
    int textWidth = MeasureText(menuHeadingText, fontSize);
    float titlePosX = (GetScreenWidth() - textWidth) / 2.0f;
    float titlePosY = (GetScreenHeight() - fontSize) / 4.0f;
    DrawText(menuHeadingText, titlePosX, titlePosY, fontSize, BLACK);

    // Buttons
    const Vector2 BUTTON_COORDS = {titlePosX + 20, titlePosY + 100};       
    const int BUTTON_WIDTH = 250;   // The width of each button
    const int BUTTON_HEIGHT = 60;   // The height of each button
    const int FONT_SIZE = 20;
    const int BUTTON_GAP = 30;      // The gap between the start button and the exit button in pixels

    Button play_btn = Button({BUTTON_COORDS.x, BUTTON_COORDS.y, BUTTON_WIDTH, BUTTON_HEIGHT}, "Play", FONT_SIZE);
    play_btn.Draw();

    Button exit_btn = Button({BUTTON_COORDS.x, BUTTON_COORDS.y + BUTTON_HEIGHT + BUTTON_GAP, BUTTON_WIDTH, BUTTON_HEIGHT}, "Exit", FONT_SIZE);
    exit_btn.Draw();

    if (exit_btn.IsClicked()) {
        return EXIT;
    } else if (play_btn.IsClicked()) {
        return PLAYING;
    } else {
        return MENU;
    }
}

GAME_STATE DrawPlayScreen(int cellCount, float cellSize, float gap, Snake snake) {
    Vector2 logicalCoordinates;
    Vector2 actualCoord;

    for (size_t i = 0; i < cellCount; i++) {
        float x = static_cast<float>(i);

        for (size_t j = 0; j < cellCount; j++) {
            float y = static_cast<float>(j);

            actualCoord = {cellSize * i,cellSize * j};
            logicalCoordinates = {x, y};

            // Draw the actual grid
            if (y != cellCount - 1 && y != 0 && x != 0 && x != cellCount - 1){
                DrawRectangleLinesEx({actualCoord.x, actualCoord.y, cellSize - gap, cellSize - gap}, 1.0f, BLACK);
            }
        }
    }

    const char* score = "Score: ";
    const int fontSize = 20;
    Vector2 scoreCoords = {cellSize, static_cast<float>(GetScreenHeight()) - fontSize};
    DrawText(score, scoreCoords.x, scoreCoords.y, fontSize, BLACK);

    snake.Move();
    snake.DrawCells();
    // snake.fillCell({2, 2}, BLACK, CELL_SIZE, GAP);

    return PLAYING;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    GAME_STATE gameState = MENU;

    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 500;

    const int CELL_COUNT = 15;
    const float GAP = 1.0f;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(10);
    
    while (!WindowShouldClose()) {
        const float CELL_SIZE = static_cast<float>(GetScreenWidth()) / CELL_COUNT;
        Snake snake = Snake({2, 2}, RIGHT, CELL_SIZE, GAP);

        if (gameState == MENU) {
            gameState = DrawMenu();
        } else if (gameState == EXIT) {
            break;
        } else if (gameState == PLAYING) {
            gameState = DrawPlayScreen(CELL_COUNT, CELL_SIZE, GAP, snake);
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}