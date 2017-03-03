#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>

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

StockCountProcessor sc;

TagSet process(Mat frame) {
	TagSet t = sc.process(frame);
	return t;
}

int main(int argc, char** argv) {

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

	int frame_count = 0;
	int processed_frames = 0;
	vector<TagSet> tags;
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	while (true) {
		if(frame_count % (30 * 60) == 0) {
			cout << frame_count / (30 * 60) << ":00" << endl;
		}
		cap.read(frame);
		if(frame.empty()) {
			break;
		}

		frame_count++;
		if(frame_count >= 6*30 && frame_count <= 176*30) {
			processed_frames++;
			tags.push_back(process(frame));
		} else if(frame_count > 176*30) {
			break;
		}
	}

	sliding_mode(tags, "p1_stocks", 30);
	sliding_mode(tags, "p2_stocks", 30);

	for(auto& tag : tags) {
		cout << "p1_stocks=" << tag["p1_stocks"] << ", p2_stocks=" << tag["p2_stocks"] << endl;
	}
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	auto millis = chrono::duration_cast<chrono::milliseconds> (end - begin).count();
	cout << processed_frames / (millis / (1000.0 / 30.0)) << " frames per frame" <<std::endl;

	return 0;
}
