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
	std::vector<COLOR> m_pixMatrix;
	std::string m_name;

	IMAGE();
	~IMAGE();
	std::string AskForFileName(std::string& name);
	void ResetName();
	int SaveImage(const std::string& fileName, int x = 0, int y = 0);
	void OpenImage(const std::string& fileName);
	void PutPixel(int x, int y, int width, COLOR color);
	COLOR GetPixel(int& X, int& Y) const;
};