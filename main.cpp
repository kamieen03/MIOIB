#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include <numeric>
#include "data_loader.hpp"
#include "solver.hpp"


using namespace std;

class Result
{
    public:
        float avg_score;
        float std_score;
        float avg_time;
        Result(float avg_score, float std_score, float avg_time):
            avg_score(avg_score),
            std_score(std_score),
            avg_time(avg_time) {};
        friend ostream &operator<<(ostream &os, Result const &r) { 
            return os << r.avg_score << ";" << r.std_score << ";" << r.avg_time << endl;
        }

};

Result tenfold_run_HGS(Solver *s, vector<vector<int>> instance)
{
    vector<float> scores;
    vector<float> times;
    for (int i = 0; i < 10; i++)
    {
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        vector<int> solution = s -> solve(instance);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        float time = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0;
        times.push_back(time);
        float score = (float) s->score_solution(solution, instance);
        scores.push_back(score);
    }
    float avg_score = accumulate(scores.begin(), scores.end(), 0) / 10;
    float std_score = 0;
    for(float s : scores)
    {
        std_score += pow(s-avg_score, 2);
    }
    std_score = sqrt(std_score / 10);
    float avg_time = accumulate(times.begin(), times.end(), 0) / 10;
    return Result(avg_score, std_score, avg_time);
}

Result tenfold_run_RRW(Solver *s, vector<vector<int>> instance, float T)
{
    vector<float> scores;
    vector<float> times;
    for (int i = 0; i < 10; i++)
    {
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        vector<int> solution = s->solve(instance, T);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        float time = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0;
        times.push_back(time);
        float score = (float) s->score_solution(solution, instance);
        scores.push_back(score);
    }
    float avg_score = accumulate(scores.begin(), scores.end(), 0) / 10;
    float std_score = 0;
    for(float s : scores)
    {
        std_score += pow(s-avg_score, 2);
    }
    std_score = sqrt(std_score / 10);
    float avg_time = accumulate(times.begin(), times.end(), 0) / 10;
    return Result(avg_score, std_score, avg_time);
}

int main()
{
    DataLoader dl;
    HeuristicSolver *H = new HeuristicSolver();
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    RandomSolver *R = new RandomSolver();
    RandomWalkSolver *RW = new RandomWalkSolver();

    vector<string> instance_names({"br17", "ft53"});
    for (string &i_name : instance_names)
    {
        vector<vector<int>> instance = dl.load("instancje/" + i_name + ".atsp");
        cout << tenfold_run_HGS(H, instance);
        cout << tenfold_run_HGS(G, instance);
        cout << tenfold_run_HGS(S, instance);
        cout << tenfold_run_RRW(R, instance, 1.0);
        cout << tenfold_run_RRW(RW, instance, 1.0);
    }
    return 0;
}
