#pragma once

#include "csp.h"
#include <string>

class Knapsack : public CSP {
public:
    int dt,max,B_inf;
    vector<int> v;
    vector<vector<int>> w;
    Knapsack(std::vector<int> values,std::vector<vector<int>> weights, std::vector<int> Wmax, int const Borne);
    Knapsack(Knapsack* knapsack,int B);

    int get_value_solution(std::vector<int> s);
    int get_B_inf(){return B_inf;}
};

int value_criteria(int bin,vector<int> v);
vector<int> solve_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax);
int calcul_glouton_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax);
