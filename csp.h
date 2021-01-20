#pragma once
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include "arbre_dom.h"
#include "contrainte.h"

using namespace std;

/*  T : Foward checking
 *  T : Economiser de l'espace en réordonnant les domaines au lieu de les dupliquer
 */

class CSP{
public :
    int nb_var;
    vector<vector <int>> domaines; // les domaines énumérés

    vector<Contrainte> contraintes;
    vector<vector<int>> contraintes_par_var; // indice des contraintes par variable
    map<pair<int,int>,int> contraintes_communes; // dictionnaire qui liste les contraintes communes à i et j (i<j)

    CSP() {} // constructeur par défaut
    ~CSP() {delete arbre;}

    vector<int> solve(bt_heuristic_var var_heuristic = bt_heuristic_var::random,
                      bt_heuristic_val val_heuristic = bt_heuristic_val::random,
                      bool enable_AC = true, look_ahead lookahead = look_ahead::forward_checking);

    void display_solution() const;
    void display_tree_size() const;

protected :
    Arbre_dom* arbre = nullptr;
};

class Reine : public CSP{
//    Reine();
public :
    Reine(int n);

    void display_solution() const;
};

vector<int> intersection(vector<int> &v1,vector<int> &v2);
