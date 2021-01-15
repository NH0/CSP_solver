#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <random>
#include "arbre_dom.h"

using namespace std;

typedef std::vector<int> domaine;
typedef std::vector<int>::iterator::iterator_type domaine_end;

int Arbre_dom::nb_var=0;

vector<Contrainte> temp_vec = {};
vector<Contrainte>& Arbre_dom::contraintes = temp_vec;
vector<vector<int>> temp_vec2 = {};
vector<vector<int>>& Arbre_dom::contraintes_par_var = temp_vec2;
map<pair<int,int>,int> temp_map = {};
map<pair<int,int>,int>& Arbre_dom::contraintes_communes = temp_map;

std::vector<domaine> Arbre_dom::domaines = {};
std::vector<int> Arbre_dom::val_instanciation = {};
std::vector<bool> Arbre_dom::est_instanciee = {};

int Arbre_dom::nb_instanciee = 0;

vector<int> Arbre_dom::solution = {};


Arbre_dom::Arbre_dom(std::vector<domaine>& init_domaines, vector<Contrainte>& init_contraintes, vector<vector<int>>& init_contrainte_var, map<pair<int,int>,int>& init_contrainte_comm) {
    parent = nullptr;
    fils = {};
    nb_instanciee = 0;
    domaines = init_domaines;
    for (auto &domi : domaines) {
        domaines_ends.push_back(domi.end());
    }
    nb_var = domaines.size();
    val_instanciation = vector<int>(nb_var, 0);
    est_instanciee = vector<bool>(nb_var, false);
    contraintes = init_contraintes;
    contraintes_par_var = init_contrainte_var;
    contraintes_communes = init_contrainte_comm;

    solution = {};
}

Arbre_dom::Arbre_dom(Arbre_dom* init_parent, std::vector<domaine_end>& init_domaines_ends) {
    parent = init_parent;
    fils = {};
    domaines_ends = init_domaines_ends;
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
    return solution;
}

void Arbre_dom::ajout_fils(Arbre_dom& fil) {
    fils.push_back(&fil);
}

