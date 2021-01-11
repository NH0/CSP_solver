#include "csp.h"

std::vector<int> CSP::solve(bt_heuristic heuristic) {
    if(arbre.arc_consistence()) {
        if(arbre.backtrack(heuristic)) {
            return arbre.get_solution();
        }
    }

    return {};
}

Reine::Reine(int n){
    nb_var = n;

    for (int i = 0; i<n; i++){

        contraintes_par_var.push_back({});
    }
    for (int i = 0; i<n; i++){
        vector<int> domaine_i;
        for (int j = i+1; j<n ;j++){
            domaine_i.push_back(j);
        }
        domaines.push_back(domaine_i);
    }
    for (int i = 0; i<n; i++){
        for (int j = i+1; j<n ;j++){
            // on crée la contraintes qui lie la reine i à la reine j
            Contrainte c = Contrainte(i,domaines[i],j,domaines[j]);

            // les reines ne doivent pas être sur la même ligne
            c.supprime_relations(1,domaines[i],"-",1,domaines[j],"!=",0);

            // les reines ne doivent pas être sur la même diagonale (montante)
            c.supprime_relations(1,domaines[i],"-",1,domaines[j],"!=",i-j);

            // les reines ne doivent pas être sur la même diagonale (descendante)
            c.supprime_relations(1,domaines[i],"-",1,domaines[j],"!=",j-i);

            contraintes.push_back(c);
            // on indique que les variables i et j sont contraintes par la contraintes que l'on vient de créer
            contraintes_par_var[i].push_back(contraintes.size() - 1);
            contraintes_par_var[j].push_back(contraintes.size() - 1);

            // on indique qu'il y a une contrainte commune à i et j
            contraintes_communes.insert(pair<pair<int,int>,int>({i,j},contraintes.size() - 1));
        }
    }
    arbre = Arbre_dom(domaines, contraintes, contraintes_par_var,contraintes_communes);
}
