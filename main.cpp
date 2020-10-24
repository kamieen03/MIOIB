#include <vector>
#include <list>
#include <iostream>
#include "data_loader.hpp"
#include "solver.hpp"


using namespace std;

int main()
{
    DataLoader dl;
    vector<vector<int>> instance = dl.load("instancje/ft53.atsp");
    GreedySolver solver;
    vector<int> solution = solver.solve(instance);
    for (int i = 0; i < solution.size(); i ++)
        cout << solution[i] << " ";
    cout << endl;
    cout << solver.score_solution(solution, instance) << endl;
    return 0;
}
