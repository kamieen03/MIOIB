#include "experiments.hpp"

using namespace std;

const string Result::SIGNATURE = "NAME ; BEST ; AVG ; STD ; WORST ; TIME ; TIME_QUALITY ; STEPS ; CHECKED";

Result::Result(const vector<SolverResult> &srs, const vector<float> &scores,
        const vector<float> &times, const string alg_name)
{
    worst_score = *max_element(scores.begin(), scores.end());
    avg_score = accumulate(scores.begin(), scores.end(), 0.0) / 10.0;
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
    freopen("results/exp2.csv","w",stdout);

    DataLoader dl;
    HeuristicSolver *H = new HeuristicSolver();
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    RandomSolver *R = new RandomSolver();
    RandomWalkSolver *RW = new RandomWalkSolver();

    vector<string> instances({
            "br17",
            "ftv35",
            "p43",
            "ry48p",
            "ftv55",
            "ft70",
            "kro124p",
            "ftv170",
            "rbg323",
            "rbg403"
    });
    cout << Result::SIGNATURE << endl;
    for (const string &instance_name : instances)
    {
        const vector<vector<int>> instance = dl.load("instancje/" + instance_name + ".atsp");
        cout << "INSTANCE;" << instance_name << endl;

        cout << tenfold_run("H", H, instance, -1., DataLoader::OPTIMA.at(instance_name));
        Result greedy_result = tenfold_run("G", G, instance, -1., DataLoader::OPTIMA.at(instance_name));
        float time = greedy_result.avg_time;
        cout << greedy_result;
        cout << tenfold_run("S", S, instance, -1., DataLoader::OPTIMA.at(instance_name));
        cout << tenfold_run("R", R, instance, time, DataLoader::OPTIMA.at(instance_name));
        cout << tenfold_run("RW", RW, instance, time, DataLoader::OPTIMA.at(instance_name));
    }
}


const string Result3::SIGNATURE = "NAME ; INIT ; FINAL";
void exp3()
{
    freopen("results/exp3.csv","w",stdout);

    DataLoader dl;
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    vector<string> instances({
       "br17",
       "p43",
       "ry48p",
       "rbg358"
    });
    cout << Result3::SIGNATURE << endl;
    for (const string &instance_name : instances)
    {
        const vector<vector<int>> instance = dl.load("instancje/" + instance_name + ".atsp");
        vector<vector<int>> instance_copy = instance;
        cout << "INSTANCE;" << instance_name << endl;

        for (int i = 0; i < 200; i++)
        {
            instance_copy = instance;
            cout << Result3(G->solve(instance_copy, -1.), instance, DataLoader::OPTIMA.at(instance_name), "G");
            instance_copy = instance;
            cout << Result3(S->solve(instance_copy, -1.), instance, DataLoader::OPTIMA.at(instance_name), "S");
        }
        cout << "INSTANCE END" << endl;
    }
}


void exp4()
{
    freopen("results/exp4.csv","w",stdout);

    DataLoader dl;
    GreedySolver *G = new GreedySolver();
    SteepestSolver *S = new SteepestSolver();
    vector<string> instances({
       "br17",
       "p43",
       "ry48p",
       "ft53",
       "rbg358"
    });
    
    float temp;
    for (const string &instance_name : instances)
    {
        cout << "INSTANCE;" << instance_name << endl;
        float sum_G = 0;
        float best_G = numeric_limits<float>::max();
        float sum_S = 0;
        float best_S = numeric_limits<float>::max();
        const vector<vector<int>> instance = dl.load("instancje/" + instance_name + ".atsp");
        vector<vector<int>> instance_copy = instance;
        
        for (int i = 1; i <= 300; i++)
        {
            instance_copy = instance;
            temp = Solver::score_solution(G->solve(instance_copy, -1.).solution, instance) / DataLoader::OPTIMA.at(instance_name);
            sum_G += temp;
            if (temp < best_G) best_G = temp;
            cout << "G;" << sum_G / i << ";" << best_G << endl;

            instance_copy = instance;
            temp = Solver::score_solution(S->solve(instance_copy, -1.).solution, instance) / DataLoader::OPTIMA.at(instance_name);
            sum_S += temp;
            if (temp < best_S) best_S = temp;
            cout << "S;" << sum_S / i << ";" << best_S << endl;
        }
    }
}



float distance(const vector<int> &perm1, const vector<int> &perm2)
{
    // inverse perm2
    vector<int> perm2_inv(perm2.size());
    for(int i = 0; i < perm2.size(); i++)
        perm2_inv[perm2[i]] = i;

    // compose permutations
    vector<int> composed(perm2.size());
    for(int i = 0; i < perm2.size(); i++)
        composed[i] = perm1[perm2_inv[i]];

    // compute shortest transposition distance between 'composed' and identity permutation
    float dist = 0;
    for(int i = 0; i < perm2.size(); i++)
    {
        for(; composed[i] != i; dist++)
            swap(composed[composed[i]], composed[i]);
    }
    return dist;
}

void exp5()
{
    freopen("results/exp5.csv","w",stdout);

    DataLoader dl;
    GreedySolver *G = new GreedySolver();
    vector<string> instances({
        "p43",
        "ry48p"
    });
    
    for (const string &instance_name : instances)
    {
        vector<SolverResult> srs(200);
        int best_idx = -1;
        int best_score = numeric_limits<int>::max();
        int score;
        vector<float> scores(srs.size());
        float dist;

        const vector<vector<int>> instance = dl.load("instancje/" + instance_name + ".atsp");
        vector<vector<int>> instance_copy = instance;
        cout << "INSTANCE;" << instance_name << endl;
        
        // run alg 200, compute scores and best score
        for(int i = 0; i < 200; i++)
        {
            instance_copy = instance;
            srs[i] = G->solve(instance_copy, -1.);
            score = Solver::score_solution(srs[i].solution, instance);
            scores[i] = (float) score;
            if (score < best_score)
            {
                best_score = score;
                best_idx = i;
            }
        }
        
        for(int i = 0; i < srs.size(); i++)
        {
            dist = distance(srs[i].solution, srs[best_idx].solution);
            cout << scores[i] / best_score << ";" << dist << endl;
        }
    }
}
