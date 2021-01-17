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

int Arbre_dom::get_nb_leaves() const {
    if (fils.size() == 0) {
        return 1;
    }
    int fils_leaves = 0;
    for (auto &fil : fils) {
        fils_leaves += fil->get_nb_leaves();
    }
    return fils_leaves;
}

int Arbre_dom::get_nb_nodes() const {
    if (fils.size() == 0) {
        return 1;
    }
    int fils_nodes = 0;
    for (auto &fil : fils) {
        fils_nodes += fil->get_nb_nodes();
    }
    return 1 + fils_nodes;
}

void Arbre_dom::display_tree_size() const {
    int nb_leaves = get_nb_leaves();
    int nb_nodes = get_nb_nodes();
    cout << "Number of nodes : " << nb_nodes << "\n"
         << "Number of leaves : " << nb_leaves << endl;
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

int Arbre_dom::bt_var_smallest_dom(std::vector<domaine_end> const& domaines_ends, int) {
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

int Arbre_dom::bt_var_largest_dom(std::vector<domaine_end> const& domaines_ends, int) {
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

int Arbre_dom::bt_var_random(std::vector<domaine_end> const&, int) {
    int index = sample_if_false(est_instanciee);
    if (index != -1) {
        return index;
    }
    throw runtime_error("Trying to backtrack while all var are instanciated");
}

int Arbre_dom::bt_var_constrained(std::vector<domaine_end> const&, int) {
    // Choose var with highest number of constraints
    int nb_constraints = 0;
    int var = 0;
    for (auto v = 0; v<nb_var; v++) {
        if (contraintes_par_var[v].size() > nb_constraints) {
            nb_constraints = contraintes_par_var[v].size();
            var = v;
        }
    }
    return var;
}

int Arbre_dom::bt_var_relaxed(std::vector<domaine_end> const&, int) {
    // Choose the var with the smallest number of constraints
    int nb_contraints = contraintes_par_var[0].size();
    int var = 0;
    for (auto v = 1; v<nb_var; v++) {
        if (contraintes_par_var[v].size() < nb_contraints) {
            nb_contraints = contraintes_par_var[v].size();
            var = v;
        }
    }
    return var;
}

int Arbre_dom::bt_var_linked(std::vector<domaine_end> const&, int var_instanciee) {
    // Choose the first non instanciated variable in the set of vars linked to the last instanciated var by a constraint.
    // If none, choose the first (in terms of index) non instanciated.
    if (var_instanciee == -1) {
        return 0;
    }
    int c = 0;
    Contrainte* con = &contraintes[contraintes_par_var[var_instanciee][c]];
    int other_var = con->var1 == var_instanciee ? con->var2 : con->var1;

    while (c < contraintes_par_var[var_instanciee].size() && est_instanciee[other_var]) {
        c++;
        con = &contraintes[contraintes_par_var[var_instanciee][c]];
        other_var = con->var1 == var_instanciee ? con->var2 : con->var1;
    }
    if (c < contraintes_par_var[var_instanciee].size()) {
        return other_var;
    }
    // First non instanciated variable
    return find(est_instanciee.begin(), est_instanciee.end(), false) - est_instanciee.begin();
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

bool Arbre_dom::backtrack_loop(int heuristique_var(std::vector<domaine_end> const&, int), void heuristique_val(domaine &, domaine_end),
                               look_ahead lookahead, int var_instanciee) {
    int i = heuristique_var(domaines_ends, var_instanciee);
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

        if (fil->backtrack(heuristique_var, heuristique_val, i, lookahead)) {
            return true;
        }
    }
    est_instanciee[i] = false;
    nb_instanciee--;

    return false;
}

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine_end> const&, int), void heuristique_val(domaine &, domaine_end),
                          look_ahead lookahead) {
    if (!contraintes_satisfiables()) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        solution = val_instanciation;
        return true;
    }
    return backtrack_loop(heuristique_var, heuristique_val, lookahead);
}

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine_end> const&, int), void heuristique_val(domaine &, domaine_end),
                          int var_instanciee, look_ahead lookahead) {
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
    if (lookahead == look_ahead::forward_checking) {
        forward_checking(var_instanciee);
    }
    else if (lookahead == look_ahead::maintain_arc_consistency) {
        // maintain_ac(var_instanciee);
    }
    return backtrack_loop(heuristique_var, heuristique_val, lookahead, var_instanciee);
}

bool Arbre_dom::backtrack(bt_heuristic_var var_heuristic, bt_heuristic_val val_heuristic, look_ahead lookahead) {
    int (*pheuristique_var)(std::vector<domaine_end> const&, int);
    if (var_heuristic == bt_heuristic_var::varlargest) {
        pheuristique_var = Arbre_dom::bt_var_largest_dom;
    }
    else if (var_heuristic == bt_heuristic_var::varsmallest) {
        pheuristique_var = Arbre_dom::bt_var_smallest_dom;
    }
    else if (var_heuristic == bt_heuristic_var::varrandom) {
        pheuristique_var = Arbre_dom::bt_var_random;
    }
    else if (var_heuristic == bt_heuristic_var::varconstrained) {
        pheuristique_var = Arbre_dom::bt_var_constrained;
    }
    else if (var_heuristic == bt_heuristic_var::varrelaxed) {
        pheuristique_var = Arbre_dom::bt_var_relaxed;
    }
    else if (var_heuristic == bt_heuristic_var::varlinked) {
        pheuristique_var = Arbre_dom::bt_var_linked;
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

    return backtrack(pheuristique_var, pheuristique_val, lookahead);
}

bool Arbre_dom::arc_consistence(int var){
    vector<vector<int>> aTester;
    if (var == -1){ // pour utiliser l'arc consistence dans l'arbre il faut avoir tester l'ac globalement avant
        for (int i=0; i<nb_var;i++){
            for (int j=i+1; j<nb_var;j++){
                if (contraintes_communes.find(pair<int,int>(i,j)) !=contraintes_communes.end()){
                    aTester.push_back({i,j});
                    aTester.push_back({j,i});
                }
            }
        }
    }
    else {
        for (int i=0; i<nb_var;i++){
            int m1 = min(i,var), m2 = max(i,var);
            if (contraintes_communes.find(pair<int,int>(m1,m2)) !=contraintes_communes.end()){
                aTester.push_back({i,var}); // seulement besoin de tester les variables en lien avec celle qu'on instancie
            }
        }
    }

    bool empty_var = false;

    while (not (aTester.empty() or empty_var)){
        vector<int> test = aTester[aTester.size()-1]; // c'est le couple de variable qu'on teste
        aTester.pop_back();
        vector<int> aSupprimer; // c'est la liste des variables à supprimer du domaine
        // avec cette première boucle, nous testons si il existe un support pour chaque valeur de la variable x
        for (int i=1; i < domaines[test[0]].size();i++){
            int vx = domaines[test[0]][i];
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
                aSupprimer.push_back(i);
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
            for (int index = aSupprimer.size()-1; index >=0 ; index--){
                iter_swap(domaines_ends[test[0]]-1,domaines[test[0]].begin()+index);
                domaines_ends[test[0]] --;
            }
        }

        empty_var = domaines[test[0]].empty();
    }
    if (empty_var){
        // cerr << "Probleme non realisable" << endl;
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
