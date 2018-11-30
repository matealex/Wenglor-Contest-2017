#pragma once
#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace std;
using namespace cv;

#define DLLEXPORT __declspec(dllexport)

namespace ImageOperations
{
	Mat logo;

	bool applyLogo(const int& imageNumber);

	extern "C"
	{
		DLLEXPORT void readLogo();
		DLLEXPORT void processAllImages();
		DLLEXPORT void processImage(int imageNumber);
	}
}
