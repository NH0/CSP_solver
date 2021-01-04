#pragma once
#include <iostream>
#include <vector>
#include "arbre_dom.h"

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
    vector<int*> instanciation; // nullptr si non instancié, sinon pointeur vers valeur dans domaine
    int nb_instanciee;
    vector<Contrainte> contraintes;
    vector<vector<int>> contraintes_par_var; // indice des contraintes par variable
    CSP(){nb_var = 0; nb_instanciee=0; domaines={}; contraintes = {}; arbre = Arbre_dom(); instanciation={};} // constructeur par défaut

    bool backtrack();
    vector<int> solve();

protected :
    Arbre_dom arbre;
private :
    bool var_satisfait_contraintes(const int i) const;
    bool contrainte_satisfiable(const int i) const;
    bool contrainte_satisfiable(const Contrainte* contrainte, const int val1) const;
    bool contraintes_satisfiables() const;

//    AC

};

class Reine : public CSP{
    Reine();
public :
    Reine(int n);
};

