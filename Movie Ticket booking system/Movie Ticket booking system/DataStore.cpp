#include "DataStore.h"
#include <iostream>

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

    // Users
    users.push_back({ 1, "Admin", "admin@example.com", "admin123", true });
    users.push_back({ 2, "John", "john@example.com", "pass123", false });
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

void DataStore::AddNotification(const std::string& message, NotificationType type) {
    Notification n;
    n.id = nextNotificationId++;
    n.message = message;
    n.timestamp = std::time(nullptr);
    n.type = type;
    notifications.push_back(n);
    std::cout << "Email sent to user@example.com: " << message << std::endl;
}

void DataStore::AddMovie(Movie m) {
    m.id = (int)movies.size() + 1;
    movies.push_back(m);
    AddNotification("New movie \"" + m.title + "\" has been released!", MOVIE_RELEASED);
}

void DataStore::DeleteMovie(int id) {
    for (auto it = movies.begin(); it != movies.end(); ++it) {
        if (it->id == id) {
            movies.erase(it);
            break;
        }
    }
    for (auto it = shows.begin(); it != shows.end(); ) {
        if (it->movieId == id) {
            it = shows.erase(it);
        } else {
            ++it;
        }
    }
}

void DataStore::AddShow(Show s) {
    s.id = (int)shows.size() + 1;
    shows.push_back(s);
}

void DataStore::DeleteShow(int id) {
    for (auto it = shows.begin(); it != shows.end(); ++it) {
        if (it->id == id) {
            shows.erase(it);
            return;
        }
    }
}

void DataStore::UpdateShow(int id, Show updated) {
    for (auto& s : shows) {
        if (s.id == id) {
            s.movieId = updated.movieId;
            s.hallId = updated.hallId;
            s.startTime = updated.startTime;
            return;
        }
    }
}

User* DataStore::GetUserByEmail(const std::string& email) {
    for (auto& u : users)
        if (u.email == email) return &u;
    return nullptr;
}

User* DataStore::GetUserById(int id) {
    for (auto& u : users)
        if (u.id == id) return &u;
    return nullptr;
}

void DataStore::AddBooking(const Booking& b) {
    Booking nb = b;
    nb.id = (int)bookings.size() + 1;
    bookings.push_back(nb);
}

void DataStore::CancelBooking(int bookingId) {
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->id == bookingId) {
            Show* show = GetShowById(it->showId);
            if (show) {
                for (auto& seat : show->seats) {
                    for (int seatId : it->seatIds) {
                        if (seat.id == seatId) {
                            seat.status = AVAILABLE;
                            break;
                        }
                    }
                }
            }
            AddNotification("Booking #" + std::to_string(bookingId) + " was canceled", BOOKING_CANCELED);
            bookings.erase(it);
            return;
        }
    }
}

std::vector<Show*> DataStore::GetShowsByMovieId(int movieId)
{
    std::vector<Show*> result;
    for (auto& s : shows)
        if (s.movieId == movieId) result.push_back(&s);
    return result;
}

void DataStore::AddUser(const User& u) {
    users.push_back(u);
}

std::vector<Movie>& DataStore::GetMovies() { return movies; }
std::vector<Show>& DataStore::GetShows() { return shows; }
std::vector<Hall>& DataStore::GetHalls() { return halls; }
std::vector<Cinema>& DataStore::GetCinemas() { return cinemas; }
std::vector<Notification>& DataStore::GetNotifications() { return notifications; }
std::vector<User>& DataStore::GetUsers() { return users; }
std::vector<Booking>& DataStore::GetBookings() { return bookings; }