void Arbre_dom::ajout_fils(std::vector<domaine_end>& init_domaines_ends) {
    Arbre_dom* fil = new Arbre_dom(this, init_domaines_ends);
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

bool Arbre_dom::contrainte_satisfiable(Contrainte const* const contrainte) const {
    if (est_instanciee[contrainte->var1]) {
        return contrainte_satisfiable(contrainte, val_instanciation[contrainte->var1]);
    }
    else {
        if (est_instanciee[contrainte->var2]) {
            for (auto const &i : domaines[contrainte->var1]) {
                if (contrainte->satisfaite(i, val_instanciation[contrainte->var2])) {
                    return true;
                }
            }
        }
        else {
            for (auto const &i : domaines[contrainte->var1]) {
                for (auto const &j : domaines[contrainte->var2]) {
                    if (contrainte->satisfaite(i, j)) {
                        return true;
                    }
                }
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
    return contrainte_satisfiable(&contrainte);
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

int Arbre_dom::bt_var_smallest_dom(std::vector<domaine_end> const& domaines_ends) {
    int smallest = 0;
    int smallest_s = domaines_ends[0] - domaines[0].begin();
    for (int i = 0; i<domaines.size(); i++) {
        if (not(est_instanciee[i]) && (domaines_ends[i] - domaines[i].begin()) < smallest_s) {
            smallest = i;
            smallest_s = domaines_ends[i] - domaines[i].begin();
        }
    }
    return smallest;
}

int Arbre_dom::bt_var_largest_dom(std::vector<domaine_end> const& domaines_ends) {
    int largest = 0;
    int largest_s = domaines_ends[0] - domaines[0].begin();
    for (int i = 0; i<domaines.size(); i++) {
        if (not(est_instanciee[i]) && (domaines_ends[i] - domaines[i].begin()) > largest_s) {
            largest = i;
            largest_s = domaines_ends[i] - domaines[i].begin();
        }
    }
    return largest;
}

int sample_if_false(vector<bool> const& vec) {
    static random_device alea;
    static mt19937 gen(alea());

    if (vec.empty()) {
        cerr << "Sample_if_false : Empty sample !" << endl;
        return -1;
    }

    int nb_false = count(vec.begin(), vec.end(), false);

    if (nb_false == 0) {
        cerr << "Sample_if_false : Cannot sample vector all variables are true" << endl;
        return -1;
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
    return -1;
}

int Arbre_dom::bt_var_random(std::vector<domaine_end> const&) {
    int index = sample_if_false(est_instanciee);
    if (index != -1) {
        return index;
    }
    throw runtime_error("Trying to backtrack while all var are instanciated");
}

void Arbre_dom::bt_val_smallest(domaine & val_dom, domaine_end val_dom_end) {
    if (not is_sorted(val_dom.begin(), val_dom_end)) {
        sort(val_dom.begin(), val_dom_end);
    }
}

bool reverse_comp(int const a, int const b) {
    return a > b;
}

void Arbre_dom::bt_val_largest(domaine & val_dom, domaine_end val_dom_end) {
    if (not is_sorted(val_dom.begin(), val_dom_end, reverse_comp)) {
        sort(val_dom.begin(), val_dom_end, reverse_comp);
    }
}

void Arbre_dom::bt_val_random(domaine & val_dom, domaine_end val_dom_end) {
    random_shuffle(val_dom.begin(), val_dom_end);
}

bool Arbre_dom::backtrack_loop(int heuristique_var(std::vector<domaine_end> const&), void heuristique_val(domaine &, domaine_end),
                               bool enable_forwardcheck) {
    int i = heuristique_var(domaines_ends);
    heuristique_val(domaines[i], domaines_ends[i]);
    est_instanciee[i] = true;
    nb_instanciee++;
    for (auto j = 0; j<(domaines_ends[i] - domaines[i].begin()); j++) {
        vector<domaine_end> nouv_domaines_ends = domaines_ends;
        if (j > 0) {
            iter_swap(domaines[i].begin(), domaines[i].begin() + j);
        }
        nouv_domaines_ends[i] = domaines[i].begin() + 1;
        val_instanciation[i] = domaines[i][0];

        ajout_fils(nouv_domaines_ends);
        Arbre_dom* fil = get_dernier_fils();

        if (fil->backtrack(heuristique_var, heuristique_val, i, enable_forwardcheck)) {
            return true;
        }
    }
    est_instanciee[i] = false;
    nb_instanciee--;

    return false;
}

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine_end> const&), void heuristique_val(domaine &, domaine_end),
                          bool enable_forwardcheck) {
    if (!contraintes_satisfiables()) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        solution = val_instanciation;
        return true;
    }
    return backtrack_loop(heuristique_var, heuristique_val, enable_forwardcheck);
}

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine_end> const&), void heuristique_val(domaine &, domaine_end),
                          int var_instanciee, bool enable_forwardcheck) {
//    clog << "Starting BT with i = " << var_instanciee << endl;
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
    if (enable_forwardcheck) {
        forward_checking(var_instanciee);
    }
    return backtrack_loop(heuristique_var, heuristique_val, enable_forwardcheck);
}

bool Arbre_dom::backtrack(bt_heuristic_var var_heuristic, bt_heuristic_val val_heuristic, bool enable_forwarcheck) {
    int (*pheuristique_var)(std::vector<domaine_end> const&);
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

    void (*pheuristique_val)(domaine &, domaine_end);
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

    return backtrack(pheuristique_var, pheuristique_val, enable_forwarcheck);
}

void Arbre_dom::delete_values(int var,std::vector<int>& values){
    for (auto const val : values){
        domaines[var].erase(remove(domaines[var].begin(), domaines[var].end(), val), domaines[var].end());
    }
}

bool Arbre_dom::arc_consistence(){
    vector<vector<int>> aTester;
    for (int i=0; i<nb_var;i++){
        for (int j=i+1; j<nb_var;j++){
            if (contraintes_communes.find(pair<int,int>(i,j)) !=contraintes_communes.end()){
                aTester.push_back({i,j});
                aTester.push_back({j,i});
            }
        }
    }

    bool empty_var = false;

    while (not (aTester.empty() or empty_var)){
        vector<int> test = aTester[aTester.size()-1]; // c'est le couple de variable qu'on teste
        aTester.pop_back();
        vector<int> aSupprimer; // c'est la liste des variables à supprimer du domaine
        // avec cette première boucle, nous testons si il existe un support pour chaque valeur de la variable x
        for (auto const &vx : domaines[test[0]]){
            bool support = true; // c'est le booléen qui nous aidera à determiner si une valeur de x à un support
            for (auto const &vy : domaines[test[1]]){
                int c = contraintes_communes[pair<int,int>(min(test[0],test[1]),max(test[0],test[1]))];

                if (contraintes[c].var1==test[0]){
                    support = contraintes[c].satisfaite(vx,vy);
                }
                else{
                    support = contraintes[c].satisfaite(vy,vx);
                }
                if (support){
                    break; // pas besoin de tester tous les vy si on a déjà un support
                }
            }
            if (not support){ // si l'on a pas de support : on doit supprimer vx
                aSupprimer.push_back(vx);
            }
        }
        // si ce n'est pas le cas, on ajoute des nouvelles contraintes à tester
        // car elles peuvent etre impactées par la suppression de valeur(s) du domaine de x
        if (not aSupprimer.empty()){
            for (int i=0;i<nb_var;i++){
                if (contraintes_communes.find(pair<int,int>(i,test[0])) !=contraintes_communes.end()){
                    if (min(i,test[0])==test[0] and max(i,test[0])!=test[1]){
                        aTester.push_back({max(i,test[0]),test[0]});
                    }
                    if (max(i,test[0])==test[0] and min(i,test[0])!=test[1]){
                        aTester.push_back({min(i,test[0]),test[0]});
                    }
                }
            }
            delete_values(test[0],aSupprimer);
        }

        empty_var = domaines[test[0]].empty();
    }
    if (empty_var){
        cerr << "Probleme non realisable" << endl;
        return false;
    }
    return true;
}

bool Arbre_dom::forward_checking(int const var_instanciee) {
    bool has_removed = false;
    for (int i = 0; i < contraintes_par_var[var_instanciee].size(); i++) {
        int contraintei = contraintes_par_var[var_instanciee][i];
        int var1 = contraintes[contraintei].var1;
        int var2 = contraintes[contraintei].var2;
        int other_var = var1 == var_instanciee ? var2 : var1;
        vector<int> to_be_removed {};
        for (int j = 0; j < (domaines_ends[other_var] - domaines[other_var].begin()); j++) {
            int val1 = var1 == var_instanciee ? val_instanciation[var_instanciee] : domaines[other_var][j];
            int val2 = var1 == var_instanciee ? domaines[other_var][j] : val_instanciation[var_instanciee];
            if (not contraintes[contraintei].satisfaite(val1, val2)) {
                // Do not remove immediatly to avoid errors during domain loop
                to_be_removed.push_back(j);
            }
        }
        if (not to_be_removed.empty()) {
            has_removed = true;
        }
        for (int index = to_be_removed.size() - 1; index >= 0; index--) {
            iter_swap(domaines_ends[other_var] - 1, domaines[other_var].begin() + index);
            domaines_ends[other_var]--;
        }
    }
    return has_removed;
}
