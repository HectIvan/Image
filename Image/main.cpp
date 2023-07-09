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
		case 'n': // new
			image.CreateBlank();
			break;
		case 'o': // open
			image.OpenImage(image.AskForFileName(image.m_name));
			break;
		case 's': // save
			image.SaveImage(image.AskForFileName(image.m_name));
			break;
		case 'd': // dim
			std::cout << "dim level> ";
			float dimVal;
			std::cin >> dimVal;
			image.Dim(dimVal);
			break;
		case 'p': //´place
			if (image.m_createdFile) {
				image.PlaceImage();
			}
			else
			{
				std::cout << "blank project not created";
			}
			break;
		case 'g': // gray scale
			image.GrayScale();
			break;
		default:
			break;
		}
	}
	return 0;
}
