#include "Menu.h"
#include "App.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_set>
#include <ctime>

// Helpers

bool IsButtonClicked(Rectangle btn) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return false;
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, btn);
}

void DrawButton(Rectangle btn, Color color, const char* text, int fontSize) {
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, btn);
    Color bg = hover ? Fade(color, 0.7f) : color;
    DrawRectangleRounded(btn, 0.2f, 6, bg);
    int tw = MeasureText(text, fontSize);
    DrawText(text, (int)btn.x + ((int)btn.width - tw) / 2, (int)btn.y + ((int)btn.height - fontSize) / 2, fontSize, WHITE);
}

float GetSeatPrice(int seatType) {
    switch (seatType) {
    case 0: return 8.0f;
    case 1: return 12.0f;
    case 2: return 18.0f;
    default: return 0.0f;
    }
}

// MainMenuScreen

void MainMenuScreen::Update(App& app)
{
    int screenW = GetScreenWidth();
    int panelW = 440;
    int panelX = screenW / 2 - panelW / 2;
    int headerY = 160;
    int btnH = 50;
    int btnGap = 15;
    int btnY = headerY + 120;

    Rectangle browseBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
    if (IsButtonClicked(browseBtn)) { app.currentState = MOVIE; return; }

    btnY += btnH + btnGap;

    if (app.loggedInUserId == -1) {
        Rectangle loginRegisterBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        btnY += btnH + btnGap;
        Rectangle exitBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        if (IsButtonClicked(loginRegisterBtn)) { app.currentState = LOG_IN; return; }
        if (IsButtonClicked(exitBtn)) { app.currentState = EXIT; return; }
        return;
    }

    User* user = app.data.GetUserById(app.loggedInUserId);
    if (user && user->isAdmin) {
        Rectangle adminBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        btnY += btnH + btnGap;
        Rectangle myBookBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        btnY += btnH + btnGap;
        Rectangle logoutBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        btnY += btnH + btnGap;
        Rectangle exitBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        if (IsButtonClicked(adminBtn)) { app.currentState = ADMIN_PANEL; return; }
        if (IsButtonClicked(myBookBtn)) { app.currentState = MY_BOOKINGS; return; }
        if (IsButtonClicked(logoutBtn)) { app.loggedInUserId = -1; app.currentState = LOG_IN; return; }
        if (IsButtonClicked(exitBtn)) { app.currentState = EXIT; return; }
    } else {
        Rectangle myBookBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        btnY += btnH + btnGap;
        Rectangle logoutBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        btnY += btnH + btnGap;
        Rectangle exitBtn = { (float)panelX, (float)btnY, (float)panelW, (float)btnH };
        if (IsButtonClicked(myBookBtn)) { app.currentState = MY_BOOKINGS; return; }
        if (IsButtonClicked(logoutBtn)) { app.loggedInUserId = -1; app.currentState = LOG_IN; return; }
        if (IsButtonClicked(exitBtn)) { app.currentState = EXIT; return; }
    }
}

void MainMenuScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    int panelW = 440;
    int panelX = screenW / 2 - panelW / 2;

    int headerY = 160;
    DrawRectangleRounded({ (float)panelX, (float)headerY, (float)panelW, 100 }, 0.15f, 8, Fade(DARKBLUE, 0.1f));
    DrawRectangleRoundedLines({ (float)panelX, (float)headerY, (float)panelW, 100 }, 0.15f, 8, 2.0f, Fade(DARKBLUE, 0.2f));
    DrawText("MOVIE TICKET BOOKING", screenW / 2 - MeasureText("MOVIE TICKET BOOKING", 34) / 2, headerY + 22, 34, DARKBLUE);
    DrawText("Book your favorite movies in seconds", screenW / 2 - MeasureText("Book your favorite movies in seconds", 18) / 2, headerY + 64, 18, GRAY);

    if (app.loggedInUserId != -1) {
        User* user = app.data.GetUserById(app.loggedInUserId);
        if (user) {
            std::string welcome = "Welcome, " + user->name + "!";
            DrawText(welcome.c_str(), 20, 20, 24, DARKBLUE);
        }
    }

    int btnY = headerY + 120;
    int btnH = 50;
    int btnGap = 15;

    DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, DARKBLUE, "Browse Movies", 20);
    btnY += btnH + btnGap;

    if (app.loggedInUserId == -1) {
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, DARKGREEN, "Log In / Register", 20);
        btnY += btnH + btnGap;
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, MAROON, "Exit", 20);
        return;
    }

    User* user = app.data.GetUserById(app.loggedInUserId);
    if (user && user->isAdmin) {
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, PURPLE, "Admin Panel", 20);
        btnY += btnH + btnGap;
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, DARKGREEN, "My Bookings", 20);
        btnY += btnH + btnGap;
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, MAROON, "Log Out", 20);
        btnY += btnH + btnGap;
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, MAROON, "Exit", 20);
    } else {
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, DARKGREEN, "My Bookings", 20);
        btnY += btnH + btnGap;
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, MAROON, "Log Out", 20);
        btnY += btnH + btnGap;
        DrawButton({ (float)panelX, (float)btnY, (float)panelW, (float)btnH }, MAROON, "Exit", 20);
    }
}

// LogInScreen

void LogInScreen::Update(App& app)
{
    int centerX = GetScreenWidth() / 2;
    int boxW = 400, boxH = 45;

    int panelY = 150;
    loginEmailBox.bounds = { (float)(centerX - boxW/2), (float)(panelY + 138), (float)boxW, (float)boxH };
    loginPassBox.bounds = { (float)(centerX - boxW/2), (float)(panelY + 228), (float)boxW, (float)boxH };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        loginEmailBox.isFocused = CheckCollisionPointRec(mouse, loginEmailBox.bounds);
        loginPassBox.isFocused = CheckCollisionPointRec(mouse, loginPassBox.bounds);
        if (loginPassBox.isFocused) loginEmailBox.isFocused = false;
    }

    loginEmailBox.Update();
    loginPassBox.Update();

    Rectangle loginBtn = { (float)(centerX - 100), (float)(panelY + 320), 200, 50 };
    if (IsButtonClicked(loginBtn)) {
        User* user = app.data.GetUserByEmail(loginEmailBox.text);
        if (user && user->password == loginPassBox.text) {
            app.loggedInUserId = user->id;
            loginError = "";
            loginEmailBox.text = "";
            loginPassBox.text = "";
            app.currentState = MAIN_MENU;
        } else {
            loginError = "Invalid email or password.";
        }
    }

    Rectangle guestBtn = { (float)(centerX - 125), (float)(panelY + 385), 250, 45 };
    if (IsButtonClicked(guestBtn)) {
        loginError = "";
        loginEmailBox.text = "";
        loginPassBox.text = "";
        app.currentState = MAIN_MENU;
    }

    Rectangle regBtn = { (float)(centerX - 155), (float)(panelY + 445), 310, 40 };
    if (IsButtonClicked(regBtn)) {
        loginError = "";
        loginEmailBox.text = "";
        loginPassBox.text = "";
        app.currentState = REGISTER;
    }
}

void LogInScreen::OnActivate(App& app) {
    loginEmailBox.text = "";
    loginPassBox.text = "";
    loginEmailBox.isFocused = false;
    loginPassBox.isFocused = false;
    loginError = "";
}

void LogInScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    int centerX = screenW / 2;
    int panelW = 460;
    int panelX = centerX - panelW / 2;
    int panelY = 150;
    int panelH = 530;

    DrawRectangleRounded({ (float)panelX, (float)panelY, (float)panelW, (float)panelH }, 0.15f, 8, Fade(DARKBLUE, 0.06f));
    DrawRectangleRoundedLines({ (float)panelX, (float)panelY, (float)panelW, (float)panelH }, 0.15f, 8, 2.0f, Fade(DARKBLUE, 0.15f));

    DrawText("Log In", centerX - MeasureText("Log In", 36) / 2, panelY + 32, 36, DARKBLUE);
    DrawText("Welcome back! Please enter your details.", centerX - MeasureText("Welcome back! Please enter your details.", 20) / 2, panelY + 78, 20, GRAY);

    DrawText("Email", centerX - 180, panelY + 120, 18, DARKGRAY);
    loginEmailBox.Draw();
    DrawText("Password", centerX - 180, panelY + 210, 18, DARKGRAY);
    loginPassBox.Draw();

    if (!loginError.empty()) {
        DrawText(loginError.c_str(), centerX - MeasureText(loginError.c_str(), 16) / 2, panelY + 300, 16, RED);
    }

    DrawButton({ (float)(centerX - 100), (float)(panelY + 320), 200, 50 }, DARKBLUE, "Log In", 22);
    DrawButton({ (float)(centerX - 125), (float)(panelY + 385), 250, 45 }, DARKGRAY, "Continue as Guest", 20);

    DrawButton({ (float)(centerX - 155), (float)(panelY + 445), 310, 40 }, GRAY, "Don't have an account? Register", 18);
}

// RegisterScreen

void RegisterScreen::OnActivate(App& app) {
    regNameBox.text = "";
    regEmailBox.text = "";
    regPassBox.text = "";
    regNameBox.isFocused = false;
    regEmailBox.isFocused = false;
    regPassBox.isFocused = false;
    registerError = "";
}

void RegisterScreen::Update(App& app)
{
    int centerX = GetScreenWidth() / 2;
    int boxW = 400, boxH = 45;

    int panelY = 140;
    regNameBox.bounds = { (float)(centerX - boxW/2), (float)(panelY + 128), (float)boxW, (float)boxH };
    regEmailBox.bounds = { (float)(centerX - boxW/2), (float)(panelY + 208), (float)boxW, (float)boxH };
    regPassBox.bounds = { (float)(centerX - boxW/2), (float)(panelY + 288), (float)boxW, (float)boxH };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        regNameBox.isFocused = CheckCollisionPointRec(mouse, regNameBox.bounds);
        regEmailBox.isFocused = CheckCollisionPointRec(mouse, regEmailBox.bounds);
        regPassBox.isFocused = CheckCollisionPointRec(mouse, regPassBox.bounds);
        if (regEmailBox.isFocused) { regNameBox.isFocused = false; regPassBox.isFocused = false; }
        if (regNameBox.isFocused) { regEmailBox.isFocused = false; regPassBox.isFocused = false; }
        if (regPassBox.isFocused) { regNameBox.isFocused = false; regEmailBox.isFocused = false; }
    }

    regNameBox.Update();
    regEmailBox.Update();
    regPassBox.Update();

    Rectangle regBtn = { (float)(centerX - 100), (float)(panelY + 370), 200, 50 };
    if (IsButtonClicked(regBtn)) {
        if (regNameBox.text.empty() || regEmailBox.text.empty() || regPassBox.text.empty()) {
            registerError = "Please fill in all fields.";
        } else if (app.data.GetUserByEmail(regEmailBox.text)) {
            registerError = "Email already registered.";
        } else {
            int newId = (int)app.data.GetUsers().size() + 1;
            User newUser;
            newUser.id = newId;
            newUser.name = regNameBox.text;
            newUser.email = regEmailBox.text;
            newUser.password = regPassBox.text;
            newUser.isAdmin = false;
            app.data.AddUser(newUser);
            app.loggedInUserId = newId;
            registerError = "";
            regNameBox.text = "";
            regEmailBox.text = "";
            regPassBox.text = "";
            app.currentState = MAIN_MENU;
        }
    }

    Rectangle loginBtn = { (float)(centerX - 150), (float)(panelY + 440), 300, 40 };
    if (IsButtonClicked(loginBtn)) {
        registerError = "";
        regNameBox.text = "";
        regEmailBox.text = "";
        regPassBox.text = "";
        app.currentState = LOG_IN;
    }
}

void RegisterScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    int centerX = screenW / 2;
    int panelW = 460;
    int panelX = centerX - panelW / 2;
    int panelY = 140;
    int panelH = 510;

    DrawRectangleRounded({ (float)panelX, (float)panelY, (float)panelW, (float)panelH }, 0.15f, 8, Fade(DARKGREEN, 0.06f));
    DrawRectangleRoundedLines({ (float)panelX, (float)panelY, (float)panelW, (float)panelH }, 0.15f, 8, 2.0f, Fade(DARKGREEN, 0.15f));

    DrawText("Register", centerX - MeasureText("Register", 36) / 2, panelY + 28, 36, DARKGREEN);
    DrawText("Create your account to get started.", centerX - MeasureText("Create your account to get started.", 20) / 2, panelY + 72, 20, GRAY);

    DrawText("Name", centerX - 180, panelY + 110, 18, DARKGRAY);
    regNameBox.Draw();
    DrawText("Email", centerX - 180, panelY + 190, 18, DARKGRAY);
    regEmailBox.Draw();
    DrawText("Password", centerX - 180, panelY + 270, 18, DARKGRAY);
    regPassBox.Draw();

    if (!registerError.empty()) {
        DrawText(registerError.c_str(), centerX - MeasureText(registerError.c_str(), 16) / 2, panelY + 340, 16, RED);
    }

    DrawButton({ (float)(centerX - 100), (float)(panelY + 370), 200, 50 }, DARKGREEN, "Register", 22);

    DrawButton({ (float)(centerX - 150), (float)(panelY + 440), 300, 40 }, GRAY, "Already have an account? Log In", 18);
}

// MovieScreen

void MovieScreen::OnActivate(App& app) {
    filtersDirty = true;
    resultsScroll = 0;
    languages.clear();
    genres.clear();
    dates.clear();
}

void MovieScreen::PerformFilter(App& app)
{
    if (languages.empty() || genres.empty() || dates.empty()) {
        std::unordered_set<std::string> langSet, genreSet, dateSet;
        for (auto& m : app.data.GetMovies()) {
            langSet.insert(m.language);
            genreSet.insert(m.genre);
            dateSet.insert(m.releaseDate);
        }
        languages.clear(); genres.clear(); dates.clear();
        languages.push_back("All");
        for (auto& l : langSet) languages.push_back(l);
        genres.push_back("All");
        for (auto& g : genreSet) genres.push_back(g);
        dates.push_back("All");
        for (auto& d : dateSet) dates.push_back(d);
        if (selectedLanguageIdx >= (int)languages.size()) selectedLanguageIdx = 0;
        if (selectedGenreIdx >= (int)genres.size()) selectedGenreIdx = 0;
        if (selectedDateIdx >= (int)dates.size()) selectedDateIdx = 0;
    }

    filteredMovies.clear();
    std::string lowerSearch = searchText;
    std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), [](unsigned char c) { return (char)std::tolower(c); });

    for (auto& m : app.data.GetMovies()) {
        std::string titleLower = m.title;
        std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), [](unsigned char c) { return (char)std::tolower(c); });
        if (!lowerSearch.empty() && titleLower.find(lowerSearch) == std::string::npos) continue;
        if (selectedLanguageIdx > 0 && m.language != languages[selectedLanguageIdx]) continue;
        if (selectedGenreIdx > 0 && m.genre != genres[selectedGenreIdx]) continue;
        if (selectedDateIdx > 0 && m.releaseDate != dates[selectedDateIdx]) continue;
        filteredMovies.push_back(&m);
    }
    filtersDirty = false;
}

