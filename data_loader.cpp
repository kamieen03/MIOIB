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
    string number_buffer =  " ";

    while (getline(file, buffer) && buffer != "EOF")
    {
        buffer += ' ';
        for (char &c : buffer)
        {
            if (c == ' ' && number_buffer.back() != ' ')
            {
                col = I % N; 
                row = (int) (I - col) / N;
                I++;
                instance[row][col] = stoi(number_buffer);
                number_buffer = " ";
            }
            number_buffer += c;
        }
    }
    return instance;
}

