#include "IMAGE.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// potencia de 2, multiplo de 4

IMAGE::IMAGE()
{
	m_width = 100;
	m_height = 100;
	m_name = "";
	m_BPP = 3;
	m_createdFile = false;
}

IMAGE::~IMAGE()
{
}

std::string IMAGE::AskForFileName(std::string& name)
{
	std::cout << "enter name > ";
	std::string fileName;
	std::cin >> fileName;
	std::string fileType = "bmp";

	name += fileName + "." + fileType;
	return name;
}

void IMAGE::ResetName()
{
	m_name = "";
}

void IMAGE::OpenImage(const std::string& fileName)
{
	// open file
	std::fstream openFile;
	openFile.open(fileName, std::ios::binary | std::ios::in);
	if (!openFile.is_open())
	{
		std::cout << "Failed to open file" << std::endl;
		ResetName();
		return;
	}

	// verify if bmp file
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	openFile.read(reinterpret_cast<char*>(&bmfh), sizeof(&bmfh));
	openFile.read(reinterpret_cast<char*>(&info), sizeof(&info));
	openFile.seekg(bmfh.bfOffBits);

	if (bmfh.bfType != 0x4d42)
	{
		//std::cout << "File not BMP file" << std::endl;
		MessageBoxA(nullptr, "File not BMP file", "Error!!!", MB_OK);
		openFile.close();
		return;
	}

	m_width = info.biWidth;
	m_height = info.biHeight;
	m_BPP = info.biBitCount >> 3;

	m_pixMatrix.resize(m_width * m_height);
	//const int pA = ((4 - (m_width * m_BPP) % 4) % 4);
	

	int padding = GetPitch() % 4;
	int lineMemoryWidth = GetPitch() + padding;
	// insert values in vector
	for (int line = m_height - 1; line >= 0; --line)
	{
		openFile.seekp(lineMemoryWidth * line + bmfh.bfOffBits);
		openFile.read(reinterpret_cast<char*>(&m_pixMatrix[GetPitch() * line]), GetPitch());
	}
	//for (int i = 0; i < m_height; ++i)
	/*for (int i = m_height - 1; i >= 0; --i)
	{
		//for (int j = 0; j < m_width; ++j)
		for (int j = m_width - 1; j >= 0; --j)
		{
			if (m_BPP == 4)
			{
			  openFile.read((char*)&m_pixMatrix[0], m_width * m_height * m_BPP);
			}
			else
			{
				unsigned char color[3];
				openFile.read(reinterpret_cast<char*>(color), 3);
				m_pixMatrix[i * m_width + j].m_R = static_cast<unsigned char>(color[2]);
				m_pixMatrix[i * m_width + j].m_G = static_cast<unsigned char>(color[1]);
				m_pixMatrix[i * m_width + j].m_B = static_cast<unsigned char>(color[0]);
			}
		}
		openFile.ignore(pA);
	}*/
	openFile.close();
	ResetName();
}

int IMAGE::SaveImage(const std::string& fileName, int x, int y)
{
	// pass info
	m_pixMatrix = m_blankMatrix;
	m_width = m_blankHeight;
	m_height = m_blankHeight;
	// open file
	std::ofstream createFile;
	createFile.open(fileName, std::ios::out | std::ios::binary);

	if (!createFile.is_open())
	{
		std::cout << "Could not create file" << std::endl;
		return -1;
	}

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&info, 0, sizeof(BITMAPINFOHEADER));

	// create file sizes
	unsigned char bmpPad[4] = { 0, 0, 0, 0 };
	const int pA = 0;// ((4 - (m_width * m_BPP) % 4) % 4);
	const int fileHeaderSize = 14;
	const int infoHeaderSize = 40;

	int padding = 0;
	int scanlines = m_height * m_BPP;
	int newBufferSize = 0;
	while ((scanlines + padding) % 4 != 0)
	{
		padding++;
	}

	// file type
	bmfh.bfType = 0x4d42;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = fileHeaderSize + infoHeaderSize;
	bmfh.bfSize = fileHeaderSize + infoHeaderSize + (m_width * m_height * 4);

	// file info
	info.biSize = infoHeaderSize;
	info.biWidth = m_width;
	info.biHeight = m_height;
	info.biPlanes = 1;
	info.biBitCount = 32;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 2834;
	info.biYPelsPerMeter = 2834;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	// create files
	createFile.write(reinterpret_cast<char*>(&bmfh), fileHeaderSize);
	createFile.write(reinterpret_cast<char*>(&info), infoHeaderSize);
	createFile.write(reinterpret_cast<char*>(&m_pixMatrix[0]), m_width * m_height * 4);

	//m_pixMatrix.resize(m_width * m_height);
	/*
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			unsigned char red = static_cast<unsigned char>(GetPixel(j, i).m_R);
			unsigned char green = static_cast<unsigned char>(GetPixel(j, i).m_G);
			unsigned char blue = static_cast<unsigned char>(GetPixel(j, i).m_B);
			unsigned char alpha = static_cast<unsigned char>(GetPixel(j, i).m_A);
			if ('0' == alpha)
			{
				red = 255;
				green = 255;
				blue = 255;
			}
			unsigned char color[] = { blue, green, red, alpha };
			createFile.write(reinterpret_cast<char*>(color), 4);
		}
		createFile.write(reinterpret_cast<char*>(bmpPad), pA);
	}
	*/

	createFile.close();

	return 0;
}

