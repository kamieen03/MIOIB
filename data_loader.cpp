#include "data_loader.hpp"
#include <math.h>


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

const map<string, float> DataLoader::OPTIMA = {
    {"br17",    39},
    {"ft53",    6905},
    {"ft70",    38673},
    {"ftv33",   1286},
    {"ftv35",   1473},
    {"ftv38",   1530},
    {"ftv44",   1613},
    {"ftv47",   1776},
    {"ftv55",   1608},
    {"ftv64",   1839},
    {"ftv70",   1950},
    {"ftv170",  2755},
    {"kro124p", 36230},
    {"p43",     5620},
    {"rbg323",  1326},
    {"rbg358",  1163},
    {"rbg403",  2465},
    {"rbg443",  2720},
    {"ry48p",   14422}
};

float DataLoader::asymetry(const vector<vector<int>> &instance)
{
    float mean = 0;
    float mean_diff = 0;
    for (int i = 0; i < instance.size(); i++)
    {
        for(int j = 0; j < instance.size(); j++)
        {
            if (i != j) mean += instance[i][j];
            if (j > i) mean_diff += abs(instance[i][j] - instance[j][i]);
        }
    }
    return mean_diff / mean;
}

void DataLoader::test_instances()
{
    const vector<string> names = {
        "br17",   
        "ft53",  
        "ft70",   
        "ftv33",  
        "ftv35",  
        "ftv38",  
        "ftv44",  
        "ftv47",  
        "ftv55",  
        "ftv64",  
        "ftv70",  
        "ftv170", 
        "kro124p",
        "p43",    
        "rbg323", 
        "rbg358", 
        "rbg403", 
        "rbg443", 
        "ry48p",  
    };
    for (const string &name : names)
    {
        const vector<vector<int>> instance = load("instancje/" + name + ".atsp");
        cout << name << " : " << asymetry(instance) << endl;
    }
}
