#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct operation{
    string op; // "+","-","/","*"
    operation(){op = '+';} // constructeur par défaut
    operation(const string& o){op = o;}
    double apply(double a, double b);
};

struct comparaison{
    string comp; // "<=",">=","<",">","=","!="
    comparaison(){comp = '<';} // constructeur par défaut
    comparaison(const string& c){comp = c;}
    bool apply(double a, double b);
};

struct contrainte{
    // coef1 * var1 ope coef2 * var2 comp valeur
    double coef1;
    double coef2;
    operation ope;
    comparaison comp;
    double valeur;
    contrainte(){coef1 = 0, coef2 = 0; ope = operation(); comp = comparaison(); valeur = 0;} // constructeur par défaut
    contrainte(double c1, string o, double c2, string c, double v); // attention on utilise des string
    bool satisfied(int a, int b); // verification de la satisfaction de la contrainte pour des valeurs particulières
};

class csp{
public :
    int nb_var;
    vector<vector <int>> domaines; // les domaines énumérés
    vector<contrainte> contraintes;
    csp(){nb_var = 0; domaines={}; contraintes = {contrainte()};} // constructeur par défaut
//    vector<int> solve();

private :
//    arbre (structure à définir)
//    backtrack
//    AC

};

