#include "build_in_problems.h"

Knapsack::Knapsack(std::vector<int> values,std::vector<vector<int>> weights, vector<int> Wmax, int const Borne){
    nb_var = values.size();
    w = weights;
    v = values;
    max = 0;
    dt = values[0];
    for (int i=0; i < nb_var;i++){
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
    for (int i=0;i < pow(2,v.size());i++){
        bool b = value_criteria(i,v)>=Borne;
        for (int j=0; j<w.size();j++){
            b = b and value_criteria(i,w[j])<=Wmax[j];
        }
        if(b){
            domaines[nb_var].push_back(i);
        }
    }
    nb_var += 1;

    for (int i=0; i < values.size();i++){
        int power = pow(2,i);
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
    for (auto d : domaines[domaines.size()-1]){
        if (value_criteria(d,v) >= B){
            new_dom.push_back(d);
        }
        else{
            for (auto c : contraintes_par_var[contraintes_par_var.size()-1]){
                int i = contraintes[c].var1;
                int power = pow(2,i);
                contraintes[c].supprime_relation(d/power % 2,d);
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

vector<int> solve_knapsack(std::vector<int> v,std::vector<vector<int>> w, std::vector<int> Wmax){
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

bool cmp(std::pair<int, double>& a, std::pair<int, double>& b){
    return a.second < b.second;
}

void sort(map<int, double>& M){
    vector<pair<int, double> > A;

    for (auto& it : M) {
        A.push_back(it);
    }

    sort(A.begin(), A.end(), cmp);
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
