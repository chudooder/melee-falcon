#include "frame_processor.h"

Mat game_region(Mat frame) {
    Size size = frame.size();
    Mat roi = frame(
        Range(0, size.height), 
        Range((int)(size.width * 0.12529), (int)(size.width * 0.87471)));
    return roi;
}

TagSet StockCount::process(Mat frame) {
    Mat game_roi = game_region(frame);
    Mat p1_stock_roi = game_roi(
        Range((int) (game_roi.size().height * 0.73), (int) (game_roi.size().height * 0.83)),
        Range((int) (game_roi.size().width * 0.05), (int) (game_roi.size().width * 0.25)));

    Mat result(p1_stock_roi.size(), CV_8UC1);
    
    matchTemplate(p1_stock_roi, fox_neutral, result, CV_TM_SQDIFF_NORMED);
    normalize(result, result, 0, 1, NORM_MINMAX);
    GaussianBlur(result, result, Size(5, 5), 1);
    subtract(Scalar::all(1), result, result);
    threshold(result, result, 0.75, 1.0, THRESH_BINARY);
    // convert to single channel 8bit

    Mat result_8;
    result.convertTo(result_8, CV_8UC1);

    Mat components;
    int num_stocks = connectedComponents(result_8, components) - 1;

    // imshow("frame", frame);
    // imshow("template match", result);
    // waitKey(0);

    unordered_map<string, int> tags;
    tags["p1_stocks"] = num_stocks;
    cout << "p1_stocks=" << num_stocks << endl;
    return tags;
}