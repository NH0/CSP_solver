#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <random>
#include "arbre_dom.h"

using namespace std;

typedef std::vector<int> domaine;

int Arbre_dom::nb_var=0;
vector<Contrainte> temp_vec = {};
vector<Contrainte>& Arbre_dom::contraintes = temp_vec;
vector<vector<int>> temp_vec2 = {};
vector<vector<int>>& Arbre_dom::contraintes_par_var = temp_vec2;
vector<int> Arbre_dom::solution = {};

Arbre_dom::Arbre_dom(std::vector<domaine>& init_domaines, vector<Contrainte>& init_contraintes, vector<vector<int>>& init_contrainte_var) {
    parent = nullptr;
    domaines = init_domaines;
    fils = {};
    if (nb_var == 0) {
        nb_var = domaines.size();
    }
    val_instanciation = vector<int>(nb_var, 0);
    est_instanciee = vector<bool>(nb_var, false);
    nb_instanciee = 0;

    if (Arbre_dom::contraintes.empty()) {
        Arbre_dom::contraintes = init_contraintes;
    }
    if (Arbre_dom::contraintes_par_var.empty()) {
        Arbre_dom::contraintes_par_var = init_contrainte_var;
    }
}

Arbre_dom::Arbre_dom(Arbre_dom* init_parent, vector<domaine>& init_domaines) {
    parent = init_parent;
    domaines = init_domaines;
    fils = {};
    val_instanciation = parent->val_instanciation;
    est_instanciee = parent->est_instanciee;
    nb_instanciee = parent->nb_instanciee;
}

Arbre_dom::Arbre_dom(Arbre_dom* init_parent, std::vector<domaine>& init_domaines, std::vector<int> &init_instanciation, std::vector<bool> &init_est_instanciee) {
    parent = init_parent;
    domaines = init_domaines;
    fils = {};
    val_instanciation = init_instanciation;
    est_instanciee = init_est_instanciee;
    nb_instanciee = std::count(est_instanciee.begin(), est_instanciee.end(), true);
}

Arbre_dom::~Arbre_dom() {
    if (fils.size() > 0) {
        for (auto& fil : fils) {
            delete fil;
        }
    }
}

vector<domaine> const& Arbre_dom::get_domaines() const {
    return domaines;
}

vector<int> const& Arbre_dom::get_instanciation() const {
    return val_instanciation;
}

vector<int> const& Arbre_dom::get_solution() const {
    if (!solution.empty()) {
        return solution;
    }

    throw runtime_error("La solution est vide !");
}

void Arbre_dom::ajout_fils(Arbre_dom& fil) {
    fils.push_back(&fil);
}

void Arbre_dom::ajout_fils(std::vector<domaine>& init_domaines) {
    Arbre_dom* fil = new Arbre_dom(this, init_domaines);
    fils.push_back(fil);
}

void Arbre_dom::ajout_fils(std::vector<domaine>& init_domaines, std::vector<int> &init_instanciation, std::vector<bool> &init_est_instanciee ) {
    Arbre_dom* fil = new Arbre_dom(this, init_domaines, init_instanciation, init_est_instanciee);
    fils.push_back(fil);
}

void Arbre_dom::retrait_dernier_fils() {
    Arbre_dom* fil = fils.back();
    fil->~Arbre_dom();
    fils.pop_back();
}

Arbre_dom* Arbre_dom::get_dernier_fils() const {
    return fils.back();
}

int Arbre_dom::get_nb_fils() const {
    return fils.size();
}

bool Arbre_dom::var_satisfait_contraintes(int const var) const {
    for (auto c : contraintes_par_var[var]) {
        if (not(contrainte_satisfiable(c))) {
            return false;
        }
    }
    return true;
}

