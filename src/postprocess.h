#ifndef POSTPROCESS_H_
#define POSTPROCESS_H_


/***
 * Modifies tags in place such that
 * tags[i][param] = mode(tags[i-window/2] --> tags[i+window/2])
 */
void sliding_mode(vector<unordered_map<string, int>>& tags, string param, int window);

#endif