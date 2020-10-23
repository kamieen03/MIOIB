#include "data_loader.hpp"


vector<vector<int>> DataLoader::load(string filename)
{
    // get size and skipp irrelevant parts
    ifstream file;
    file.open(filename, ios::in);
    string buffer;
    for (int i = 0; i < 4; i++) getline(file, buffer);
    int N = stoi(buffer.substr(11));
    for (int i = 0; i < 3; i++) getline(file, buffer);

    vector<vector<int>> instance(N, vector<int>(N));
    int I = 0;
    int row, col;

    // fill cells 1 by 1
    while (true)
    {
        getline(file, buffer);
        if (buffer == "EOF") break;

        size_t line_pos = 1;
        while (line_pos < buffer.length())
        {
            col = I % N; 
            row = (int) (I - col) / N;
            I++;
            instance[row][col] = stoi(buffer.substr(line_pos, 8));
            line_pos += 8;
        }
    }
    return instance;
}

