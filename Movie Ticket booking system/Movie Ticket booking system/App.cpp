#include "App.h"
#include <algorithm>
#include <unordered_set>
#include <cctype>
#include <sstream>
#include <vector>

App::App()
{
    data.SeedData();
    currentState = MAIN_MENU;
    selectedMovieId = -1;
    selectedShowId = -1;
}

// Search screen removed; search now integrated into Browse Movies only.

void App::PerformFilter()
{
    // build languages/genres lists once if empty
    if (languages.empty() || genres.empty() || dates.empty()) {
        std::unordered_set<std::string> langSet, genreSet, dateSet;
        for (auto &m : data.movies) {
            langSet.insert(m.language);
            genreSet.insert(m.genre);
            dateSet.insert(m.releaseDate);
        }
        languages.clear(); genres.clear(); dates.clear();
        languages.push_back("All");
        for (auto &l : langSet) languages.push_back(l);
        genres.push_back("All");
        for (auto &g : genreSet) genres.push_back(g);
        dates.push_back("All");
        for (auto &d : dateSet) dates.push_back(d);
        // ensure selected indices valid
        if (selectedLanguageIdx >= (int)languages.size()) selectedLanguageIdx = 0;
        if (selectedGenreIdx >= (int)genres.size()) selectedGenreIdx = 0;
        if (selectedDateIdx >= (int)dates.size()) selectedDateIdx = 0;
    }

    filteredMovies.clear();
    filteredMovies.reserve(data.movies.size());

    std::string lowerSearch = searchText;
    // to lower for case-insensitive match
    std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), [](unsigned char c){ return (char)std::tolower(c); });

    for (auto &m : data.movies) {
        // text filter
        std::string titleLower = m.title;
        std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), [](unsigned char c){ return (char)std::tolower(c); });
        if (!lowerSearch.empty() && titleLower.find(lowerSearch) == std::string::npos) continue;

        // language filter
        if (selectedLanguageIdx > 0) {
            if (m.language != languages[selectedLanguageIdx]) continue;
        }

        // genre filter
        if (selectedGenreIdx > 0) {
            if (m.genre != genres[selectedGenreIdx]) continue;
        }

        // date filter
        if (selectedDateIdx > 0) {
            if (m.releaseDate != dates[selectedDateIdx]) continue;
        }

        filteredMovies.push_back(&m);
    }

    filtersDirty = false;
}

// UpdateSearch removed - search functionality now exists only in UpdateMovieScreen

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
    Rectangle exitBtn = { 760, 520, 400, 60 };

    if (IsButtonClicked(browseBtn)) currentState = MOVIE;
    if (IsButtonClicked(exitBtn))   currentState = EXIT;
}

void App::DrawMainMenu()
{
    DrawText("MOVIE TICKET BOOKING", 700, 250, 40, DARKGRAY);

    Rectangle browseBtn = { 760, 400, 400, 60 };
    Rectangle exitBtn = { 760, 520, 400, 60 };

    DrawRectangleRec(browseBtn, DARKBLUE);
    DrawText("Browse Movies", 870, 418, 20, WHITE);

    DrawRectangleRec(exitBtn, MAROON);
    DrawText("Exit", 930, 538, 20, WHITE);
}

