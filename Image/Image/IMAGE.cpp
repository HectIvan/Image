#include "IMAGE.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>

IMAGE::IMAGE()
{
	m_width = 100;
	m_height = 100;
	m_BPP = 4;
	m_posPix = 0;
	for (int i = 0; i < m_height; ++i)
	{
		std::vector<COLOR> colorVec;
		colorVec.reserve(m_width);
		m_pixMatrix.push_back(colorVec);
		for (int j = 0; j < m_pixMatrix[i].size(); ++j)
		{
			COLOR color(0, 0, 0 ,0);
			m_pixMatrix[i][j] = color;
		}
	}
}

IMAGE::~IMAGE()
{
}

std::vector<char> IMAGE::OpenImage(const std::string& fileName)
{
	std::fstream openFile(fileName.c_str(), std::ios::binary | std::ios::ate);
	if (!openFile.is_open())
	{
		return;
	}
	SIZE_T fileEnd = openFile.tellg();
	openFile.seekp(std::streampos::fpos(0));

	std::vector<char> infoBuffer;
	infoBuffer.resize(fileEnd);
	openFile.read(infoBuffer.data(), fileEnd);

	return infoBuffer;
}

int IMAGE::SaveImage()
{
	std::cout << "File name: ";
	std::string name;
	std::cin >> name;
	std::cout << "File type: ";
	std::string fileType;
	std::cin >> fileType;
	name += "." + fileType;

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&info, 0, sizeof(BITMAPINFOHEADER));

	//(name.c_str(), 0, 0, fname, 0);

	bmfh.bfType = 0x4d42;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfOffBits = 0x36;

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = m_width;
	info.biHeight = m_height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 2834;
	info.biYPelsPerMeter = 2834;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	char fname[_MAX_FNAME];
	char imageName[_MAX_FNAME];

	_splitpath(name.c_str(), NULL, NULL, fname, NULL);
	sprintf(imageName, "%s.bmp", fname);

	HANDLE file = CreateFileA(imageName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL)
	{
		std::cout << "NULL FILE" << std::endl;
		CloseHandle(file);
		return -1;
	}

	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof(BITMAPFILEHEADER), &bwritten, NULL) == false)
	{
		std::cout << "Failed to write file" << std::endl;
		CloseHandle(file);
		return -2;
	}
}

void IMAGE::CopyImage(IMAGE& imageSource)
{
	
}

void IMAGE::PutPixel(unsigned char* image, int x, int y, int width, int height, int BPP, COLOR color)
{
	/*BPP = 4;
	width = 100;
	height = 100;
	m_posPix = (x + y * width) * BPP;
	image[m_posPix + 0] = 'R';
	image[m_posPix + 1] = 'G';
	image[m_posPix + 2] = 'B';
	image[m_posPix + 3] = 'A';*/
	for (int i = 0; i < m_pixMatrix.size(); ++i)
	{
		if (i == y)
		{
			for (int j = 0; j < m_pixMatrix[i].size(); ++j)
			{
				if (j == x)
				{
					m_pixMatrix[i][j] = color;
				}
			}
		}
	}
}

void IMAGE::GetPixel(IMAGE& imageSource, int X, int Y)
{
	for (int i = 0; i < m_pixMatrix.size(); ++i)
	{
		for (int j = 0; j < m_pixMatrix[i].size(); ++j)
		{
			if (j == Y && i == X)
			{
				// pixel found
				std::cout << "Pixel found at \nX -> " << X << "\nY -> " << Y 
							 << "\n colors -> R:" << m_pixMatrix[i][j].m_R 
							 << "\nG:" << m_pixMatrix[i][j].m_G 
							 << "\nB:" << m_pixMatrix[i][j].m_B 
							 << "\nAlpha:" << m_pixMatrix[i][j].m_A;
			}
		}
	}
}
