#include "DataStore.h"

void DataStore::SeedData()
{
    movies.push_back({ 1, "Inception", "English", "Sci-Fi", "2010" });

    Show show;
    show.id = 1;
    show.movieId = 1;
    show.hallId = 1;
    show.startTime = "18:00";

    int seatId = 1;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Seat seat;
            seat.id = seatId++;
            seat.row = i;
            seat.number = j;
            seat.type = SILVERTIER;
            seat.status = AVAILABLE;
            show.seats.push_back(seat);
        }
    }
    shows.push_back(show);
}

Movie* DataStore::GetMovieById(int id)
{
    for (auto& m : movies) {
        if (m.id == id) return &m;
    }
    return nullptr;
}

Show* DataStore::GetShowById(int id)
{
    for (auto& s : shows) {
        if (s.id == id) return &s;
    }
    return nullptr;
}