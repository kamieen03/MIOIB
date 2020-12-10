#include "solver.hpp"
const int MAX_INT = numeric_limits<int>::max();

int argmin(vector<int> &row)
{
    int argmin = 0;
    int min = row[0];
    for (int i = 1; i < row.size(); i++)
    {
        if (row[i] < min)
        {
            argmin = i;
            min = row[i];
        }
    }
    return argmin;
}

void Solver::random_permutation(vector<int> &perm)
{
    int n = perm.size();
    vector<int> sack(n);
    for (int i = 0; i < n; i++)
    {
        sack[i] = i;
    }

    for (int i = 0; i < n; i++)
    {
        uniform_int_distribution<int> distribution(0,n-1-i);
        int idx = distribution(this->rand_gen);
        perm[i] = sack[idx];
        sack.erase(sack.begin() + idx);
    }
}

int Solver::score_solution(const vector<int> &solution, const vector<vector<int>> &instance)
{
    int score = 0;
    for (int i = 1; i < solution.size(); i++)
        score += instance[solution[i-1]][solution[i]];
    score += instance[solution[solution.size()-1]][solution[0]];
    return score;
}

int Solver::dynamic_score_solution(
            const vector<vector<int>> &instance,
            vector<int> &solution,
            int i,
            int j,
            int score)
{
    int N = solution.size();
    if (j < i) swap(i,j);
    if (j == N-1 && i == 0) swap(i,j);


    int city_before_i = solution[(i-1+N)%N];
    int city_i = solution[i];
    int city_after_i = solution[i+1];
    int city_before_j = solution[j-1];
    int city_j = solution[j];
    int city_after_j = solution[(j+1)%N];
    
    if (abs(i-j) == 1 || abs(i-j) == N-1)
    {
        score -= instance[city_before_i][city_i];
        score -= instance[city_i][city_j];
        score -= instance[city_j][city_after_j];

        score += instance[city_before_i][city_j];
        score += instance[city_j][city_i];
        score += instance[city_i][city_after_j];
    }
    else
    {
        score -= instance[city_before_i][city_i];
        score -= instance[city_i][city_after_i];
        score -= instance[city_before_j][city_j];
        score -= instance[city_j][city_after_j];

        score += instance[city_before_i][city_j];
        score += instance[city_j][city_after_i];
        score += instance[city_before_j][city_i];
        score += instance[city_i][city_after_j];
    }
    return score;
}



SolverResult HeuristicSolver::solve(vector<vector<int>> &instance, float T)
{
    vector<int> solution(instance.size(), 0);
    int idx = 0;

    for (int i = 1; i < instance.size(); i++)
    {
        for (int j = 0; j < instance.size(); j++) instance[j][idx] = MAX_INT;
        int amin = argmin(instance[idx]);
        solution[i] = amin;
        idx = amin;
    }
    return SolverResult(solution, 0, 0, 0);
}



SolverResult GreedySolver::solve(vector<vector<int>> &instance, float T)
{
    vector<int> solution(instance.size());
    random_permutation(solution);
    int score = score_solution(solution, instance);
    int init_score = score;
    bool improved;
    int new_score;
    int steps = 0;
    int checked_solutions = 0;
    while(true)
    {
        improved = false;
        for(int i = 0; i < instance.size() - 1; i++)
        {
            for(int j = i+1; j < instance.size(); j++)
            {
                checked_solutions++;
                new_score = dynamic_score_solution(instance, solution, i, j, score);
                if (new_score < score)
                {
                    score = new_score;
                    swap(solution[i], solution[j]);
                    improved = true;
                    steps++;
                    break;
                }
            }
            if (improved) break;
        }
        if (!improved) break;
    }
    return SolverResult(solution, steps, checked_solutions, init_score);
}



SolverResult RandomSolver::solve(vector<vector<int>> &instance, float T)
{
    vector<int> temp(instance.size());
    random_permutation(temp);
    vector<int> best(temp);
    int best_score = score_solution(temp, instance); int score;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int checked_solutions = 0;

    while(true)
    {
        chrono::steady_clock::time_point time = chrono::steady_clock::now();
        auto time_passed = chrono::duration_cast<chrono::microseconds> (time - begin).count();
        if (time_passed > T) break;

        random_permutation(temp);
        checked_solutions++;
        if ((score = score_solution(temp, instance)) < best_score)
        {
            best = temp;
            best_score = score;
        }
    }
    return SolverResult(best, 0, checked_solutions, 0);
}



