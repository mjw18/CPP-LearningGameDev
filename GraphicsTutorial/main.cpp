#include "MainGame.h"

#include <iostream>

void printStuff(Callback callback)
{
	Foo foo;

	float b = (foo.*callback)(5);
	std::cout << b << std::endl;
}

int main(int argc, char* args[])
{
	printStuff(&Foo::doStuff);

	MainGame game;
	game.run();

	return 0;
}