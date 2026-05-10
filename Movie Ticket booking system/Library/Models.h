#pragma once
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum SeatType { SILVERTIER, GOLDTIER, PLATINUMTIER };
enum SeatStatus { AVAILABLE, LOCKED, BOOKED };

struct Movie {
	int id;
	string title;
	string language;
	string genre;
	string releaseDate;
};

struct Hall {
	int id;
	int cinemaId;
	string name;
};

struct Cinema {
	int id;
	string name;
	string city;
	vector<Hall> halls;
};

struct Seat {
	int id;
	int row;
	int number;
	SeatType type;
	SeatStatus status;
	time_t lockTime;
};

struct Show {
	int id;
	int movieId;
	int hallId;
	string startTime;
	vector<Seat> seats;
};

struct Booking {
	int id;
	int showId;
	vector<int> seatIds;
	float totalPrice;
};