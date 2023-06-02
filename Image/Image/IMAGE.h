#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>
#include "COLOR.h"

class IMAGE
{
public:
	int m_height;
	int m_width;
	int m_BPP;
	int m_posPix;
	std::vector<std::vector<COLOR>> m_pixMatrix;

	IMAGE();
	~IMAGE();
	std::vector<char> OpenImage(const std::string& fileName);
	int SaveImage();
	void CopyImage(IMAGE& imageSource);
	void PutPixel(unsigned char* image, int x, int y, int width, int height, int BPP, COLOR color);
	void GetPixel(IMAGE& imageSource, int X, int Y);
};