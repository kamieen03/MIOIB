#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;

class SolverResult
{
    public:
        vector<int> solution;
        int steps;              // G, S only
        int checked_solutions;  // G, S, R, RW only
        int init_val;
        SolverResult(){};
        SolverResult(vector<int> solution, int steps, int checked_solutions, int init_val):
            solution(solution),
            steps(steps),
            checked_solutions(checked_solutions),
            init_val(init_val) {};
};

class Solver
{
    public:
        mt19937 rand_gen;

        Solver()
        {
            random_device rd;
            rand_gen = mt19937(rd());
        };
        static int score_solution(const vector<int> &solution, const vector<vector<int>> &instance);
        int dynamic_score_solution(
                const vector<vector<int>> &instance,
                vector<int> &solution,
                int i,
                int j,
                int score);
        void random_permutation(vector<int> &permutation);
        virtual SolverResult solve(vector<vector<int>> &instance, float T) = 0;
};


class HeuristicSolver: public Solver
{
    public:
        SolverResult solve(vector<vector<int>> &instance, float T);
};

class GreedySolver: public Solver
{
    public:
        GreedySolver(): Solver(){};
        SolverResult solve(vector<vector<int>> &instance, float T);
};

class RandomSolver: public Solver
{
    public:
        RandomSolver(): Solver(){};
        SolverResult solve(vector<vector<int>> &instance, float T); // T in microseconds
};

class RandomWalkSolver: public Solver
{
    public:
        RandomWalkSolver(): Solver(){};
        SolverResult solve(vector<vector<int>> &instance, float T); // T in microseconds
};

class SteepestSolver: public Solver
{
    public:
        SteepestSolver(): Solver(){};
        SolverResult solve(vector<vector<int>> &instance, float T);
};

class SASolver: public Solver
{
    private:
        uniform_real_distribution<float> unif;
        float MAX_T;
        float T;
        int L;  // number of steps a single temperature value holds for
        const float C = 0.9;    // T gets multiplied by C every L steps

        bool accept(int s1, int s2);
        float compute_max_t(const vector<vector<int>> &instance);

    public:
        SASolver(): Solver()
        {
            unif = uniform_real_distribution<float>(0.0, 1.0);
        };
        SolverResult solve(vector<vector<int>> &instance, float T);
};


