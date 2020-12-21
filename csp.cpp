#include "csp.h"
#include <stdexcept>

double Operation::apply(double a, double b){
    if (op == "+"){
        return a+b;
    }
    if (op == "-"){
        return a-b;
    }
    if (op == "*"){
        return a*b;
    }
    if (op == "/"){
        return a/b;
    }
    else {
        throw std::invalid_argument(op);
    }
}

bool Comparaison::apply(double a, double b){
    if (comp == "="){
        return a==b;
    }
    if (comp == "!="){
        return a != b;
    }
    if (comp == "<="){
        return a <= b;
    }
    if (comp == ">="){
        return a >= b;
    }
    if (comp == "<"){
        return a < b;
    }
    if (comp == ">"){
        return a > b;
    }
    else {
        throw std::invalid_argument(comp);
    }
}

Contrainte::Contrainte(double c1, int i1, string o, double c2, int i2, string c, double v){
    coef1 = c1;
    var1 = i1;
    coef2 = c2;
    var2 = i2;
    ope = Operation(o);
    comp = Comparaison(c);
    valeur = v;
}

bool Contrainte::satisfaite(int v1, int v2){
    return(comp.apply(ope.apply(v1*coef1,v2*coef2),valeur));
}

bool CSP::var_satisfait_contraintes(const int i) const {
    for (const int& j : contraintes_par_var[i]) {
        if (!contrainte_satisfiable(j)) {
            return false;
        }
    }
    return true;
}

bool CSP::contrainte_satisfiable(const int i) const {
    Contrainte contrainte = contraintes[i];
    for (auto const &i : domaines[contrainte.var1]) {
        for (auto const &j : domaines[contrainte.var2]) {
            if (contrainte.satisfaite(i, j)) {
                return true;
            }
        }
    }
    return false;
}

Reine::Reine(int n){
    nb_var = n;
    for (int i = 0; i<n; i++) {
        contraintes_par_var.push_back({});
    }
    for (int i = 0; i<n; i++){
        vector<int> domaine_i;
        for (int j = 0; j<n ;j++){
            domaine_i.push_back(j);
            if (i<j){
                Contrainte c = Contrainte(1,i,"-",1,j,"!=",0);
                contraintes.push_back(c);
                contraintes_par_var[i].push_back(contraintes.size() - 1);
                contraintes_par_var[j].push_back(contraintes.size() - 1);
            }
        }
        domaines.push_back(domaine_i);
    }
    arbre = Arbre_dom(domaines);
}
