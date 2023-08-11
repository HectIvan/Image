#include "IMAGE.h"

void CopyImage(IMAGE& image);
void CopyImage(IMAGE& image)
{
	image.OpenImage(image.AskForFileName(image.m_name));
	image.ResetName();
}

float lsR = 0.55f;
float lsG = 0.60f;
float lsB = 0.03f;

COLOR GreyScale(int px, int py, COLOR color)
{
	float R = color._color.R / 255.0f;
	float G = color._color.G / 255.0f;
	float B = color._color.B / 255.0f;

	float lum = (R * lsR + G * lsG + B * lsB) * 255.0f;
	unsigned char lumInt = static_cast<char>(min(255, lum));

	return COLOR(lumInt, lumInt, lumInt);
}

LinearColor Identity(const IMAGE& img, int pX, int pY)
{
	Vector2D textCoordSize(1.0f / img.GetWidth(), 1.0f / img.GetHeight());

	return LinearColor(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY)));
}

LinearColor Blur(const IMAGE& img, int pX, int pY)
{
	return LinearColor();
}

LinearColor SobelX(const IMAGE& img, int pX, int pY)
{
	const float kernel[] = { -1.0f, 0.0f, 1.0f,
							 -2.0f, 0.0f, 2.0f,
							 -1.0f, 0.0f, 1.0f };

	Vector2D textCoordSize(1.0f / img.GetWidth(), 1.0f / img.GetHeight());

	LinearColor p00(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY - 1)));
	LinearColor p01(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY - 1)));
	LinearColor p02(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY - 1)));

	LinearColor p10(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY + 0)));
	LinearColor p11(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY)));
	LinearColor p12(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY + 0)));

	LinearColor p20(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY + 1)));
	LinearColor p21(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY + 1)));
	LinearColor p22(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY + 1)));

	LinearColor FinalColor = p00 * kernel[0] +
		p01 * kernel[1] +
		p02 * kernel[2] +
		p10 * kernel[3] +
		p11 * kernel[4] +
		p12 * kernel[5] +
		p20 * kernel[6] +
		p21 * kernel[7] +
		p22 * kernel[8];

	return FinalColor;
}

/*LinearColor Identity(const IMAGE& img, int pX, int pY)
{
	const float kernel[] = { 0,0,0,0,1,0,0,0,0 };

	Vector2D texCoordSize(1.0f / img.m_width, 1.0f / img.m_height);

	LinearColor p00(img.PointSample(texCoordSize.x * (pX - 1), texCoordSize.y * (pY - 1)));
	LinearColor p01(img.PointSample(texCoordSize.x * (pX), texCoordSize.y * (pY - 1)));
	LinearColor p02(img.PointSample(texCoordSize.x * (pX + 1), texCoordSize.y * (pY - 1)));

	LinearColor p10(img.PointSample(texCoordSize.x * (pX - 1), texCoordSize.y * (pY + 0)));
	LinearColor p11(img.PointSample(texCoordSize.x * (pX), texCoordSize.y * (pY + 0)));
	LinearColor p12(img.PointSample(texCoordSize.x * (pX + 1), texCoordSize.y * (pY + 0)));

	LinearColor p20(img.PointSample(texCoordSize.x * (pX - 1), texCoordSize.y * (pY + 1)));
	LinearColor p21(img.PointSample(texCoordSize.x * (pX), texCoordSize.y * (pY + 1)));
	LinearColor p22(img.PointSample(texCoordSize.x * (pX + 1), texCoordSize.y * (pY + 1)));

	LinearColor finalColor = p00 * kernel[0] +
		p01 * kernel[1] +
		p02 * kernel[2] +
		p10 * kernel[3] +
		p11 * kernel[4] +
		p12 * kernel[5] +
		p20 * kernel[6] +
		p21 * kernel[7] +
		p22 * kernel[8];

	return finalColor;
}*/

