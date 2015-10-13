#include "SpriteMaker.h"

SpriteMaker::SpriteMaker()
{
	/*
		Basic Init :
			the folder,
			the name,
			the extesion,
			the color for the mask
			color ratio
	*/
	m_folder = "PLACEIMAGEHERE/";
	m_colorForMask = NULL;
	m_colorRatio = 10;
	m_imageName = "input";
	m_imageExtension = "bmp";
	m_colorForMask = new Color(Color::White);
}

SpriteMaker::SpriteMaker(string p_imageName, string p_extension, int p_colorRatio) : m_imageName(p_imageName), m_imageExtension(p_extension), m_colorRatio(p_colorRatio)
{
	m_colorForMask = new Color(Color::White);
	m_folder = "PLACEIMAGEHERE/";
	m_input.loadFromFile(m_folder + m_imageName + "." + m_imageExtension);
	m_width = m_input.getSize().x;
	m_height = m_input.getSize().y;
}

//Destroy the color for the mask and every colors added for the transparency
SpriteMaker::~SpriteMaker()
{
	delete m_colorForMask;

	for (int i = 0; i < m_colors.size(); ++i)
	{
		delete m_colors[i];
	}
}

/*
	Name : mainExecution
	Return : void
	Attributes : 
	Behavior : Display a main menu and effectue an operation corresponding to the user demand
*/
void SpriteMaker::mainExecution()
{
	char choice = '0';
	int r, g, b;

	while (choice != '8')
	{
		cout << "==| Current Image : " << m_folder << m_imageName << "." << m_imageExtension << " |==" << endl << endl;

		cout << endl << "What do you want to do ?" << endl 
			<< "    1. Change image name" << endl
			<< "    2. Change image extension" << endl
			<< "    3. Change the color for the mask creation" << endl 
			<< "    4. Add a color to be transparent" << endl 
			<< "    5. Remove a color for the transparency" << endl
			<< "    6. Set the possible delta between the input image and your colors for the transparency" << endl
			<< "    7. Launch" << endl
			<< "    8. Quit" << endl;

		cin >> choice;

		cout << endl;

		switch (choice)
		{
		case '1':
			changeImageName();
			break;

		case '2':
			changeImageExtension();
			break;

		case '3':
			changeColorForMask();
			break;

		case '4' :
			addColorForTransparency();			
			break;

		case '5' :
			removeColorForTransparency();
			break;

		case '6' :
			changeColorRatio();
			break;

		case '7':
			operate();
			break;

		default:
			break;
		}
	}
}

/*
	Name : operate
	Return : void
	Attributes :
	Behavior : Load the image, create the black and white mask; create the sprite image; calculate the perfect hitbox
*/
void SpriteMaker::operate()
{	
	
	//Load the image, stop the functin if the input image is not found
	if (!m_input.loadFromFile(m_folder + m_imageName + "." + m_imageExtension))
		return;

	//Get the width & height
	m_width = m_input.getSize().x;
	m_height = m_input.getSize().y;

	//Create the mask and the sprite
	m_collisionMask.create(m_width, m_height, Color::White);
	m_sprite.create(m_width, m_height, Color::White);
	Color * tmp = NULL;

	//For each pixel
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			//If it's quit the same color as the m_colorForMask
			if (quitTheSameColor(m_input.getPixel(j, i), *m_colorForMask))
			{
				//The mask pixel will be black
				m_collisionMask.setPixel(j, i, Color::Black);
			}

			//For each color stocked in the array
			for (int k = 0; k < m_colors.size(); ++k)
			{
				//If it's quit the same color as the colors actually selected
				if (quitTheSameColor(m_input.getPixel(j, i), *(m_colors[k])))
				{
					//The pixel for the sprite image will become transparent
					tmp = new Color(m_colors[k]->r, m_colors[k]->g, m_colors[k]->b, 0);
					m_sprite.setPixel(j, i, *tmp);
				}
				else
				{
					//Else it will be a simple duplication
					m_sprite.setPixel(j, i, m_input.getPixel(j, i));
				}
			}
		}
	}

	//Function : perfectHitBox
	calculatePerfectHitBox();
	
	//Save the mask and the sprite
	m_collisionMask.saveToFile(m_folder + m_imageName + "_Mask." + m_imageExtension);
	m_sprite.saveToFile(m_folder + m_imageName + "_Sprite.png");

	//Clear the memory
	delete tmp;
}

