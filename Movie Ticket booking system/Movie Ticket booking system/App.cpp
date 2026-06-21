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
    DrawText("MOVIE TICKET BOOKING", 700, 250, 40, DARKGRAY);

    Rectangle browseBtn = { 760, 400, 400, 60 };
    Rectangle exitBtn = { 760, 500, 400, 60 };

    DrawRectangleRec(browseBtn, DARKBLUE);
    DrawText("Browse Movies", 870, 418, 20, WHITE);

    DrawRectangleRec(exitBtn, MAROON);
    DrawText("Exit", 930, 518, 20, WHITE);
}

void App::DrawMovieScreen()
{
    int screenW = GetScreenWidth();
    DrawText("Select a Movie", screenW / 2 - MeasureText("Select a Movie", 30) / 2, 60, 30, DARKGRAY);

    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    int btnW = 700;
    int btnX = screenW / 2 - btnW / 2;
    int y = 150;

    for (auto& movie : data.movies)
    {
        Rectangle btn = { (float)btnX, (float)y, (float)btnW, 50 };
        DrawRectangleRec(btn, DARKBLUE);

        DrawText(movie.title.c_str(), btnX + 20, y + 15, 20, WHITE);

        int genreX = btnX + btnW / 2 - MeasureText(movie.genre.c_str(), 18) / 2;
        DrawText(movie.genre.c_str(), genreX, y + 16, 18, LIGHTGRAY);

        int dateX = btnX + btnW - MeasureText(movie.releaseDate.c_str(), 18) - 20;
        DrawText(movie.releaseDate.c_str(), dateX, y + 16, 18, LIGHTGRAY);

        y += 70;
    }
}
void App::UpdateMovieScreen()
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) { currentState = MAIN_MENU; return; }

    int screenW = GetScreenWidth();
    int btnW = 700;
    int btnX = screenW / 2 - btnW / 2;
    int y = 150;

    for (auto& movie : data.movies)
    {
        Rectangle btn = { (float)btnX, (float)y, (float)btnW, 50 };
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

void App::UpdateSeatSelection()
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) { currentState = SHOWTIME_SELECTION; return; }

    Show* show = data.GetShowById(selectedShowId);
    if (!show) return;

    int screenW = GetScreenWidth();
    int cols = 0, rows = 0;
    for (auto& seat : show->seats)
    {
        if (seat.number + 1 > cols) cols = seat.number + 1;
        if (seat.row + 1 > rows) rows = seat.row + 1;
    }
    int gridW = cols * (seatSize + seatPad) - seatPad;
    int gridH = rows * (seatSize + seatPad) - seatPad;
    int gridX = screenW / 2 - gridW / 2;
    int gridY = 160;

    // Click a seat
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        for (auto& seat : show->seats)
        {
            int x = gridX + seat.number * (seatSize + seatPad);
            int y = gridY + seat.row * (seatSize + seatPad);
            Rectangle rect = { (float)x, (float)y, (float)seatSize, (float)seatSize };
            if (CheckCollisionPointRec(mouse, rect))
            {
                if (seat.status == AVAILABLE) { seat.status = LOCKED; seat.lockTime = time(0); }
                else if (seat.status == LOCKED)      seat.status = AVAILABLE;
                break;
            }
        }
    }

    // Expire locks after 10 seconds
    std::time_t now = std::time(nullptr);
    for (auto& seat : show->seats)
        if (seat.status == LOCKED && difftime(now, seat.lockTime) > 10)
            seat.status = AVAILABLE;

    float total = 0;
    for (auto& seat : show->seats)
        if (seat.status == LOCKED || seat.status == BOOKED)
            total += GetSeatPrice(seat.type);

    int totalY = gridY + gridH + 40;
    int confirmW = 200, confirmH = 60;
    Rectangle confirmBtn = {
        (float)(screenW / 2 - confirmW / 2),
        (float)(totalY + 50),
        (float)confirmW,
        (float)confirmH
    };

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

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    int cols = 0;
    int rows = 0;
    for (auto& seat : show->seats)
    {
        if (seat.number + 1 > cols) cols = seat.number + 1;
        if (seat.row + 1 > rows) rows = seat.row + 1;
    }
    int gridW = cols * (seatSize + seatPad) - seatPad;
    int gridH = rows * (seatSize + seatPad) - seatPad;
    int gridX = screenW / 2 - gridW / 2;
    int gridY = 160;

    // Title
    std::string header = "Select Seats  -  " + movie->title + "  " + show->startTime;
    DrawText(header.c_str(), screenW / 2 - MeasureText(header.c_str(), 24) / 2, 80, 24, DARKGRAY);

    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    int screenBarW = gridW;
    DrawRectangle(gridX, gridY - 30, screenBarW, 14, DARKGRAY);
    DrawText("SCREEN", gridX + screenBarW / 2 - MeasureText("SCREEN", 14) / 2, gridY - 28, 14, WHITE);

    // Seats
    for (auto& seat : show->seats)
    {
        int x = gridX + seat.number * (seatSize + seatPad);
        int y = gridY + seat.row * (seatSize + seatPad);

        Color color;
        if (seat.status == LOCKED)     color = ORANGE;
        else if (seat.status == BOOKED)     color = RED;
        else if (seat.type == PLATINUMTIER) color = PURPLE;
        else if (seat.type == GOLDTIER)     color = GOLD;
        else                                color = GRAY;

        DrawRectangle(x, y, seatSize, seatSize, color);
    }

    float total = 0;
    for (auto& seat : show->seats)
        if (seat.status == LOCKED || seat.status == BOOKED)
            total += GetSeatPrice(seat.type);
    std::string totalText = "Total: $" + std::to_string((int)total);
    int totalY = gridY + gridH + 40;
    DrawText(totalText.c_str(), screenW / 2 - MeasureText(totalText.c_str(), 28) / 2, totalY, 28, DARKBLUE);

    int confirmW = 200, confirmH = 60;
    Rectangle confirmBtn = {
        (float)(screenW / 2 - confirmW / 2),
        (float)(totalY + 50),
        (float)confirmW,
        (float)confirmH
    };
    DrawRectangleRec(confirmBtn, DARKGREEN);
    DrawText("CONFIRM", confirmBtn.x + confirmW / 2 - MeasureText("CONFIRM", 22) / 2, confirmBtn.y + 18, 22, WHITE);

    int legendY = (int)confirmBtn.y + confirmH + 30;
    int legendStartX = screenW / 2 - 250;

    DrawRectangle(legendStartX, legendY, 28, 28, GRAY);   DrawText("Silver  $8", legendStartX + 36, legendY + 5, 18, DARKGRAY);
    DrawRectangle(legendStartX + 170, legendY, 28, 28, GOLD);   DrawText("Gold    $12", legendStartX + 206, legendY + 5, 18, DARKGRAY);
    DrawRectangle(legendStartX + 340, legendY, 28, 28, PURPLE); DrawText("Platinum $18", legendStartX + 376, legendY + 5, 18, DARKGRAY);

    int legend2Y = legendY + 40;
    DrawRectangle(legendStartX, legend2Y, 28, 28, GRAY);   DrawText("Available", legendStartX + 36, legend2Y + 5, 18, DARKGRAY);
    DrawRectangle(legendStartX + 170, legend2Y, 28, 28, ORANGE); DrawText("Selected", legendStartX + 206, legend2Y + 5, 18, DARKGRAY);
    DrawRectangle(legendStartX + 340, legend2Y, 28, 28, RED);    DrawText("Booked", legendStartX + 376, legend2Y + 5, 18, DARKGRAY);
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

