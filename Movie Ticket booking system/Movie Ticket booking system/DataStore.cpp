#include "DataStore.h"

void DataStore::SeedData()
{
    // Movies
    movies.push_back({ 1, "Inception",      "English", "Sci-Fi",  "2010" });
    movies.push_back({ 2, "The Dark Knight", "English", "Action",  "2008" });
    movies.push_back({ 3, "Interstellar",   "English", "Sci-Fi",  "2014" });

    // Cinemas/Halls 
    Cinema c1 = { 1, "CineCity",   "Sofia", { 1, 2 } };
    Cinema c2 = { 2, "StarScreen", "Varna", { 3 } };
    cinemas.push_back(c1);
    cinemas.push_back(c2);

    halls.push_back({ 1, 1, "Hall A" });
    halls.push_back({ 2, 1, "Hall B" });
    halls.push_back({ 3, 2, "Hall A" });

    auto MakeSeats = [](int rows, int cols) {
        std::vector<Seat> seats;
        int seatId = 1;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
            {
                Seat s;
                s.id = seatId++;
                s.row = i;
                s.number = j;
                s.status = AVAILABLE;
                s.lockTime = 0;
                // Bottom rows = platinum, middle = gold, top = silver
                if (i >= rows - 2) s.type = PLATINUMTIER;
                else if (i >= rows - 4) s.type = GOLDTIER;
                else                    s.type = SILVERTIER;
                seats.push_back(s);
            }
        return seats;
        };

    // Shows 
    shows.push_back({ 1, 1, 1, "18:00", MakeSeats(6, 8) });
    shows.push_back({ 2, 2, 2, "20:00", MakeSeats(6, 8) });
    shows.push_back({ 3, 3, 3, "19:00", MakeSeats(6, 8) });
    shows.push_back({ 4, 1, 1, "21:00", MakeSeats(6, 8) });
}

Movie* DataStore::GetMovieById(int id)
{
    for (auto& m : movies)  if (m.id == id) return &m;
    return nullptr;
}

Show* DataStore::GetShowById(int id)
{
    for (auto& s : shows)   if (s.id == id) return &s;
    return nullptr;
}

Hall* DataStore::GetHallById(int id)
{
    for (auto& h : halls)   if (h.id == id) return &h;
    return nullptr;
}

Cinema* DataStore::GetCinemaById(int id)
{
    for (auto& c : cinemas) if (c.id == id) return &c;
    return nullptr;
}

    std::vector<Show*> DataStore::GetShowsByMovieId(int movieId)
{
    std::vector<Show*> result;
    for (auto& s : shows)
        if (s.movieId == movieId) result.push_back(&s);
    return result;
}