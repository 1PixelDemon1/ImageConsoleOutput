#include "PNGstConsoleIMage.h"

void PNGstConsoleIMage::getPNGImageInfo(std::string file) {
	lodepng::decode(map, size.width, size.height, file.c_str());
}


void PNGstConsoleIMage::setPosition(int16_t x, int16_t y) {
	position.X = x;
	position.Y = y;
}

COORD PNGstConsoleIMage::getPosition() {
	return position;
}

void PNGstConsoleIMage::draw() {

	size_t span{};
	for (size_t i = 0; i < size.height; i++)
	{
		for (size_t j = 0; j < size.width; j++)
		{
			if(map[span + 3] != 0) SetPixel(hdc, position.X + j, position.Y + i, RGB((int)map[span], (int)map[span + 1], (int)map[span + 2]));
			span+=4;
		}
		
	}

}