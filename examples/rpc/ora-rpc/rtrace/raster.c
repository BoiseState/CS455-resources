#include <stdio.h>
#include "raster.h"

char           *
ReadSunRaster(fp, pColormap, pRast)
  FILE           *fp;
  colormap_t     *pColormap;
  rasterfile     *pRast;
{
  int             pixels;
  char           *pImage;

  if (fread(pRast, 1, sizeof(rasterfile), fp)
      != sizeof(rasterfile))
    fprintf(stderr, "ReadSunRaster: Error reading raster header\n");
  if (pRast->ras_type != RT_STANDARD)
    fprintf(stderr, "ReadSunRaster: not an RT_STANDARD rasterfile\n");
  pColormap = (colormap_t *) malloc(sizeof(colormap_t));
  if (fread(pColormap, 1, pRast->ras_maplength, fp)
      != pRast->ras_maplength)
    fprintf(stderr, "ReadSunRaster: Error reading colormap\n");

  /* allocate room for and read in the image */
  pixels = pRast->ras_width * pRast->ras_height;
  switch (pRast->ras_depth) {
  case 8:
    break;
  case 16:
    pixels <<= 1;
  default:
    fprintf(stderr, "ReadSunRaster: image must be 8 or 16 deep\n");
  }
  pImage = (char *) malloc(pixels);
  if (fread(pImage, 1, pixels, fp) != pixels)
    free(pImage);
  if (!pImage)
    fprintf(stderr, "ReadSunRaster: no memory or bad read\n");

  return (pImage);
}

WriteSunRaster(fp, pColormap, pRast, pData)
  FILE           *fp;
  colormap_t     *pColormap;
  rasterfile     *pRast;
  char           *pData;
{
  int             pixels;

  if (fwrite(pRast, 1, sizeof(rasterfile), fp)
      != sizeof(rasterfile))
    fprintf(stderr, "WriteSunRaster: Error writing raster header\n");
  if (pRast->ras_type != RT_STANDARD)
    fprintf(stderr, "WriteSunRaster: not an RT_STANDARD rasterfile\n");

  /*
   * if we have a valid colormap, write it out, should
   * catch errors here!
   */
  if ((pRast->ras_maptype != RMT_NONE) && (!pRast->ras_maplength) && !pColormap) {
    if (fwrite(pColormap, 1, pRast->ras_maplength, fp)
	!= pRast->ras_maplength)
      fprintf(stderr, "WriteSunRaster: Error writing colormap\n");
  }
  /* write-out image data */
  pixels = pRast->ras_width * pRast->ras_height;
  switch (pRast->ras_depth) {
  case 8:
    break;
  case 16:
    pixels <<= 1;
  default:
    fprintf(stderr, "WriteSunRaster: image must be 8 or 16 deep\n");
  }
  /*
   * if pData is NULL, assume we repeat 0's.. useful for
   * somethings...
   */
  if (pData) {
    if (fwrite(pData, 1, pixels, fp) != pixels)
      fprintf(stderr, "WriteSunRaster: bad write\n");
  } else
    while (pixels--)
      fputc((char) 0, fp);

  fflush(fp);
}

#ifdef RAS_IO_TEST
main(argc, argv)
  int             argc;
  char          **argv;
{
  colormap_t      cm;
  rasterfile      rast;
  FILE           *fp_in, *fp_out;
  char           *d;

  fp_in = fopen(argv[1], "r");
  fp_out = fopen(argv[2], "w");
  d = ReadSunRaster(fp_in, &cm, &rast);
  WriteSunRaster(fp_out, &cm, &rast, d);

  /* now try writing out NULL colormap and data! */
  fp_out = fopen(argv[3], "w");
  rast.ras_maptype = RMT_NONE;
  rast.ras_maplength = 0;
  WriteSunRaster(fp_out, NULL, &rast, NULL);
}
#endif