void MovieScreen::Update(App& app)
{
    int screenW = GetScreenWidth();

    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) {
        app.currentState = MAIN_MENU;
        return;
    }

    Rectangle inputBox = { 220, 70, (float)(screenW - 440), 50 };
    int ch;
    while ((ch = GetCharPressed()) > 0) {
        if (ch >= 32 && ch <= 126) {
            searchText.push_back((char)ch);
            filtersDirty = true;
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !searchText.empty()) {
        searchText.pop_back();
        filtersDirty = true;
    }

    cursorTimer += GetFrameTime();
    if (cursorTimer >= 0.5f) {
        cursorVisible = !cursorVisible;
        cursorTimer = 0.0f;
    }

    int pillY = (int)inputBox.y + (int)inputBox.height + 12;
    Rectangle langBtn = { (float)inputBox.x, (float)pillY, 160, 36 };
    Rectangle genreBtn = { (float)inputBox.x + 180, (float)pillY, 160, 36 };
    Rectangle dateBtn = { (float)inputBox.x + 360, (float)pillY, 160, 36 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, langBtn) && !languages.empty()) {
            selectedLanguageIdx = (selectedLanguageIdx + 1) % (int)languages.size();
            filtersDirty = true;
        }
        if (CheckCollisionPointRec(mouse, genreBtn) && !genres.empty()) {
            selectedGenreIdx = (selectedGenreIdx + 1) % (int)genres.size();
            filtersDirty = true;
        }
        if (CheckCollisionPointRec(mouse, dateBtn) && !dates.empty()) {
            selectedDateIdx = (selectedDateIdx + 1) % (int)dates.size();
            filtersDirty = true;
        }
    }

    if (filtersDirty) PerformFilter(app);

    int resultsX = (int)inputBox.x;
    int resultsY = pillY + 56;
    int btnW = (int)inputBox.width;
    int itemH = 82;
    int visibleCount = (GetScreenHeight() - resultsY - 40) / itemH;

    int wheel = (int)GetMouseWheelMove();
    if (wheel != 0) {
        resultsScroll -= wheel;
        int maxScroll = (int)filteredMovies.size() - visibleCount;
        if (resultsScroll > maxScroll) resultsScroll = (maxScroll > 0) ? maxScroll : 0;
        if (resultsScroll < 0) resultsScroll = 0;
    }

    Vector2 mouse = GetMousePosition();
    int y = resultsY;
    int idx = 0;
    for (auto mptr : filteredMovies) {
        if (idx++ < resultsScroll) continue;
        if (idx - resultsScroll > visibleCount) break;
        Rectangle item = { (float)resultsX, (float)y, (float)btnW, (float)itemH };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, item)) {
            app.selectedMovieId = mptr->id;
            app.currentState = SHOWTIME_SELECTION;
            return;
        }
        y += itemH;
    }
}

void MovieScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();

    DrawText("Select a Movie", screenW / 2 - MeasureText("Select a Movie", 36) / 2, 28, 36, DARKGRAY);
    DrawButton({ 20, 20, 120, 40 }, GRAY, "< Back", 20);

    Rectangle inputBox = { 220, 70, (float)(screenW - 440), 50 };
    DrawRectangleRounded(inputBox, 0.2f, 6, RAYWHITE);
    DrawRectangleRoundedLines(inputBox, 0.2f, 6, 2.0f, Fade(DARKGRAY, 0.6f));

    int magX = (int)inputBox.x + 18;
    int magY = (int)inputBox.y + (int)inputBox.height / 2;
    DrawCircle(magX, magY, 8, Fade(DARKBLUE, 0.9f));
    DrawLine(magX + 6, magY + 6, magX + 14, magY + 14, Fade(DARKBLUE, 0.9f));

    std::string displayText = searchText.empty() ? "Type title to search..." : searchText;
    Color textColor = searchText.empty() ? Fade(DARKGRAY, 0.7f) : DARKGRAY;
    DrawText(displayText.c_str(), (int)inputBox.x + 40, (int)inputBox.y + 12, 22, textColor);
    if (cursorVisible && !searchText.empty()) {
        int textW = MeasureText(displayText.c_str(), 22);
        DrawRectangle((int)(inputBox.x + 40 + textW), (int)(inputBox.y + 12), 2, 26, DARKGRAY);
    }

    int pillY = (int)inputBox.y + (int)inputBox.height + 12;
    Rectangle langBtn = { (float)inputBox.x, (float)pillY, 160, 36 };
    Rectangle genreBtn = { (float)inputBox.x + 180, (float)pillY, 160, 36 };
    Rectangle dateBtn = { (float)inputBox.x + 360, (float)pillY, 160, 36 };
    DrawRectangleRounded(langBtn, 0.3f, 6, Fade(BLUE, 0.85f));
    DrawRectangleRounded(genreBtn, 0.3f, 6, Fade(BLUE, 0.85f));
    DrawRectangleRounded(dateBtn, 0.3f, 6, Fade(BLUE, 0.85f));
    DrawText((std::string("Language: ") + (languages.empty() ? "All" : languages[selectedLanguageIdx])).c_str(), (int)langBtn.x + 12, (int)langBtn.y + 8, 16, WHITE);
    DrawText((std::string("Genre: ") + (genres.empty() ? "All" : genres[selectedGenreIdx])).c_str(), (int)genreBtn.x + 12, (int)genreBtn.y + 8, 16, WHITE);
    DrawText((std::string("Year: ") + (dates.empty() ? "All" : dates[selectedDateIdx])).c_str(), (int)dateBtn.x + 12, (int)dateBtn.y + 8, 16, WHITE);

    if (filtersDirty) PerformFilter(app);

    int resultsX = (int)inputBox.x;
    int resultsY = pillY + 56;
    int btnW = (int)inputBox.width;
    int itemH = 82;
    int visibleCount = (GetScreenHeight() - resultsY - 40) / itemH;

    Vector2 mouse = GetMousePosition();
    int y = resultsY;
    int idx = 0;
    for (auto mptr : filteredMovies) {
        if (idx++ < resultsScroll) continue;
        if (idx - resultsScroll > visibleCount) break;
        Rectangle item = { (float)resultsX, (float)y, (float)btnW, (float)itemH };
        bool hover = CheckCollisionPointRec(mouse, item);
        Color base = hover ? Fade(DARKBLUE, 0.95f) : Fade(BLUE, 0.88f);
        DrawRectangleRounded(item, 0.2f, 6, base);

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

        std::string info = mptr->language + " | " + mptr->genre + " | " + mptr->releaseDate;
        DrawText(info.c_str(), resultsX + 18, y + itemH - 22, 16, Fade(WHITE, 0.8f));
        DrawText(("$" + std::to_string((int)GetSeatPrice(0)) + "+").c_str(), resultsX + btnW - 90, y + itemH / 2 - 10, 22, GOLD);
        y += itemH;
    }
}

// ShowtimeScreen

void ShowtimeScreen::Update(App& app)
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) {
        app.currentState = MOVIE;
        return;
    }

    int screenW = GetScreenWidth();
    int btnW = 600;
    int btnX = screenW / 2 - btnW / 2;
    int y = 180;

    std::vector<Show*> shows = app.data.GetShowsByMovieId(app.selectedMovieId);
    for (auto show : shows) {
        Rectangle showBtn = { (float)btnX, (float)y, (float)btnW, 55 };
        if (IsButtonClicked(showBtn)) {
            app.selectedShowId = show->id;
            app.currentState = SEAT_SELECTION;
            return;
        }
        y += 80;
    }
}

void ShowtimeScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    DrawButton({ 20, 20, 120, 40 }, GRAY, "< Back", 20);

    Movie* movie = app.data.GetMovieById(app.selectedMovieId);
    if (movie) {
        DrawText(movie->title.c_str(), screenW / 2 - MeasureText(movie->title.c_str(), 32) / 2, 50, 32, DARKBLUE);
        std::string info = movie->language + " | " + movie->genre + " | " + movie->releaseDate;
        DrawText(info.c_str(), screenW / 2 - MeasureText(info.c_str(), 18) / 2, 95, 18, GRAY);
    }

    DrawText("Select Showtime", screenW / 2 - MeasureText("Select Showtime", 26) / 2, 135, 26, DARKGRAY);

    int btnW = 600;
    int btnX = screenW / 2 - btnW / 2;
    int y = 180;

    std::vector<Show*> shows = app.data.GetShowsByMovieId(app.selectedMovieId);
    if (shows.empty()) {
        DrawText("No showtimes available for this movie.", screenW / 2 - MeasureText("No showtimes available for this movie.", 22) / 2, y + 10, 22, LIGHTGRAY);
        return;
    }

    for (auto show : shows) {
        Hall* hall = app.data.GetHallById(show->hallId);
        std::string label = show->startTime + "  |  Hall: " + (hall ? hall->name : "N/A");
        int price = (int)GetSeatPrice(2);
        std::string priceLabel = "From $" + std::to_string(price);
        DrawButton({ (float)btnX, (float)y, (float)btnW, 55 }, BLUE, label.c_str(), 20);
        DrawText(priceLabel.c_str(), btnX + btnW - 130, y + 16, 18, GOLD);
        y += 80;
    }
}

// SeatSelectionScreen

void SeatSelectionScreen::OnActivate(App& app) {
    app.selectedSeats.clear();
}

void SeatSelectionScreen::Update(App& app)
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) {
        app.currentState = SHOWTIME_SELECTION;
        return;
    }

    Show* show = app.data.GetShowById(app.selectedShowId);
    if (!show) { app.currentState = MOVIE; return; }

    time_t now = std::time(nullptr);
    for (auto& seat : show->seats) {
        if (seat.status == LOCKED && now - seat.lockTime > 10) {
            seat.status = AVAILABLE;
            seat.lockTime = 0;
            auto it = std::find(app.selectedSeats.begin(), app.selectedSeats.end(), seat.id);
            if (it != app.selectedSeats.end()) app.selectedSeats.erase(it);
        }
    }

    const int seatSize = 55;
    const int seatPad = 12;
    int cols = 8, rows = 6;
    int screenW = GetScreenWidth();
    int gridW = cols * (seatSize + seatPad) - seatPad;
    int gridH = rows * (seatSize + seatPad) - seatPad;
    int gridX = screenW / 2 - gridW / 2;
    int gridY = 160;

    Vector2 mouse = GetMousePosition();

    for (auto& seat : show->seats) {
        Rectangle seatRect = {
            (float)(gridX + seat.number * (seatSize + seatPad)),
            (float)(gridY + seat.row * (seatSize + seatPad)),
            (float)seatSize, (float)seatSize
        };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, seatRect)) {
            if (seat.status == AVAILABLE) {
                seat.status = LOCKED;
                seat.lockTime = std::time(nullptr);
                app.selectedSeats.push_back(seat.id);
            } else if (seat.status == LOCKED) {
                auto it = std::find(app.selectedSeats.begin(), app.selectedSeats.end(), seat.id);
                if (it != app.selectedSeats.end()) {
                    seat.status = AVAILABLE;
                    seat.lockTime = 0;
                    app.selectedSeats.erase(it);
                }
            }
        }
    }

    int totalY = gridY + gridH + 40;
    Rectangle confirmBtn = { (float)(screenW / 2 - 100), (float)(totalY + 50), 200, 55 };
    if (IsButtonClicked(confirmBtn) && !app.selectedSeats.empty()) {
        app.currentState = PAYMENT_SELECTION;
    }
}

void SeatSelectionScreen::Draw(App& app)
{
    DrawButton({ 20, 20, 120, 40 }, GRAY, "< Back", 20);

    Show* show = app.data.GetShowById(app.selectedShowId);
    if (!show) return;

    Movie* movie = app.data.GetMovieById(show->movieId);
    if (movie) {
        DrawText(movie->title.c_str(), 20, 80, 22, DARKBLUE);
        std::string showInfo = show->startTime + " | Hall: " + std::to_string(show->hallId);
        DrawText(showInfo.c_str(), 20, 110, 18, GRAY);
    }

    const int seatSize = 55;
    const int seatPad = 12;
    int cols = 8, rows = 6;
    int screenW = GetScreenWidth();
    int gridW = cols * (seatSize + seatPad) - seatPad;
    int gridH = rows * (seatSize + seatPad) - seatPad;
    int gridX = screenW / 2 - gridW / 2;
    int gridY = 160;

    int screenBarW = gridW - 60;
    int screenBarH = 24;
    int screenBarX = screenW / 2 - screenBarW / 2;
    int screenBarY = gridY - 50;
    DrawRectangleRounded({ (float)screenBarX, (float)screenBarY, (float)screenBarW, (float)screenBarH }, 0.5f, 8, Fade(LIGHTGRAY, 0.6f));
    DrawRectangleRoundedLines({ (float)screenBarX, (float)screenBarY, (float)screenBarW, (float)screenBarH }, 0.5f, 8, 1.5f, Fade(DARKGRAY, 0.3f));
    DrawText("SCREEN", screenW / 2 - MeasureText("SCREEN", 14) / 2, screenBarY + 5, 14, Fade(DARKGRAY, 0.7f));

    Vector2 mouse = GetMousePosition();

    for (auto& seat : show->seats) {
        Rectangle seatRect = {
            (float)(gridX + seat.number * (seatSize + seatPad)),
            (float)(gridY + seat.row * (seatSize + seatPad)),
            (float)seatSize, (float)seatSize
        };

        Color seatColor;
        switch (seat.type) {
            case PLATINUMTIER: seatColor = DARKPURPLE; break;
            case GOLDTIER: seatColor = GOLD; break;
            default: seatColor = LIGHTGRAY; break;
        }

        if (seat.status == BOOKED) {
            seatColor = Fade(RED, 0.4f);
        } else if (seat.status == LOCKED) {
            seatColor = Fade(GREEN, 0.6f);
        } else if (CheckCollisionPointRec(mouse, seatRect)) {
            seatColor = Fade(seatColor, 0.7f);
        }

        DrawRectangleRounded(seatRect, 0.15f, 6, seatColor);
        DrawRectangleRoundedLines(seatRect, 0.15f, 6, 1.5f, Fade(BLACK, 0.2f));
    }

    int legendY = gridY + gridH + 20;
    DrawText("Available", 20, legendY, 16, LIGHTGRAY);
    DrawRectangleRounded({ 120, (float)legendY, 20, 20 }, 0.2f, 6, LIGHTGRAY);
    DrawText("Selected", 160, legendY, 16, GREEN);
    DrawRectangleRounded({ 245, (float)legendY, 20, 20 }, 0.2f, 6, GREEN);
    DrawText("Booked", 285, legendY, 16, RED);
    DrawRectangleRounded({ 350, (float)legendY, 20, 20 }, 0.2f, 6, Fade(RED, 0.4f));

    DrawText("Silver: $8  Gold: $12  Platinum: $18", 20, legendY + 30, 16, DARKGRAY);

    float total = 0;
    for (int sid : app.selectedSeats) {
        Seat* s = show->GetSeatById(sid);
        if (s) total += GetSeatPrice(s->type);
    }
    int totalY = gridY + gridH + 40;
    std::string totalText = "Selected: " + std::to_string((int)app.selectedSeats.size()) + " seats  |  Total: $" + std::to_string((int)total);
    DrawText(totalText.c_str(), screenW / 2 - MeasureText(totalText.c_str(), 22) / 2, totalY, 22, DARKGRAY);

    DrawButton({ (float)(screenW / 2 - 100), (float)(totalY + 50), 200, 55 }, DARKGREEN, "Proceed to Payment", 18);
}

