#include <iostream>
#include "../Library1/library1.h"
#include "../Library2/Library2.h"
int main()
{
	int n;
	cin >> n;
	cout << cou(n);
	Book jungleBook = createBook("Jungle book","Rick Riordan",1967,20.50);
	printBook(jungleBook);
}