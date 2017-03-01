#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

void sliding_mode(vector<unordered_map<string, int>>& tags, string param, int window) {
    vector<int> new_vals(tags.size());
    unordered_map<int, int> counts;


    // initialize counts with window [0, window/2]
    for(unsigned i=0; i<min((int) tags.size(), window/2); i++) {
        counts[tags[i][param]] += 1;
    }

    // loop through tags
    for(int i=0; i<tags.size(); i++) {
        int imin = i - window/2;
        int imax = i + window/2;
        if(imin >= 0) {
            counts[tags[imin][param]] -= 1;
        }
        if(imax < tags.size()) {
            counts[tags[imax][param]] += 1;
        }

        // find maximum of tag counts
        int mode = -1;
        int mode_count = -1;
        for(auto& pair : counts) {
            if(pair.second > mode_count) {
                mode = pair.first;
                mode_count = pair.second;
            }
        }

        // write mode into new_vals
        new_vals[i] = mode;
    }

    // write new vals back into tags
    for(int i=0; i<tags.size(); i++) {
        tags[i][param] = new_vals[i];
    }
}