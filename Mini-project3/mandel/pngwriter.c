#include <stdlib.h>
#include "pngwriter.h"

png_data* png_create (int nWidth, int nHeight)
{
	int i;
	
	png_data* pData = (png_data*) malloc (sizeof (png_data));
	
	pData->nWidth = nWidth;
	pData->nHeight = nHeight;
	pData->pPixels = (png_bytepp) malloc (nHeight * sizeof (png_bytep));
	for (i = 0; i < nHeight; i++)
		pData->pPixels[i] = (png_bytep) malloc (3 * nWidth * sizeof (png_byte));
	
	return pData;
}

#define CHECK_RGB_BOUNDS(x) if (x > 255) x = 255; if (x < 0) x = 0;
 
void png_plot (png_data* pData, int x, int y, int r, int g, int b)
{
	if (x >= pData->nWidth)
		return;
	if (y >= pData->nHeight)
		return;

	CHECK_RGB_BOUNDS(r)
	CHECK_RGB_BOUNDS(g)
	CHECK_RGB_BOUNDS(b)
	
	pData->pPixels[pData->nHeight - y - 1][3 * x - 3] = (char) r;
	pData->pPixels[pData->nHeight - y - 1][3 * x - 2] = (char) g;
	pData->pPixels[pData->nHeight - y - 1][3 * x - 1] = (char) b;
}

void png_write (png_data* pData, char* szFileName)
{
	FILE* fp;
	png_structp png_ptr;
	png_infop info_ptr;

	fp = fopen (szFileName, "wb");
	if( fp == NULL)
		return;

	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct (png_ptr);
	png_init_io (png_ptr, fp);
	png_set_compression_level (png_ptr, PNGWRITER_DEFAULT_COMPRESSION);

	png_set_IHDR (png_ptr, info_ptr, pData->nWidth, pData->nHeight,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_set_gAMA (png_ptr, info_ptr, 0.7);
   
	png_write_info (png_ptr, info_ptr);
	png_write_image (png_ptr, pData->pPixels);
	png_write_end (png_ptr, info_ptr);
	png_destroy_write_struct (&png_ptr, &info_ptr);
	fclose (fp);
	
	free (pData);
}
