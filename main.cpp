#include "experiments.hpp"

using namespace std;

int main(int argc, char** argv)
{
    DataLoader l;
    l.test_instances();
    if (argc == 2)
    {
        string nums = string(argv[1]);
        if (nums.find("2") != string::npos) {
            exp2();
        }
        if (nums.find("3") != string::npos) {
            exp3();
        }
        if (nums.find("4") != string::npos) {
            exp4();
        }
        if (nums.find("5") != string::npos) {
            exp5();
        }
        if (nums.find("SA") != string::npos) {
            expSA();
        }
        if (nums.find("T") != string::npos) {
            expT();
        }
    }
    return 0;
}
