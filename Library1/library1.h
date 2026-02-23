#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Book {
    string title;
    string author;
    int year;
    double price;
};
Book createBook(string title, string author, int year, double price);
void printBook(Book book);
