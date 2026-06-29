#include "App.h"

App::App()
    : currentState(LOG_IN)
    , loggedInUserId(-1)
    , selectedMovieId(-1)
    , selectedShowId(-1)
    , bookingMode(ONLINE)
    , selectedPayment(CREDIT_CARD)
{
    data.SeedData();
}

Screen* App::GetCurrentScreen() {
    switch (currentState) {
        case MAIN_MENU:          return &mainMenuScreen;
        case LOG_IN:             return &logInScreen;
        case REGISTER:           return &registerScreen;
        case MOVIE:              return &movieScreen;
        case SHOWTIME_SELECTION: return &showtimeScreen;
        case SEAT_SELECTION:     return &seatSelectionScreen;
        case PAYMENT_SELECTION:  return &paymentScreen;
        case BOOKING:            return &bookingScreen;
        case MY_BOOKINGS:        return &myBookingsScreen;
        case ADMIN_PANEL:        return &adminPanelScreen;
        default:                 return nullptr;
    }
}

void App::Update() {
    AppState oldState = currentState;
    Screen* screen = GetCurrentScreen();
    if (screen) screen->Update(*this);
    if (oldState != currentState) {
        Screen* newScreen = GetCurrentScreen();
        if (newScreen) newScreen->OnActivate(*this);
    }
}

void App::Draw() {
    Screen* screen = GetCurrentScreen();
    if (screen) screen->Draw(*this);
}

void App::Display()
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(1920, 1080, "Movie Ticket Booking System");
    SetTargetFPS(60);

    logInScreen.OnActivate(*this);

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
