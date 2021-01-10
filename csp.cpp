#include "csp.h"

vector<int> intersection(vector<int> &v1,vector<int> &v2){
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    vector<int> v3;
    set_intersection(v1.begin(),v1.end(), v2.begin(),v2.end(), back_inserter(v3));
    return v3;
}

bool CSP::arc_consistance(){
    vector<vector<int>> aTester;
    for (int i=0; i<nb_var;i++){
        for (int j=i+1; j<nb_var;j++){
            if (not contraintes_communes[{i,j}].empty()){
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
                support = true;
                for (auto const &c : contraintes_communes[{min(test[0],test[1]),max(test[0],test[1])}]){
                    // savoir si x est la var1 ou non de la contrainte (ce n'est pas forcément symétrique)
                    if (contraintes[c].var1==test[0]){
                        support = contraintes[c].satisfaite(vx,vy);
                    }
                    else{
                        support = contraintes[c].satisfaite(vy,vx);
                    }
                    if (not support){
                        break;
                    }
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
                if (contraintes_communes[{min(i,test[0]),max(i,test[0])}].empty()){
                    if (min(i,test[0])==test[0] and max(i,test[0])!=test[1]){
                        aTester.push_back({max(i,test[0]),test[0]});
                    }
                    if (max(i,test[0])==test[0] and min(i,test[0])!=test[1]){
                        aTester.push_back({min(i,test[0]),test[0]});
                    }
                }
            }
        }
        // on supprime les valeurs sans support du domaine de x
        for (auto const s : aSupprimer){
            domaines[test[0]].erase(remove(domaines[test[0]].begin(), domaines[test[0]].end(), s), domaines[test[0]].end());
        }
        empty_var = domaines[test[0]].empty();
    }
    if (empty_var){
        cerr << "Probleme non realisable" << endl;
        return false;
    }
    return true;
}

std::vector<int> CSP::solve(bt_heuristic heuristic) {
    if(arc_consistance()) {
        if(arbre.backtrack(heuristic)) {
            return arbre.get_solution();
        }
    }

    return {};
}

Reine::Reine(int n){
    nb_var = n;
    for (int i = 0; i<n; i++) {
        contraintes_par_var.push_back({});
    }
    for (int i = 0; i<n; i++){
        vector<int> domaine_i;
        for (int j = i+1; j<n ;j++){
            domaine_i.push_back(j);

            // les reines ne doivent pas être sur la même ligne
            contraintes.push_back(Contrainte(1,i,"-",1,j,"!=",0));
            contraintes_par_var[i].push_back(contraintes.size() - 1);
            contraintes_par_var[j].push_back(contraintes.size() - 1);
            // les reines ne doivent pas être sur la même diagonale (montante)
            contraintes.push_back(Contrainte(1,i,"-",1,j,"!=",abs(i-j)));
            contraintes_par_var[i].push_back(contraintes.size() - 1);
            contraintes_par_var[j].push_back(contraintes.size() - 1);
            // les reines ne doivent pas être sur la même diagonale (descendante)
            contraintes.push_back(Contrainte(1,i,"-",1,j,"!=",-abs(i-j)));
            contraintes_par_var[i].push_back(contraintes.size() - 1);
            contraintes_par_var[j].push_back(contraintes.size() - 1);

            int taille = contraintes.size();
            vector<int> communes = {taille - 3, taille - 2, taille - 1};
            contraintes_communes.insert(pair<pair<int,int>,vector<int>>({i,j},communes));
        }
        domaines.push_back(domaine_i);
    }
    arbre = Arbre_dom(domaines, contraintes, contraintes_par_var);
}
