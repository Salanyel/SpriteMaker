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
	vector<Color * > m_colors;
	Color * m_colorForMask;
	string m_folder;
	string m_imageName;
	string m_imageExtension;
	Image m_input;
	Image m_collisionMask;
	Image m_sprite;
	int m_width;
	int m_height;
	int m_colorRatio;

public:
	SpriteMaker();
	SpriteMaker(string p_imageName, string p_extension, int p_colorRatio);
	~SpriteMaker();

	void configure();
	void treatInput();
	bool QuitTheSameColor(Color p_a, Color p_b);
	void changeColorForMask();
	void addColorForTransparency();
	void removeColorForTransparency();
	void changeColorRatio();
	void calculatePerfectHitBox();
	void changeImageName(); 
	void changeImageExtension();
};