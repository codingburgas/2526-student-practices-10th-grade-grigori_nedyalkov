#pragma once
#include <iostream>
#include <raylib.h>
#include <ctime>
#include "DataStore.h"
#include "utils.h"
using namespace std;

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
    const int seatStartX = 100;
    const int seatStartY = 150;
    const int seatSize = 40;
    const int seatPad = 10;
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