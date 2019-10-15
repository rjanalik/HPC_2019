#ifndef PNGWRITER_H_
#define PNGWRITER_H_

#include <png.h>

#define PNGWRITER_DEFAULT_COMPRESSION 6

typedef struct
{
	png_bytepp pPixels;
	int nWidth;
	int nHeight;
} png_data;

png_data* png_create (int nWidth, int nHeight);
void png_plot (png_data* pData, int x, int y, int r, int g, int b);
void png_write (png_data* pData, char* szFileName);

#endif /*PNGWRITER_H_*/
