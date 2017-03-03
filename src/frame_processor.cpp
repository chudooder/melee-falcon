#include "frame_processor.h"
#include <limits>
#include <string>

string CHAR_NAMES[] = {
    "bowser",
    "captain_falcon",
    "doctor_mario",
    "donkey_kong",
    "falco",
    "fox",
    "ganondorf",
    "ice_climbers",
    "jigglypuff",
    "kirby",
    "link",
    "luigi",
    "mario",
    "marth",
    "mewtwo",
    "mr._game_&_watch",
    "ness",
    "peach",
    "pichu",
    "pikachu",
    "roy",
    "samus",
    "yoshi",
    "young_link",
    "zelda"
};

Mat game_region(Mat frame) {
    Size size = frame.size();
    Mat roi = frame(
        Range(0, size.height), 
        Range((int)(size.width * 0.15625), (int)(size.width * 0.84375)));
    return roi;
}

StockCountProcessor::StockCountProcessor() {
    // load all character icon files
    for(string& s : CHAR_NAMES) {
        Mat raw_icon = 
            imread("img/stock_icons/" + s + "_neutral.png", IMREAD_UNCHANGED);

        Mat icon;
        cvtColor(raw_icon, icon, CV_BGRA2BGR);

        Mat alpha;
        vector<Mat> channels(4);
        split(raw_icon, channels);

        alpha = channels[3];

        Mat mask(icon.size(), icon.type());
        cvtColor(alpha, mask, CV_GRAY2BGR);

        stock_icons[s] = pair<Mat, Mat>(icon, mask);
    }
}

TagSet StockCountProcessor::process(Mat frame) {
    Mat game_roi = game_region(frame);

    TagSet tags;
    for(int player = 0; player < 2; player++) {

        Mat stock_roi = game_roi(
            Range(
                (int) (game_roi.size().height * 0.73), 
                (int) (game_roi.size().height * 0.83)),
            Range(
                (int) (game_roi.size().width * 0.25 * player), 
                (int) (game_roi.size().width * 0.25 * (player + 1)))
        );

        if(characters[player].length() == 0) {
            characters[player] = guess_character(stock_roi);
            cout << "p" << player << " is " << characters[player] << endl;
        }

        string cname = characters[player];

        Mat result(stock_roi.size(), CV_8UC1);

        Mat icon = stock_icons[cname].first;
        Mat mask = stock_icons[cname].second;
        
        matchTemplate(stock_roi, icon, result, CV_TM_SQDIFF, mask);
        normalize(result, result, 0, 1, NORM_MINMAX);
        GaussianBlur(result, result, Size(5, 5), 1);
        subtract(Scalar::all(1), result, result);
        threshold(result, result, 0.8, 1.0, THRESH_BINARY);
        // convert to single channel 8bit

        Mat result_8;
        result.convertTo(result_8, CV_8UC1);

        Mat components;
        int num_stocks = connectedComponents(result_8, components) - 1;

        // imshow("frame", game_roi);
        // imshow("template match", result);
        // waitKey(0);

        
        tags["p"+to_string(player+1)+"_stocks"] = num_stocks;
    }
    return tags;
}

string StockCountProcessor::guess_character(Mat icon_region) {
    // loop over characters and find most likely match

    double best_dist = numeric_limits<double>::max();
    string best_match = "";

    for(string name : CHAR_NAMES) {
        Mat icon = stock_icons[name].first;
        Mat result(icon_region.size(), CV_8UC1);
        matchTemplate(icon_region, icon, result, CV_TM_SQDIFF_NORMED);

        double min, max;
        minMaxLoc(result, &min, &max);
        if(min < best_dist) {
            best_match = name;
            best_dist = min;
        }
    }

    return best_match;
}