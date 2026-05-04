#pragma once
#include <iostream>
#include <raylib.h>
#include <ctime>
#include "DataStore.h"
#include "utils.h"
using namespace std;

class App {
public:
    App();
    void Display();
    void Draw();
    void Update();
private:
    AppState state;
    DataStore data;
    bool IsConfirmButtonClicked();
};