/*
	Name : quitTheSameColor
	Return : bool
		True if the colors are the same, false in every other case
	Attributes :
		p_a (Color) ==> One of the two colors that will be compared
		p_b (Color) ==> One of the two colors that will be compared
	Behavior : Get the red, green and blue components and compare them. If the three components are equal (an error of m_colorRatio is
				tolerated, the function will return true.
*/
bool SpriteMaker::quitTheSameColor(Color p_a, Color p_b)
{
	int quitTheSame = 0;

	//Red
	if (p_a.r - m_colorRatio <= p_b.r && p_b.r <= p_a.r + m_colorRatio)
		quitTheSame++;

	//Green
	if (p_a.g - m_colorRatio <= p_b.g && p_b.g <= p_a.g + m_colorRatio)
		quitTheSame++;

	//Blue
	if (p_a.b - m_colorRatio <= p_b.b && p_b.b <= p_a.b + m_colorRatio)
		quitTheSame++;

	if (quitTheSame == 3)
		return true;
	else
		return false;
}

/*
	Name : changeColorForMask
	Return : void
	Attributes :
	Behavior : Ask the red, green and blue values for a new color that will be detected as the mask
*/
void SpriteMaker::changeColorForMask()
{

	int r, g, b;

	cout << "Enter the red component of your color : " << endl;
	cin >> r;
	cout << "Green :" << endl;
	cin >> g;
	cout << "Blue :" << endl;
	cin >> b;

	m_colorForMask = new Color(r, g, b);
}

/*
	Name : addColorForTransparency
	Return : void
	Attributes :
	Behavior : Ask red, green and blue values then add the corresponding color in the array for the transparency
*/
void SpriteMaker::addColorForTransparency()
{
	int r, g, b;
	Color * tmp;

	cout << "Enter the red component of your color : " << endl;
	cin >> r;
	cout << "Green :" << endl;
	cin >> g;
	cout << "Blue :" << endl;
	cin >> b;
	tmp = new Color(r, g, b, 255);

	m_colors.push_back(tmp);
	tmp = NULL;
}

/*
	Name : removeColorForTransparency
	Return : void
	Attributes :
	Behavior : Display all colors actually present in the array then delete them one by one if needed
*/
void SpriteMaker::removeColorForTransparency()
{
	char choice = 'a';
	int index = -1;

	//While choice != 'z' and the array is not empty
	while (choice != 'z' && !m_colors.empty())
	{
		//Display all options (quit and all colors)
		cout << "Current color : " << endl;
		cout << "    z. Quit" << endl;

		for (int i = 0; i < m_colors.size(); ++i)
		{
			cout << "    " << i << " -> R:" << (int)m_colors[i]->r << " G:" << (int)m_colors[i]->g << " B:" << (int)m_colors[i]->b << endl;
		}
		cout << endl << "Color to erase (enter 1 or 2, ...) : ";

		//Get the user input
		cin >> choice;

		//If the input is a char, get the corresponding index
		if (48 <= choice && choice <= 57)
		{
			index = atoi(&choice);
		}
		else if (choice != 'z')
		{
			index = m_colors.size();
		}

		//If the index is valid and the quit option not selected
		if (index < m_colors.size() && choice != 'z')
		{
			//Erase the color
			m_colors.erase(m_colors.begin() + index);
		}

		cout << endl;
	}
}

/*
	Name : changeColorRatio
	Return : void
	Attributes :
	Behavior : Set the delta when comparing two images
*/
void SpriteMaker::changeColorRatio()
{
	cout << "Enter the possible delta between the input image colors and your colors for the transparency." << endl;
	cout << "This value is used to compare two colors" << endl;
	cin >> m_colorRatio;
}

/*
	Name : calculatePerfectHitBox
	Return : void
	Attributes :
	Behavior : Calculate the perfect hit box from a sprite : get the first pixel encoutered in each axis and from the bottom and top
*/
void SpriteMaker::calculatePerfectHitBox()
{	
	int lowerY = m_height;
	int higherY = -1;
	int lowerX = m_width;
	int higherX = -1;
	int hitBoxHeight = 0;
	int hitBoxWidth = 0;

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			if (m_sprite.getPixel(j, i).a != 0)
			{
				if (j < lowerX)
					lowerX = j;

				if (j > higherX)
					higherX = j;

				if (i < lowerY)
					lowerY = i;

				if (i > higherY)
					higherY = i;
			}
		}
	}

	hitBoxHeight = higherY - lowerY;
	hitBoxWidth = higherX - lowerX;

	cout << "-------------------------" << endl;
	cout << "| Hitbox height : " << hitBoxHeight << endl;
	cout << "| Hitbox width : " << hitBoxWidth << endl;
	cout << "-------------------------" << endl;
}

/*
	Name : changeImageName
	Return : void
	Attributes :
	Behavior : Ask to the user to enter the name of the image
*/
void SpriteMaker::changeImageName()
{
	cout << "Enter the name of your image (without the extension) : ";
	cin.ignore();
	getline(cin, m_imageName);
}

/*
	Name : changeImageExtension
	Return : void
	Attributes :
	Behavior : Ask to the user to enter the extension of the image
*/
void SpriteMaker::changeImageExtension()
{
	cout << "Enter the enxtesion of your image (without the '.') : ";
	cin.ignore();
	getline(cin, m_imageExtension);
}