#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;


class DataLoader
{
    public:
        static const map<string, float> OPTIMA;
        vector<vector<int>> load(string filename);
        float asymetry(const vector<vector<int>> &instance);
        void test_instances();
};