// PaymentScreen

void PaymentScreen::Update(App& app)
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) {
        app.currentState = SEAT_SELECTION;
        return;
    }

    Show* payShow = app.data.GetShowById(app.selectedShowId);
    if (payShow) {
        time_t now = std::time(nullptr);
        for (auto& seat : payShow->seats) {
            if (seat.status == LOCKED && now - seat.lockTime > 10) {
                seat.status = AVAILABLE;
                seat.lockTime = 0;
                auto it = std::find(app.selectedSeats.begin(), app.selectedSeats.end(), seat.id);
                if (it != app.selectedSeats.end()) app.selectedSeats.erase(it);
            }
        }
    }

    int screenW = GetScreenWidth();
    int toggleW = 130, gap = 10;
    int totalW = toggleW * 2 + gap;
    int toggleX = screenW / 2 - totalW / 2;
    int toggleY = 260;

    Rectangle onlineToggle = { (float)toggleX, (float)toggleY, (float)toggleW, 40 };
    Rectangle walkinToggle = { (float)(toggleX + toggleW + gap), (float)toggleY, (float)toggleW, 40 };

    if (IsButtonClicked(onlineToggle)) app.bookingMode = ONLINE;
    if (IsButtonClicked(walkinToggle)) app.bookingMode = WALK_IN;

    int btnW = 300;
    int centerX = screenW / 2 - btnW / 2;
    int ccY = 370;
    int cashY = 450;

    Rectangle ccBtn = { (float)centerX, (float)ccY, (float)btnW, 55 };
    Rectangle cashBtn = { (float)centerX, (float)cashY, (float)btnW, 55 };

    if (IsButtonClicked(ccBtn)) {
        app.selectedPayment = CREDIT_CARD;
        app.currentState = BOOKING;
    }
    if (IsButtonClicked(cashBtn) && app.bookingMode == WALK_IN) {
        app.selectedPayment = CASH;
        app.currentState = BOOKING;
    }
}

void PaymentScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    DrawButton({ 20, 20, 120, 40 }, GRAY, "< Back", 20);

    Show* show = app.data.GetShowById(app.selectedShowId);
    if (show) {
        Movie* movie = app.data.GetMovieById(show->movieId);
        if (movie) {
            DrawText(movie->title.c_str(), screenW / 2 - MeasureText(movie->title.c_str(), 26) / 2, 70, 26, DARKBLUE);
        }
    }
    int toggleW = 130, gap = 10;
    int totalW = toggleW * 2 + gap;
    int toggleX = screenW / 2 - totalW / 2;
    int toggleY = 260;

    DrawText("Booking Mode", screenW / 2 - MeasureText("Booking Mode", 22) / 2, 210, 22, DARKGRAY);

    Color onlineCol = (app.bookingMode == ONLINE) ? DARKBLUE : GRAY;
    Color walkinCol = (app.bookingMode == WALK_IN) ? DARKBLUE : GRAY;
    DrawButton({ (float)toggleX, (float)toggleY, (float)toggleW, 40 }, onlineCol, "Online", 18);
    DrawButton({ (float)(toggleX + toggleW + gap), (float)toggleY, (float)toggleW, 40 }, walkinCol, "Walk-in", 18);

    DrawText("Select Payment Method", screenW / 2 - MeasureText("Select Payment Method", 22) / 2, 330, 22, DARKGRAY);

    int btnW = 300;
    int centerX = screenW / 2 - btnW / 2;
    DrawButton({ (float)centerX, 370, (float)btnW, 55 }, DARKBLUE, "Credit Card", 20);

    Color cashColor = (app.bookingMode == WALK_IN) ? DARKGRAY : Fade(DARKGRAY, 0.3f);
    DrawButton({ (float)centerX, 450, (float)btnW, 55 }, cashColor, "Cash", 20);

    if (app.bookingMode == ONLINE) {
        DrawText("Cash only available for Walk-in bookings", screenW / 2 - MeasureText("Cash only available for Walk-in bookings", 16) / 2, 520, 16, RED);
    }
}

// BookingScreen

void BookingScreen::OnActivate(App& app) {
    Show* show = app.data.GetShowById(app.selectedShowId);
    if (!show) return;

    float totalPrice = 0;
    for (int sid : app.selectedSeats) {
        Seat* s = show->GetSeatById(sid);
        if (s) {
            totalPrice += GetSeatPrice(s->type);
            s->status = BOOKED;
            s->lockTime = 0;
        }
    }

    int cinemaId = 1;
    Hall* hall = app.data.GetHallById(show->hallId);
    if (hall) cinemaId = hall->cinemaId;

    Booking booking;
    booking.showId = app.selectedShowId;
    booking.cinemaId = cinemaId;
    booking.seatIds = app.selectedSeats;
    booking.totalPrice = totalPrice;
    booking.paymentMethod = (int)app.selectedPayment;
    booking.userId = app.loggedInUserId;
    app.data.AddBooking(booking);
    app.data.AddNotification("Booking confirmed for show #" + std::to_string(app.selectedShowId), BOOKING_MADE);
}

void BookingScreen::Update(App& app)
{
    int screenW = GetScreenWidth();
    int panelW = 520;
    int panelX = screenW / 2 - panelW / 2;

    Rectangle menuBtn = { (float)panelX, 680, (float)panelW, 55 };
    if (IsButtonClicked(menuBtn)) {
        app.selectedSeats.clear();
        app.currentState = MAIN_MENU;
    }
}

void BookingScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    int panelW = 520;
    int panelX = screenW / 2 - panelW / 2;

    Show* show = app.data.GetShowById(app.selectedShowId);
    if (!show) return;
    Movie* movie = app.data.GetMovieById(show->movieId);
    if (!movie) return;

    DrawRectangleRounded({ (float)panelX, 140, (float)panelW, 560 }, 0.15f, 8, Fade(DARKGREEN, 0.06f));
    DrawRectangleRoundedLines({ (float)panelX, 140, (float)panelW, 560 }, 0.15f, 8, 2.0f, Fade(DARKGREEN, 0.15f));

    int circleX = screenW / 2;
    int circleY = 220;
    DrawCircle(circleX, circleY, 40, GREEN);
    DrawCircle(circleX, circleY, 38, GREEN);
    DrawLine(circleX - 18, circleY - 2, circleX - 5, circleY + 12, WHITE);
    DrawLine(circleX - 5, circleY + 12, circleX + 18, circleY - 12, WHITE);

    DrawText("Booking Confirmed!", screenW / 2 - MeasureText("Booking Confirmed!", 30) / 2, 290, 30, DARKGREEN);

    int detailY = 340;
    int lineH = 35;
    DrawText(movie->title.c_str(), screenW / 2 - MeasureText(movie->title.c_str(), 24) / 2, detailY, 24, DARKGRAY);
    detailY += lineH;

    std::string showtime = "Showtime: " + show->startTime;
    DrawText(showtime.c_str(), screenW / 2 - MeasureText(showtime.c_str(), 20) / 2, detailY, 20, GRAY);
    detailY += lineH;

    std::string paymentStr = (app.selectedPayment == CREDIT_CARD) ? "Credit Card" : "Cash";
    std::string mode = (app.bookingMode == ONLINE) ? "Online" : "Walk-in";
    std::string paymentInfo = "Payment: " + paymentStr + " (" + mode + ")";
    DrawText(paymentInfo.c_str(), screenW / 2 - MeasureText(paymentInfo.c_str(), 20) / 2, detailY, 20, GRAY);
    detailY += lineH;

    std::string seatNos;
    float totalPrice = 0;
    for (size_t i = 0; i < app.selectedSeats.size(); ++i) {
        Seat* s = show->GetSeatById(app.selectedSeats[i]);
        if (s) {
            if (!seatNos.empty()) seatNos += ", ";
            seatNos += std::to_string(s->row + 1) + "-" + std::to_string(s->number + 1);
            totalPrice += GetSeatPrice(s->type);
        }
    }

    std::string seatText = "Seats: " + seatNos;
    DrawText(seatText.c_str(), screenW / 2 - MeasureText(seatText.c_str(), 18) / 2, detailY, 18, DARKGRAY);
    detailY += lineH;

    std::string ref = "REF: #" + std::to_string(show->id) + "-" + std::to_string((int)app.selectedSeats.size());
    DrawText(ref.c_str(), screenW / 2 - MeasureText(ref.c_str(), 16) / 2, detailY, 16, LIGHTGRAY);
    detailY += 45;

    std::string totalStr = "Total: $" + std::to_string((int)totalPrice) + ".00";
    DrawText(totalStr.c_str(), screenW / 2 - MeasureText(totalStr.c_str(), 28) / 2, detailY, 28, DARKGREEN);

    DrawButton({ (float)panelX, 680, (float)panelW, 55 }, DARKGREEN, "Back to Main Menu", 20);
}

// MyBookingsScreen

void MyBookingsScreen::Update(App& app)
{
    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) {
        app.currentState = MAIN_MENU;
        return;
    }

    int screenW = GetScreenWidth();
    int btnW = 740;
    int btnX = screenW / 2 - btnW / 2;
    int y = 130;

    for (auto& booking : app.data.GetBookings()) {
        if (booking.userId != app.loggedInUserId) { y += 105; continue; }
        Rectangle cancelBtn = { (float)(btnX + btnW - 120), (float)(y + 25), 100, 35 };
        if (IsButtonClicked(cancelBtn)) {
            int bid = booking.id;
            app.data.CancelBooking(bid);
            return;
        }
        y += 105;
    }
}

void MyBookingsScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    DrawButton({ 20, 20, 120, 40 }, GRAY, "< Back", 20);
    DrawText("My Bookings", screenW / 2 - MeasureText("My Bookings", 32) / 2, 35, 32, DARKBLUE);

    int btnW = 740;
    int btnX = screenW / 2 - btnW / 2;
    int y = 130;

    bool hasBookings = false;
    for (auto& booking : app.data.GetBookings()) {
        if (booking.userId != app.loggedInUserId) continue;
        hasBookings = true;

        DrawRectangleRounded({ (float)btnX, (float)y, (float)btnW, 95 }, 0.1f, 6, Fade(BLUE, 0.08f));
        DrawRectangleRoundedLines({ (float)btnX, (float)y, (float)btnW, 95 }, 0.1f, 6, 1.5f, Fade(BLUE, 0.2f));

        Show* show = app.data.GetShowById(booking.showId);
        if (show) {
            Movie* movie = app.data.GetMovieById(show->movieId);
            if (movie) {
                DrawText(movie->title.c_str(), btnX + 16, y + 12, 20, DARKBLUE);
            }
            std::string showTime = "Time: " + show->startTime;
            DrawText(showTime.c_str(), btnX + 16, y + 38, 16, GRAY);
        }

        std::string seatInfo = "Seats: " + std::to_string((int)booking.seatIds.size());
        DrawText(seatInfo.c_str(), btnX + btnW / 2 - 30, y + 15, 18, DARKGRAY);

        std::string priceInfo = "$" + std::to_string((int)booking.totalPrice);
        DrawText(priceInfo.c_str(), btnX + btnW / 2 - 20, y + 40, 22, DARKGREEN);

        DrawButton({ (float)(btnX + btnW - 120), (float)(y + 25), 100, 35 }, RED, "Cancel", 16);

        y += 105;
    }

    if (!hasBookings) {
        DrawText("No bookings yet. Browse movies to book tickets!", screenW / 2 - MeasureText("No bookings yet. Browse movies to book tickets!", 22) / 2, y + 30, 22, LIGHTGRAY);
    }
}

static std::vector<Seat> MakeSeats(int rows, int cols) {
    std::vector<Seat> seats;
    int seatId = 1;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            Seat s;
            s.id = seatId++;
            s.row = i;
            s.number = j;
            s.status = AVAILABLE;
            s.lockTime = 0;
            if (i >= rows - 2) s.type = PLATINUMTIER;
            else if (i >= rows - 4) s.type = GOLDTIER;
            else s.type = SILVERTIER;
            seats.push_back(s);
        }
    return seats;
}

// AdminPanelScreen

void AdminPanelScreen::OnActivate(App& app) {
    inAddMovieForm = false;
    inAddShowForm = false;
    adminFormError = "";
}

