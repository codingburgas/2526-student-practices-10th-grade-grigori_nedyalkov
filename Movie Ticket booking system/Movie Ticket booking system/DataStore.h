#pragma once
#include "../Library/Models.h"
#include <vector>

class DataStore {
public:
    std::vector<Movie>  movies;
    std::vector<Show>   shows;
    std::vector<Hall>   halls;
    std::vector<Cinema> cinemas;

    void SeedData();

    Movie* GetMovieById(int id);
    Show* GetShowById(int id);
    Hall* GetHallById(int id);
    Cinema* GetCinemaById(int id);
    std::vector<Show*> GetShowsByMovieId(int movieId);
};