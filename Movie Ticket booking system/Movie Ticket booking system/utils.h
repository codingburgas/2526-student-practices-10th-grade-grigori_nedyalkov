#pragma once

enum AppState
{
    EXIT,
    MAIN_MENU,
    LOG_IN,
    REGISTER,
    MAIN_PAGE,
    MOVIE,
    SEAT_SELECTION,
    BOOKING
};

inline float GetSeatPrice(int seatType)
{
    switch (seatType)
    {
    case 0: return 8.0f;   // SILVERTIER
    case 1: return 12.0f;  // GOLDTIER
    case 2: return 18.0f;  // PLATINUMTIER
    default: return 0.0f;
    }
}