#include <iostream>
#include <cctype>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"
#include "build_in_problems.h"

using namespace std;

// Fonctions de l'interface

bool reponse_oui_non(const string s){
    string rep="rep";
    while (rep != "O" and rep!="N"){
        cout << s << endl;
        cout << "Repondre par O (pour oui) ou N (pour non)" << endl;
        cin >> rep;
    }
    return rep=="O";
}

bool est_entier(const string s){
    int debut = 0;
    if (s[debut] == '-'){
        debut = 1;
    }
    for (int i=debut; i<s.size();i++){
        if (not (isdigit(s[i]) or s[i]==' ')){
            return false;
        }
    }
    return true;
}

bool est_reel(const string s){
    int debut = 0;
    if (s[debut] == '-'){
        debut = 1;
    }
    int point = 0;
    for (int i=debut; i<s.size();i++){
        if (not (isdigit(s[i]))){
            if (s[i]=='.'){
                point += 1;
            }
            else{
               return false;
            }

        }
    }
    return point <= 1;
}

int reponse_entier(const string s){
    string rep = "rep";
    bool b = false;
    while (not b){
        cout << s << endl;
        cout << "Donnez un entier strictement positif" << endl;
        cin >> rep;
        rep.erase(remove(rep.begin(), rep.end(), ' '), rep.end());
        b = est_entier(rep);
        if (b){
            b = stoi(rep) > 0;
        }
    }
    return stoi(rep);
}

int reponse_entier_relatif(const string s){
    string rep = "rep";
    while (not est_entier(rep)){
        cout << s << endl;
        cout << "Donnez un entier relatif (mettre un - devant l'entier s'il est négatif)" << endl;
        cin >> rep;
        rep.erase(remove(rep.begin(), rep.end(), ' '), rep.end());
    }
    return stoi(rep);
}

double reponse_reel(const string s){
    string rep = "rep";
    while (not est_reel(rep)){
        cout << s << endl;
        cout << "Donnez un reel (mettre un - pour dire qu'il est négatf et un . si il n'est pas entier" << endl;
        cin >> rep;
        rep.erase(remove(rep.begin(), rep.end(), ' '), rep.end());
    }
    return stod(rep);
}

vector<int> obtenir_domaine(){ // obtenir un domaine
    vector<int> dom;
    if (reponse_oui_non("Le domaine est-il un intervalle ?")){
        int debut, fin;
        debut = reponse_entier_relatif("Quel est le premier entier de l'intervalle ?");
        fin = reponse_entier_relatif("Quel est le dernier entier de l'intervalle (il sera inclu) ?");
        for (int i = debut; i<= fin ; i++){
            dom.push_back(i);
        }
    }
    else{
        int d = reponse_entier("Quelle est la taille du domaine ?");
        for (int i=0;i<d;i++){
            dom.push_back(reponse_entier_relatif("Quelle est la "+to_string(i)+"e valeur ?"));
        }
    }
    return dom;
}

vector<vector<int>> obtenir_domaine(int n){  // si les domaines sont identiques
    vector<vector<int>> dom;
    if (reponse_oui_non("Le domaine est-il un intervalle ?")){
        int debut, fin;
        debut = reponse_entier_relatif("Quel est le premier entier de l'intervalle ?");
        fin = reponse_entier_relatif("Quel est le dernier entier de l'intervalle (il sera inclu) ?");
        for (int i = 0; i<n ; i++){
            dom.push_back({});
            for (int j = debut; j<= fin ; j++){
                dom[i].push_back(j);
            }
        }
    }
    else{
        int d = reponse_entier("Quelle est la taille du domaine ?");
        for (int i=0;i<n;i++){
            dom.push_back({});
            for (int j=0;j<d;j++){
                dom[i].push_back(reponse_entier_relatif("Quelle est la "+to_string(j)+"e valeur de la variable " + to_string(i) + " ?"));
            }
        }
    }
    return dom;
}


