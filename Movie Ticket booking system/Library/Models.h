#pragma once
#include <string>
#include <vector>
#include <ctime>

enum SeatType { SILVERTIER, GOLDTIER, PLATINUMTIER };
enum SeatStatus { AVAILABLE, LOCKED, BOOKED };

struct Movie {
    int id;
    std::string title;
    std::string language;
    std::string genre;
    std::string releaseDate;
};

struct Hall {
    int id;
    int cinemaId;
    std::string name;
};

struct Cinema {
    int id;
    std::string name;
    std::string city;
    std::vector<int> hallIds;
};

struct Seat {
    int id;
    int row;
    int number;
    SeatType type;
    SeatStatus status;
    std::time_t lockTime;
};

struct Show {
    int id;
    int movieId;
    int  hallId;
    std::string startTime;
    std::vector<Seat> seats;

    Seat* GetSeatById(int seatId) {
        for (auto& s : seats) {
            if (s.id == seatId) return &s;
        }
        return nullptr;
    }
};

struct User {
    int id;
    std::string name;
    std::string email;
    std::string password;
    bool isAdmin;
};

struct Booking {
    int id;
    int showId;
    int cinemaId;
    std::vector<int> seatIds;
    float totalPrice;
    int paymentMethod;
    int userId;
};

enum NotificationType { MOVIE_RELEASED, BOOKING_MADE, BOOKING_CANCELED };

struct Notification {
    int id;
    std::string message;
    std::time_t timestamp;
    NotificationType type;
};