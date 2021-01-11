#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "arbre_dom.h"
#include "contrainte.h"

using namespace std;

/*  T : Foward checking
 *  T : Faire des heuristiques pour le BT
 */

class CSP{
public :
    int nb_var;
    vector<vector <int>> domaines; // les domaines énumérés

    vector<Contrainte> contraintes;
    vector<vector<int>> contraintes_par_var; // indice des contraintes par variable
    map<pair<int,int>,vector<int>> contraintes_communes; // dictionnaire qui liste les contraintes communes à i et j (i<j)

    CSP(){nb_var = 0; domaines={}; contraintes = {}; arbre = Arbre_dom(domaines, contraintes, contraintes_par_var);} // constructeur par défaut

    bool arc_consistance();
    vector<int> solve(bt_heuristic_var var_heuristic, bt_heuristic_val val_heuristic);

protected :
    Arbre_dom arbre;
};

class Reine : public CSP{
    Reine();
public :
    Reine(int n);
};

vector<int> intersection(vector<int> &v1,vector<int> &v2);
