#ifndef FRAME_PROCESSOR_H_
#define FRAME_PROCESSOR_H_

#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <utility>

using namespace std;
using namespace cv;

typedef unordered_map<string, int> TagSet;

class Processor {
public:
    virtual TagSet process(Mat frame) = 0;
};

class StockCountProcessor : Processor {
    unordered_map<string, pair<Mat, Mat>> stock_icons;
    unordered_map<int, string> characters;
public:
    StockCountProcessor();
    virtual TagSet process(Mat frame);
private:
    string guess_character(Mat icon_region);
};

#endif