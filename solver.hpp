#include <vector>

using namespace std;

class Solver
{
    public:
        int score_solution(const vector<int> &solution, const vector<vector<int>> &instance);
        int dynamic_score_solution(
                const vector<vector<int>> &instance,
                vector<int> &solution,
                int i,
                int j,
                int score);
        void random_permutation(vector<int> &permutation);
};


class HeuristicSolver: public Solver
{
    public:
        vector<int> solve(vector<vector<int>> instance);
};

class GreedySolver: public Solver
{
    public:
        vector<int> solve(const vector<vector<int>> &instance);
};

class RandomSolver: public Solver
{
    public:
        vector<int> solve(const vector<vector<int>> &instance, float T); // T in seconds
};

class RandomWalkSolver: public Solver
{
    public:
        vector<int> solve(const vector<vector<int>> &instance, float T); // T in seconds
};

