#include "App.h"

App::App()
{
}

void App::Display()
{
    InitWindow(1920, 1080, "Movie Ticket booking system");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Update();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Draw();
        EndDrawing();
    }
    CloseWindow();
}

void App::Draw()
{
}

void App::Update()
{
}
