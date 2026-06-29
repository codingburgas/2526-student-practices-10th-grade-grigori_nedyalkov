#pragma once
#include <string>
#include <vector>
#include <raylib.h>
#include "DataStore.h"
#include "utils.h"
#include "Menu.h"

class App {
public:
    App();
    void Display();

    // Shared state accessible by all screens
    DataStore data;
    AppState currentState;
    int loggedInUserId;
    int selectedMovieId;
    int selectedShowId;
    BookingMode bookingMode;
    PaymentMethod selectedPayment;
    std::vector<int> selectedSeats;

private:
    // Screen instances
    MainMenuScreen mainMenuScreen;
    LogInScreen logInScreen;
    RegisterScreen registerScreen;
    MovieScreen movieScreen;
    ShowtimeScreen showtimeScreen;
    SeatSelectionScreen seatSelectionScreen;
    PaymentScreen paymentScreen;
    BookingScreen bookingScreen;
    MyBookingsScreen myBookingsScreen;
    AdminPanelScreen adminPanelScreen;

    // Internal helpers
    Screen* GetCurrentScreen();
    void Update();
    void Draw();
};
