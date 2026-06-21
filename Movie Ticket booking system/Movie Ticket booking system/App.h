#pragma once
#include <iostream>
#include <raylib.h>
#include <ctime>
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
};