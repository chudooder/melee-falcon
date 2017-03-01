#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "frame_processor.h"
#include "postprocess.h"

using namespace cv;
using namespace std;

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch ( depth ) {
		case CV_8U:  r = "8U"; break;
		case CV_8S:  r = "8S"; break;
		case CV_16U: r = "16U"; break;
		case CV_16S: r = "16S"; break;
		case CV_32S: r = "32S"; break;
		case CV_32F: r = "32F"; break;
		case CV_64F: r = "64F"; break;
    	default:     r = "User"; break;
 	}

 	r += "C";
 	r += (chans+'0');

 	return r;
}

TagSet process(Mat frame) {
	StockCount sc;
	TagSet t = sc.process(frame);
	return t;
}

int main(int argc, char** argv) {

	// cvtColor(fox_neutral, fox_neutral, COLOR_RGB2GRAY);

	if (argc <= 1) {
		cout << "Path to file required." << endl;
		return 1;
	}

	const string filename = argv[1];
	cout << filename << endl;
	Mat frame;
	VideoCapture cap(filename);

	if(!cap.isOpened()) {
		cout << "Unable to open video capture." << endl;
		return 1;
	}

	int frameCount;
	vector<TagSet> tags;
	while (true) {
		if(frameCount % (30 * 60) == 0) {
			cout << frameCount / (30 * 60) << ":00" << endl;
		}
		cap.read(frame);
		if(frame.empty()) {
			break;
		}

		frameCount++;
		if(frameCount >= 600 && frameCount <= 1900) {
			tags.push_back(process(frame));
		} else if(frameCount == 1901) {
			break;
		}
	}

	sliding_mode(tags, "p1_stocks", 11);

	for(auto& tag : tags) {
    	cout << "p1_stocks=" << tag["p1_stocks"] << endl;
	}

	cout << frameCount << " frames grabbed." << endl;

	return 0;
}
