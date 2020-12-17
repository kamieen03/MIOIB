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
                const vector<int> &solution,
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
        float MIN_T;
        float T;
        int L;  // number of steps a single temperature value holds for
        float C;    // T gets multiplied by C every L steps

        bool accept(int s1, int s2);
        float compute_t_bounds(const vector<vector<int>> &instance);

    public:
        SASolver(): Solver()
        {
            unif = uniform_real_distribution<float>(0.0, 1.0);
        };
        SolverResult solve(vector<vector<int>> &instance, float T);
        void setC(float c){C=c;};
};

class TabuSolver: public Solver
{
    private:
        int K;
        int EPOCHS_TO_STOP = 50;
        vector<pair<int,int>*> candidates;
        vector<pair<int,int>> pairs;
        int worst_candidate_gain = 0;
        int best_candidate_gain = 0;

        vector<vector<int>> tabu;           // upper half holds tabu counters. Lower half holds temporal (i,j) moves scores
        void init_tabu(int N);
        void update_tabu(pair<int, int> *IJ);

        bool compare_pairs(pair<int,int> *p1, pair<int,int> *p2) {
            return tabu[p1->second][p1->first] < tabu[p2->second][p2->first];
        };
        void construct_candidates(const vector<vector<int>> &instance, const vector<int> &solution,
            const int score);
        pair<int, int> *choose_IJ(const vector<vector<int>> &instance, const vector<int> &solution,
            int &checked_solutions, const int score, const int best_score);

    public:
        TabuSolver(): Solver(){};
        SolverResult solve(vector<vector<int>> &instance, float T);
        void set_k(int k){K = k;};
};
