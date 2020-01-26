#include "Image.h"
#include <string>

using namespace std;

int main()
{
	Image domo;
	Image grim;
	Image overlap;
	Color mask;

	mask.r = 255;
	mask.g = 0;
	mask.b = 0;

	domo.loadImage("domo_blur.ppm");
	grim.loadImage("grim.ppm");
	overlap = domo.superimpose(grim, mask);
	overlap.saveImage("overlap.ppm");

	

	return 0;
}
