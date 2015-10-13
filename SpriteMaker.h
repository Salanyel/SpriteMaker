#pragma once
#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>
#include <vector>

using namespace std;
using namespace sf;

class SpriteMaker
{
private :
	//Array containg all colors needed for the transparency
	vector<Color * > m_colors;
	//One unique color to create the black & white mask
	Color * m_colorForMask;
	//String which is the folder where the image will be loaded and saved
	string m_folder;
	//Image name (for loading it)
	string m_imageName;
	//Image extension (for loading it)
	string m_imageExtension;
	//Object image which will load the image from the folder, name & extension
	Image m_input;
	//Object image that will be created from the input for the mask
	Image m_collisionMask;
	//Object image that will be created from the input for the transparent sprite
	Image m_sprite;

	//Width of m_input
	int m_width;
	//Height of m_input
	int m_height;
	//Delta available to detect a color in a compare between two pixels
	int m_colorRatio;

public:
	//Basic constructor
	SpriteMaker();
	//Constructor with single name, extension and a color ratio
	SpriteMaker(string p_imageName, string p_extension, int p_colorRatio);
	//Destructor
	~SpriteMaker();

	//Main execution of the tool
	void mainExecution();

	//Operate the Sprite and masks creation
	void operate();

	//Compare two color and return true if they're quit the same
	bool quitTheSameColor(Color p_a, Color p_b);

	//Change the color detected for the black and white mask
	void changeColorForMask();

	//Add a color for the transparency
	void addColorForTransparency();

	//Remove a color from m_color
	void removeColorForTransparency();

	//Change the color ratio used in QuitTheSameColor
	void changeColorRatio();

	//Calculate the perfect hit box for the sprite
	void calculatePerfectHitBox();

	//Change the name of the image that will be loaded
	void changeImageName(); 

	//Change the extension of the image that will be loaded
	void changeImageExtension();
};