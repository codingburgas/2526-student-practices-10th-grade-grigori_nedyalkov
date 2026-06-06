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
    case MAIN_MENU:          UpdateMainMenu();          break;
    case MOVIE:              UpdateMovieScreen();       break;
    case SHOWTIME_SELECTION: UpdateShowtimeSelection(); break;
    case SEAT_SELECTION:     UpdateSeatSelection();     break;
    case BOOKING:            UpdateBooking();           break;
    default: break;
    }
}

void App::Draw()
{
    switch (currentState)
    {
    case MAIN_MENU:          DrawMainMenu();          break;
    case MOVIE:              DrawMovieScreen();       break;
    case SHOWTIME_SELECTION: DrawShowtimeSelection(); break;
    case SEAT_SELECTION:     DrawSeatSelection();     break;
    case BOOKING:            DrawBooking();           break;
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
        Rectangle btn = {600, (float)y, 700, 50 };
        if (IsButtonClicked(btn))
        {
            selectedMovieId = movie.id;
            selectedShowId = -1;
            currentState = SHOWTIME_SELECTION;
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

    // Title
    string header = "Select Seats  -  " + movie->title + "  " + show->startTime;
    DrawText(header.c_str(), 100, 80, 24, DARKGRAY);

    // Back
    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    // Screen
    DrawRectangle(100, 110, 400, 12, DARKGRAY);
    DrawText("SCREEN", 260, 112, 14, WHITE);

    // Seats
    for (auto& seat : show->seats)
    {
        int x = seatStartX + seat.number * (seatSize + seatPad);
        int y = seatStartY + seat.row * (seatSize + seatPad);

        Color color;
        if (seat.status == LOCKED)     color = ORANGE;
        else if (seat.status == BOOKED)     color = RED;
        else if (seat.type == PLATINUMTIER) color = PURPLE;
        else if (seat.type == GOLDTIER)     color = GOLD;
        else                                color = GRAY;

        DrawRectangle(x, y, seatSize, seatSize, color);
    }

    // Total
    float total = 0;
    for (auto& seat : show->seats)
        if (seat.status == LOCKED || seat.status == BOOKED)
            total += GetSeatPrice(seat.type);

    string totalText = "Total: $" + to_string((int)total);
    DrawText(totalText.c_str(), 100, 870, 28, DARKBLUE);

    // Seat type legend with prices
    DrawRectangle(100, 950, 30, 30, GRAY);   DrawText("Silver $8", 140, 955, 18, DARKGRAY);
    DrawRectangle(300, 950, 30, 30, GOLD);   DrawText("Gold $12", 340, 955, 18, DARKGRAY);
    DrawRectangle(500, 950, 30, 30, PURPLE); DrawText("Platinum $18", 540, 955, 18, DARKGRAY);

    // Available/Selected/Booked legend
    DrawRectangle(100, 1000, 30, 30, GRAY);   DrawText("Available", 140, 1005, 18, DARKGRAY);
    DrawRectangle(300, 1000, 30, 30, ORANGE); DrawText("Selected", 340, 1005, 18, DARKGRAY);
    DrawRectangle(500, 1000, 30, 30, RED);    DrawText("Booked", 540, 1005, 18, DARKGRAY);

    // Confirm
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

void App::UpdateShowtimeSelection()
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) { currentState = MOVIE; return; }

    Movie* movie = data.GetMovieById(selectedMovieId);
    if (!movie) return;

    vector<Show*> movieShows = data.GetShowsByMovieId(selectedMovieId);

    int y = 200;
    for (auto& show : movieShows)
    {
        Rectangle btn = { 660, (float)y, 600, 55 };
        if (IsButtonClicked(btn))
        {
            selectedShowId = show->id;
            currentState = SEAT_SELECTION;
            return;
        }
        y += 80;
    }
}

void App::DrawShowtimeSelection()
{
    Movie* movie = data.GetMovieById(selectedMovieId);
    if (!movie) return;

    // Title
    string header = "Showtimes  -  " + movie->title;
    DrawText(header.c_str(), 600, 100, 30, DARKGRAY);

    // Movie details
    DrawText(("Genre: " + movie->genre).c_str(), 620, 145, 20, GRAY);
    DrawText(("Language: " + movie->language).c_str(), 850, 145, 20, GRAY);
    DrawText(("Released: " + movie->releaseDate).c_str(), 1050, 145, 20, GRAY);

    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    vector<Show*> movieShows = data.GetShowsByMovieId(selectedMovieId);

    if (movieShows.empty())
    {
        DrawText("No showtimes available.", 700, 400, 24, GRAY);
        return;
    }

    int y = 200;
    for (auto& show : movieShows)
    {
        Rectangle btn = { 660, (float)y, 600, 55 };
        Vector2 mouse = GetMousePosition();
        Color btnColor = CheckCollisionPointRec(mouse, btn) ? DARKBLUE : BLUE;

        DrawRectangleRec(btn, btnColor);

        // Show time
        DrawText(("Time: " + show->startTime).c_str(), 680, y + 10, 22, WHITE);

        // Count available seats
        int available = 0;
        for (auto& seat : show->seats)
            if (seat.status == AVAILABLE) available++;

        string seatsText = "Seats available: " + to_string(available);
        DrawText(seatsText.c_str(), 980, y + 10, 20, LIGHTGRAY);

        // Hall info
        Hall* hall = data.GetHallById(show->hallId);
        if (hall)
            DrawText(hall->name.c_str(), 850, y + 10, 20, YELLOW);

        y += 80;
    }
}