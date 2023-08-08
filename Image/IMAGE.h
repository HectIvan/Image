#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>
#include <cmath>
#include "Vector2D.h"
#include <functional>
//#include "COLOR.h"
using uint8 = unsigned int;

template<typename T>
using SPtr = std::shared_ptr<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;

template<class T>
static inline T clamp(const T X, const T Min, const T Max)
{
	return X < Min ? Min : X < Max ? X : Max;
}

template<class T, class U>
static T lerp(const T& A, const T& B, const U& Alpha)
{
	return static_cast<T>(A + Alpha * (B - A));
}

namespace TEXTURE_ADDRESS
{
	enum E
	{
		WRAP = 1,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE
	};
}
struct COLOR final
{
	COLOR() = default;
	COLOR(uint8 r, uint8 g, uint8 b, uint8 a = 255)
	{
		_color.R = r;
		_color.G = g;
		_color.B = b;
		_color.A = a;

	}
	COLOR(const COLOR& other)
	{
		_color.fullColor = other._color.fullColor;
	}
	COLOR(int other)
	{
		_color.fullColor = other;
	}

	operator int() const
	{
		return _color.fullColor;
	}

	union
	{
		int fullColor;
		struct
		{
			uint8 R;
			uint8 G;
			uint8 B;
			uint8 A;
		};
	}_color;
};

struct LinearColor final
{
	float lsR = 0.55f;
	float lsG = 0.60f;
	float lsB = 0.03f;

	LinearColor() = default;
	LinearColor(float R, float G, float B, float A = 1.0f) 
		: m_R(R), 
		  m_G(G),
		  m_B(B),
		  m_A(A)
	{}

	explicit LinearColor(const COLOR& other)
		: m_R(other._color.R / 255.0f),
		  m_G(other._color.G / 255.0f),
		  m_B(other._color.B / 255.0f),
		  m_A(other._color.A / 255.0f)
	{}

	COLOR ToColor()
	{
		return COLOR(static_cast<uint8>(clamp(m_R * 255.0f, 0.0f, 255.0f)),
					 static_cast<uint8>(clamp(m_G * 255.0f, 0.0f, 255.0f)),
					 static_cast<uint8>(clamp(m_B * 255.0f, 0.0f, 255.0f)),
					 static_cast<uint8>(clamp(m_A * 255.0f, 0.0f, 255.0f)));
	}

	LinearColor operator+(const LinearColor& rhs) const
	{
		return LinearColor(m_R + rhs.m_R, m_G + rhs.m_G, m_B + rhs.m_G, m_A + rhs.m_A);
	}

	LinearColor& operator+=(const LinearColor& rhs)
	{
		m_R += rhs.m_R;
		m_G += rhs.m_G;
		m_B += rhs.m_B;
		m_A += rhs.m_A;
		return *this;
	}

	LinearColor operator-(const LinearColor& rhs) const
	{
		return LinearColor(m_R - rhs.m_R, m_G - rhs.m_G, m_B - rhs.m_G, m_A - rhs.m_A);
	}

	LinearColor& operator-=(const LinearColor& rhs)
	{
		m_R -= rhs.m_R;
		m_G -= rhs.m_G;
		m_B -= rhs.m_B;
		m_A -= rhs.m_A;
		return *this;
	}

	LinearColor operator*(const LinearColor& rhs) const
	{
		return LinearColor(m_R * rhs.m_R, m_G * rhs.m_G, m_B * rhs.m_G, m_A * rhs.m_A);
	}

	LinearColor operator*(const float& rhs) const
	{
		return LinearColor(m_R * rhs, m_G * rhs, m_B * rhs, m_A * rhs);
	}

	LinearColor& operator*=(const LinearColor& rhs)
	{
		m_R *= rhs.m_R;
		m_G *= rhs.m_G;
		m_B *= rhs.m_B;
		m_A *= rhs.m_A;
		return *this;
	}

	LinearColor operator/(const LinearColor& rhs) const
	{
		return LinearColor(m_R / rhs.m_R, m_G / rhs.m_G, m_B / rhs.m_G, m_A / rhs.m_A);
	}

	LinearColor& operator/=(const LinearColor& rhs)
	{
		m_R /= rhs.m_R;
		m_G /= rhs.m_G;
		m_B /= rhs.m_B;
		m_A /= rhs.m_A;
		return *this;
	}
	float m_R;
	float m_G;
	float m_B;
	float m_A;
};

inline LinearColor operator*(float Scalar, const LinearColor& color)
{
	return color.operator*(Scalar);
}

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
	std::vector<char> m_pixMatrix;
	std::string m_name;

	IMAGE() = default;
	IMAGE(int width, int height, int bpp);
	~IMAGE() = default;
	std::string AskForFileName(std::string& name);
	void ResetName();
	int SaveImage(const std::string& fileName, int x = 0, int y = 0);
	void OpenImage(const std::string& fileName);
	void PutPixel(int x, int y, COLOR color);
	COLOR GetPixel(int x, int y) const;
	int GetHeight() const;
	int GetWidth() const;
	int GetPitch() const { return m_width * m_BPP; }
	const char* GetMemoryPtr() const { return &m_pixMatrix[0]; }
	void BitBlt(const IMAGE& src,
				int x,
				int y,
				int srcIniX,
				int srcIniY,
				int srcEndX,
				int srcEndY);

	void ProcessImage(std::function<LinearColor(const IMAGE&, int, int)> procFunction);
	void AdjustToTextureAdress(float& u, float& v, TEXTURE_ADDRESS::E textAddr = TEXTURE_ADDRESS::E::WRAP) const;
	COLOR PointSample(float u, float v, TEXTURE_ADDRESS::E textAddr = TEXTURE_ADDRESS::E::WRAP) const;
	COLOR Linearample(float u, float v, TEXTURE_ADDRESS::E textAddr = TEXTURE_ADDRESS::E::WRAP) const;

	void Scale(int sca);
	void Rotation(int rot);
	void Dim(float dimFactor);
	void PlaceImage();
	void GrayScale();
	void CreateBlank();
};