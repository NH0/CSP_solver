#include "build_in_problems.h"

Knapsack::Knapsack(std::vector<int> values,std::vector<vector<int>> weights, vector<int> Wmax, int const Borne){
    nb_var = values.size();
    w = weights;
    v = values;
    max = 0;
    dt = values[0];

    int bin_var_dom_size = 1;
    for (int i=0; i < nb_var;i++){
        bin_var_dom_size *= 2;
        max += v[i];
        domaines.push_back({0,1});
        contraintes_par_var.push_back({});
        for (int j=i+1; j < nb_var;j++){
            if (v[i] != v[j]){
                dt = min(dt,abs(values[i]-values[j]));
            }
        }
    }

    domaines.push_back({});
    contraintes_par_var.push_back({});
    for (int i=0;i < bin_var_dom_size;i++){
        bool b = value_criteria(i,v)>=Borne;
        for (int j=0; j<w.size();j++){
            b = b and value_criteria(i,w[j])<=Wmax[j];
        }
        if(b){
            domaines[nb_var].push_back(i);
        }
    }
    nb_var += 1;

    int power = 1;
    for (int i=0; i < nb_var - 1;i++){
        if (i>0){
            power *= 2;
        }
        Contrainte c = Contrainte(i,nb_var-1);
        for (auto d : domaines[nb_var-1]){
            c.ajoute_relation(d/power % 2,d);
        }
        contraintes.push_back(c);
        contraintes_par_var[i].push_back(contraintes.size()-1);
        contraintes_par_var[nb_var-1].push_back(contraintes.size()-1);
        contraintes_communes.insert(pair<pair<int,int>,int>({i,nb_var-1},contraintes.size() - 1));
    }

    arbre = Arbre_dom(domaines, contraintes, contraintes_par_var, contraintes_communes);
}

Knapsack::Knapsack(Knapsack* knapsack,int B){
    nb_var = knapsack->nb_var;
    domaines = knapsack->domaines;
    max = knapsack->max;
    dt = knapsack->dt;
    v = knapsack->v;
    w = knapsack->w;
    contraintes = knapsack->contraintes;
    contraintes_par_var = knapsack->contraintes_par_var;
    contraintes_communes = knapsack->contraintes_communes;
    arbre = Arbre_dom(domaines, contraintes, contraintes_par_var, contraintes_communes);

    // change the domain of the value of the criteria
    std::vector<int> new_dom = {};
    for (auto d : domaines[nb_var-1]){
        if (value_criteria(d,v) >= B){
            new_dom.push_back(d);
        }
        else{
            for (auto c : contraintes_par_var[contraintes_par_var.size()-1]){
                contraintes[c].supprime_relation(0,d);
                contraintes[c].supprime_relation(1,d);
            }
        }
    }
    domaines[domaines.size()-1] = new_dom;
}

int value_criteria(int bin,vector<int> v){
    int val = 0, b = bin, cpt = 0;
    while (b != 0){
        if (b % 2 == 1){
            val += v[cpt];
        }
        cpt += 1;
        b /= 2;
    }
    return val;
}

int Knapsack::get_value_solution(std::vector<int> s){
    int val = 0;
    for (int i = 0; i< nb_var-1;i++){
        val += v[i]*s[i];
    }
    return(val);
}

vector<int> solve_max_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax){
    int B_inf = calcul_glouton_knapsack(v,w,Wmax);

    Knapsack* knapsack = new Knapsack(v,w,Wmax,B_inf);
    vector<int> sol;

    while (not knapsack->domaines[knapsack->nb_var-1].empty()) {
        //sol = knapsack->solve(bt_heuristic_var::varlargest, bt_heuristic_val::vallargest, false, look_ahead::forward_checking);
        sol = knapsack->solve(bt_heuristic_var::largest_domain, bt_heuristic_val::largest, false, look_ahead::forward_checking);
        B_inf = knapsack->get_value_solution(sol);
        knapsack = new Knapsack(knapsack,B_inf+knapsack->dt);
    }

    return sol;
}

bool cmp_second_of_pair(std::pair<int, double>& a, std::pair<int, double>& b){
    return a.second < b.second;
}

void sort(map<int, double>& M){
    vector<pair<int, double> > A;

    for (auto& it : M) {
        A.push_back(it);
    }

    sort(A.begin(), A.end(), cmp_second_of_pair);
}

int calcul_glouton_knapsack(std::vector<int> v,std::vector<vector<int>> w, vector<int> Wmax){
    std::map<int,double> ratio;
    for (int i=0; i<v.size();i++){
        ratio[i] = 0;
        for (int j=0;j<w.size();j++){
             ratio[i] += w[j][i]/v[i];
        }
    }
    sort(ratio);
    vector<int> s;
    for (int j=0;j<w.size();j++){
         s.push_back(0);
    }
    int c = 0;
    for (auto& it : ratio){
        bool b = true;
        for (int j=0;j<w.size();j++){
            b = b and (s[j]+w[j][it.first] <= Wmax[j]);
        }
        if(b){
            c += v[it.first];
            for (int j=0;j<w.size();j++){
                s[j] += w[j][it.first];
            }
        }
    }
    return c;
}

void knapsack_problems(int n, vector<int> &v, vector<vector<int>> &w, vector<int> &W){
    if (n==1){
        w = {{23,31,29,44,53,38,63,85,89,82}};
        v = {92,57,49,68,60,43,67,84,87,72};
        W = {165};
    }
    if (n==2){
        w = {{12,7,11,8,9}};
        v = {24,13,23,15,16};
        W = {26};
    }
    if (n==3){
        w = {{56,59,80,64,75,17}};
        v = {50,50,64,46,50,5};
        W = {190};
    }
    if (n==4){
        w = {{31,10,20,19,4,3,6}};
        v = {70,20,39,37,7,5,10};
        W = {50};
    }
    if (n==5){
        w = {{25,35,45,5,25,3,2,2}};
        v = {350,400,450,20,70,8,5,5};
        W = {104};
    }
    if (n==6){ //optimal weight of 169
        w = {{41,50,49,59,55,57,60}};
        v = {442,525,511,593,546,564,617};
        W = {170};
    }
    if (n==7){ // optimal profit of 1458
        w = {{70,73,77,80,82,87,90,94,98,106,110,113,115,118,120}};
        v  = {135,139,149,150,156,163,173,184,192,201,210,214,221,229,240};
        W = {750};
    }
    if (n==8){ // optimal profit of 13549094
        w = {{382745,799601,909247,729069,467902,44328,34610,698150,823460,903959,853665,551830,
              610856,670702,488960,951111,323046,446298,931161,31385,496951,264724,224916,169684}};
        v = {825594,1677009,1676628,1523970,943972,97426,69666,1296457,1679693,1902996,1844992,1049289,
             1252836,1319836,953277,2067538,675367,853655,1826027,65731,901489,577243,466257,369261};
        W = {6404180};
    }
}
