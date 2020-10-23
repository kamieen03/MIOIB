#include <vector>

using namespace std;

class Solver
{
    public:
        virtual vector<int> solve(vector<vector<int>> instance) = 0;
        int score_solution(const vector<int> &solution, const vector<vector<int>> &instance);
};


class HeuristicSolver: public Solver
{
    public:
        vector<int> solve(vector<vector<int>> instance);
};

