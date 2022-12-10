#pragma once
#include <string>
using namespace std;

struct Stock
{
    string ticker;
    float* rets;
    int size = 0;
    Stock(string s, float* r, int i):ticker(s),rets(r), size(i){};
};


