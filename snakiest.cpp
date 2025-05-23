#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;
/*
In C++, there isn't a built-in Vector2 class in the standard library. However, developers often create their own Vector2 structure to represent 2D vectors, commonly used for positions, velocities, or directions in graphics programming and game development.
*/
Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};

int cell = 30;
int cellCount = 25;



// element looking for and the deque vector2
bool element_in_deque(Vector2 element, deque<Vector2> deque)
{
    for (long long unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

double lastTime = 0;
bool triggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastTime >= interval)
    {
        lastTime = currentTime;
        return true;
    }
    return false;
}

class food
{
public:
    Vector2 position;
    Texture2D texture;

    // load image fuction
    food(deque<Vector2> snakeBody)
    {
        Image img = LoadImage("apple.png");
        // setting image height and width to fit into our cell
        texture = LoadTextureFromImage(img);
        texture.width = cell;
        texture.height = cell;
        UnloadImage(img);
        position = generateRandomPos(snakeBody);
    }

    ~food()
    {
        UnloadTexture(texture);
    }

    void draw()
    {
        DrawTexture(texture,  position.x * cell,  position.y * cell, WHITE);
    }

    Vector2 generateRandomcell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 generateRandomPos(deque<Vector2> snakeBody)
    {

        Vector2 position = generateRandomcell();
        while (element_in_deque(position, snakeBody))
        {
            position = generateRandomcell();
        }
        return position;
    }
};

class snake
{
public:
    deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
    Vector2 dir = {1, 0};
    bool addsegment = false;

    void draw()
    {
        for (long long unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle seg = Rectangle{ x * cell,  y * cell, (float)cell, (float)cell};
            DrawRectangleRounded(seg, 0.5, 6, green);
        }
    }

    void update()
    {
        body.push_front(Vector2Add(body[0], dir));
        if (addsegment == true)
        {
            addsegment = false;
        }
        else
        {
            body.pop_back();
        }
    }

    void reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        dir = {1, 0};
    }
};

class Game
{
public:
    snake s = snake();
    food f = food(s.body);
    bool running = true;
    int score = 0;

    void draw()
    {
        f.draw();
        s.draw();
    }

    void update()
    {
        if (running == true)
        {
            s.update();
            checkCollisionWithFood();
            checkCollisionWithEdges();
            checkCollisionWithTail();
        }
    }

    void checkCollisionWithFood()
    {
        if (Vector2Equals(s.body[0], f.position))
        {
            f.position = f.generateRandomPos(s.body);
            s.addsegment = true;
            score++;
        }
    }

    void checkCollisionWithEdges()
    {
        if (s.body[0].x == cell || s.body[0].x == -1)
        {
            GameOver();
        }
        if (s.body[0].y == cell || s.body[0].y == -1)
        {
            GameOver();
        }
    }

    void GameOver()
    {
        s.reset();
        f.position = f.generateRandomPos(s.body);
        running = false;
        score = 0;
    }

    void checkCollisionWithTail()
    {
        deque<Vector2> head = s.body;
        head.pop_front();
        if (element_in_deque(s.body[0], head))
        {
            GameOver();
        }
    }
};

int main()
{

    InitWindow( cell * cellCount,  cell * cellCount, "snakiest");
    SetTargetFPS(60);

    Game g = Game();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if (triggered(0.1))
        {
            g.update();
        }

        if (IsKeyPressed(KEY_UP) && g.s.dir.y != 1)
        {
            g.s.dir = {0, -1};
            g.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && g.s.dir.y != -1)
        {
            g.s.dir = {0, 1};
            g.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && g.s.dir.x != 1)
        {
            g.s.dir = {-1, 0};
            g.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && g.s.dir.x != -1)
        {
            g.s.dir = {1, 0};
            g.running = true;
        }

        ClearBackground(BLACK);
        DrawRectangleLinesEx(Rectangle{ (float)cell * cellCount + 10, (float)cell * cellCount + 10}, 5, darkgreen);

        DrawText(" sankiest ",70 , 20, 40, RAYWHITE);
        DrawText(TextFormat(" Score : %i ", g.score), 70, 700, 40, GRAY);
        g.draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}