void AdminPanelScreen::Update(App& app)
{
    int screenW = GetScreenWidth();
    int centerX = screenW / 2;

    Rectangle backBtn = { 20, 20, 120, 40 };
    if (IsButtonClicked(backBtn)) {
        app.currentState = MAIN_MENU;
        return;
    }

    if (inAddMovieForm) {
        int boxW = 400, boxH = 45;
        adminMovieTitle.bounds = { (float)(centerX - boxW / 2), 250, (float)boxW, (float)boxH };
        adminMovieLanguage.bounds = { (float)(centerX - boxW / 2), 320, (float)boxW, (float)boxH };
        adminMovieGenre.bounds = { (float)(centerX - boxW / 2), 390, (float)boxW, (float)boxH };
        adminMovieReleaseDate.bounds = { (float)(centerX - boxW / 2), 460, (float)boxW, (float)boxH };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            adminMovieTitle.isFocused = CheckCollisionPointRec(mouse, adminMovieTitle.bounds);
            adminMovieLanguage.isFocused = CheckCollisionPointRec(mouse, adminMovieLanguage.bounds);
            adminMovieGenre.isFocused = CheckCollisionPointRec(mouse, adminMovieGenre.bounds);
            adminMovieReleaseDate.isFocused = CheckCollisionPointRec(mouse, adminMovieReleaseDate.bounds);
            if (adminMovieLanguage.isFocused) { adminMovieTitle.isFocused = false; adminMovieGenre.isFocused = false; adminMovieReleaseDate.isFocused = false; }
            if (adminMovieTitle.isFocused) { adminMovieLanguage.isFocused = false; adminMovieGenre.isFocused = false; adminMovieReleaseDate.isFocused = false; }
            if (adminMovieGenre.isFocused) { adminMovieTitle.isFocused = false; adminMovieLanguage.isFocused = false; adminMovieReleaseDate.isFocused = false; }
            if (adminMovieReleaseDate.isFocused) { adminMovieTitle.isFocused = false; adminMovieLanguage.isFocused = false; adminMovieGenre.isFocused = false; }
        }

        adminMovieTitle.Update();
        adminMovieLanguage.Update();
        adminMovieGenre.Update();
        adminMovieReleaseDate.Update();

        Rectangle saveMovieBtn = { (float)(centerX - 120), 530, 110, 45 };
        Rectangle cancelMovieBtn = { (float)(centerX + 10), 530, 110, 45 };

        if (IsButtonClicked(saveMovieBtn)) {
            if (adminMovieTitle.text.empty() || adminMovieLanguage.text.empty() ||
                adminMovieGenre.text.empty() || adminMovieReleaseDate.text.empty()) {
                adminFormError = "All fields are required.";
            } else {
                Movie m;
                m.id = (int)app.data.GetMovies().size() + 1;
                m.title = adminMovieTitle.text;
                m.language = adminMovieLanguage.text;
                m.genre = adminMovieGenre.text;
                m.releaseDate = adminMovieReleaseDate.text;
                app.data.AddMovie(m);
                adminMovieTitle.text = "";
                adminMovieLanguage.text = "";
                adminMovieGenre.text = "";
                adminMovieReleaseDate.text = "";
                adminFormError = "";
                inAddMovieForm = false;
            }
        }
        if (IsButtonClicked(cancelMovieBtn)) {
            adminMovieTitle.text = "";
            adminMovieLanguage.text = "";
            adminMovieGenre.text = "";
            adminMovieReleaseDate.text = "";
            adminFormError = "";
            inAddMovieForm = false;
        }
        return;
    }

    Rectangle addMovieBtn = { (float)(screenW / 2 - 120), 100, 240, 45 };
    if (IsButtonClicked(addMovieBtn)) {
        inAddMovieForm = true;
        adminFormError = "";
        return;
    }

    int listY = 170;
    int btnW = 740;
    int btnX = screenW / 2 - btnW / 2;

    for (auto& movie : app.data.GetMovies()) {
        Rectangle manageBtn = { (float)(btnX + btnW - 240), (float)(listY + 7), 100, 35 };
        Rectangle delMovieBtn = { (float)(btnX + btnW - 120), (float)(listY + 7), 100, 35 };

        if (IsButtonClicked(delMovieBtn)) {
            if (adminExpandedMovieId == movie.id) adminExpandedMovieId = -1;
            app.data.DeleteMovie(movie.id);
            return;
        }
        if (IsButtonClicked(manageBtn)) {
            adminExpandedMovieId = (adminExpandedMovieId == movie.id) ? -1 : movie.id;
            inAddShowForm = false;
            adminEditingShowId = -1;
            adminFormError = "";
            return;
        }

        if (adminExpandedMovieId == movie.id) {
            int showY = listY + 55;
            std::vector<Show*> movieShows = app.data.GetShowsByMovieId(movie.id);
            for (auto& show : movieShows) {
                Rectangle editShowBtn = { (float)(btnX + btnW - 230), (float)(showY + 3), 100, 35 };
                Rectangle delShowBtn = { (float)(btnX + btnW - 120), (float)(showY + 3), 100, 35 };
                if (IsButtonClicked(editShowBtn)) {
                    adminEditingShowId = (adminEditingShowId == show->id) ? -1 : show->id;
                    if (adminEditingShowId == show->id) {
                        inAddShowForm = false;
                        adminEditShowStartTime.text = show->startTime;
                        adminEditHallStepper = show->hallId;
                    }
                    adminFormError = "";
                    return;
                }
                if (IsButtonClicked(delShowBtn)) {
                    if (adminEditingShowId == show->id) adminEditingShowId = -1;
                    app.data.DeleteShow(show->id);
                    return;
                }
                showY += 50;
            }

            if (adminEditingShowId != -1) {
                Show* editingShow = app.data.GetShowById(adminEditingShowId);
                if (editingShow) {
                    int formY = showY;
                    int boxW = 250, boxH = 40;
                    adminEditShowStartTime.bounds = { (float)(btnX + 20), (float)(formY + 16), (float)boxW, (float)boxH };
                    Rectangle hallMinusBtn = { (float)(btnX + 370), (float)(formY + 16), 30, 40 };
                    Rectangle hallPlusBtn = { (float)(btnX + 410), (float)(formY + 16), 30, 40 };

                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        Vector2 mouse = GetMousePosition();
                        adminEditShowStartTime.isFocused = CheckCollisionPointRec(mouse, adminEditShowStartTime.bounds);
                    }

                    if (IsButtonClicked(hallMinusBtn) && adminEditHallStepper > 1) adminEditHallStepper--;
                    if (IsButtonClicked(hallPlusBtn)) adminEditHallStepper++;

                    adminEditShowStartTime.Update();

                    Rectangle saveEditBtn = { (float)(btnX + 470), (float)(formY + 16), 90, 40 };
                    Rectangle cancelEditBtn = { (float)(btnX + 570), (float)(formY + 16), 90, 40 };

                    if (IsButtonClicked(saveEditBtn)) {
                        if (adminEditShowStartTime.text.empty()) {
                            adminFormError = "Start time is required.";
                        } else {
                            editingShow->startTime = adminEditShowStartTime.text;
                            editingShow->hallId = adminEditHallStepper;
                            adminEditingShowId = -1;
                            adminFormError = "";
                            return;
                        }
                    }
                    if (IsButtonClicked(cancelEditBtn)) {
                        adminEditingShowId = -1;
                        adminFormError = "";
                    }
                }
            }

            if (adminEditingShowId == -1) {
                Rectangle addShowBtn = { (float)(btnX + btnW / 2 - 80), (float)showY, 160, 35 };
                if (IsButtonClicked(addShowBtn)) {
                    inAddShowForm = !inAddShowForm;
                    adminFormError = "";
                    adminFormHallStepper = 1;
                    adminShowStartTime.text = "";
                    return;
                }
            }

            if (inAddShowForm) {
                int formY = showY + 50;
                int boxW = 250, boxH = 40;
                adminShowStartTime.bounds = { (float)(btnX + 20), (float)formY, (float)boxW, (float)boxH };
                Rectangle hallMinusBtn = { (float)(btnX + 370), (float)formY, 30, 40 };
                Rectangle hallPlusBtn = { (float)(btnX + 410), (float)formY, 30, 40 };

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse = GetMousePosition();
                    adminShowStartTime.isFocused = CheckCollisionPointRec(mouse, adminShowStartTime.bounds);
                }

                if (IsButtonClicked(hallMinusBtn) && adminFormHallStepper > 1) adminFormHallStepper--;
                if (IsButtonClicked(hallPlusBtn)) adminFormHallStepper++;

                adminShowStartTime.Update();

                Rectangle saveShowBtn = { (float)(btnX + 470), (float)formY, 90, 40 };
                Rectangle cancelShowBtn = { (float)(btnX + 570), (float)formY, 90, 40 };

                if (IsButtonClicked(saveShowBtn)) {
                    if (adminShowStartTime.text.empty()) {
                        adminFormError = "Start time is required.";
                    } else {
                        Show s;
                        s.id = 0;
                        s.movieId = movie.id;
                        s.hallId = adminFormHallStepper;
                        s.startTime = adminShowStartTime.text;
                        s.seats = MakeSeats(6, 8);
                        app.data.AddShow(s);
                        inAddShowForm = false;
                        adminFormError = "";
                        return;
                    }
                }
                if (IsButtonClicked(cancelShowBtn)) {
                    inAddShowForm = false;
                    adminFormError = "";
                }
            }
        }
        listY += 48;
        if (adminExpandedMovieId == movie.id) {
            listY += 55;
            listY += (int)app.data.GetShowsByMovieId(movie.id).size() * 50;
            if (adminEditingShowId != -1) listY += 80;
            if (inAddShowForm) listY += 100;
            else listY += 50;
        }
    }
}

