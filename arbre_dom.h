#ifndef ARBRE_H
#define ARBRE_H
#include <vector>

class Arbre_dom {
    typedef std::vector<int> domaine;
private:
    std::vector<domaine> domaines;
    std::vector<Arbre_dom*> fils; // vide si une feuille
    Arbre_dom* parent; // pointeur 0 si racine
public:
    Arbre_dom() : parent(0) {domaines = {}; fils = {};};
    Arbre_dom(std::vector<domaine>& init_domaines) : parent(0) {domaines = init_domaines; fils = {};}
    Arbre_dom(Arbre_dom* init_parent, std::vector<domaine>& init_domaines);
    ~Arbre_dom();

    const std::vector<domaine>& get_domaines() const;

    void ajout_fils(Arbre_dom& fil);
    void ajout_fils(std::vector<domaine> init_domaines);
    void retrait_dernier_fils();
    Arbre_dom* get_dernier_fils() const;

    int get_nb_fils() const;

};

#endif // ARBRE_H
