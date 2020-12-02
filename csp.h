#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Operation{
    string op; // "+","-","/","*"
    Operation(){op = '+';} // constructeur par défaut
    Operation(const string& o){op = o;}
    double apply(double a, double b);
};

struct Comparaison{
    string comp; // "<=",">=","<",">","=","!="
    Comparaison(){comp = '<';} // constructeur par défaut
    Comparaison(const string& c){comp = c;}
    bool apply(double a, double b);
};

struct Contrainte{
    // coef1 * var1 ope coef2 * var2 comp valeur
    double coef1;
    int var1; // indice de la variable 1
    double coef2;
    int var2; // indice de la variable 2
    Operation ope;
    Comparaison comp;
    double valeur;
    Contrainte(){coef1 = 0, coef2 = 0; var1 = 0; var2 = 0; ope = Operation(); comp = Comparaison(); valeur = 0;} // constructeur par défaut
    Contrainte(double c1, int i1, string o, double c2, int i2, string c, double v); // attention on utilise des string
    bool satisfaite(int v1, int v2);
};

class CSP{
public :
    int nb_var;
    vector<vector <int>> domaines; // les domaines énumérés
    vector<Contrainte> contraintes;
    CSP(){nb_var = 0; domaines={}; contraintes = {};} // constructeur par défaut
//    vector<int> solve();

private :
//    arbre (structure à définir)
//    backtrack
//    AC

};

class Reine : public CSP{
    Reine();
public :
    Reine(int n);
};

