#include <iostream>
#include "csp.h"
using namespace std;

int main()
{
    reine r5 = reine(5);
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

    Coloration col = Coloration("../fpsol2.i.2.col");
    cout << "Nombre de domaines :" << col.domaines.size() << endl;
    for (int c= 0; c<col.contraintes.size() / 2; c++){
        cout << c << " eme contrainte : "   ;     cout << "var" << col.contraintes[c].var1 << "*" << col.contraintes[c].coef1 << col.contraintes[c].ope.op;
        cout << "var" << col.contraintes[c].var2 << "*" << col.contraintes[c].coef2 << col.contraintes[c].comp.comp;
        cout <<  col.contraintes[c].valeur << endl;
    }
    return 0;
}
