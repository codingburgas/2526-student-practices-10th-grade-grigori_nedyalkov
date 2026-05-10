#include "App.h"

App::App()
{
    data.SeedData();
    currentState = MAIN_MENU;
    selectedMovieId = -1;
    selectedShowId = -1;
}

bool App::IsButtonClicked(Rectangle btn)
{
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, btn)
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void App::Display()
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(1920, 1080, "Movie Ticket Booking System");
    SetTargetFPS(60);

    while (!WindowShouldClose() && currentState != EXIT)
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
    switch (currentState)
    {
    case MAIN_MENU:      UpdateMainMenu();     break;
    case MOVIE:          UpdateMovieScreen();  break;
    case SEAT_SELECTION: UpdateSeatSelection(); break;
    case BOOKING:        UpdateBooking();      break;
    default: break;
    }
}

void App::Draw()
{
    switch (currentState)
    {
    case MAIN_MENU:      DrawMainMenu();      break;
    case MOVIE:          DrawMovieScreen();   break;
    case SEAT_SELECTION: DrawSeatSelection(); break;
    case BOOKING:        DrawBooking();       break;
    default: break;
    }
}

void App::UpdateMainMenu()
{
    Rectangle browseBtn = { 760, 400, 400, 60 };
    Rectangle exitBtn = { 760, 500, 400, 60 };

    if (IsButtonClicked(browseBtn)) currentState = MOVIE;
    if (IsButtonClicked(exitBtn))   currentState = EXIT;
}

void App::DrawMainMenu()
{
    DrawText("MOVIE TICKET BOOKING", 600, 250, 40, DARKGRAY);

    Rectangle browseBtn = { 760, 400, 400, 60 };
    Rectangle exitBtn = { 760, 500, 400, 60 };

    DrawRectangleRec(browseBtn, DARKBLUE);
    DrawText("Browse Movies", 870, 418, 20, WHITE);

    DrawRectangleRec(exitBtn, MAROON);
    DrawText("Exit", 930, 518, 20, WHITE);
}

void App::UpdateMovieScreen()
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) { currentState = MAIN_MENU; return; }

    int y = 150;
    for (auto& movie : data.movies)
    {
        Rectangle btn = { 600, (float)y, 700, 50 };
        if (IsButtonClicked(btn))
        {
            selectedMovieId = movie.id;
            // Auto-select first show for this movie
            for (auto& show : data.shows)
            {
                if (show.movieId == movie.id)
                {
                    selectedShowId = show.id;
                    break;
                }
            }
            currentState = SEAT_SELECTION;
            return;
        }
        y += 70;
    }
}

void App::DrawMovieScreen()
{
    DrawText("Select a Movie", 820, 80, 30, DARKGRAY);

    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    int y = 150;
    for (auto& movie : data.movies)
    {
        Rectangle btn = { 600, (float)y, 700, 50 };
        DrawRectangleRec(btn, DARKBLUE);
        DrawText(movie.title.c_str(), 620, y + 8, 20, WHITE);
        DrawText(movie.genre.c_str(), 900, y + 8, 18, LIGHTGRAY);
        DrawText(movie.releaseDate.c_str(), 1150, y + 8, 18, LIGHTGRAY);
        y += 70;
    }
}
void App::UpdateSeatSelection()
{
    Rectangle backBtn = { 20,  20,  120, 40 };
    Rectangle confirmBtn = { 100, 700, 200, 60 };

    if (IsButtonClicked(backBtn)) { currentState = MOVIE; return; }

    Show* show = data.GetShowById(selectedShowId);
    if (!show) return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        for (auto& seat : show->seats)
        {
            int x = seatStartX + seat.number * (seatSize + seatPad);
            int y = seatStartY + seat.row * (seatSize + seatPad);
            Rectangle rect = { (float)x, (float)y, (float)seatSize, (float)seatSize };

            if (CheckCollisionPointRec(mouse, rect))
            {
                if (seat.status == AVAILABLE) { seat.status = LOCKED; seat.lockTime = time(0); }
                else if (seat.status == LOCKED)      seat.status = AVAILABLE;
                break;
            }
        }
    }
    time_t now = time(0);
    for (auto& seat : show->seats)
        if (seat.status == LOCKED && difftime(now, seat.lockTime) > 10)
            seat.status = AVAILABLE;

    if (IsButtonClicked(confirmBtn))
    {
        for (auto& seat : show->seats)
            if (seat.status == LOCKED)
                seat.status = BOOKED;

        currentState = BOOKING;
    }
}

void App::DrawSeatSelection()
{
    Show* show = data.GetShowById(selectedShowId);
    Movie* movie = data.GetMovieById(selectedMovieId);
    if (!show || !movie) return;

    string header = "Select Seats  -  " + movie->title + "  " + show->startTime;
    DrawText(header.c_str(), 100, 80, 24, DARKGRAY);

    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    DrawRectangle(100, 1000, 30, 30, GREEN);  DrawText("Available", 140, 1005, 18, DARKGRAY);
    DrawRectangle(300, 1000, 30, 30, ORANGE); DrawText("Selected", 340, 1005, 18, DARKGRAY);
    DrawRectangle(500, 1000, 30, 30, RED);    DrawText("Booked", 540, 1005, 18, DARKGRAY);

    for (auto& seat : show->seats)
    {
        int x = seatStartX + seat.number * (seatSize + seatPad);
        int y = seatStartY + seat.row * (seatSize + seatPad);
        Color color = (seat.status == AVAILABLE) ? GREEN
            : (seat.status == LOCKED) ? ORANGE
            : RED;
        DrawRectangle(x, y, seatSize, seatSize, color);
    }

    DrawRectangle(100, 110, 400, 12, DARKGRAY);
    DrawText("SCREEN", 260, 112, 14, WHITE);

    Rectangle confirmBtn = { 100, 700, 200, 60 };
    DrawRectangleRec(confirmBtn, DARKGREEN);
    DrawText("CONFIRM", 148, 718, 22, WHITE);
}

void App::UpdateBooking()
{
    Rectangle menuBtn = { 760, 600, 400, 60 };
    if (IsButtonClicked(menuBtn)) currentState = MAIN_MENU;
}

void App::DrawBooking()
{
    DrawText("Booking Confirmed!", 700, 350, 40, DARKGREEN);
    DrawText("Your seats have been reserved.", 650, 420, 24, DARKGRAY);

    Rectangle menuBtn = { 760, 600, 400, 60 };
    DrawRectangleRec(menuBtn, DARKBLUE);
    DrawText("Back to Main Menu", 820, 618, 20, WHITE);
}