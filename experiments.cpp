#include "experiments.hpp"

using namespace std;

const string Result::SIGNATURE = "NAME ; BEST ; AVG ; STD ; WORST ; TIME ; STEPS ; CHECKED";

Result::Result(const vector<SolverResult> &srs, const vector<float> &scores,
        const vector<float> &times, const string alg_name)
{
    worst_score = *max_element(scores.begin(), scores.end());
    avg_score = accumulate(scores.begin(), scores.end(), 0) / 10.0;
    std_score = 0;
    for(float s : scores)
    {
        std_score += pow(s-avg_score, 2);
    }
    std_score = sqrt(std_score / 10.0);
    best_score = *min_element(scores.begin(), scores.end());
    avg_time = accumulate(times.begin(), times.end(), 0) / 10.0;
    time_quality = 1 / (avg_score * avg_time);  //the bigger the better
    steps = 0;
    for (const SolverResult &sr : srs)
        steps += sr.steps / 10.0;
    checked_solutions = 0;
    for(const SolverResult &sr : srs)
        checked_solutions += sr.checked_solutions / 10.0;
    this->alg_name = alg_name;
}


Result tenfold_run(const string alg_name, Solver *s, const vector<vector<int>> instance,
                   const float T, const float instance_optimum)
{
    vector<float> scores;
    vector<float> times;
    vector<SolverResult> srs(10);
    for (int i = 0; i < 10; i++)
    {
        vector<vector<int>> instance_copy = instance;
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        srs[i] = s->solve(instance_copy, T);
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        float time = chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        times.push_back(time);
        float score = (float) Solver::score_solution(srs[i].solution, instance);
        scores.push_back(score);
    }
    for(float &score : scores)
        score /= instance_optimum;  //the smaller the better
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

    vector<pair<string, float> > instances({    // (name, optimum)
            make_pair("br17", 1000.0),          //TODO
            make_pair("ft53", 2000.0)
    });
    cout << Result::SIGNATURE << endl;
    for (auto &name_opt : instances)
    {
        const vector<vector<int>> instance = dl.load("instancje/" + name_opt.first + ".atsp");
        cout << "INSTANCE;" << name_opt.first << endl;

        cout << tenfold_run("H", H, instance, -1., name_opt.second);
        Result greedy_result = tenfold_run("G", G, instance, -1., name_opt.second);
        float time = greedy_result.avg_time;
        cout << greedy_result;
        cout << tenfold_run("S", S, instance, -1., name_opt.second);
        cout << tenfold_run("R", R, instance, time, name_opt.second);
        cout << tenfold_run("RW", RW, instance, time, name_opt.second);
    }
}


const string Result3::SIGNATURE = "NAME ; INIT ; FINAL";
void exp3()
{
    DataLoader dl;
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    vector<pair<string, float> > instances({    // (name, optimum)
            make_pair("br17", 1000.0),          //TODO
            make_pair("ft53", 2000.0)
    });
    cout << Result3::SIGNATURE << endl;
    for (const auto &name_opt : instances)
    {
        const vector<vector<int>> instance = dl.load("instancje/" + name_opt.first + ".atsp");
        vector<vector<int>> instance_copy = instance;
        cout << "INSTANCE;" << name_opt.first << endl;

        for (int i = 0; i < 200; i++)
        {
            instance_copy = instance;
            cout << Result3(G->solve(instance_copy, -1.), instance, name_opt.second, "G");
            instance_copy = instance;
            cout << Result3(S->solve(instance_copy, -1.), instance, name_opt.second, "S");
        }
    }
}


void exp4()
{
    DataLoader dl;
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    vector<pair<string, float> > instances({    // (name, optimum)
            make_pair("br17", 1000.0),          //TODO
            make_pair("ft53", 2000.0)
    });
    
    float temp;
    for (const auto &name_opt : instances)
    {
        cout << "INSTANCE;" << name_opt.first << endl;
        float sum_G = 0;
        float best_G = numeric_limits<float>::max();
        float sum_S = 0;
        float best_S = numeric_limits<float>::max();
        const vector<vector<int>> instance = dl.load("instancje/" + name_opt.first + ".atsp");
        vector<vector<int>> instance_copy = instance;
        
        for (int i = 1; i <= 300; i++)
        {
            instance_copy = instance;
            temp = Solver::score_solution(G->solve(instance_copy, -1.).solution, instance) / name_opt.second;
            sum_G += temp;
            if (temp < best_G) best_G = temp;
            cout << "G;" << sum_G / i << ";" << best_G << endl;

            instance_copy = instance;
            temp = Solver::score_solution(S->solve(instance_copy, -1.).solution, instance) / name_opt.second;
            sum_S += temp;
            if (temp < best_S) best_S = temp;
            cout << "S;" << sum_S / i << ";" << best_S << endl;
        }
    }
}
