#include <vector>
#include <list>
#include <iostream>
#include <random>


using namespace std;

vector<int> random_permutation(int n)
{
    vector<int> sack(n);
    for (int i = 0; i < n; i++)
    {
        sack[i] = i;
    }
    vector<int> perm(n);

    std::random_device rd;
    for (int i = 0; i < n; i++)
    {
        std::uniform_int_distribution<int> distribution(0,n-1-i);
        int idx = distribution(rd);
        perm[i] = sack[idx];
        sack.erase(sack.begin() + idx);
    }
    return perm;
}

int main()
{
    auto perm = random_permutation(10);
    for (auto x: perm) cout << x << " ";
    cout << endl;
    return 0;
}
