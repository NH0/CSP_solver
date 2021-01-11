#include <iostream>
#include <cctype>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"

using namespace std;

// Fonctions de l'interface

bool reponse_oui_non(const string s){
    string rep="rep";
    while (rep != "O" and rep!="N"){
        cout << s << endl;
        cout << "Repondre par O (pour oui) ou N (pour non)";
        cin >> rep;
    }
    return rep=="O";
}

bool est_entier(const string s){
    int debut = 0;
    while(s[debut]==' '){
        debut +=1;
    }
    if (s[debut] == '-'){
        debut = 1;
        while (s[debut]==' '){
            debut += 1;
        }
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
    while(s[debut]==' '){
        debut +=1;
    }
    if (s[debut] == '-'){
        debut = 1;
        while (s[debut]==' '){
            debut += 1;
        }
    }
    int point = 0;
    for (int i=debut; i<s.size();i++){
        if (not (isdigit(s[i]) or s[i]==' ' )){
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
    bool b = est_entier(rep);
    while (not b){
        cout << s << endl;
        cout << "Donnez un entier strictement positif";
        cin >> rep;
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
        cout << "Donnez un entier relatif (mettre un - devant l'entier s'il est négatif)";
        cin >> rep;
    }
    return stoi(rep);
}

double reponse_reel(const string s){
    string rep = rep;
    while (not est_reel(rep)){
        cout << s << endl;
        cout << "Donnez un reel (mettre un - pour dire qu'il est négatf et un . si il n'est pas entier";
        cin >> rep;
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
}

void obtenir_domaine(vector<vector<int>> dom){  // si les domaines sont identiques
    if (reponse_oui_non("Le domaine est-il un intervalle ?")){
        int debut, fin;
        debut = reponse_entier_relatif("Quel est le premier entier de l'intervalle ?");
        fin = reponse_entier_relatif("Quel est le dernier entier de l'intervalle (il sera inclu) ?");
        for (int i = 0; i<dom.size() ; i++){
            dom.push_back({});
            for (int j = debut; j<= fin ; j++){
                dom[i].push_back(j);
            }
        }
    }
    else{
        int d = reponse_entier("Quelle est la taille du domaine ?");
        for (int i=0;i<dom.size();i++){
            dom.push_back({});
            for (int j=0;j<d;j++){
                dom[i].push_back(reponse_entier_relatif("Quelle est la "+to_string(j)+"e valeur de la variable " + to_string(i) + " ?"));
            }
        }
    }
}


CSP creation_csp(){
    CSP csp=CSP();
    cout <<"Eventuel blabla d intro" << endl;
    if (reponse_oui_non("Voulez-vous résoudre un CSP d'un type predefini (coloration d'un graphe, probleme des reines, ...) ?")){
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
    cout <<"Eventuel blabla pour dire qu'on se met à créer des trucs" << endl;
    csp.nb_var = reponse_entier("Combien votre probleme a t il de variables ?");
    if (reponse_oui_non("Les domaines sont-ils identiques ?")){
        obtenir_domaine(csp.domaines);
    }
    else{
        for (int i=0;i<csp.nb_var;i++){
            csp.domaines.push_back(obtenir_domaine());
        }
    }
    cout <<"Allez hop on passe aux contraintes" << endl;
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
    cout << "Explication contrainte générique coefficient1*variable1 operateur coefficient2*variable2 comparateur valeur" << endl;
    while(reponse_oui_non("Voulez-vous ajouter une nouvelle contrainte générique ?")){
        double coef1 = reponse_reel("Quelle est la valeur du premier coefficient ?");
        int var1 = reponse_entier("Quelle est la premiere variable ?");
        string ope = "_";
        while (ope == "*" or ope == "+" or ope == "-" or ope == "/"){
            cout << "Tapez l'opérateur que vous souhaitez utiliser parmi : +, *, -, /" << endl;
            cin >> ope;
        }
        double coef2 = reponse_reel("Quelle est la valeur du second coefficient ?");
        int var2 = reponse_entier("Quelle est la seconde variable ?");

        string comp = "_";
        while (comp == "=" or comp == "!=" or comp == "<=" or comp == ">=" or comp == "<" or comp == ">" or comp == "=|"){
            cout << "Tapez le comparateur que vous souhaitez utiliser parmi : =, !=, <=, >=, <, >, =| (égal à +/- la valeur) " << endl;
            cin >> comp;
        }
        double valeur = reponse_reel("Quelle est la valeur à laquelle on compare la combinaison des variables ?");

        if (var1>var2){
            int var3 = var2;
            double coef3 = coef2;
            var2 = var1;
            var1 = var3;
            coef2 = coef1;
            coef1 = coef3;
        }

        if (csp.contraintes_communes.find(pair<int,int>(var1,var2)) != csp.contraintes_communes.end()){
            csp.contraintes.push_back(Contrainte(var1,csp.domaines[var1],var2,csp.domaines[var2]));
            csp.contraintes_communes.insert(pair<pair<int,int>,int>({var1,var2},csp.contraintes.size() - 1));
            csp.contraintes_par_var[var1].push_back(csp.contraintes.size() - 1);
            csp.contraintes_par_var[var2].push_back(csp.contraintes.size() - 1);
        }
        csp.contraintes[csp.contraintes.size() - 1].supprime_relations(coef1,csp.domaines[var1],ope,coef2,csp.domaines[var2],comp,valeur);
    }
    cout << "Explication contrainte locale" << endl;
    while(reponse_oui_non("Voulez vous ajouter une contrainte locale ?")){
        int var1 = reponse_entier("Quelle est la premiere variable ?");
        int val1 = reponse_entier_relatif("Quelle est la valeur de cette premiere variable (elle doit etre dans le domaine)?");
        int var2 = reponse_entier("Quelle est la seconde variable ?");
        int val2 = reponse_entier_relatif("Quelle est la valeur de cette seconde variable (elle doit etre dans le domaine)?");

        if (var1>var2){
            int var3 = var2;
            double val3 = val2;
            var2 = var1;
            var1 = var3;
            val2 = val1;
            val1 = val3;
        }

        if (csp.contraintes_communes.find(pair<int,int>(var1,var2)) != csp.contraintes_communes.end()){
            Contrainte c = Contrainte(var1,var2);
            c.ajoute_relation(val1,val2);
            csp.contraintes.push_back(c);
            csp.contraintes_communes.insert(pair<pair<int,int>,int>({var1,var2},csp.contraintes.size() - 1));
            csp.contraintes_par_var[var1].push_back(csp.contraintes.size() - 1);
            csp.contraintes_par_var[var2].push_back(csp.contraintes.size() - 1);
        }
        else{
            csp.contraintes[csp.contraintes_communes[pair<int,int>(var1,var2)]].ajoute_relation(val1,val2);
        }
    }
    return csp;
}

// Fin des fontions de l'interface

int main(){
//    test_arc_consistence_voitures();

    Coloration col = Coloration("../fpsol2.i.2.col", 300);
    vector<int> valeurs2 = col.solve(bt_heuristic::varlargest);
    int count = 0;
    for (auto i : valeurs2) {
        cout << i << endl;
        count++;
    }
    cout << "nb : " << count << endl;

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
