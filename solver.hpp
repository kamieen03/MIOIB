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

class GreedySolver: public Solver
{
    private:
        vector<int> random_permutation(int n);
        int dynamic_score_solution(
                vector<vector<int>> &instance,
                vector<int> &solution,
                int i,
                int j,
                int score);

    public:
        vector<int> solve(vector<vector<int>> instance);
};

