#include "SpriteMaker.h"

SpriteMaker::SpriteMaker()
{
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

SpriteMaker::~SpriteMaker()
{
	delete m_colorForMask;
	for (int i = 0; i < m_colors.size(); ++i)
	{
		delete m_colors[i];
	}
}

void SpriteMaker::configure()
{
	char choice = '0';
	int r, g, b;

	while (choice != '8')
	{
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
			treatInput();
			break;

		default:
			break;
		}
	}
}

void SpriteMaker::treatInput()
{	
	
	if (!m_input.loadFromFile(m_folder + m_imageName + "." + m_imageExtension))
		return;

	m_width = m_input.getSize().x;
	m_height = m_input.getSize().y;

	m_collisionMask.create(m_width, m_height, Color::White);
	m_sprite.create(m_width, m_height, Color::White);
	Color * tmp = NULL;

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			if (QuitTheSameColor(m_input.getPixel(j, i), *m_colorForMask))
			{
				m_collisionMask.setPixel(j, i, Color::Black);
			}

			for (int k = 0; k < m_colors.size(); ++k)
			{
				if (QuitTheSameColor(m_input.getPixel(j, i), *(m_colors[k])))
				{
					tmp = new Color(m_colors[k]->r, m_colors[k]->g, m_colors[k]->b, 0);
					m_sprite.setPixel(j, i, *tmp);
				}
				else
				{
					m_sprite.setPixel(j, i, m_input.getPixel(j, i));
				}
			}
		}
	}

	calculatePerfectHitBox();
	m_collisionMask.saveToFile(m_folder + m_imageName + "_Mask." + m_imageExtension);
	m_sprite.saveToFile(m_folder + m_imageName + "_Sprite.png");

	delete tmp;
}

bool SpriteMaker::QuitTheSameColor(Color p_a, Color p_b)
{
	int quitTheSame = 0;

	if (p_a.r - m_colorRatio <= p_b.r && p_b.r <= p_a.r + m_colorRatio)
		quitTheSame++;

	if (p_a.g - m_colorRatio <= p_b.g && p_b.g <= p_a.g + m_colorRatio)
		quitTheSame++;

	if (p_a.b - m_colorRatio <= p_b.b && p_b.b <= p_a.b + m_colorRatio)
		quitTheSame++;

	if (quitTheSame == 3)
		return true;
	else
		return false;
}

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

void SpriteMaker::removeColorForTransparency()
{
	char choice = 'a';
	int index = -1;

	while (choice != 'z' && !m_colors.empty())
	{
		cout << "Current color : " << endl;
		cout << "    z. Quit" << endl;

		for (int i = 0; i < m_colors.size(); ++i)
		{
			cout << "    " << i << " -> R:" << (int)m_colors[i]->r << " G:" << (int)m_colors[i]->g << " B:" << (int)m_colors[i]->b << endl;
		}
		cout << endl << "Color to erase (enter 1 or 2, ...) : ";
		cin >> choice;

		if (48 <= choice && choice <= 57)
		{
			index = atoi(&choice);
		}
		else if (choice != 'z')
		{
			index = m_colors.size();
		}

		if (index < m_colors.size() && choice != 'z')
		{
			m_colors.erase(m_colors.begin() + index);
		}

		cout << endl;
	}
}

void SpriteMaker::changeColorRatio()
{
	cout << "Enter the possible delta between the input image colors and your colors for the transparency." << endl;
	cout << "This value is used to compare two colors" << endl;
	cin >> m_colorRatio;
}

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

void SpriteMaker::changeImageName()
{
	cout << "Enter the name of your image (without the extension) : ";
	cin.ignore();
	getline(cin, m_imageName);
}

void SpriteMaker::changeImageExtension()
{
	cout << "Enter the enxtesion of your image (without the '.') : ";
	cin.ignore();
	getline(cin, m_imageExtension);
}