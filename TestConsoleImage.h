#pragma once
typedef char byte_;

#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <thread>


class TestConsoleImage
{
private:
	
	size_t lineDelta;		// How many threads will be made.
	size_t allowedTries;	// How many collision between threads shall we handle. 

	byte_* map;
	size_t mapSize;

	struct {
		size_t width;
		size_t height;
	} size;
	
	COORD position;

	HDC hdc;

	void getBMPImageInfo(std::string file);

#ifndef SINGLE_THREAD_CONSOLE_IMAGE // If programmer wants to use single-thread drawing.
	void drawSegment(size_t line);
	void drawSegments(size_t from, size_t to);
#endif // SINGLE_THREAD_CONSOLE_IMAGE


public:


	TestConsoleImage(std::string filename, 
		size_t lineDelta = 4, 
		size_t allowedTries = 15) : lineDelta{ lineDelta }, hdc{GetDC(GetConsoleWindow())}, allowedTries{allowedTries}, position{}
	{		
		getBMPImageInfo(filename);
	}

	void setPosition(int16_t x, int16_t y);

	COORD getPosition();

	void draw();
	
	virtual ~TestConsoleImage() {
		delete[] map;
	}
};



#ifndef TEST_CONSOLE_IMAGE_ // To avoid cyclic include. 

#ifdef SINGLE_THREAD_CONSOLE_IMAGE // If programmer wants to use single-thread drawing.

void TestConsoleImage::draw() {

	size_t span{};
	for (size_t i = 0; i < size.height; i++)
	{
		for (size_t j = 0; j < size.width; j++)
		{
			SetPixel(hdc, position.X + j, position.Y + size.height - i, RGB((int)map[span + j * 3 + 2], (int)map[span + j * 3 + 1], (int)map[span + j * 3]));
		}
		span += size.width * 3;
	}

}

#else

void TestConsoleImage::drawSegments(size_t from, size_t to) {
	size_t span{ from * size.width * 3 }; // Previous lines.
	size_t tries{};
	for (size_t i = from; i < to; i++)
	{
		for (int32_t j = 0; j < size.width; j++)
		{			
			if (SetPixel(hdc, position.X + j, position.Y + size.height - i, RGB((int)map[span + j * 3 + 2], (int)map[span + j * 3 + 1], (int)map[span + j * 3])) == -1) {
				tries++;
				if (tries < allowedTries) j--;
				else tries = 0;
			}
		}
		span += size.width * 3;
	}
}

void TestConsoleImage::draw() {

	std::vector<std::thread> threads;

	size_t span{ size.height / lineDelta };
	for (size_t i = 0; i < lineDelta; i++) {
		threads.push_back(std::thread(&TestConsoleImage::drawSegments, this, i * span, i * span + span));
	}

	for (auto& thread : threads) {
		thread.join();
	}
}

#endif // SINGLE_THREAD_CONSOLE_IMAGE
#endif // !TEST_CONSOLE_IMAGE_



