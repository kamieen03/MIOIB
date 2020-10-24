#include "solver.hpp"
#include <limits>
#include <algorithm>
#include <random>
#include <iostream>

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

int Solver::score_solution(const vector<int> &solution, const vector<vector<int>> &instance)
{
    int score = 0;
    for (int i = 1; i < solution.size(); i++)
        score += instance[solution[i-1]][solution[i]];
    score += instance[solution[solution.size()-1]][solution[0]];
    return score;
}



vector<int> HeuristicSolver::solve(vector<vector<int>> instance)
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
    return solution;
}


vector<int> GreedySolver::random_permutation(int n)
{
    vector<int> sack(n);
    for (int i = 0; i < n; i++)
    {
        sack[i] = i;
    }
    vector<int> perm(n);

    std::random_device rd;
    for (int i = 0; i < n; i++)
    {
        std::uniform_int_distribution<int> distribution(0,n-1-i);
        int idx = distribution(rd);
        perm[i] = sack[idx];
        sack.erase(sack.begin() + idx);
    }
    return perm;
}

int GreedySolver::dynamic_score_solution(
            vector<vector<int>> &instance,
            vector<int> &solution,
            int i,
            int j,
            int score)
{
    int N = solution.size();

    int city_before_i = solution[(i-1+N)%N];
    int city_i = solution[i];
    int city_after_i = solution[i+1];
    int city_before_j = solution[j-1];
    int city_j = solution[j];
    int city_after_j = solution[(j+1)%N];

    if (j == i+1)
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


vector<int> GreedySolver::solve(vector<vector<int>> instance)
{
    vector<int> solution = random_permutation(instance.size());
    int score = this->score_solution(solution, instance);
    bool improved;
    int new_score;

    while(true)
    {
        improved = false;
        for(int i = 0; i < instance.size() - 1; i++)
        {
            for(int j = i+1; j < instance.size(); j++)
            {
                new_score = this->dynamic_score_solution(instance, solution, i, j, score);
                if (new_score < score)
                {
                    score = new_score;
                    swap(solution[i], solution[j]);
                    improved = true;
                    break;
                }
            }
            if (improved) break;
        }
        if (!improved) break;
    }
    return solution;
}
