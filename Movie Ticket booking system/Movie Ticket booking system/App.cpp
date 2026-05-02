#include "App.h"

App::App()
{
    data.SeedData();
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
    Show* show = data.GetShowById(1);
    if (!show) return;

    int startX = 100;
    int startY = 100;
    int size = 40;
    int padding = 10;

    for (auto& seat : show->seats) {
        int x = startX + seat.number * (size + padding);
        int y = startY + seat.row * (size + padding);

        Color color = GREEN;

        if (seat.status == LOCKED) color = ORANGE;
        if (seat.status == BOOKED) color = RED;

        DrawRectangle(x, y, size, size, color);
    }
}

void App::Update()
{
}
