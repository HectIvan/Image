#include "IMAGE.h"

void CopyImage(IMAGE& image);
void CopyImage(IMAGE& image)
{
	image.OpenImage(image.AskForFileName(image.m_name));
	image.ResetName();
	image.SaveImage(image.AskForFileName(image.m_name));
}
int main()
{
	IMAGE image;
	CopyImage(image);
	return 0;
}
