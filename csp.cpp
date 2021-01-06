#include "csp.h"
#include <stdexcept>

double Operation::apply(double a, double b) const{
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

bool Comparaison::apply(double a, double b) const{
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

bool Contrainte::satisfaite(const int v1, const int v2) const{
    return(comp.apply(ope.apply(v1*coef1,v2*coef2),valeur));
}

bool CSP::var_satisfait_contraintes(const int var) const {
    for (const int& j : contraintes_par_var[var]) {
        if (!contrainte_satisfiable(j)) {
            return false;
        }
    }
    return true;
}

bool CSP::contrainte_satisfiable(const Contrainte* contrainte, const int val1) const {
    /**
     * @brief Contrainte est-elle satisfiable avec var1 = val1 ?
     */
    const int* pval2 = instanciation[contrainte->var2];
    if (pval2 != nullptr) {
        return contrainte->satisfaite(val1, *pval2);
    }
    else {
        for (auto const &j : domaines[contrainte->var2]) {
            if (contrainte->satisfaite(val1, j)) {
                return true;
            }
        }
    }

    return false;
}

bool CSP::contrainte_satisfiable(const int i) const {
    /**
     * @brief Contrainte d'indice i est-elle satisfiable par instanciatin ?
     */
    const Contrainte contrainte = contraintes[i];
    // Si la variable est instanciée
    const int* pval1 = instanciation[contrainte.var1];
    if (pval1 != nullptr) {
        return contrainte_satisfiable(&contrainte, *pval1);
    }
    else {
        const int* pval2 = instanciation[contrainte.var2];
        if (pval2 != nullptr) {
            for (auto const &i : domaines[contrainte.var1]) {
                if (contrainte.satisfaite(i, *pval2)) {
                    return true;
                }
            }
        }
        else {
            for (auto const &i : domaines[contrainte.var1]) {
                for (auto const &j : domaines[contrainte.var2]) {
                    if (contrainte.satisfaite(i, j)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool CSP::contraintes_satisfiables() const {
    /***
     * Toutes les contraintes sont-elles satisfaites par instanciation ?
     */
    for (auto i = 0; i<contraintes.size(); i++) {
        if (!contrainte_satisfiable(i)) {
            return false;
        }
    }
    return true;
}

bool CSP::backtrack() {
    if (!contraintes_satisfiables()) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        return true;
    }
    int i = 0;
    while (instanciation[i] != nullptr) {
        i++;
    }
    nb_instanciee++;
    for (auto j = 0; j<domaines[i].size(); j++) {
        instanciation[i] = &domaines[i][j];
        if (backtrack()) {
            return true;
        }
    }
    instanciation[i] = nullptr;
    nb_instanciee--;

    return false;
}

std::vector<int> CSP::solve() {
    if(backtrack()) {
        std::vector<int> valeurs;
        for (auto i = 0; i<nb_var; i++) {
            valeurs.push_back(*instanciation[i]);
        }
        return valeurs;
    }
    return {};
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
    instanciation = std::vector<int*>(nb_var, nullptr);
}
