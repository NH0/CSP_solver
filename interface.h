#pragma once
#include "arbre_dom.h"
#include "coloration.h"
#include "build_in_problems.h"

// Fonctions de l'interface

bool reponse_oui_non(const string s);

bool est_entier(const string s);
bool est_reel(const string s);

int reponse_entier(const string s);
int reponse_entier_relatif(const string s);
double reponse_reel(const string s);

vector<int> obtenir_domaine();
vector<vector<int>> obtenir_domaine(int n);

bt_heuristic_val get_heuristic_val();
bt_heuristic_var get_heuristic_var();
look_ahead get_look_ahead();

void interface();
