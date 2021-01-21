#pragma once

#include "csp.h"
#include <string>

class Knapsack : public CSP {
public:
    int dt,max;
    vector<int> v;
    vector<vector<int>> w;
    Knapsack(std::vector<int> values,std::vector<vector<int>> weights, std::vector<int> Wmax, int const Borne);
    Knapsack(Knapsack* knapsack,int B);

    int get_value_solution(std::vector<int> s);
};

int value_criteria(int bin,vector<int> v);
vector<int> solve_max_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax);
vector<int> solve_max_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax,bt_heuristic_var hvar, bt_heuristic_val hval, look_ahead look);
int calcul_glouton_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax);
void knapsack_problems(int n, vector<int> &v, vector<vector<int>> &w, vector<int> &W);
