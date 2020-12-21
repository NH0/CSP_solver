#include <iostream>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"

using namespace std;

int main()
{
    Reine r5 = Reine(5);
    cout << "Nombre de domaines :" << r5.domaines.size() << endl;
    for (int i = 0; i< r5.domaines.size(); i++){
        cout << "Domaine "<< i << " : " ;
        for (int j =0; j<r5.domaines[i].size(); j++){
            cout << r5.domaines[i][j] <<',';
        }
        cout << endl;
    }
    for (int c= 0; c<r5.contraintes.size(); c++){
        cout << c << " eme contrainte : "   ;     cout << "var" << r5.contraintes[c].var1 << "*" << r5.contraintes[c].coef1 << r5.contraintes[c].ope.op;
        cout << "var" << r5.contraintes[c].var2 << "*" << r5.contraintes[c].coef2 << r5.contraintes[c].comp.comp;
        cout <<  r5.contraintes[c].valeur << endl;
    }

//    Coloration col = Coloration("../huck.col");
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
