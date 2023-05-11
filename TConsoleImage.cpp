#pragma once
#define T_CONSOLE_IMAGE_
#include "TConsoleImage.h"

void TConsoleImage::getBMPImageInfo(std::string file) {
	std::ifstream bmp(file, std::ios::binary);

	byte_ header[54];
	bmp.read(header, 54);

	size.width = *(int*)(header + 18) - *(int*)(header + 18) % 2;
	size.height = *(int*)(header + 22);
	
	mapSize = size.width * size.height * 3; //24 bytes
	map = new byte_[mapSize];
	bmp.read(map, mapSize);
	
	bmp.close();
}


void TConsoleImage::setPosition(int16_t x, int16_t y) {
	position.X = x;
	position.Y = y;
}

COORD TConsoleImage::getPosition() {
	return position;
}