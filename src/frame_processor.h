#ifndef FRAME_PROCESSOR_H_
#define FRAME_PROCESSOR_H_

#include <string>
#include <unordered_map>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

typedef unordered_map<string, int> TagSet;

class Processor {
public:
    virtual TagSet process(Mat frame) = 0;
};

class StockCount : Processor {
    Mat fox_neutral = imread("img/stock_icons/fox_neutral.png", CV_LOAD_IMAGE_COLOR);
public:
    TagSet process(Mat frame);
};

#endif