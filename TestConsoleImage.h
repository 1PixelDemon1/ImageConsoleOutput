#pragma once

typedef char byte_;

#include <string>
#include <fstream>
#include <windows.h>


class TestConsoleImage
{

private:
	byte_* map;
	size_t mapSize;

	struct {
		size_t width;
		size_t height;
	} size;
	
	HDC hdc;

	void getBMPImageInfo(std::string file) {
		std::ifstream bmp(file, std::ios::binary);

		byte_ header[54];
		bmp.read(header, 54);

		
		size.width = *(int*)(header + 18);
		size.height = *(int*)(header + 22);
		mapSize = size.width * size.height * 3;
		map = new byte_[mapSize];
		bmp.read(map, mapSize);
		bmp.close();
	}


public:
	TestConsoleImage() {
		std::string file{"file.bmp"};
		getBMPImageInfo(file);
		hdc = GetDC(GetConsoleWindow());

	}

	void draw() {
		int index{};
		for (size_t i = 0; i < mapSize; i += 3)
		{
			SetPixel(hdc, index % size.width, size.height - index / (size.width), RGB((int) map[i + 2], (int) map[i + 1], (int) map[i]));
			index++;
		}

	}

	virtual ~TestConsoleImage() {
		delete[] map;
	}
};

