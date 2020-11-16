#include "experiments.hpp"


using namespace std;

Result::Result(const vector<SolverResult> &srs, const vector<float> &scores,
        const vector<float> &times, const string alg_name)
{
    best_score = *max_element(scores.begin(), scores.end());
    avg_score = accumulate(scores.begin(), scores.end(), 0) / 10.0;
    std_score = 0;
    for(float s : scores)
    {
        std_score += pow(s-avg_score, 2);
    }
    std_score = sqrt(std_score / 10.0);
    worst_score = *min_element(scores.begin(), scores.end());
    avg_time = accumulate(times.begin(), times.end(), 0) / 10.0;
    steps = 0;
    for (const SolverResult &sr : srs)
        steps += sr.steps / 10.0;
    checked_solutions = 0;
    for(const SolverResult &sr : srs)
        checked_solutions += sr.checked_solutions / 10.0;
    this->alg_name = alg_name;
}


Result tenfold_run(string alg_name, Solver *s, vector<vector<int>> instance, float T)
{
    vector<float> scores;
    vector<float> times;
    vector<SolverResult> srs(10);
    for (int i = 0; i < 10; i++)
    {
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        if (alg_name == "R" || alg_name == "RW")
             srs[i] = s->solve(instance, T);
        else
             srs[i] = s->solve(instance);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        float time = chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0;
        times.push_back(time);
        float score = (float) Solver::score_solution(srs[i].solution, instance);
        scores.push_back(score);
    }
    return Result(srs, scores, times, alg_name);
}

void exp2()
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
        cout << tenfold_run("H", H, instance, -1.);
        cout << tenfold_run("G", G, instance, -1.);
        cout << tenfold_run("S", S, instance, -1.);
        cout << tenfold_run("R", R, instance, 1.0);
        cout << tenfold_run("RW", RW, instance, 1.0);
    }
}


void exp3()
{
    DataLoader dl;
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    vector<string> instance_names({"br17", "ft53"});
    for (string &i_name : instance_names)
    {
        vector<vector<int>> instance = dl.load("instancje/" + i_name + ".atsp");
        for (int i = 0; i < 200; i++)
        {
            cout << Result3(G->solve(instance), instance, "G");
            cout << Result3(S->solve(instance), instance, "S");
        }
    }
}