LinearColor SobelY(const IMAGE& img, int pX, int pY)
{
	const float kernel[] = { 1.0f, 2.0f, 1.0f,
							 0.0f, 0.0f, 0.0f,
							 -1.0f, -2.0f, -1.0f };

	Vector2D textCoordSize(1.0f / img.GetWidth(), 1.0f / img.GetHeight());

	LinearColor p00(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY - 1)));
	LinearColor p01(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY - 1)));
	LinearColor p02(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY - 1)));

	LinearColor p10(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY + 0)));
	LinearColor p11(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY)));
	LinearColor p12(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY + 0)));

	LinearColor p20(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY + 1)));
	LinearColor p21(img.PointSample(textCoordSize.x * (pX), textCoordSize.y * (pY + 1)));
	LinearColor p22(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY + 1)));

	LinearColor FinalColor = p00 * kernel[0] +
		p01 * kernel[1] +
		p02 * kernel[2] +
		p10 * kernel[3] +
		p11 * kernel[4] +
		p12 * kernel[5] +
		p20 * kernel[6] +
		p21 * kernel[7] +
		p22 * kernel[8];

	return FinalColor;
}

LinearColor NormalMap(const IMAGE& img, int pX, int pY) 
{
	/*Vector2D textCoordSize(1.0f / img.m_width, 1.0f / img.m_height);

	// sobelX
	LinearColor p00(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY - 1)));
	LinearColor p01(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY + 0)));
	LinearColor p02(img.PointSample(textCoordSize.x * (pX - 1), textCoordSize.y * (pY + 1)));

	LinearColor p20(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY - 1)));
	LinearColor p21(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY + 0)));
	LinearColor p22(img.PointSample(textCoordSize.x * (pX + 1), textCoordSize.y * (pY + 1)));


	LinearColor p10(img.PointSample(textCoordSize.x * (pX + 0), textCoordSize.y * (pY - 1)));
	LinearColor p12(img.PointSample(textCoordSize.x * (pX + 0), textCoordSize.y * (pY + 1)));

	LinearColor SobelX = p00 * -1 + p01 * -2 + p02 * -1 + p20 + p21 * 2 + p22;
	LinearColor SobelY = p00 + p10 * 2 + p20 - p02 + p02 * -2 - p12;

	float lumX = (SobelX.m_R * lsR + SobelX.m_G * lsG + SobelX.m_B * lsB);
	float lumY = (SobelY.m_R * lsR + SobelY.m_G * lsG + SobelY.m_B * lsB);

	return LinearColor(lumX, lumY, 0.0f);*/

	Vector2D texCoordsSize(1.0f / img.GetWidth(), 1.0f / img.GetHeight());

	LinearColor p00(img.PointSample(texCoordsSize.x * (pX - 1), texCoordsSize.y * (pY - 1)));
	LinearColor p01(img.PointSample(texCoordsSize.x * (pX), texCoordsSize.y * (pY - 1)));
	LinearColor p02(img.PointSample(texCoordsSize.x * (pX + 1), texCoordsSize.y * (pY - 1)));

	LinearColor p10(img.PointSample(texCoordsSize.x * (pX - 1), texCoordsSize.y * (pY + 0)));
	LinearColor p12(img.PointSample(texCoordsSize.x * (pX + 1), texCoordsSize.y * (pY + 0)));

	LinearColor p20(img.PointSample(texCoordsSize.x * (pX - 1), texCoordsSize.y * (pY + 1)));
	LinearColor p21(img.PointSample(texCoordsSize.x * (pX), texCoordsSize.y * (pY + 1)));
	LinearColor p22(img.PointSample(texCoordsSize.x * (pX + 1), texCoordsSize.y * (pY + 1)));

	LinearColor sobelX = p00 + 2 * p10 + p20 - p02 - 2 * p12 - p22;
	LinearColor sobelY = p00 + 2 * p01 + p02 - p20 - 2 * p21 - p22;

	float lumY = (sobelY.m_R * lsR + sobelY.m_G * lsG + sobelY.m_B * lsB);
	float lumX = (sobelX.m_R * lsR + sobelX.m_G * lsG + sobelX.m_B * lsB);

	return LinearColor(lumX, lumX, lumX);
}

int main()
{
	IMAGE image;
	image.OpenImage("snake.bmp");
	image.ProcessImage(NormalMap);
	image.SaveImage("snakeNormal.bmp");
	return 0;
}
