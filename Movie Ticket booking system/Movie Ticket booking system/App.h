#pragma once
#include <string>
#include <vector>
#include <raylib.h>
#include "DataStore.h"
#include "utils.h"

class App {
public:
    App();
    void Display();
    void Draw();
    void Update();
private:
    AppState currentState;
    DataStore data;
    int selectedMovieId;
    int selectedShowId;
    const int seatStartX = 0;
    const int seatStartY = 160;
    const int seatSize = 55;
    const int seatPad = 12;
    // Search screen state
    std::string searchText;
    float cursorTimer = 0.0f; // for blinking cursor
    bool cursorVisible = true;
    int selectedLanguageIdx = 0; // index into languages vector, -1 = no filter
    int selectedGenreIdx = 0; // index into genres vector, -1 = no filter
    std::vector<std::string> languages;
    std::vector<std::string> genres;
    std::vector<std::string> dates;
    std::vector<const Movie*> filteredMovies;
    bool filtersDirty = true;
    int resultsScroll = 0;
    int selectedDateIdx = 0; // index into dates vector
    //Update helpers
    void UpdateMainMenu();
    void UpdateMovieScreen();
    void UpdateSeatSelection();
    void UpdateBooking();
    void UpdateShowtimeSelection();
    //Draw helpers
    void DrawMainMenu();
    void DrawMovieScreen();
    void DrawSeatSelection();
    void DrawBooking();

    bool IsButtonClicked(Rectangle btn);
    void DrawShowtimeSelection();
    void DrawNotifications();
    void PerformFilter();
};