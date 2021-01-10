#ifndef ARBRE_H
#define ARBRE_H
#include <vector>
#include "contrainte.h"

enum struct bt_heuristic {
    varlargest,
    varsmallest,
    varrandom
};

class Arbre_dom {
    typedef std::vector<int> domaine;
private:
    static std::vector<Contrainte> contraintes;
    static int nb_var;
    static std::vector<int> solution;

    std::vector<domaine> domaines;
    std::vector<int> val_instanciation;
    std::vector<bool> est_instanciee;
    int nb_instanciee;

    std::vector<Arbre_dom*> fils; // vide si une feuille
    Arbre_dom* parent; // pointeur 0 si racine

    bool contrainte_satisfiable(int const i) const;
    bool contrainte_satisfiable(Contrainte const* const contrainte, int const val1) const;
    bool contraintes_satisfiables() const;

    static int bt_smallest_dom(std::vector<domaine> const& domaines, std::vector<bool> const& est_instanciee);
    static int bt_largest_dom(std::vector<domaine> const& domaines, std::vector<bool> const& est_instanciee);
    static int bt_random(std::vector<bool> const& est_instanciee);
    static int bt_random(std::vector<domaine> const& domaines, std::vector<bool> const& est_instanciee);

public:
    Arbre_dom() : parent(nullptr) {nb_instanciee = 0;};
    Arbre_dom(std::vector<domaine>& init_domaines, std::vector<Contrainte>& init_contraintes);
    Arbre_dom(Arbre_dom* init_parent, std::vector<domaine>& init_domaines);
    Arbre_dom(Arbre_dom* init_parent, std::vector<domaine>& init_domaines, std::vector<int> &init_instanciation, std::vector<bool> &init_est_instanciee);
    ~Arbre_dom();

    std::vector<domaine> const& get_domaines() const;
    std::vector<int> const& get_instanciation() const;
    std::vector<int> const& get_solution() const;


    void ajout_fils(Arbre_dom& fil);
    void ajout_fils(std::vector<domaine>& init_domaines);
    void ajout_fils(std::vector<domaine>& init_domaines, std::vector<int> &init_instanciation, std::vector<bool> &init_est_instanciee);
    void retrait_dernier_fils();
    Arbre_dom* get_dernier_fils() const;

    int get_nb_fils() const;

    bool backtrack(int heuristique_var(std::vector<domaine> const&, std::vector<bool> const&));
    bool backtrack(bt_heuristic heuristic);

};

#endif // ARBRE_H
