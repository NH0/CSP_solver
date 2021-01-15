#ifndef ARBRE_H
#define ARBRE_H
#include <vector>
#include <map>
#include <utility>
#include "contrainte.h"

enum struct bt_heuristic_var {
    varlargest,
    varsmallest,
    varrandom
};

enum struct bt_heuristic_val {
    vallargest,
    valsmallest,
    valrandom
};

enum struct look_ahead {
    none,
    forward_checking,
    maintain_arc_consistency
};

class Arbre_dom {
    typedef std::vector<int> domaine;
    typedef std::vector<int>::iterator domaine_end;
private:

    static int nb_var;
    static std::vector<Contrainte>& contraintes;
    static std::vector<std::vector<int>>& contraintes_par_var;
    static std::map<std::pair<int,int>,int>& contraintes_communes;
    static std::vector<domaine> domaines;
    static std::vector<int> val_instanciation;
    static std::vector<bool> est_instanciee;
    static int nb_instanciee;

    static std::vector<int> solution;

    std::vector<domaine_end> domaines_ends;

    std::vector<Arbre_dom*> fils; // vide si une feuille
    Arbre_dom* parent; // pointeur nullptr si racine

    bool var_satisfait_contraintes(int const var) const;
    bool contrainte_satisfiable(int const c) const;
    bool contrainte_satisfiable(Contrainte const* const contrainte) const;
    bool contrainte_satisfiable(Contrainte const* const contrainte, int const val1) const;
    bool contraintes_satisfiables() const;

    static int bt_var_smallest_dom(std::vector<domaine_end> const& domaines_ends);
    static int bt_var_largest_dom(std::vector<domaine_end> const& domaines_ends);
    static int bt_var_random(std::vector<domaine_end> const& domaines_ends);

    static void bt_val_smallest(domaine & val_dom, domaine_end dom_end);
    static void bt_val_largest(domaine & val_dom, domaine_end dom_end);
    static void bt_val_random(domaine & val_dom, domaine_end dom_end);

    bool backtrack_loop(int heuristique_var(std::vector<domaine_end> const&), void heuristique_val(domaine &, domaine_end),
                        look_ahead lookahead);
    bool backtrack(int heuristique_var(std::vector<domaine_end> const&), void heuristique_val(domaine &, domaine_end),
                   look_ahead lookahead);
    bool backtrack(int heuristique_var(std::vector<domaine_end> const&), void heuristique_val(domaine &, domaine_end),
                   int var_instanciee, look_ahead lookahead);

public:
    Arbre_dom() : parent(nullptr) {nb_instanciee = 0;};
    Arbre_dom(std::vector<domaine>& init_domaines, std::vector<Contrainte>& init_contraintes, std::vector<std::vector<int>>& init_contrainte_var, std::map<std::pair<int,int>,int>& init_contrainte_comm);
    Arbre_dom(Arbre_dom* init_parent, std::vector<domaine_end>& init_domaines_ends);
    ~Arbre_dom();

    std::vector<domaine> const& get_domaines() const;
    std::vector<int> const& get_instanciation() const;
    std::vector<int> const& get_solution() const;

    void ajout_fils(Arbre_dom& fil);
    void ajout_fils(std::vector<domaine_end>& init_domaines_ends);
    void retrait_dernier_fils();
    Arbre_dom* get_dernier_fils() const;

    int get_nb_fils() const;

    bool backtrack(bt_heuristic_var var_heuristic, bt_heuristic_val val_heuristic, look_ahead lookahead);

    bool arc_consistence(int var);
    bool forward_checking(int const var_instanciee);

};

#endif // ARBRE_H
