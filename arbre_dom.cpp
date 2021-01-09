#include <algorithm>
#include <stdexcept>
#include "arbre_dom.h"

using namespace std;

typedef std::vector<int> domaine;

int Arbre_dom::nb_var=0;
vector<Contrainte> Arbre_dom::contraintes = {};
vector<int> Arbre_dom::solution = {};

Arbre_dom::Arbre_dom(std::vector<domaine>& init_domaines, vector<Contrainte>& init_contraintes) {
    parent = nullptr;
    domaines = init_domaines;
    fils = {};
    nb_var = domaines.size();
    val_instanciation = vector<int>(nb_var, 0);
    est_instanciee = vector<bool>(nb_var, false);
    nb_instanciee = 0;

    Arbre_dom::contraintes = init_contraintes;
}

Arbre_dom::Arbre_dom(Arbre_dom* init_parent, vector<domaine>& init_domaines) {
    parent = init_parent;
    domaines = init_domaines;
    fils = {};
    val_instanciation = vector<int>(parent->val_instanciation);
    est_instanciee = vector<bool>(parent->est_instanciee);
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

bool Arbre_dom::contrainte_satisfiable(int const i) const {
    /**
     * @brief Contrainte d'indice i est-elle satisfiable par instanciatin ?
     */
    Contrainte const contrainte = contraintes[i];
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

int* Arbre_dom::plus_petite_a_instancier() const {
    return nullptr;
}

int* Arbre_dom::plus_grande_a_instancier() const {
    return nullptr;
}

int* Arbre_dom::alea_a_instancier() const {
    return nullptr;
}

bool Arbre_dom::backtrack() {
    if (!contraintes_satisfiables()) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        solution = vector<int>(val_instanciation);
        return true;
    }
    int i = 0;
    // On prend la premiere non instanciée : HEURISTIQUE
    while (est_instanciee[i]) {
        i++;
    }
    for (auto j = 0; j<domaines[i].size(); j++) {
        // On prend la première valeur dans le domaine : HEURISTIQUE
        vector<domaine> nouv_domaines = vector<domaine>(domaines);
        nouv_domaines[i] = {domaines[i][j]};
        vector<int> nouv_val_instanciation = vector<int>(val_instanciation);
        nouv_val_instanciation[i] = nouv_domaines[i][0];
        vector<bool> nouv_est_instanciee = vector<bool>(est_instanciee);
        nouv_est_instanciee[i] = true;
        ajout_fils(nouv_domaines, nouv_val_instanciation, nouv_est_instanciee);

        if (get_dernier_fils()->backtrack()) {
            return true;
        }
    }

    return false;
}
