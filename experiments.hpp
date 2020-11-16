#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <math.h>
#include <numeric>
#include "data_loader.hpp"
#include "solver.hpp"

using namespace std;


class Result
{
    public:
        int best_score;
        float avg_score;
        float std_score;
        int worst_score;
        float avg_time;
        float steps;
        float checked_solutions;
        string alg_name;
        Result(const vector<SolverResult> &srs, const vector<float> &scores,
               const vector<float> &times, const string alg_name);
        friend ostream &operator<<(ostream &os, Result const &r)
        { 
            return os << r.alg_name << ";" <<r.avg_score << ";" << r.std_score << ";" << r.avg_time << endl;
        }
};
Result tenfold_run(string alg_name, Solver *s, vector<vector<int>> instance, float T);
void exp2();


class Result3
{
    public:
        int init_score;
        int final_score;
        string alg_name;
        Result3(const SolverResult &sr, const vector<vector<int>> &instance, const string alg_name):
            init_score(sr.init_val),
            final_score(Solver::score_solution(sr.solution, instance)),
            alg_name(alg_name) {};
        friend ostream &operator<<(ostream &os, Result3 const &r) { 
            return os << r.alg_name << ";" << r.init_score << ";" << r.final_score << endl;
        }
};
void exp3();