void App::DrawShowtimeSelection()
{
    Movie* movie = data.GetMovieById(selectedMovieId);
    if (!movie) return;

    int screenW = GetScreenWidth();

    std::string header = "Showtimes  -  " + movie->title;
    DrawText(header.c_str(), screenW / 2 - MeasureText(header.c_str(), 30) / 2, 60, 30, DARKGRAY);

    std::string genreStr = "Genre: " + movie->genre;
    std::string langStr = "Language: " + movie->language;
    std::string relStr = "Released: " + movie->releaseDate;
    DrawText(genreStr.c_str(), screenW / 2 - 280, 105, 20, GRAY);
    DrawText(langStr.c_str(), screenW / 2 - 50, 105, 20, GRAY);
    DrawText(relStr.c_str(), screenW / 2 + 160, 105, 20, GRAY);

    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, GRAY);
    DrawText("< Back", 35, 30, 20, WHITE);

    std::vector<Show*> movieShows = data.GetShowsByMovieId(selectedMovieId);
    if (movieShows.empty())
    {
        DrawText("No showtimes available.", screenW / 2 - 150, 400, 24, GRAY);
        return;
    }

    int btnW = 600;
    int btnX = screenW / 2 - btnW / 2;
    int y = 180;

    for (auto& show : movieShows)
    {
        Rectangle btn = { (float)btnX, (float)y, (float)btnW, 55 };
        Vector2 mouse = GetMousePosition();
        Color btnColor = CheckCollisionPointRec(mouse, btn) ? DARKBLUE : BLUE;
        DrawRectangleRec(btn, btnColor);

        DrawText(("Time: " + show->startTime).c_str(), btnX + 20, y + 17, 22, WHITE);

        Hall* hall = data.GetHallById(show->hallId);
        if (hall)
        {
            int hallX = btnX + btnW / 2 - MeasureText(hall->name.c_str(), 20) / 2;
            DrawText(hall->name.c_str(), hallX, y + 17, 20, YELLOW);
        }

        int available = 0;
        for (auto& seat : show->seats)
            if (seat.status == AVAILABLE) available++;
        std::string seatsText = "Seats: " + std::to_string(available);
        int seatsX = btnX + btnW - MeasureText(seatsText.c_str(), 20) - 20;
        DrawText(seatsText.c_str(), seatsX, y + 17, 20, LIGHTGRAY);

        y += 80;
    }
}
void App::UpdateShowtimeSelection()
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) { currentState = MOVIE; return; }

    Movie* movie = data.GetMovieById(selectedMovieId);
    if (!movie) return;

    int screenW = GetScreenWidth();
    int btnW = 600;
    int btnX = screenW / 2 - btnW / 2;
    int y = 180;

    std::vector<Show*> movieShows = data.GetShowsByMovieId(selectedMovieId);
    for (auto& show : movieShows)
    {
        Rectangle btn = { (float)btnX, (float)y, (float)btnW, 55 };
        if (IsButtonClicked(btn))
        {
            selectedShowId = show->id;
            currentState = SEAT_SELECTION;
            return;
        }
        y += 80;
    }
}