#include "ImageOperations.h"

bool ImageOperations::applyLogo(const int& imageNumber)
{
	if (!experimental::filesystem::exists("Output"))
	{
		experimental::filesystem::create_directory("Output");
	}

	auto sourceImagePath = ".\\Input\\input_" + to_string(imageNumber) + ".bmp";
	auto destinationImagePath = ".\\Output\\output_" + to_string(imageNumber) + ".bmp";

	Mat sourceImage = imread(sourceImagePath, IMREAD_COLOR);

	if (sourceImage.empty())
	{
		cout << "Could not open or find the image " << imageNumber << '\n';
		return false;
	}

	if (sourceImage.rows > logo.rows && sourceImage.cols > logo.cols)
	{
		for (auto row = logo.rows - 1; row < sourceImage.rows; row += logo.rows)
		{
			auto currentRow = sourceImage.ptr<Vec3b>(row);

			for (auto column = logo.cols - 1; column < sourceImage.cols; column += logo.cols)
			{
				if (currentRow[column][0] == 255)
				{
					auto k = column + 1;
					while (currentRow[k][0] == 255 && k < sourceImage.cols)
					{
						currentRow[k][2] = 255;
						k++;
					}
					auto rightPadding = k - column;

					if (rightPadding < logo.cols)
					{
						k = column - 1;
						while (k >= 0)
						{
							if (currentRow[k][0] == 255)
							{
								currentRow[k][2] = 255;
								k--;
							}
							else
							{
								break;
							}
						}
						auto leftPadding = column - k;

						if (rightPadding + leftPadding - 1 == logo.cols)
						{
							k = row + 1;
							while (sourceImage.ptr<Vec3b>(k)[column][0] == 255 && k < sourceImage.rows)
							{
								k++;
							}
							auto bottomPadding = k - row;

							if (bottomPadding < logo.rows)
							{
								k = row - 1;
								while (k >= 0)
								{
									if (sourceImage.ptr<Vec3b>(k)[column][0] == 255)
									{
										k--;
									}
									else
									{
										break;
									}
								}
								auto topPadding = row - k;

								if (topPadding + bottomPadding - 1 == logo.rows)
								{
									for (auto boundingBoxRow = row - topPadding; boundingBoxRow < row + bottomPadding + 1; boundingBoxRow++)
									{
										auto currentBoundingBoxRow = sourceImage.ptr<Vec3b>(boundingBoxRow);

										for (auto boundingBoxColumn = column - leftPadding;
										     boundingBoxColumn < column + rightPadding + 1;
										     boundingBoxColumn = (boundingBoxRow == row - topPadding || boundingBoxRow == row + bottomPadding) ? boundingBoxColumn + 1 : column + rightPadding)
										{
											if (boundingBoxRow == -1 || boundingBoxRow == sourceImage.rows || boundingBoxColumn == -1 || boundingBoxColumn == sourceImage.cols)
											{
												goto continueLoop;
											}
											if (boundingBoxRow == row - topPadding || boundingBoxRow == row + bottomPadding || boundingBoxColumn == column - leftPadding || boundingBoxColumn == column + rightPadding)
											{
												if (currentBoundingBoxRow[boundingBoxColumn][0] == 255)
												{
													goto endLoopCurrentBox;
												}
											}

										continueLoop:
											if (boundingBoxColumn == column + rightPadding)
											{
												break;
											}
										}
									}

									for (auto boxRow = row - topPadding + 1; boxRow < row + bottomPadding; boxRow++)
									{
										auto currentBoxRow = sourceImage.ptr<Vec3b>(boxRow);

										for (auto boxColumn = column - leftPadding + 1; boxColumn < column + rightPadding; boxColumn++)
										{
											if (currentBoxRow[boxColumn][0] == 0)
											{
												goto endLoopCurrentBox;
											}
										}
									}
									{
										Rect sourceRectangle(Point(0, 0), logo.size());
										Rect destinationRectangle(Point(column - leftPadding + 1, row - topPadding + 1), logo.size());
										logo(sourceRectangle).copyTo(sourceImage(destinationRectangle));
									}
								endLoopCurrentBox: continue;
								}
							}
						}
					}
				}
			}
		}
	}

	imwrite(destinationImagePath, sourceImage);
	return true;
}

void ImageOperations::readLogo()
{
	logo = imread(".\\Logo\\wenglorLogo.bmp", IMREAD_COLOR);
}

void ImageOperations::processAllImages()
{
	auto i = 1;
	while (applyLogo(i))
	{
		++i;
	}
}

void ImageOperations::processImage(int imageNumber)
{
	applyLogo(imageNumber);
}
