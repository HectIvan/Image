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
	int m_blankHeight;
	int m_blankWidth;
	int m_BPP;
	bool m_createdFile;
	std::vector<COLOR> m_blankMatrix;
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
	int GetPitch() { return m_width * m_BPP; }

	void Scale(int sca);
	void Rotation(int rot);
	void Dim(float dimFactor);
	void PlaceImage();
	void GrayScale();
	void CreateBlank();
};