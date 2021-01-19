#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <random>
#include <utility>
#include <stack>
#include <deque>
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
    int smallest = -1;
    int smallest_s = -1;
    for (int i = 0; i<domaines.size(); i++) {
        if (not(est_instanciee[i])) {
            if (smallest == -1) {
                smallest = i;
                smallest_s = domaines_ends[i] - domaines[i].begin();
            }
            else {
                if ((domaines_ends[i] - domaines[i].begin()) < smallest_s) {
                smallest = i;
                smallest_s = domaines_ends[i] - domaines[i].begin();
                }
            }
        }
    }
    if (smallest == -1) {
        throw runtime_error("Trying to select a variable to initialize, but all variables are initialized !");
    }
    return smallest;
}

int Arbre_dom::bt_var_largest_dom(std::vector<domaine_end> const& domaines_ends, int) {
    int largest = -1;
    int largest_s = -1;
    for (int i = 0; i<domaines.size(); i++) {
        if (not(est_instanciee[i])) {
            if (largest == -1) {
                largest = i;
                largest_s = domaines_ends[i] - domaines[i].begin();
            }
            else {
                if ((domaines_ends[i] - domaines[i].begin()) > largest_s) {
                largest = i;
                largest_s = domaines_ends[i] - domaines[i].begin();
                }
            }
        }
    }
    if (largest == -1) {
        throw runtime_error("Trying to select a variable to initialize, but all variables are initialized !");
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
    throw runtime_error("Trying to select a variable to initialize, but all variables are initialized !");
}

int Arbre_dom::bt_var_constrained(std::vector<domaine_end> const&, int) {
    // Choose var with highest number of constraints
    int nb_constraints = -1;
    int var = -1;
    for (auto v = 0; v<nb_var; v++) {
        if (not(est_instanciee[v])) {
            if (var == -1) {
                var = v;
                nb_constraints = contraintes_par_var[v].size();
            }
            else {
                if (contraintes_par_var[v].size() > nb_constraints) {
                    nb_constraints = contraintes_par_var[v].size();
                    var = v;
                }
            }
        }

    }
    if (var == -1) {
        throw runtime_error("Trying to select a variable to initialize, but all variables are initialized !");
    }
    return var;
}

int Arbre_dom::bt_var_relaxed(std::vector<domaine_end> const&, int) {
    // Choose the var with the smallest number of constraints
    int nb_constraints = -1;
    int var = -1;
    for (auto v = 0; v<nb_var; v++) {
        if (not(est_instanciee[v])) {
            if (var == -1) {
                var = v;
                nb_constraints = contraintes_par_var[v].size();
            }
            else {
                if (contraintes_par_var[v].size() < nb_constraints) {
                    nb_constraints = contraintes_par_var[v].size();
                    var = v;
                }
            }
        }

    }
    if (var == -1) {
        throw runtime_error("Trying to select a variable to initialize, but all variables are initialized !");
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

void Arbre_dom::bt_val_smallest(domaine & val_dom, domaine_end val_dom_end, int) {
    if (not is_sorted(val_dom.begin(), val_dom_end)) {
        sort(val_dom.begin(), val_dom_end);
    }
}

bool reverse_comp(int const a, int const b) {
    return a > b;
}

void Arbre_dom::bt_val_largest(domaine & val_dom, domaine_end val_dom_end, int) {
    if (not is_sorted(val_dom.begin(), val_dom_end, reverse_comp)) {
        sort(val_dom.begin(), val_dom_end, reverse_comp);
    }
}

void Arbre_dom::bt_val_random(domaine & val_dom, domaine_end val_dom_end, int) {
    random_shuffle(val_dom.begin(), val_dom_end);
}

int count_nb_appearance_first(int val, set<pair<int,int>> const & constraint_pairs) {
    int count = 0;
    for (auto &p : constraint_pairs) {
        if (p.first == val) {
            count++;
        }
    }
    return count;
}

int count_nb_appearance_second(int val, set<pair<int,int>> const & constraint_pairs) {
    int count = 0;
    for (auto &p : constraint_pairs) {
        if (p.second == val) {
            count++;
        }
    }
    return count;
}

int count_nb_appearance(int val, set<pair<int,int> > const & constraint_pairs, bool first) {
    if (first) {
        return count_nb_appearance_first(val, constraint_pairs);
    }
    return count_nb_appearance_second(val, constraint_pairs);
}

int count_total_nb_appearance(int val, vector<int> & constraints_indexes, int var_instanciee, vector<Contrainte> const & contraintes) {
    int nb_appearance = 0;
    for (auto &c : constraints_indexes) {
        bool first = var_instanciee == contraintes[c].var1;
        nb_appearance += count_nb_appearance(val, contraintes[c].c, first);
    }
    return nb_appearance;
}

int count_nb_unique_appearance(int val, vector<int> & constraints_indexes, int var_instanciee, vector<Contrainte> const & contraintes) {
    int nb_unique_appearance = 0;
    for (auto &c : constraints_indexes) {
        bool first = var_instanciee == contraintes[c].var1;
        int nb_appearance = count_nb_appearance(val, contraintes[c].c, first);
        if (nb_appearance == 1) {
            nb_unique_appearance++;
        }
    }
    return nb_unique_appearance;
}

void Arbre_dom::bt_val_most_supported(domaine & val_dom, domaine_end dom_end, int var_instanciee) {
    auto appears_more = [var_instanciee](int const val1, int const val2) {
        return count_total_nb_appearance(val1, contraintes_par_var[var_instanciee], var_instanciee, contraintes)
                > count_total_nb_appearance(val2, contraintes_par_var[var_instanciee], var_instanciee, contraintes);
    };
    sort(val_dom.begin(), dom_end, appears_more);
}

void Arbre_dom::bt_val_least_supported(domaine & val_dom, domaine_end dom_end, int var_instanciee) {
    auto appears_more = [var_instanciee](int const val1, int const val2) {
        return count_total_nb_appearance(val1, contraintes_par_var[var_instanciee], var_instanciee, contraintes)
                < count_total_nb_appearance(val2, contraintes_par_var[var_instanciee], var_instanciee, contraintes);
    };
    sort(val_dom.begin(), dom_end, appears_more);
}

void Arbre_dom::bt_val_most_filtration(domaine & val_dom, domaine_end dom_end, int var_instanciee) {
    auto appears_more = [var_instanciee](int const val1, int const val2) {
        return count_nb_unique_appearance(val1, contraintes_par_var[var_instanciee], var_instanciee, contraintes) / contraintes_par_var[var_instanciee].size()
                > count_nb_unique_appearance(val2, contraintes_par_var[var_instanciee], var_instanciee, contraintes) / contraintes_par_var[var_instanciee].size();
    };
    sort(val_dom.begin(), dom_end, appears_more);
}

void Arbre_dom::bt_val_fewest_filtration(domaine & val_dom, domaine_end dom_end, int var_instanciee) {
    auto appears_more = [var_instanciee](int const val1, int const val2) {
        return count_nb_unique_appearance(val1, contraintes_par_var[var_instanciee], var_instanciee, contraintes) / contraintes_par_var[var_instanciee].size()
                < count_nb_unique_appearance(val2, contraintes_par_var[var_instanciee], var_instanciee, contraintes) / contraintes_par_var[var_instanciee].size();
    };
    sort(val_dom.begin(), dom_end, appears_more);
}

bool Arbre_dom::backtrack_loop(int heuristique_var(std::vector<domaine_end> const&, int), void heuristique_val(domaine &, domaine_end, int),
                               look_ahead lookahead, int var_instanciee) {
    int i = heuristique_var(domaines_ends, var_instanciee);
    heuristique_val(domaines[i], domaines_ends[i], i);
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

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine_end> const&, int), void heuristique_val(domaine &, domaine_end, int),
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

bool Arbre_dom::backtrack(int heuristique_var(std::vector<domaine_end> const&, int), void heuristique_val(domaine &, domaine_end, int),
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
        bool solvable = arc_consistence(var_instanciee);
        if (not solvable) {
            return false;
        }
    }
    return backtrack_loop(heuristique_var, heuristique_val, lookahead, var_instanciee);
}

bool Arbre_dom::backtrack(bt_heuristic_var var_heuristic, bt_heuristic_val val_heuristic, look_ahead lookahead) {
    int (*pheuristique_var)(std::vector<domaine_end> const&, int);
    if (var_heuristic == bt_heuristic_var::largest_domain) {
        pheuristique_var = Arbre_dom::bt_var_largest_dom;
    }
    else if (var_heuristic == bt_heuristic_var::smallest_domain) {
        pheuristique_var = Arbre_dom::bt_var_smallest_dom;
    }
    else if (var_heuristic == bt_heuristic_var::random) {
        pheuristique_var = Arbre_dom::bt_var_random;
    }
    else if (var_heuristic == bt_heuristic_var::most_constraints) {
        pheuristique_var = Arbre_dom::bt_var_constrained;
    }
    else if (var_heuristic == bt_heuristic_var::least_constraints) {
        pheuristique_var = Arbre_dom::bt_var_relaxed;
    }
    else if (var_heuristic == bt_heuristic_var::linked_to_previous_var) {
        pheuristique_var = Arbre_dom::bt_var_linked;
    }
    else {
        throw runtime_error("Calling backtrack with non existent heuristic !");
    }

    void (*pheuristique_val)(domaine &, domaine_end, int);
    if (val_heuristic == bt_heuristic_val::largest) {
        pheuristique_val = Arbre_dom::bt_val_largest;
    }
    else if (val_heuristic == bt_heuristic_val::smallest) {
        pheuristique_val = Arbre_dom::bt_val_smallest;
    }
    else if (val_heuristic == bt_heuristic_val::random) {
        pheuristique_val = Arbre_dom::bt_val_random;
    }
    else if (val_heuristic == bt_heuristic_val::most_supported) {
        pheuristique_val = Arbre_dom::bt_val_most_supported;
    }
    else if (val_heuristic == bt_heuristic_val::least_supported) {
        pheuristique_val = Arbre_dom::bt_val_least_supported;
    }
    else if (val_heuristic == bt_heuristic_val::most_filtration) {
        pheuristique_val = Arbre_dom::bt_val_most_filtration;
    }
    else if (val_heuristic == bt_heuristic_val::fewest_filtration) {
        pheuristique_val = Arbre_dom::bt_val_fewest_filtration;
    }
    else {
        throw runtime_error("Calling backtrack with non existent heuristic !");
    }

    return backtrack(pheuristique_var, pheuristique_val, lookahead);
}

bool Arbre_dom::arc_consistence(int var){
    stack<vector<int>> aTester;
    if (var == -1){ // pour utiliser l'arc consistence dans l'arbre il faut avoir tester l'ac globalement avant
        for (int i=0; i<nb_var;i++){
            for (int j=i+1; j<nb_var;j++){
                if (contraintes_communes.find(pair<int,int>(i,j)) !=contraintes_communes.end()){
                    aTester.push({i,j});
                    aTester.push({j,i});
                }
            }
        }
    }
    else {
        for (int i=0; i<nb_var;i++){
            int m1 = min(i,var), m2 = max(i,var);
            if (contraintes_communes.find(pair<int,int>(m1,m2)) !=contraintes_communes.end()){
                aTester.push({i,var}); // seulement besoin de tester les variables en lien avec celle qu'on instancie
            }
        }
    }

    bool empty_var = false;

    while (not (aTester.empty() or empty_var)){
        vector<int> test = aTester.top(); // c'est le couple de variable qu'on teste
        aTester.pop();
        vector<vector<int>::iterator> aSupprimer; // c'est la liste des variables à supprimer du domaine
        // avec cette première boucle, nous testons si il existe un support pour chaque valeur de la variable x
        for (auto iter = domaines[test[0]].begin(); iter != domaines_ends[test[0]]; iter++){
            int vx = *iter;
            bool support = true; // c'est le booléen qui nous aidera à determiner si une valeur de x à un support
            for (auto iter2 = domaines[test[1]].begin(); iter2 != domaines_ends[test[1]]; iter2++){
                int vy = *iter2;
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
                aSupprimer.push_back(iter);
            }
        }
        // si ce n'est pas le cas, on ajoute des nouvelles contraintes à tester
        // car elles peuvent etre impactées par la suppression de valeur(s) du domaine de x
        if (not aSupprimer.empty()){
            for (int i=0;i<nb_var;i++){
                if (contraintes_communes.find(pair<int,int>(i,test[0])) != contraintes_communes.end()){
                    if (min(i,test[0])==test[0] and max(i,test[0])!=test[1]){
                        aTester.push({max(i,test[0]),test[0]});
                    }
                    if (max(i,test[0])==test[0] and min(i,test[0])!=test[1]){
                        aTester.push({min(i,test[0]),test[0]});
                    }
                }
            }
            for (auto & iter : aSupprimer){
                iter_swap(domaines_ends[test[0]] - 1, iter);
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
            iter_swap(domaines_ends[other_var] - 1, domaines[other_var].begin() + to_be_removed[index]);
            domaines_ends[other_var]--;
        }
    }
    return has_removed;
}