void App::DrawMovieScreen()
{
    int screenW = GetScreenWidth();

    // Page header
    const int headerY = 28;
    DrawText("Select a Movie", screenW / 2 - MeasureText("Select a Movie", 36) / 2, headerY, 36, DARKGRAY);

    // Back button (subtle)
    Rectangle backBtn = { 20, 20, 120, 40 };
    DrawRectangleRec(backBtn, Fade(GRAY, 0.95f));
    DrawRectangleLines((int)backBtn.x, (int)backBtn.y, (int)backBtn.width, (int)backBtn.height, DARKGRAY);
    DrawText("< Back", 36, 30, 20, WHITE);

    // Search area (clean, elevated)
    Rectangle inputBox = { 220, 70, screenW - 440, 50 };
    DrawRectangleRec(inputBox, RAYWHITE);
    DrawRectangleLines((int)inputBox.x, (int)inputBox.y, (int)inputBox.width, (int)inputBox.height, Fade(DARKGRAY, 0.6f));

    // Magnifier icon
    int magX = inputBox.x + 18;
    int magY = inputBox.y + inputBox.height/2;
    DrawCircle(magX, magY, 8, Fade(DARKBLUE, 0.9f));
    DrawLine(magX + 6, magY + 6, magX + 14, magY + 14, Fade(DARKBLUE, 0.9f));

    // Search text and cursor
    std::string displayText = searchText.empty() ? "Type title to search..." : searchText;
    Color textColor = searchText.empty() ? Fade(DARKGRAY, 0.7f) : DARKGRAY;
    DrawText(displayText.c_str(), (int)inputBox.x + 40, (int)inputBox.y + 12, 22, textColor);
    if (cursorVisible && !searchText.empty()) {
        int textW = MeasureText(displayText.c_str(), 22);
        DrawRectangle(inputBox.x + 40 + textW, inputBox.y + 12, 2, 26, DARKGRAY);
    }

    // Filter pills (flat design)
    int pillY = (int)inputBox.y + (int)inputBox.height + 12;
    Rectangle langBtn = { (float)inputBox.x, (float)pillY, 160, 36 };
    Rectangle genreBtn = { (float)inputBox.x + 180, (float)pillY, 160, 36 };
    Rectangle dateBtn = { (float)inputBox.x + 360, (float)pillY, 160, 36 };
    DrawRectangleRec(langBtn, Fade(BLUE, 0.85f));
    DrawRectangleRec(genreBtn, Fade(BLUE, 0.85f));
    DrawRectangleRec(dateBtn, Fade(BLUE, 0.85f));
    DrawText((std::string("Language: ") + (languages.empty() ? "All" : languages[selectedLanguageIdx])).c_str(), (int)langBtn.x + 12, (int)langBtn.y + 8, 16, WHITE);
    DrawText((std::string("Genre: ") + (genres.empty() ? "All" : genres[selectedGenreIdx])).c_str(), (int)genreBtn.x + 12, (int)genreBtn.y + 8, 16, WHITE);
    DrawText((std::string("Year: ") + (dates.empty() ? "All" : dates[selectedDateIdx])).c_str(), (int)dateBtn.x + 12, (int)dateBtn.y + 8, 16, WHITE);

    // Ensure filtered list is up to date
    if (filtersDirty) PerformFilter();

    // Results panel (centered)
    int resultsX = (int)inputBox.x;
    int resultsY = pillY + 56;
    int btnW = (int)inputBox.width;
    int itemH = 82;
    int visibleCount = (GetScreenHeight() - resultsY - 40) / itemH;

    // Draw visible results with hover highlight and clean typography
    Vector2 mouse = GetMousePosition();
    int y = resultsY;
    int idx = 0;
    for (auto mptr : filteredMovies)
    {
        if (idx++ < resultsScroll) continue;
        if (idx - resultsScroll > visibleCount) break;

        Rectangle item = { (float)resultsX, (float)y, (float)btnW, (float)itemH };
        bool hover = CheckCollisionPointRec(mouse, item);
        Color base = hover ? Fade(DARKBLUE, 0.95f) : Fade(BLUE, 0.88f);
        // draw subtle border behind the item so it doesn't overlay item content
        Rectangle borderRect = { item.x - 2, item.y - 2, item.width + 4, item.height + 4 };
        DrawRectangleRec(borderRect, Fade(DARKGRAY, 0.12f));
        DrawRectangleRec(item, base);

        // Title: up to 3 wrapped lines
        int titleW = btnW - 260;
        std::vector<std::string> lines;
        {
            std::istringstream iss(mptr->title);
            std::string word, cur;
            while (iss >> word) {
                if (cur.empty()) cur = word;
                else {
                    std::string cand = cur + " " + word;
                    if (MeasureText(cand.c_str(), 20) <= titleW) cur = cand;
                    else { lines.push_back(cur); cur = word; }
                }
            }
            if (!cur.empty()) lines.push_back(cur);
        }
        for (int li = 0; li < (int)lines.size() && li < 3; ++li) {
            DrawText(lines[li].c_str(), resultsX + 18, y + 10 + li * 20, 20, WHITE);
        }

        // Genre and year
        DrawText(mptr->genre.c_str(), resultsX + btnW/2 - MeasureText(mptr->genre.c_str(), 16)/2, y + 24, 16, Fade(RAYWHITE, 0.9f));
        DrawText(mptr->releaseDate.c_str(), resultsX + btnW - MeasureText(mptr->releaseDate.c_str(), 16) - 20, y + 24, 16, Fade(RAYWHITE, 0.9f));

        y += itemH;
    }
}
void App::UpdateMovieScreen()
{
    // Back button
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) { currentState = MAIN_MENU; return; }

    // Handle text input for search
    int ch = 0;
    bool changed = false;
    while ((ch = GetCharPressed()) > 0)
    {
        if (ch >= 32 && ch <= 125)
        {
            searchText.push_back((char)ch);
            changed = true;
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !searchText.empty()) { searchText.pop_back(); changed = true; }

    // cursor timer
    cursorTimer += GetFrameTime();
    if (cursorTimer >= 0.5f) { cursorVisible = !cursorVisible; cursorTimer = 0.0f; }

    // compute layout to match DrawMovieScreen
    int screenW = GetScreenWidth();
    Rectangle inputBox = { 220, 70, screenW - 440, 50 };
    int pillY = (int)inputBox.y + (int)inputBox.height + 12;
    // language/genre/date toggles (matching draw positions)
    Rectangle langBtn = { (float)inputBox.x, (float)pillY, 160, 36 };
    Rectangle genreBtn = { (float)inputBox.x + 180, (float)pillY, 160, 36 };
    Rectangle dateBtn = { (float)inputBox.x + 360, (float)pillY, 160, 36 };
    if (IsButtonClicked(langBtn)) { selectedLanguageIdx = (selectedLanguageIdx + 1) % (std::max(1, (int)languages.size())); changed = true; }
    if (IsButtonClicked(genreBtn)) { selectedGenreIdx = (selectedGenreIdx + 1) % (std::max(1, (int)genres.size())); changed = true; }
    if (IsButtonClicked(dateBtn)) { selectedDateIdx = (selectedDateIdx + 1) % (std::max(1, (int)dates.size())); changed = true; }

    // mouse wheel scroll
    float wheel = GetMouseWheelMove();
    int resultsX = (int)inputBox.x;
    int resultsY = pillY + 56;
    int btnW = (int)inputBox.width;
    int itemH = 82;
    int visibleCount = (GetScreenHeight() - resultsY - 40) / itemH;

    if (wheel != 0 && !filteredMovies.empty()) {
        resultsScroll -= (int)wheel * 3;
        if (resultsScroll < 0) resultsScroll = 0;
        int maxScroll = std::max(0, (int)filteredMovies.size() - visibleCount);
        if (resultsScroll > maxScroll) resultsScroll = maxScroll;
    }

    if (changed) filtersDirty = true;
    if (filtersDirty) PerformFilter();

    // Click handling on visible results (use same layout as Draw)
    int y = resultsY;
    int idx = 0;
    for (auto mptr : filteredMovies)
    {
        if (idx++ < resultsScroll) { y += itemH; continue; }
        if (idx - resultsScroll > visibleCount) break;
        Rectangle btn = { (float)resultsX, (float)y, (float)btnW, (float)itemH };
        if (IsButtonClicked(btn))
        {
            selectedMovieId = mptr->id;
            selectedShowId = -1;
            currentState = SHOWTIME_SELECTION;
            return;
        }
        y += itemH;
        if (y > GetScreenHeight()) break;
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