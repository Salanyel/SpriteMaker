#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <iostream>
#include <string>

//My lib
#include "SpriteMaker.h"

using namespace std;
using namespace sf;

int main()
{
	SpriteMaker * spriteMaker = NULL;

	spriteMaker = new SpriteMaker();
	spriteMaker->configure();

	return EXIT_SUCCESS;
}