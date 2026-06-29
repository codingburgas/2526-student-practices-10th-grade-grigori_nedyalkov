#pragma once
#include <string>
#include <vector>
#include <raylib.h>
#include "../Library/Models.h"

class App;

class Screen {
public:
    virtual ~Screen() = default;
    virtual void Update(App& app) = 0;
    virtual void Draw(App& app) = 0;
    virtual void OnActivate(App& app) {}
};

// UI Helpers

struct TextInputBox {
    Rectangle bounds;
    std::string text;
    bool isFocused;
    bool isPassword;

    TextInputBox() : isFocused(false), isPassword(false) {}

    void Update() {
        if (!isFocused) return;
        int ch;
        while ((ch = GetCharPressed()) > 0) {
            if (ch >= 32 && ch <= 126) {
                text.push_back((char)ch);
            }
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !text.empty()) {
            text.pop_back();
        }
    }

    void Draw() {
        DrawRectangleRounded(bounds, 0.2f, 6, RAYWHITE);
        DrawRectangleRoundedLines(bounds, 0.2f, 6, 2.0f, isFocused ? DARKBLUE : Fade(DARKGRAY, 0.6f));
        std::string displayText = isPassword ? std::string(text.size(), '*') : text;
        Color textCol = text.empty() ? Fade(DARKGRAY, 0.5f) : DARKGRAY;
        DrawText(displayText.c_str(), (int)bounds.x + 8, (int)bounds.y + (int)bounds.height / 2 - 10, 20, textCol);
        if (isFocused) {
            bool cursorVis = ((int)(GetTime() * 2) % 2 == 0);
            if (cursorVis) {
                int textW = MeasureText(displayText.c_str(), 20);
                DrawRectangle((int)bounds.x + 8 + textW, (int)bounds.y + 8, 2, (int)bounds.height - 16, DARKGRAY);
            }
        }
    }
};

bool IsButtonClicked(Rectangle btn);

void DrawButton(Rectangle btn, Color color, const char* text, int fontSize);

// Screen Classes

class MainMenuScreen : public Screen {
public:
    void Update(App& app);
    void Draw(App& app);
};

class LogInScreen : public Screen {
public:
    TextInputBox loginEmailBox;
    TextInputBox loginPassBox;
    std::string loginError;

    void Update(App& app);
    void Draw(App& app);
    void OnActivate(App& app);
};

class RegisterScreen : public Screen {
public:
    TextInputBox regNameBox;
    TextInputBox regEmailBox;
    TextInputBox regPassBox;
    std::string registerError;

    void Update(App& app);
    void Draw(App& app);
    void OnActivate(App& app);
};

class MovieScreen : public Screen {
public:
    std::string searchText;
    float cursorTimer = 0.0f;
    bool cursorVisible = true;
    int selectedLanguageIdx = 0;
    int selectedGenreIdx = 0;
    int selectedDateIdx = 0;
    std::vector<std::string> languages;
    std::vector<std::string> genres;
    std::vector<std::string> dates;
    std::vector<const Movie*> filteredMovies;
    bool filtersDirty = true;
    int resultsScroll = 0;

    void Update(App& app);
    void Draw(App& app);
    void OnActivate(App& app);
    void PerformFilter(App& app);
};

class ShowtimeScreen : public Screen {
public:
    void Update(App& app);
    void Draw(App& app);
};

class SeatSelectionScreen : public Screen {
public:
    void Update(App& app);
    void Draw(App& app);
    void OnActivate(App& app);
};

class PaymentScreen : public Screen {
public:
    void Update(App& app);
    void Draw(App& app);
};

class BookingScreen : public Screen {
public:
    void Update(App& app);
    void Draw(App& app);
    void OnActivate(App& app);
};

class MyBookingsScreen : public Screen {
public:
    void Update(App& app);
    void Draw(App& app);
};

class AdminPanelScreen : public Screen {
public:
    bool inAddMovieForm = false;
    bool inAddShowForm = false;
    int adminExpandedMovieId = -1;
    int adminEditingShowId = -1;
    int adminFormHallStepper = 1;
    int adminEditHallStepper = 1;
    TextInputBox adminEditShowStartTime;
    TextInputBox adminMovieTitle;
    TextInputBox adminMovieLanguage;
    TextInputBox adminMovieGenre;
    TextInputBox adminMovieReleaseDate;
    TextInputBox adminShowStartTime;
    std::string adminFormError;

    void Update(App& app);
    void Draw(App& app);
    void OnActivate(App& app);
};
