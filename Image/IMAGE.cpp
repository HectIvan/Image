#include "IMAGE.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
// potencia de 2, multiplo de 4

IMAGE::IMAGE()
{
	m_width = 100;
	m_height = 100;
	m_name = "C:/Users/theun/Documents/Visual Studio Projects/Image/Image/";
	m_BPP = 3;
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
	m_name = "C:/Users/theun/Documents/Visual Studio Projects/Image/Image/";
}

void IMAGE::OpenImage(const std::string& fileName)
{
	// open file
	std::ifstream openFile;
	openFile.open(fileName, std::ios::binary | std::ios::in);
	if (!openFile.is_open())
	{
		std::cout << "Failed to open file" << std::endl;
		return;
	}

	// verify if bmp file
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	openFile.read(reinterpret_cast<char*>(&bmfh), sizeof(BITMAPFILEHEADER));
	openFile.read(reinterpret_cast<char*>(&info), sizeof(BITMAPINFOHEADER));

	if (bmfh.bfType != 0x4d42)
	{
		std::cout << "File not BMP file" << std::endl;
		openFile.close();
		return;
	}

	m_width = info.biWidth;
	m_height = info.biHeight;
	int bpp = info.biBitCount;
	m_BPP = bpp / 8;
	// iuc3wgvb4yrbge43wfug32ufhw
	m_pixMatrix.resize(m_width * m_height);
	const int pA = ((4 - (m_width * m_BPP) % 4) % 4);
	// insert values in vector
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			if (m_BPP != 3)
			{
				unsigned char color[4];
				openFile.read(reinterpret_cast<char*>(color), 4);
				m_pixMatrix[i * m_width + j].m_A = static_cast<unsigned char>(color[3]);
				m_pixMatrix[i * m_width + j].m_R = static_cast<unsigned char>(color[2]);
				m_pixMatrix[i * m_width + j].m_G = static_cast<unsigned char>(color[1]);
				m_pixMatrix[i * m_width + j].m_B = static_cast<unsigned char>(color[0]);
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
	}
	openFile.close();
}

int IMAGE::SaveImage(const std::string& fileName, int x = 0, int y = 0)
{
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
	const int pA = ((4 - (m_width * m_BPP) % 4) % 4);
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
	info.biBitCount = m_BPP * 8;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 2834;
	info.biYPelsPerMeter = 2834;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	// create files
	createFile.write(reinterpret_cast<char*>(&bmfh), fileHeaderSize);
	createFile.write(reinterpret_cast<char*>(&info), infoHeaderSize);

	//m_pixMatrix.resize(m_width * m_height);
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
				red = '255';
				green = '255';
				blue = '255';
			}
			unsigned char color[] = { blue, green, red, alpha };
			createFile.write(reinterpret_cast<char*>(color), 4);
		}
		createFile.write(reinterpret_cast<char*>(bmpPad), pA);
	}
	createFile.close();
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

// based on "Designed by Hugo." (22 jan 2021). Creating a Bitmap Image (.bmp) using C++ | Tutorial. Youtube. https://www.youtube.com/watch?v=vqT5j38bWGg&t=999s&ab_channel=DesignedbyHugo && https://www.youtube.com/watch?v=NcEE5xmpgQ0&t=713s&ab_channel=DesignedbyHugo