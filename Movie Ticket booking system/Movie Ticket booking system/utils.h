#pragma once

enum AppState
{
    EXIT,
    MAIN_MENU,
    LOG_IN,
    REGISTER,
    MOVIE,
    SHOWTIME_SELECTION,
    SEAT_SELECTION,
    PAYMENT_SELECTION,
    MY_BOOKINGS,
    BOOKING,
    ADMIN_PANEL
};

enum BookingMode { ONLINE, WALK_IN };
enum PaymentMethod { CREDIT_CARD, CASH };

float GetSeatPrice(int seatType);