void AdminPanelScreen::Draw(App& app)
{
    int screenW = GetScreenWidth();
    int centerX = screenW / 2;

    DrawButton({ 20, 20, 120, 40 }, GRAY, "< Back", 20);

    DrawText("Admin Panel", screenW / 2 - MeasureText("Admin Panel", 34) / 2, 30, 34, PURPLE);
    DrawText("Manage Movies and Showtimes", screenW / 2 - MeasureText("Manage Movies and Showtimes", 18) / 2, 68, 18, GRAY);

    if (inAddMovieForm) {
        int panelY = 160;
        int panelH = 460;
        int panelW = 500;
        int panelX = centerX - panelW / 2;
        DrawRectangleRounded({ (float)panelX, (float)panelY, (float)panelW, (float)panelH }, 0.15f, 8, Fade(DARKGREEN, 0.06f));
        DrawRectangleRoundedLines({ (float)panelX, (float)panelY, (float)panelW, (float)panelH }, 0.15f, 8, 2.0f, Fade(DARKGREEN, 0.15f));
        DrawText("Add New Movie", centerX - MeasureText("Add New Movie", 28) / 2, panelY + 20, 28, DARKGREEN);
        DrawText("Title", centerX - 180, 230, 18, DARKGRAY);
        adminMovieTitle.Draw();
        DrawText("Language", centerX - 180, 300, 18, DARKGRAY);
        adminMovieLanguage.Draw();
        DrawText("Genre", centerX - 180, 370, 18, DARKGRAY);
        adminMovieGenre.Draw();
        DrawText("Release Date", centerX - 180, 440, 18, DARKGRAY);
        adminMovieReleaseDate.Draw();
        if (!adminFormError.empty()) {
            DrawText(adminFormError.c_str(), centerX - MeasureText(adminFormError.c_str(), 16) / 2, 505, 16, RED);
        }
        DrawButton({ (float)(centerX - 120), 530, 110, 45 }, DARKGREEN, "Save", 20);
        DrawButton({ (float)(centerX + 10), 530, 110, 45 }, MAROON, "Cancel", 20);
        return;
    }

    DrawButton({ (float)(screenW / 2 - 120), 100, 240, 45 }, DARKGREEN, "Add Movie", 20);

    int listY = 170;
    int btnW = 740;
    int btnX = screenW / 2 - btnW / 2;

    for (auto& movie : app.data.GetMovies()) {
        Rectangle rowBg = { (float)btnX, (float)listY, (float)btnW, 48 };
        DrawRectangleRounded(rowBg, 0.1f, 6, Fade(PURPLE, 0.08f));
        DrawRectangleRoundedLines(rowBg, 0.1f, 6, 1.5f, Fade(PURPLE, 0.2f));
        DrawText(movie.title.c_str(), btnX + 16, listY + 14, 20, PURPLE);

        DrawButton({ (float)(btnX + btnW - 240), (float)(listY + 7), 100, 35 }, DARKGRAY, "Shows", 16);
        DrawButton({ (float)(btnX + btnW - 120), (float)(listY + 7), 100, 35 }, RED, "Delete", 16);

        if (adminExpandedMovieId == movie.id) {
            int showY = listY + 55;
            std::vector<Show*> movieShows = app.data.GetShowsByMovieId(movie.id);
            if (movieShows.empty()) {
                DrawText("No shows for this movie.", btnX + 12, showY + 8, 18, LIGHTGRAY);
                showY += 40;
            } else {
                for (auto& show : movieShows) {
                    Hall* hall = app.data.GetHallById(show->hallId);
                    std::string showText = show->startTime + "  |  Hall: " + (hall ? hall->name : "N/A");
                    DrawRectangleRounded({ (float)(btnX + 12), (float)(showY), (float)(btnW - 24), 40 }, 0.08f, 6, Fade(DARKGRAY, 0.2f));
                    DrawText(showText.c_str(), btnX + 24, showY + 10, 18, WHITE);
                    DrawButton({ (float)(btnX + btnW - 230), (float)(showY + 3), 100, 35 }, DARKGRAY, "Edit", 16);
                    DrawButton({ (float)(btnX + btnW - 120), (float)(showY + 3), 100, 35 }, RED, "Delete", 16);
                    showY += 50;
                }
            }

            if (adminEditingShowId != -1) {
                Show* editingShow = app.data.GetShowById(adminEditingShowId);
                if (editingShow) {
                    int formY = showY;
                    int boxW = 250, boxH = 40;
                    DrawText("Start Time", btnX + 20, formY - 2, 14, DARKGRAY);
                    adminEditShowStartTime.bounds = { (float)(btnX + 20), (float)(formY + 16), (float)boxW, (float)boxH };
                    adminEditShowStartTime.Draw();

                    std::string hallLabel = "Hall ID: " + std::to_string(adminEditHallStepper);
                    DrawText(hallLabel.c_str(), btnX + 290, formY + 24, 18, DARKGRAY);

                    DrawButton({ (float)(btnX + 370), (float)(formY + 16), 30, 40 }, GRAY, "-", 22);
                    DrawButton({ (float)(btnX + 410), (float)(formY + 16), 30, 40 }, GRAY, "+", 22);
                    DrawButton({ (float)(btnX + 470), (float)(formY + 16), 90, 40 }, DARKGREEN, "Save", 18);
                    DrawButton({ (float)(btnX + 570), (float)(formY + 16), 90, 40 }, MAROON, "Cancel", 18);

                    if (!adminFormError.empty()) {
                        DrawText(adminFormError.c_str(), btnX + 20, formY + 66, 18, RED);
                    }
                }
            }

            if (adminEditingShowId == -1)
                DrawButton({ (float)(btnX + btnW / 2 - 80), (float)showY, 160, 35 }, DARKGREEN, "Add Show", 18);

            if (inAddShowForm) {
                int formY = showY + 50;
                DrawText("Start Time", btnX + 20, formY - 18, 14, DARKGRAY);
                adminShowStartTime.Draw();

                std::string hallLabel = "Hall ID: " + std::to_string(adminFormHallStepper);
                DrawText(hallLabel.c_str(), btnX + 290, formY + 8, 18, DARKGRAY);

                DrawButton({ (float)(btnX + 370), (float)formY, 30, 40 }, GRAY, "-", 22);
                DrawButton({ (float)(btnX + 410), (float)formY, 30, 40 }, GRAY, "+", 22);
                DrawButton({ (float)(btnX + 470), (float)formY, 90, 40 }, DARKGREEN, "Save", 18);
                DrawButton({ (float)(btnX + 570), (float)formY, 90, 40 }, MAROON, "Cancel", 18);

                if (!adminFormError.empty()) {
                    DrawText(adminFormError.c_str(), btnX + 20, formY + 50, 18, RED);
                }
            }
        }

        listY += 48;
        if (adminExpandedMovieId == movie.id) {
            listY += 55;
            listY += (int)app.data.GetShowsByMovieId(movie.id).size() * 50;
            if (adminEditingShowId != -1) listY += 80;
            if (inAddShowForm) listY += 100;
            else listY += 50;
        }
    }
}