// replaces pixel in 
void IMAGE::PutPixel(int x, int y, int width, COLOR color)
{
	// find pixel color
	m_pixMatrix[y * m_width + x] = color;
}

// get pixel in a specific location
COLOR IMAGE::GetPixel(int& X, int& Y) const
{
	// get colors of pixel
	return m_pixMatrix[Y * m_width + X];
}

// scales of image array
void IMAGE::Scale(int sca)
{
	
}

// rotation of image array
void IMAGE::Rotation(int rot)
{
	// cosinus & sinus of angle / producto punto
}

// dims the image array
void IMAGE::Dim(float dimFactor)
{
	dimFactor = std::clamp(dimFactor, 0.0f, 1.0f);
	for (int i = 0; i < m_pixMatrix.size(); ++i)
	{
		m_pixMatrix[i].m_R *= dimFactor;
		m_pixMatrix[i].m_G *= dimFactor;
		m_pixMatrix[i].m_B *= dimFactor;
	}
}

void IMAGE::PlaceImage()
{
	if (m_pixMatrix.size() == 0)
	{
		std::cout << "no image loaded" << std::endl;
		return;
	}
	int x, y;
	std::cout << "image poition" << std::endl << "X > ";
	std::cin >> x;
	std::cout << std::endl << "Y > ";
	std::cin >> y;

	int yPos = 0;
	int xPos = 0;
	for (int i = 0; i < m_blankMatrix.size(); ++i)
	{
		if (i == m_width) {
			++yPos;
			xPos = 0;
		}

		if (xPos == x && yPos == y)
		{
			break;
		}
		++xPos;
	}

	int manager = 0;
	int yLine = 0;
	int imageProjection = 0;
	//for (int i = yPos * xPos; i < m_blankMatrix.size(); ++i)
	for (int i = m_blankMatrix.size(); i < yPos * xPos; --i)
	{
		if (imageProjection < m_pixMatrix.size())
		{
			m_blankMatrix[i] = m_pixMatrix[imageProjection];
		}
		++manager;
		if (manager >= m_width)
		{
			manager = 1;
			i += -m_width + m_blankWidth;// +m_width - yLine;
			++yLine;
		}
		//if (manager >= m_height)
		//{
		//	break;
		//}
		++imageProjection;
	}
}

void IMAGE::GrayScale()
{
	for (int i = 0; i < m_pixMatrix.size(); ++i)
	{
		m_pixMatrix[i].m_R = (m_pixMatrix[i].m_R  + m_pixMatrix[i].m_G + m_pixMatrix[i].m_B) / 3;
		m_pixMatrix[i].m_G = (m_pixMatrix[i].m_R + m_pixMatrix[i].m_G + m_pixMatrix[i].m_B) / 3;
		m_pixMatrix[i].m_B = (m_pixMatrix[i].m_R + m_pixMatrix[i].m_G + m_pixMatrix[i].m_B) / 3;
	}
}
void IMAGE::CreateBlank()
{
	int x, y;
	std::cout << "image size" << std::endl << "X > ";
	std::cin >> x;
	std::cout << std::endl << "Y > ";
	std::cin >> y;
	m_blankMatrix.resize(x * y);
	m_blankHeight = y;
	m_blankWidth = x;
	for (int i = 0; i < m_blankMatrix.size(); ++i)
	{
		m_blankMatrix[i].m_R = 255;
		m_blankMatrix[i].m_G = 255;
		m_blankMatrix[i].m_B = 255;
	}
	m_createdFile = true;
}
// based on "Designed by Hugo." (22 jan 2021). Creating a Bitmap Image (.bmp) using C++ | Tutorial. Youtube. https://www.youtube.com/watch?v=vqT5j38bWGg&t=999s&ab_channel=DesignedbyHugo && https://www.youtube.com/watch?v=NcEE5xmpgQ0&t=713s&ab_channel=DesignedbyHugo