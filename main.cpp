#include <iostream>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"

using namespace std;

void test_reine(int nb_reines, bool ac){
    Reine r = Reine(nb_reines);
    if (ac){
        r.arc_consistance();
    }
    cout << "Nombre de domaines :" << r.domaines.size() << endl;
    for (int i = 0; i< r.domaines.size(); i++){
        cout << "Domaine "<< i << " : " ;
        for (int j =0; j<r.domaines[i].size(); j++){
            cout << r.domaines[i][j] <<',';
        }
        cout << endl;
    }
    if (not ac){
        for (int c= 0; c<r.contraintes.size(); c++){
            cout << c << " eme contrainte : "   ;     cout << "var" << r.contraintes[c].var1 << "*" << r.contraintes[c].coef1 << r.contraintes[c].ope.op;
            cout << "var" << r.contraintes[c].var2 << "*" << r.contraintes[c].coef2 << r.contraintes[c].comp.comp;
            cout <<  r.contraintes[c].valeur << endl;
        }
    }
}

void test_arc_consistence_voitures(){
    // exemple du cours
    vector<string> pieces = {"enjoliveurs","caisse","capote","pare-chocs"};
    vector<string> couleurs = {"bleu","jaune","rouge"};
    CSP csp;
    csp.nb_var = pieces.size();
    // on écrit que les comaines sont tous bleu, jaune, rouge

    for (int i=0;i<csp.nb_var;i++){
        csp.domaines.push_back({0,1,2});
        csp.contraintes_par_var.push_back({});
    }

    // on écrit que pour les 3 parties caisse, capote et pare-chocs, les couleurs doivent être identiques
    csp.contraintes.push_back(Contrainte(1,1,"-",1,2,"=",0));
    csp.contraintes_par_var[1].push_back(0);
    csp.contraintes_par_var[2].push_back(0);
    csp.contraintes.push_back(Contrainte(1,2,"-",1,3,"=",0));
    csp.contraintes_par_var[2].push_back(1);
    csp.contraintes_par_var[3].push_back(1);
    csp.contraintes.push_back(Contrainte(1,3,"-",1,1,"=",0));
    csp.contraintes_par_var[3].push_back(2);
    csp.contraintes_par_var[1].push_back(2);
    // on écrit que pour l'enjoliveur et la caisse doivent être de même couleur
    csp.contraintes.push_back(Contrainte(1,0,"-",1,1,"=",0));
    csp.contraintes_par_var[0].push_back(3);
    csp.contraintes_par_var[1].push_back(3);
    // ... mais que si l'un des 2 est bleu (couleur correspondant à 0) alors aucune couleur de l'autre ne correspond
    csp.contraintes.push_back(Contrainte(1,0,"*",1,1,">",0));
    csp.contraintes_par_var[0].push_back(4);
    csp.contraintes_par_var[1].push_back(4);

    csp.arc_consistance();

    for (int i=0;i<pieces.size();i++){
        cout << pieces[i] << " :";
        for (int j=0;j<csp.domaines[i].size();j++){
            cout << " " << couleurs[csp.domaines[i][j]] << ",";
        }
        cout <<endl;
    }
}

int main(){
    test_arc_consistence_voitures();

//    Coloration col = Coloration("./fpsol2.i.2.col", 300);
//    vector<int> valeurs2 = col.solve();
//    for (auto i : valeurs2) {
//        cout << i << endl;
//    }

//    cout << "Nombre de domaines :" << col.domaines.size() << endl;
//    for (int c= 0; c<col.contraintes.size(); c++){
//        cout << c << " eme contrainte : "   ;     cout << "var" << col.contraintes[c].var1 << "*" << col.contraintes[c].coef1 << col.contraintes[c].ope.op;
//        cout << "var" << col.contraintes[c].var2 << "*" << col.contraintes[c].coef2 << col.contraintes[c].comp.comp;
//        cout <<  col.contraintes[c].valeur << endl;
//    }

//    vector<vector<int>> domaines = {{1,2,30},{4,5,6}};
//    Arbre_dom arbre = Arbre_dom(domaines);
//    arbre.ajout_fils({{8,9},{10}});
//    arbre.ajout_fils({{11,12},{13}});
//    Arbre_dom fils = *arbre.get_dernier_fils();
//    cout << "Dom : " << fils.get_domaines()[0][0] << endl;
//    fils.ajout_fils({{50,51},{52,53,54}});
//    Arbre_dom fils2 = *fils.get_dernier_fils();
//    cout << "Dom : " << fils2.get_domaines()[1][1];
//    arbre.retrait_dernier_fils();
//    arbre.retrait_dernier_fils();
//    cout << arbre.get_nb_fils()<< endl;

    return 0;
}
