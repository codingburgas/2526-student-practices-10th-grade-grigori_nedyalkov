#pragma once
#include "../Library/Models.h"
#include <vector>
#include <string>

class DataStore {
public:
    void SeedData();
    void AddNotification(const std::string& message, NotificationType type);
    void AddMovie(Movie m);
    void DeleteMovie(int id);
    void AddShow(Show s);
    void DeleteShow(int id);
    void UpdateShow(int id, Show updated);
    void AddUser(const User& u);
    void AddBooking(const Booking& b);
    void CancelBooking(int bookingId);

    std::vector<Movie>& GetMovies();
    std::vector<Show>& GetShows();
    std::vector<Hall>& GetHalls();
    std::vector<Cinema>& GetCinemas();
    std::vector<Notification>& GetNotifications();
    std::vector<User>& GetUsers();
    std::vector<Booking>& GetBookings();

    // Lookup methods
    User* GetUserByEmail(const std::string& email);
    User* GetUserById(int id);
    Movie* GetMovieById(int id);
    Show* GetShowById(int id);
    Hall* GetHallById(int id);
    Cinema* GetCinemaById(int id);
    std::vector<Show*> GetShowsByMovieId(int movieId);

private:
    std::vector<Movie>  movies;
    std::vector<Show>   shows;
    std::vector<Hall>   halls;
    std::vector<Cinema> cinemas;
    std::vector<Notification> notifications;
    std::vector<User> users;
    std::vector<Booking> bookings;
    int nextNotificationId = 1;
};