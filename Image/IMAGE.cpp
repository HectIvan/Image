#include "IMAGE.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using std::fstream;
// potencia de 2, multiplo de 4

IMAGE::IMAGE(int width, int height, int bpp)
{
	assert(m_width > 0 && m_width < 16383);
	assert(m_height > 0 && m_height < 16383);
	m_name = "";
	m_BPP = 3;
	m_createdFile = false;
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
	fstream openFile(fileName.c_str(), std::ios::binary | std::ios::in);
	//openFile.open(fileName.c_str(), std::ios::binary | std::ios::in);
	bool open = openFile.is_open();
	assert(open);
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
	//size_t resize = m_width * m_height;
	//m_pixMatrix.resize(resize);
	//const int pA = ((4 - (m_width * m_BPP) % 4) % 4);
	

	int padding = GetPitch() % 4;
	int lineMemoryWidth = GetPitch() + padding;
	// insert values in vector
	for (int line = m_height - 1; line >= 0; --line)
	{
		openFile.seekp(lineMemoryWidth * line + bmfh.bfOffBits);
		openFile.read(reinterpret_cast<char*>(&m_pixMatrix[GetPitch() * (m_height - 1 -line)]), GetPitch());
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
	//m_pixMatrix = m_blankMatrix;
	//m_width = m_blankHeight;
	//m_height = m_blankHeight;
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
	info.biBitCount = static_cast<WORD>(m_BPP * 3);
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 3780;
	info.biYPelsPerMeter = 3780;
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
void IMAGE::PutPixel(int x, int y, COLOR color)
{
	assert(x < 0 && x >= m_width);
	assert(y < 0 && y >= m_height);
	assert(m_BPP < 3 && m_BPP > 4);
	
	int startPos = GetPitch() * y + (x * m_BPP);

	m_pixMatrix[startPos + 0] = color._color.B;
	m_pixMatrix[startPos + 1] = color._color.G;
	m_pixMatrix[startPos + 2] = color._color.R;
	if (m_BPP == 4) m_pixMatrix[startPos + 3] = color._color.A;
}

// get pixel in a specific location
COLOR IMAGE::GetPixel(int x, int y) const
{
	assert(x >= 0 && x < m_width);
	assert(y >= 0 && y < m_height);

	int startPos = GetPitch() * y + (x * m_BPP);
	
	return COLOR(m_pixMatrix[startPos + 2],
				 m_pixMatrix[startPos + 1],
				 m_pixMatrix[startPos + 0],
				 m_BPP == 3 ? 255 : m_pixMatrix[startPos + 3]);
}

int IMAGE::GetHeight() const
{
	return m_height;
}

int IMAGE::GetWidth() const
{
	return m_width;
}

void IMAGE::BitBlt(const IMAGE& src,
				   int x, 
				   int y, 
				   int srcIniX, 
				   int srcIniY, 
				   int srcEndX, 
				   int srcEndY)
{
	if (x < 0) { srcIniX += abs(x); }
	if (y < 0) { srcIniY += abs(y); }
	if (x + srcEndX >= m_width) { srcIniX -= (x + srcEndX) - m_width - 1; }
	if (x + srcEndY >= m_height) { srcIniY -= (y + srcEndY) - m_height - 1; }
	
	int minX = min(srcIniX, srcEndX);
	int minY = min(srcIniY, srcEndY);
	srcEndX = max(srcIniX, srcEndX);
	srcEndY = max(srcIniY, srcEndY);
	srcIniX = minX;
	srcIniY = minY;

	int realwidth = srcEndX - srcIniX;
	if (realwidth <= 0 || srcEndY - srcIniY <= 0)
	{
		return;
	}

	int memLineIni = (srcIniX * m_BPP);
#pragma omp parallel for
	for (int iY = srcIniY; iY < srcEndY; ++iY)
	{
		const char* pSrc = src.GetMemoryPtr() + src.GetPitch() * iY + memLineIni;
		char* pDest = &m_pixMatrix[GetPitch() * (y + iY) + (x * m_BPP)];
		memcpy(pDest, pSrc, realwidth + m_BPP);
	}
}

void IMAGE::ProcessImage(std::function<LinearColor(const IMAGE&, int, int)> procFunction)
{
	IMAGE rt(m_width, m_height, m_BPP);

#pragma omp parallel for  // 
	for (int iY = 0; iY < m_height; ++iY)
	{
		COLOR currColor;
		for (int iX = 0; iX < m_width; ++iX)
		{
			LinearColor color = procFunction(*this, iX, iY);
			rt.PutPixel(iX, iY, color.ToColor());
		}
	}

	*this = std::move(rt); // 
}

void IMAGE::AdjustToTextureAdress(float& u, float& v, TEXTURE_ADDRESS::E textAddr) const
{
	switch (textAddr)
	{
	case TEXTURE_ADDRESS::WRAP:
		u = std::fmodf(u, 1.0f);
		v = std::fmodf(v, 1.0f);
		break;
	case TEXTURE_ADDRESS::MIRROR:
		if (u < 0.0 || u > 1.0f)
		{
			u = 1.0f - std::fmodf(u, 1.0f);
		}
		if (v < 0.0 || v > 1.0f)
		{
			v = 1.0f - std::fmodf(v, 1.0f);
		}
		break;
	case TEXTURE_ADDRESS::CLAMP:
		u = std::clamp(u, 0.0f, 1.0f);
		v = std::clamp(v, 0.0f, 1.0f);
		break;
	case TEXTURE_ADDRESS::BORDER:
		break;
	case TEXTURE_ADDRESS::MIRROR_ONCE:
		break;
	default:
		break;
	}
}

COLOR IMAGE::PointSample(float u, float v, TEXTURE_ADDRESS::E textAddr) const
{
	AdjustToTextureAdress(u, v, textAddr);

	int pxX = static_cast<int>((m_width - 1) * u);
	int pxY = static_cast<int>((m_height - 1) * v);

	return GetPixel(pxX, pxY);
}

COLOR IMAGE::Linearample(float u, float v, TEXTURE_ADDRESS::E textAddr) const
{
	AdjustToTextureAdress(u, v, textAddr);

	float pxX1 = (m_width - 1) * u;
	float pxY1 = (m_height - 1) * v;
	int ipxX1 = static_cast<int>(pxX1);
	int ipxY1 = static_cast<int>(pxY1);

	float xRes1 = pxX1 - ipxX1;
	float yRes1 = pxY1 - ipxY1;

	int ipxX2 = xRes1 > 0.0f ? ipxX1 + 1 : std::clamp(ipxX1, 0, (m_width - 1));
	int ipxY2 = yRes1 > 0.0f ? ipxY1 + 1 : std::clamp(ipxY1, 0, (m_height - 1));

	LinearColor Q11(GetPixel(ipxX1, ipxY1));
	LinearColor Q21(GetPixel(ipxX2, ipxY1));
	LinearColor Q12(GetPixel(ipxX1, ipxY2));
	LinearColor Q22(GetPixel(ipxX2, ipxY2));

	LinearColor xy1R = lerp(Q11, Q21, xRes1);
	LinearColor	xy2R = lerp(Q12, Q22, xRes1);
	LinearColor fColor = lerp(xy1R, xy2R, yRes1);

	return fColor.ToColor();
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
		m_pixMatrix[i] *= dimFactor;
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
	//vector2D textCoordSize(1.0f / img)
	/*for (int i = 0; i < m_pixMatrix.size(); ++i)
	{
		m_pixMatrix[i].m_R = (m_pixMatrix[i].m_R  + m_pixMatrix[i].m_G + m_pixMatrix[i].m_B) / 3;
		m_pixMatrix[i].m_G = (m_pixMatrix[i].m_R + m_pixMatrix[i].m_G + m_pixMatrix[i].m_B) / 3;
		m_pixMatrix[i].m_B = (m_pixMatrix[i].m_R + m_pixMatrix[i].m_G + m_pixMatrix[i].m_B) / 3;
	}*/
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
		m_blankMatrix[i] = 255;
	}
	m_createdFile = true;
}
