#include <SFML/Graphics.hpp>

//SpriteMaker lib
#include "SpriteMaker.h"

using namespace std;
using namespace sf;

int main()
{	
	SpriteMaker * spriteMaker = NULL;

	//Create A SpriteMaker
	spriteMaker = new SpriteMaker();
	spriteMaker->MainExecution();

	return EXIT_SUCCESS;
}