CSP creation_csp(){
    CSP csp=CSP();
    cout <<"Ce solveur possede quelques types de probleme predefini, sinon nous creerons le CSP 'manuellement'." << endl;
    if (reponse_oui_non("Voulez-vous résoudre un CSP d'un type predefini (coloration d'un graphe, probleme des reines) ?")){
        if (reponse_oui_non("Voulez-vous resoudre un probleme de coloration ?")){
            cout << "Entrez le nom du fichier décrivant le probleme que vous voulez résoudre (il doit etre dans le repertoire du .exe)" << endl;
            string fichier;
            cin >> fichier;
            return(Coloration(fichier, 300));
        }
        else if (reponse_oui_non("Voulez-vous resoudre le probleme des reines ?")){
            int nb_reines = reponse_entier("Combien de reines ?");
            return Reine(nb_reines);
        }
        else {
            cout << "Nous n avons plus d'autres types de probleme deja configures" << endl;
        }
    }
    cout <<"N'ayant pas trouve votre bonheur dans nos problemes predefinis, il vous donc creer le CSP." << endl;
    cout << "Nous allons commencer par la definition des domaines." << endl;
    csp.nb_var = reponse_entier("Combien votre probleme a t il de variables ?");
    if (reponse_oui_non("Les domaines sont-ils identiques ?")){
        csp.domaines = obtenir_domaine(csp.nb_var);
    }
    else{
        for (int i=0;i<csp.nb_var;i++){
            csp.domaines.push_back(obtenir_domaine());
        }
    }
    cout <<"Nous allons à présent définir les contraintes du CSP, selon deux types de contraintes : contraintes génériques ou locales" << endl;
    cout << "Une contrainte générique est de la forme coefficient1*variable1 operateur coefficient2*variable2 comparateur valeur" << endl;
    cout << "Les coefficients ainsi que la valeur sont des réels" << endl;
    cout << "L'opérateur peut etre : +, -, * ou /" << endl;
    cout << "Le comparateur peut etre :  =, !=, <=, >=, <, >, =| (ce dernier signifie que l'expression est égale à +/- la valeur)" << endl;
    cout << "Une contrainte (que nous appelerons locale) fait le lien entre une valeur vi d'une variable i et une valeur vj d'une variable j" << endl;
    cout << "Elle permet d'ajouter vi dans les supports de vj et réciproquement." << endl;
    cout << "La contrainte locale concerne donc un seul couple de valeurs quant à la contrainte générique elle s'applique à tous les couples possibles." << endl;
    for (int i=0;i<csp.nb_var;i++){ // il faut absolument que chaque variable ait son vecteur dans le vecteur de vecteur contraintes_par_var
        csp.contraintes_par_var.push_back({});
    }
    if(reponse_oui_non("Y a t il une contrainte alldiff sur les variables ?")){
        for (int i=0; i<csp.nb_var;i++){
            for (int j=i+1; j<csp.nb_var;j++){
                Contrainte c = Contrainte(i,csp.domaines[i],j,csp.domaines[j]);
                c.supprime_relations(1,csp.domaines[i],"-",1,csp.domaines[j],"!=",0);
                csp.contraintes.push_back(c);
                csp.contraintes_communes.insert(pair<pair<int,int>,int>({i,j},csp.contraintes.size() - 1));
                csp.contraintes_par_var[i].push_back(csp.contraintes.size() - 1);
                csp.contraintes_par_var[j].push_back(csp.contraintes.size() - 1);
            }
        }
    }
    cout << "Une contrainte générique est de la forme coefficient1*variable1 operateur coefficient2*variable2 comparateur valeur" << endl;
    cout << "Les coefficients ainsi que la valeur sont des réels" << endl;
    cout << "L'opérateur peut etre : +, -, * ou /" << endl;
    cout << "Le comparateur peut etre :  =, !=, <=, >=, <, >, =| (ce dernier signifie que l'expression est égale à +/- la valeur)" << endl;
    while(reponse_oui_non("Voulez-vous ajouter une nouvelle contrainte générique ?")){
        int var1 = reponse_entier("Quelle est la premiere variable ?");
        int var2 = reponse_entier("Quelle est la seconde variable ?");

        bool changement = var1>var2; // on veut toujours i<j dans les contraintes
        if (changement){
            int var3 = var2;
            var2 = var1;
            var1 = var3;
        }

        if (csp.contraintes_communes.find(pair<int,int>(var1,var2)) != csp.contraintes_communes.end()){
            cout <<"La contrainte entre les variables " + to_string(var1) + " et " + to_string(var2) + " n'existe pas encore" << endl;
            if (reponse_oui_non( "Voulez-vous l'initialiser en contrainte vide (aucune relation entre de valeurs de "+ to_string(var1) + " et " + to_string(var2) + " ne sera inscrite). Si non, elle sera initialisée pleine.")){
                csp.contraintes.push_back(Contrainte(var1,var2));
            }
            else{
                csp.contraintes.push_back(Contrainte(var1,csp.domaines[var1],var2,csp.domaines[var2]));
            }
            csp.contraintes_communes.insert(pair<pair<int,int>,int>({var1,var2},csp.contraintes.size() - 1));
            csp.contraintes_par_var[var1].push_back(csp.contraintes.size() - 1);
            csp.contraintes_par_var[var2].push_back(csp.contraintes.size() - 1);
        }


        double coef1 = reponse_reel("Quelle est la valeur du premier coefficient ?");
        string ope = "_";
        while (ope != "*" or ope != "+" or ope != "-" or ope != "/"){
            cout << "Tapez l'opérateur que vous souhaitez utiliser parmi : +, *, -, /" << endl;
            cin >> ope;
        }
        double coef2 = reponse_reel("Quelle est la valeur du second coefficient ?");
        string comp = "_";
        while (comp != "=" or comp != "!=" or comp != "<=" or comp != ">=" or comp != "<" or comp != ">" or comp != "=|"){
            cout << "Tapez le comparateur que vous souhaitez utiliser parmi : =, !=, <=, >=, <, >, =| (égal à +/- la valeur) " << endl;
            cin >> comp;
        }
        double valeur = reponse_reel("Quelle est la valeur à laquelle on compare la combinaison des variables ?");

        if (changement){
            double coef3 = coef2;
            coef2 = coef1;
            coef1 = coef3;
        }

        if (reponse_oui_non("Voulez-vous supprimez les couples ne respectant pas cette contrainte ? Si non, ce sont couples qui vérifiront cette contrainte qui seront ajoutés")){
            csp.contraintes[csp.contraintes.size() - 1].supprime_relations(coef1,csp.domaines[var1],ope,coef2,csp.domaines[var2],comp,valeur);
        }
        else{
            csp.contraintes[csp.contraintes.size() - 1].ajoute_relations(coef1,csp.domaines[var1],ope,coef2,csp.domaines[var2],comp,valeur);
        }
    }
    cout << "Une contrainte (que nous appelerons locale) fait le lien entre une valeur vi d'une variable i et une valeur vj d'une variable j" << endl;
    cout << "Elle permet d'ajouter vi dans les supports de vj et réciproquement." << endl;
    while(reponse_oui_non("Voulez vous ajouter une contrainte locale ?")){
        int var1 = reponse_entier("Quelle est la premiere variable ?");
        int var2 = reponse_entier("Quelle est la seconde variable ?");

        bool changement = var1>var2; // on veut toujours i<j dans les contraintes
        if (changement){
            int var3 = var2;
            var2 = var1;
            var1 = var3;
        }

        if (csp.contraintes_communes.find(pair<int,int>(var1,var2)) != csp.contraintes_communes.end()){
            cout <<"La contrainte entre les variables " + to_string(var1) + " et " + to_string(var2) + " n'existe pas encore" << endl;
            if (reponse_oui_non( "Voulez-vous l'initialiser en contrainte vide (aucune relation entre de valeurs de "+ to_string(var1) + " et " + to_string(var2) + " ne sera inscrite). Si non, elle sera initialisée pleine.")){
                csp.contraintes.push_back(Contrainte(var1,var2));
            }
            else{
                csp.contraintes.push_back(Contrainte(var1,csp.domaines[var1],var2,csp.domaines[var2]));
            }
            csp.contraintes_communes.insert(pair<pair<int,int>,int>({var1,var2},csp.contraintes.size() - 1));
            csp.contraintes_par_var[var1].push_back(csp.contraintes.size() - 1);
            csp.contraintes_par_var[var2].push_back(csp.contraintes.size() - 1);
        }

        int val1 = reponse_entier_relatif("Quelle est la valeur de cette premiere variable (elle doit etre dans le domaine)?");
        int val2 = reponse_entier_relatif("Quelle est la valeur de cette seconde variable (elle doit etre dans le domaine)?");

        if (changement){
            double val3 = val2;
            val2 = val1;
            val1 = val3;
        }

        if (reponse_oui_non("Voulez-vous supprimez les couples ne respectant pas cette contrainte ? Si non, ce sont couples qui vérifiront cette contrainte qui seront ajoutés")){
            csp.contraintes[csp.contraintes_communes[pair<int,int>(var1,var2)]].supprime_relation(val1,val2);
        }
        else{
            csp.contraintes[csp.contraintes_communes[pair<int,int>(var1,var2)]].ajoute_relation(val1,val2);
        }
    }
    cout << "La création du CSP est finie !" << endl;
    return csp;
}

