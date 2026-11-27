#include <iostream>
#include <raylib.h>
#include <vector>

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
    Vector2 head;
    Vector2 tail;
    SNAKE_DIRECTION nextDirection;
    SNAKE_DIRECTION previousDirection = RIGHT;
    std::vector<Vector2> cellsOccupiedBySnake;
    float elapsedTime = 0.0f;
    Color color = BLACK;          // Snake color
    float movementSpeed = 0.6f;

public:
    Snake(Vector2 head, Vector2 tail, SNAKE_DIRECTION direction)
        : head(head), tail(tail), nextDirection(direction) {}

    // Takes in a logical coordinate (Vector2) of a cell and fills its corresponding actual cell on the screen
    void fillCell(Vector2 coord, Color cellColor, float cellSize, float gap) {
        Rectangle actualCell = {coord.x * cellSize, coord.y * cellSize, cellSize - gap, cellSize - gap};
        DrawRectangleRec(actualCell, cellColor);
    }

    void DrawCells(float cellSize, float gap) {
        for (int i = 0; i < this->cellsOccupiedBySnake.size(); i++) {
            fillCell(this->cellsOccupiedBySnake[i], BLACK, cellSize, gap);
        }
        fillCell(head, color, cellSize, gap);
    }

    void Move() {
        // Choose direction every single game tick
        if (IsKeyDown(KEY_W)) nextDirection = UP;
        if (IsKeyDown(KEY_A)) nextDirection = LEFT;
        if (IsKeyDown(KEY_S)) nextDirection = DOWN;
        if (IsKeyDown(KEY_D)) nextDirection = RIGHT;

        float dt = GetFrameTime();

        this->elapsedTime = this->elapsedTime + dt;

        if (elapsedTime > this->movementSpeed) {
            this->cellsOccupiedBySnake.push_back(head);

            // Debugging
            // std::cout << "Previous: " << previousDirection << std::endl;
            // std::cout << "Next: " << nextDirection << std::endl;

            // So that the sanke cannot travel backwards
            if (previousDirection == RIGHT && nextDirection == LEFT) nextDirection = RIGHT;
            if (previousDirection == LEFT && nextDirection == RIGHT) nextDirection = LEFT;
            if (previousDirection == UP && nextDirection == DOWN) nextDirection = UP;
            if (previousDirection == DOWN && nextDirection == UP) nextDirection = DOWN;

            switch (nextDirection) {
                case UP : this->head.y--; break;
                case LEFT : this->head.x--; break;
                case DOWN : this->head.y++; break;
                case RIGHT : this->head.x++; break;
            }
            previousDirection = nextDirection;

            this->elapsedTime = 0.0f;
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

GAME_STATE DrawPlayScreen(Snake& snake) {
    const int CELL_COUNT = 15;
    const float CELL_SIZE = static_cast<float>(GetScreenWidth()) / CELL_COUNT;
    const float GAP = 1.0f;
    Vector2 logicalCoordinate;
    Vector2 actualCoord;

    for (size_t i = 0; i < CELL_COUNT; i++) {
        float x = static_cast<float>(i);

        for (size_t j = 0; j < CELL_COUNT; j++) {
            float y = static_cast<float>(j);

            actualCoord = {CELL_SIZE * i,CELL_SIZE * j};
            logicalCoordinate = {x, y};

            // Draw the actual grid
            if (y != CELL_COUNT - 1 && y != 0 && x != 0 && x != CELL_COUNT - 1){
                DrawRectangleLinesEx({actualCoord.x, actualCoord.y, CELL_SIZE - GAP, CELL_SIZE - GAP}, 1.0f, BLACK);
            }
        }
    }

    const char* score = "Score: ";
    const int fontSize = 20;
    Vector2 scoreCoords = {CELL_SIZE, static_cast<float>(GetScreenHeight()) - fontSize};
    DrawText(score, scoreCoords.x, scoreCoords.y, fontSize, BLACK);

    snake.DrawCells(CELL_SIZE, GAP);
    snake.Move();

    return PLAYING;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    GAME_STATE gameState = MENU;

    const int SCREEN_WIDTH = 500;
    const int SCREEN_HEIGHT = 500;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    // SetTargetFPS(10);

    Snake snake = Snake({2, 2}, {2, 2}, RIGHT);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (gameState == MENU) {
            gameState = DrawMenu();
        } else if (gameState == EXIT) {
            break;
        } else if (gameState == PLAYING) {
            gameState = DrawPlayScreen(snake);
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}