#pragma once
#include "../Library/Models.h"
#include <vector>

class DataStore {
public:
	vector<Movie> movies;
	vector<Show> shows;

	void SeedData();
	Movie* GetMovieById(int id);
	Show* GetShowById(int id);
private:
};