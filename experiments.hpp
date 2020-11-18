#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <math.h>
#include <numeric>
#include <limits>
#include "data_loader.hpp"
#include "solver.hpp"

using namespace std;


class Result
{
    public:
        float best_score;
        float avg_score;
        float std_score;
        float worst_score;
        float avg_time;
        float time_quality;
        float steps;
        float checked_solutions;
        string alg_name;
        Result(const vector<SolverResult> &srs, const vector<float> &scores,
               const vector<float> &times, const string alg_name);
        friend ostream &operator<<(ostream &os, Result const &r)
        { 
            return os << r.alg_name << ";" <<
                         r.best_score << ";" <<
                         r.avg_score << ";" <<
                         r.std_score << ";" <<
                         r.worst_score << ";" <<
                         r.avg_time << ";" <<
                         r.time_quality << ";" <<
                         r.steps << ";" <<
                         r.checked_solutions <<endl;
        }
        static const string SIGNATURE;
};
Result tenfold_run(const string alg_name, Solver *s, const vector<vector<int>> instance, 
        const float T, const float instance_optimum);
void exp2();


class Result3
{
    public:
        float init_score;   //the lower the better
        float final_score;
        string alg_name;
        Result3(const SolverResult &sr, const vector<vector<int>> &instance,
                const float instance_optimum, const string alg_name):
            init_score(sr.init_val / instance_optimum),
            final_score(Solver::score_solution(sr.solution, instance) / instance_optimum),
            alg_name(alg_name) {};
        friend ostream &operator<<(ostream &os, Result3 const &r) { 
            return os << r.alg_name << ";" << r.init_score << ";" << r.final_score << endl;
        }
        static const string SIGNATURE;
};
void exp3();


void exp4();


float distance(const vector<int> &perm1, const vector<int> &perm2);
void exp5();
