#include "interface.h"

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

bt_heuristic_val get_heuristic_val(){
    cout << "Vous allez selectionner une heuristique de selection de valeur parmi : " ;
    cout << "- choix de la plus petite valeur" << endl;
    cout << "- choix de la plus grande valeur" << endl;
    cout << "- choix de la valeur la moins supportee" << endl;
    cout << "- choix de la valeur la plus supportee" << endl;
    cout << "- choix de la valeur la moins filtrante" << endl;
    cout << "- choix de la valeur la plus filtrante" << endl;
    cout << "- choix d'une valeur au hasard" << endl;
    cout << " choix de la plus petite valeur, choix de la plus grande valeur, choix aléatoire de la valeur";

    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la plus petite valeur ?")){
        return(bt_heuristic_val::smallest);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la plus grande valeur ?")){
        return(bt_heuristic_val::largest);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la valeur la moins supportee ?")){
        return(bt_heuristic_val::least_supported);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la valeur la plus supportee ?")){
        return(bt_heuristic_val::most_supported);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la valeur la moins filtrante ?")){
        return(bt_heuristic_val::fewest_filtration);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la valeur la plus filtrante ?")){
        return(bt_heuristic_val::most_filtration);
    }
    cout << "Ce sera la methode de selection aleatoire" << endl;
    return(bt_heuristic_val::random);
}

bt_heuristic_var get_heuristic_var(){
    cout << "Vous allez selectionner une heuristique de selection de variable parmi : " << endl;
    cout << "- choix de la / d'une des variables ayant le plus petit domaine" << endl;
    cout << "- choix de la / d'une des variables ayant le plus grand domaine" << endl;
    cout << "- choix de la / d'une des variables ayant le moins de contraintes" << endl;
    cout << "- choix de la / d'une des variables ayant le plus de contraintes" << endl;
    cout << "- choix d'une variable liée à la variable qui vient d'etre instanciee" << endl;
    cout << "- choix d'une variable au hasard" << endl;

    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la / d'une des variables ayant le plus petit domaine ?")){
        return(bt_heuristic_var::smallest_domain);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la / d'une des variables ayant le plus grand domaine ?")){
        return(bt_heuristic_var::largest_domain);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la / d'une des variables ayant le moins de contraintes ?")){
        return(bt_heuristic_var::least_constraints);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection de la / d'une des variables ayant le plus de contraintes ?")){
        return(bt_heuristic_var::most_constraints);
    }
    if (reponse_oui_non("Voulez-vous l'heuristique de selection d'une variable liée à la variable qui vient d'etre instanciee ?")){
        return(bt_heuristic_var::linked_to_previous_var);
    }
    cout << "Ce sera la methode de selection aleatoire" << endl;
    return(bt_heuristic_var::random);

}

look_ahead get_look_ahead(){
    cout << "Vous allez selectionner une methode de look ahead soit du forward-checking, soit le maintien de l'arc consistence soit rien" << endl;
    if (not reponse_oui_non("Voulez-vous utilisez une methode de look ahead ?")){
       return look_ahead::none;
    }
    else{
        if(reponse_oui_non("Voulez-vous utilisez le forward-checking ?")){
            return look_ahead::forward_checking;
        }
        else{
            cout << "Ce sera donc la methode de maintien de l'arc consistence" << endl;
            return look_ahead::maintain_arc_consistency;
        }
    }
}


void interface(){
    cout <<"Ce solveur possede quelques types de probleme predefini, sinon nous creerons le CSP 'manuellement'." << endl;
    if (reponse_oui_non("Voulez-vous résoudre un CSP d'un type predefini (coloration d'un graphe, probleme des reines) ?")){
        if (reponse_oui_non("Voulez-vous resoudre un probleme de coloration ?")){
            cout << "Entrez le nom du fichier décrivant le probleme que vous voulez résoudre (il doit etre dans le repertoire du .exe)" << endl;
            string fichier;
            cin >> fichier;
            int n = reponse_entier("Entrez une borne superieure pour le probleme de coloration.");
            int sol = solve_mincol(fichier,n);
            cout << "Le nombre chromatique du graphe est " << to_string(sol) << endl;
        }
        else if (reponse_oui_non("Voulez-vous resoudre le probleme des reines ?")){
            int nb_reines = reponse_entier("Combien de reines ?");
            Reine r = Reine(nb_reines);
            vector<int> sol = r.solve(bt_heuristic_var::largest_domain, bt_heuristic_val::largest, true, look_ahead::forward_checking);
            r.display_solution();
        }
        else if (reponse_oui_non("Voulez-vous resoudre un knapsack (multidimensionnel) ?")){
            int n = reponse_entier("Combien d'objets à mettre dans le sac ?");
            int d = reponse_entier("Combien de contraintes de capacité ?");
            vector<int> values = {};
            vector<vector<int>> weights = {};
            vector<int> Wmax={};
            for (int j = 0 ; j<d;j++){
                weights.push_back({});
                Wmax.push_back(reponse_entier("Capacité maximale pour la contrainte " + to_string(j) + " ?"));
            }
            for (int i = 0; i < n; i++){
                cout << "Coefficients liés à l'objet " << i << endl;
                values.push_back(reponse_entier("Valeur de l'objet ?"));
                for (int j = 0; j<d;j++){
                    weights[j].push_back(reponse_entier("Poids de l'objet pour la contrainte " + to_string(j) + " ?"));
                }
            }
            vector<int> sol = solve_max_knapsack(values,weights,Wmax);

            for (int s=0 ;s < sol.size();s++){
                if (s == sol.size() -1){
                    cout << "Valeur de la solution : " << value_criteria(sol[s],values);
                }
                else{
                    if (sol[s] == 1){
                        cout << "L'objet " + to_string(s) + " est pris dans le sac";
                    }
                    else{
                        "L'objet " + to_string(s) + " n'est pris pas dans le sac";
                    }
                }

            }

        }
        else {
            cout << "Nous n'avons plus d'autres types de probleme deja configures" << endl;
        }
    }
    else{
        CSP csp=CSP();
        cout <<"Mode de création 'manuel'" << endl;
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
        bt_heuristic_val hval = get_heuristic_val();
        bt_heuristic_var hvar = get_heuristic_var();
        look_ahead look = get_look_ahead();
        bool b = reponse_oui_non("Voulez-vous vérifier si votre problème est arc-consistant avant de démarrer la résolution ?");

        vector<int> sol = csp.solve(hvar,hval,b,look);
        csp.display_solution();
    }
}

// Fin des fontions de l'interface