bool Arbre_dom::contrainte_satisfiable(Contrainte const* const contrainte, int const val1) const {
    /**
     * @brief Contrainte est-elle satisfiable avec var1 = val1 ?
     */

    if (est_instanciee[contrainte->var2]) {
        return contrainte->satisfaite(val1, val_instanciation[contrainte->var2]);
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

bool Arbre_dom::contrainte_satisfiable(int const c) const {
    /**
     * @brief Contrainte d'indice i est-elle satisfiable par instanciatin ?
     */
    Contrainte const contrainte = contraintes[c];
    // Si la variable est instanciée
    if (est_instanciee[contrainte.var1]) {
        return contrainte_satisfiable(&contrainte, val_instanciation[contrainte.var1]);
    }
    else {
        if (est_instanciee[contrainte.var2]) {
            for (auto const &i : domaines[contrainte.var1]) {
                if (contrainte.satisfaite(i, val_instanciation[contrainte.var2])) {
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

bool Arbre_dom::contraintes_satisfiables() const {
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

int Arbre_dom::bt_var_smallest_dom(vector<domaine> const& domaines, vector<bool> const& est_instanciee) {
    int smallest = 0;
    int smallest_s = domaines[0].size();
    for (int i = 0; i<domaines.size(); i++) {
        if (not(est_instanciee[i]) && domaines[i].size() < smallest_s) {
            smallest = i;
            smallest_s = domaines[i].size();
        }
    }
    return smallest;
}

int Arbre_dom::bt_var_largest_dom(vector<domaine> const& domaines, vector<bool> const& est_instanciee) {
    int largest = 0;
    int largest_s = domaines[0].size();
    for (int i = 0; i<domaines.size(); i++) {
        if (not(est_instanciee[i]) && domaines[i].size() > largest_s) {
            largest = i;
            largest_s = domaines[i].size();
        }
    }
    return largest;
}

int sample_if_false(vector<bool> const& vec) {
    static random_device alea;
    static mt19937 gen(alea());

    int nb_false = 0;
    for (auto i : vec) {
        if (!i) {
            nb_false++;
        }
    }
    if (nb_false == 0) {
        throw runtime_error("Cannot sample vector : all variables are true");
    }

    uniform_int_distribution<> distrib(0, nb_false - 1);
    int ieme_element = distrib(gen);
    nb_false = 0;
    for (int i = 0; i<vec.size(); i++) {
        if (!vec[i]) {
            if(nb_false == ieme_element) {
                return i;
            }
            nb_false++;
        }
    }

    throw runtime_error("Empty sample !");
}

int Arbre_dom::bt_var_random(std::vector<domaine> const&, vector<bool> const& est_instanciee) {
    return sample_if_false(est_instanciee);
}

int Arbre_dom::bt_var_random(vector<bool> const& est_instanciee) {
    return sample_if_false(est_instanciee);
}

void Arbre_dom::bt_val_smallest(domaine & val_dom) {
    if (not is_sorted(val_dom.begin(), val_dom.end())) {
        partial_sort(val_dom.begin(), val_dom.end(), val_dom.end());
    }
}

bool reverse_comp(int const a, int const b) {
    return a > b;
}

void Arbre_dom::bt_val_largest(domaine & val_dom) {
    if (not is_sorted(val_dom.begin(), val_dom.end(), reverse_comp)) {
        partial_sort(val_dom.begin(), val_dom.end(), val_dom.end(), reverse_comp);
    }
}

void Arbre_dom::bt_val_random(domaine & val_dom) {
    random_shuffle(val_dom.begin(), val_dom.end());
}

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine> const&, vector<bool> const&), void heuristique_val(domaine &)) {
    if (!contraintes_satisfiables()) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        solution = val_instanciation;
        return true;
    }
    int i = heuristique_var(domaines, est_instanciee);
    heuristique_val(domaines[i]);
    for (auto j = 0; j<domaines[i].size(); j++) {
        vector<domaine> nouv_domaines = domaines;
        nouv_domaines[i] = {domaines[i][j]};
        vector<int> nouv_val_instanciation = val_instanciation;
        nouv_val_instanciation[i] = nouv_domaines[i][0];
        vector<bool> nouv_est_instanciee = est_instanciee;
        nouv_est_instanciee[i] = true;
        ajout_fils(nouv_domaines, nouv_val_instanciation, nouv_est_instanciee);

        if (get_dernier_fils()->backtrack(heuristique_var, heuristique_val, i)) {
            return true;
        }
    }

    return false;
}

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine> const&, vector<bool> const&), void heuristique_val(domaine &), int var_instanciee) {
    if (not(var_satisfait_contraintes(var_instanciee))) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        if (contraintes_satisfiables()) {
            solution = val_instanciation;
            return true;
        }
        return false;
    }
    int i = heuristique_var(domaines, est_instanciee);
    heuristique_val(domaines[i]);
    for (auto j = 0; j<domaines[i].size(); j++) {
        vector<domaine> nouv_domaines = domaines;
        nouv_domaines[i] = {domaines[i][j]};
        vector<int> nouv_val_instanciation = val_instanciation;
        nouv_val_instanciation[i] = nouv_domaines[i][0];
        vector<bool> nouv_est_instanciee = est_instanciee;
        nouv_est_instanciee[i] = true;
        ajout_fils(nouv_domaines, nouv_val_instanciation, nouv_est_instanciee);

        if (get_dernier_fils()->backtrack(heuristique_var, heuristique_val, i)) {
            return true;
        }
    }

    return false;
}

bool Arbre_dom::backtrack(bt_heuristic_var var_heuristic, bt_heuristic_val val_heuristic) {
    int (*pheuristique_var)(std::vector<domaine> const&, vector<bool> const&);
    if (var_heuristic == bt_heuristic_var::varlargest) {
        pheuristique_var = Arbre_dom::bt_var_largest_dom;
    }
    else if (var_heuristic == bt_heuristic_var::varsmallest) {
        pheuristique_var = Arbre_dom::bt_var_smallest_dom;
    }
    else if (var_heuristic == bt_heuristic_var::varrandom) {
        pheuristique_var = Arbre_dom::bt_var_random;
    }
    else {
        throw runtime_error("Calling backtrack with non existent heuristic !");
    }

    void (*pheuristique_val)(domaine &);
    if (val_heuristic == bt_heuristic_val::vallargest) {
        pheuristique_val = Arbre_dom::bt_val_largest;
    }
    else if (val_heuristic == bt_heuristic_val::valsmallest) {
        pheuristique_val = Arbre_dom::bt_val_smallest;
    }
    else if (val_heuristic == bt_heuristic_val::valrandom) {
        pheuristique_val = Arbre_dom::bt_val_random;
    }
    else {
        throw runtime_error("Calling backtrack with non existent heuristic !");
    }

    return backtrack(pheuristique_var, pheuristique_val);
}
