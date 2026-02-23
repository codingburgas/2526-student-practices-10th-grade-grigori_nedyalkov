#include "library1.h"

Book createBook(string title, string author, int year, double price)
{
	Book* book = new Book;
	book->title = title;
	book->author = author;
	book->year = year;
	book->price = price;
	return *book;

}

void printBook(Book book)
{
	cout << "title: " << book.title << endl;
	cout << "author: " << book.author << endl;
	cout << "year: " << book.year << endl;
	cout << "price: " << book.price << endl;
}