// Fin des fontions de l'interface

int main(){
    vector<vector<int>> weights = {{31,10,20,19,4,3,6},{41,50,49,59,55,57,60}};
    vector<int> values = {70,20,39,37,7,5,10};
    vector<int> Wmax = {50,170};

    int B = calcul_glouton_knapsack(values,weights,Wmax);

    cout << B << endl;

    Knapsack k = Knapsack(values,weights,Wmax,B);

    vector<int> sol = k.solve(bt_heuristic_var::varlargest, bt_heuristic_val::valsmallest);

    for (auto s : sol){
        cout << s << ",";
    }

//    vector<int> sol = solve_knapsack(values,weights,Wmax);



//    cout << "Min number of colorations : " << solve_mincol("../thib.col", 10) + 1 << endl;
//    cout << "Min number of colorations : " << solve_mincol("../fpsol2.i.2.col", 40) << endl;
<<<<<<< Updated upstream
    Reine r = Reine(4);
    r.solve(bt_heuristic_var::varrandom, bt_heuristic_val::valrandom, true, look_ahead::forward_checking);
    r.display_tree_size();
=======
//    Reine r = Reine(50);
//    r.solve(bt_heuristic_var::varsmallest, bt_heuristic_val::valsmallest, true, look_ahead::forward_checking);
>>>>>>> Stashed changes

    return 0;
}
