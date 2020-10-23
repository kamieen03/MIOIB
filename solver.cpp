#include "solver.hpp"
#include <limits>

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
    score += instance[solution.size()-1][0];
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



