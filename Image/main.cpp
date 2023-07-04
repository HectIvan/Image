#include "IMAGE.h"

void CopyImage(IMAGE& image);
void CopyImage(IMAGE& image)
{
	image.OpenImage(image.AskForFileName(image.m_name));
	image.ResetName();
}
int main()
{
	IMAGE image;
	bool active = true;
	while (active)
	{
		char a;
		std::cout << "process > ";
		std::cin >> a;
		a = tolower(a);
		switch (a)
		{
		case 'c':
			image.CreateBlank();
			break;
		case 'o':
			image.OpenImage(image.AskForFileName(image.m_name));
			break;
		case 's':
			image.SaveImage(image.AskForFileName(image.m_name));
			break;
		case 'd':
			std::cout << "dim level> ";
			float dimVal;
			std::cin >> dimVal;
			image.Dim(dimVal);
			break;
		case 'p':
			//image.PlaceImage();
			break;
		case 'g':
			image.GrayScale();
			break;
		default:
			break;
		}
	}
	return 0;
}
