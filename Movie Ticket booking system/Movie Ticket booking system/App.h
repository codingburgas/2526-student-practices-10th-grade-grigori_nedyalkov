#pragma once
#include <iostream>
#include <raylib.h>
#include <ctime>
#include "DataStore.h"
using namespace std;

class App {
public:
    App();
    void Display();
    void Draw();
    void Update();
private:
    DataStore data;
    bool IsConfirmButtonClicked();
};