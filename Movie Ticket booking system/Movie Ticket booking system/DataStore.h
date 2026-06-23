#pragma once
#include "../Library/Models.h"
#include <vector>
#include <string>

class DataStore {
public:
    std::vector<Movie>  movies;
    std::vector<Show>   shows;
    std::vector<Hall>   halls;
    std::vector<Cinema> cinemas;
    std::vector<Notification> notifications;

    void SeedData();
    void AddNotification(const std::string& message, NotificationType type);
    void AddMovie(const std::string& title, const std::string& language, const std::string& genre, const std::string& releaseDate);

    Movie* GetMovieById(int id);
    Show* GetShowById(int id);
    Hall* GetHallById(int id);
    Cinema* GetCinemaById(int id);
    std::vector<Show*> GetShowsByMovieId(int movieId);
private:
    int nextNotificationId = 1;
};