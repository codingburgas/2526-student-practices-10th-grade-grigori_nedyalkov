#include "App.h"

App::App()
{
    data.SeedData();
}
bool App::IsConfirmButtonClicked()
{
    Rectangle btn = { 100, 400, 200, 60 };

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}
void App::Display()
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
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

void App::Update()
{
    Show* show = data.GetShowById(1);
    if (!show) return;

    int startX = 100;
    int startY = 100;
    int size = 40;
    int padding = 10;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();

        for (auto& seat : show->seats)
        {
            int x = startX + seat.number * (size + padding);
            int y = startY + seat.row * (size + padding);

            Rectangle rect = { (float)x, (float)y, (float)size, (float)size };

            if (CheckCollisionPointRec(mouse, rect))
            {
                if (seat.status == AVAILABLE)
                {
                    seat.status = LOCKED;
                    seat.lockTime = time(0);
                }
                else if (seat.status == LOCKED)
                {
                    seat.status = AVAILABLE;
                }
                break;
            }
        }
    }
    time_t now = time(0);

    for (auto& seat : show->seats)
    {
        if (seat.status == LOCKED)
        {
            double seconds = difftime(now, seat.lockTime);
            if (seconds > 10)
            {
                seat.status = AVAILABLE;
            }
        }
    }
    if (IsConfirmButtonClicked())
    {
        Show* show = data.GetShowById(1);
        if (!show) return;

        for (auto& seat : show->seats)
        {
            if (seat.status == LOCKED)
            {
                seat.status = BOOKED;
            }
        }
    }
}

void App::Draw()
{
    Show* show = data.GetShowById(1);
    if (!show) return;

    int startX = 100;
    int startY = 100;
    int size = 40;
    int padding = 10;

    Rectangle btn = { 100, 400, 200, 60 };

    DrawRectangleRec(btn, DARKGREEN);
    DrawText("CONFIRM", 120, 420, 20, WHITE);
    for (auto& seat : show->seats)
    {
        int x = startX + seat.number * (size + padding);
        int y = startY + seat.row * (size + padding);

        Color color = GREEN;

        if (seat.status == LOCKED) color = ORANGE;
        if (seat.status == BOOKED) color = RED;

        DrawRectangle(x, y, size, size, color);
    }
}