#include <vector>

using namespace std;

class SolverResult
{
    public:
        vector<int> solution;
        int steps;              // G, S only
        int checked_solutions;  // G, S, R, RW only
        int init_val;
        SolverResult(){};
        SolverResult(vector<int> solution, int steps, int checked_solution, int init_val):
            solution(solution),
            steps(steps),
            checked_solutions(checked_solutions),
            init_val(init_val) {};
};

class Solver
{
    public:
        static int score_solution(const vector<int> &solution, const vector<vector<int>> &instance);
        int dynamic_score_solution(
                const vector<vector<int>> &instance,
                vector<int> &solution,
                int i,
                int j,
                int score);
        void random_permutation(vector<int> &permutation);
        virtual SolverResult solve(vector<vector<int>> instance) {};
        virtual SolverResult solve(const vector<vector<int>> &instance, float T) {}; // T in seconds
};


class HeuristicSolver: public Solver
{
    public:
        SolverResult solve(vector<vector<int>> instance);;
};

class GreedySolver: public Solver
{
    public:
        SolverResult solve(const vector<vector<int>> instance);
};

class RandomSolver: public Solver
{
    public:
        SolverResult solve(const vector<vector<int>> &instance, float T); // T in seconds
};

class RandomWalkSolver: public Solver
{
    public:
        SolverResult solve(const vector<vector<int>> &instance, float T); // T in seconds
};

class SteepestSolver: public Solver
{
    public:
        SolverResult solve(const vector<vector<int>> instance);
};
