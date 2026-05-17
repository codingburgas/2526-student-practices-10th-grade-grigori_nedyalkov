#pragma once
#include "../Library/Models.h"
#include <vector>

class DataStore {
public:
    vector<Movie>  movies;
    vector<Show>   shows;
    vector<Hall>   halls;
    vector<Cinema> cinemas;

    void SeedData();

    Movie* GetMovieById(int id);
    Show* GetShowById(int id);
    Hall* GetHallById(int id);
    Cinema* GetCinemaById(int id);
    vector<Show*> GetShowsByMovieId(int movieId);
};