#include "arbre_dom.h"

typedef std::vector<int> domaine;

Arbre_dom::Arbre_dom(Arbre_dom* init_parent, std::vector<domaine>& init_domaines) {
    parent = init_parent;
    domaines = init_domaines;
}

Arbre_dom::~Arbre_dom() {
    if (fils.size() > 0) {
        for (auto& fil : fils) {
            delete fil;
        }
    }
}

const std::vector<domaine>& Arbre_dom::get_domaines() const {
    return domaines;
}

void Arbre_dom::ajout_fils(Arbre_dom& fil) {
    fils.push_back(&fil);
}

void Arbre_dom::ajout_fils(std::vector<domaine> init_domaines) {
    Arbre_dom* fil = new Arbre_dom(this, init_domaines);
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