SolverResult RandomWalkSolver::solve(vector<vector<int>> &instance, float T)
{
    vector<int> temp(instance.size());
    random_permutation(temp);
    vector<int> best(temp);
    int score = score_solution(temp, instance);
    int best_score = score;
    int checked_solutions = 0;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    uniform_int_distribution<int> distribution(0,temp.size()-1);
    int i, j;
    while(true)
    {
        chrono::steady_clock::time_point time = chrono::steady_clock::now();
        auto time_passed = chrono::duration_cast<chrono::microseconds> (time - begin).count();
        if (time_passed > T) break;

        i = distribution(this->rand_gen);
        j = distribution(this->rand_gen);
        if (i == j) continue;
         
        score = dynamic_score_solution(instance, temp, i, j, score);
        swap(temp[i], temp[j]);
        if (score < best_score)
        {
            best_score = score;
            best = temp;
        }
        checked_solutions++;
    }
    return SolverResult(best, 0, checked_solutions, 0);
}



SolverResult SteepestSolver::solve(vector<vector<int>> &instance, float T)
{
    vector<int> solution(instance.size());
    random_permutation(solution);
    int score = score_solution(solution, instance);
    int init_score = score;
    int best_ngbh_score = score;
    bool improved;
    int new_score, best_I, best_J;
    int steps = 0;
    int checked_solutions = 0;

    while(true)
    {
        improved = false;
        for(int i = 0; i < instance.size() - 1; i++)
        {
            for(int j = i+1; j < instance.size(); j++)
            {
                checked_solutions++;
                new_score = dynamic_score_solution(instance, solution, i, j, score);
                if (new_score < best_ngbh_score)
                {
                    best_I = i; best_J = j;
                    best_ngbh_score = new_score;
                    improved = true;
                }
            }
        }
        if (!improved) break;
        swap(solution[best_I], solution[best_J]);
        score = best_ngbh_score;
        steps++;
    }
    return SolverResult(solution, steps, checked_solutions, init_score);
}



float SASolver::compute_max_t(const vector<vector<int>> &instance)
{
    int score, new_score;
    vector<int> temp(instance.size());
    float avg = 0;
    for(int i = 0; i < 1000; i++)
    {
        random_permutation(temp);
        score = score_solution(temp, instance);
        new_score = dynamic_score_solution(instance, temp, 0, 1, score);
        avg += (float) abs(score-new_score);
    }
    avg /= -1000.0;
    return avg / log(C);
}

bool SASolver::accept(int s1, int s2)
{
    if(s2 <= s1) return true;
    float p = exp( (s1-s2) / T );

    return unif(rand_gen) < p;
}

SolverResult SASolver::solve(vector<vector<int>> &instance, float T)
{
    {
        MAX_T = compute_max_t(instance);
        T = MAX_T;
        const int N = instance.size();
        L = N*N / 10;
    }
    vector<int> solution(instance.size());
    random_permutation(solution);
    int score = score_solution(solution, instance);
    int init_score = score;
    bool accepted;
    int new_score;
    int steps = 0;
    int checked_solutions = 0;
    int epochs_wout_improvement = 0;

    while(true)
    {
        accepted = false;
        for(int i = 0; i < instance.size() - 1; i++)
        {
            for(int j = i+1; j < instance.size(); j++)
            {
                checked_solutions++;
                if(checked_solutions % L == 0) T *= C;
                new_score = dynamic_score_solution(instance, solution, i, j, score);
                if(accept(score, new_score))
                {
                    if(new_score < score)
                        epochs_wout_improvement = 0;
                    score = new_score;
                    swap(solution[i], solution[j]);
                    accepted = true;
                    steps++;
                    break;
                }
            }
            if (accepted) break;
        }
        if (epochs_wout_improvement >= 3) break;
        epochs_wout_improvement++;
    }
    return SolverResult(solution, steps, checked_solutions, init_score);
}


