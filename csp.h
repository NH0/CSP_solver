#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "arbre_dom.h"

using namespace std;

struct Operation{
    string op; // "+","-","/","*"
    Operation(){op = '+';} // constructeur par défaut
    Operation(const string& o){op = o;}
    double apply(double a, double b) const;
};

struct Comparaison{
    string comp; // "<=",">=","<",">","=","!=","=|"
    Comparaison(){comp = '<';} // constructeur par défaut
    Comparaison(const string& c){comp = c;}
    bool apply(double a, double b) const;
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
    bool satisfaite(const int v1, const int v2) const;
};

class CSP{
public :
    int nb_var;
    vector<vector <int>> domaines; // les domaines énumérés

    vector<Contrainte> contraintes;
    vector<vector<int>> contraintes_par_var; // indice des contraintes par variable
    map<pair<int,int>,vector<int>> contraintes_communes; // dictionnaire qui liste les contraintes communes à i et j (i<j)

    CSP(){nb_var = 0; domaines={}; contraintes = {}; arbre = Arbre_dom(domaines, contraintes);} // constructeur par défaut

    bool arc_consistance();
    vector<int> solve();

protected :
    Arbre_dom arbre;
private :
    bool var_satisfait_contraintes(const int var) const;

};

class Reine : public CSP{
    Reine();
public :
    Reine(int n);
};

vector<int> intersection(vector<int> &v1,vector<int> &v2);
