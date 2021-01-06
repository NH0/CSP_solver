#include "csp.h"
#include <stdexcept>

double Operation::apply(double a, double b) const{
    if (op == "+"){
        return a+b;
    }
    if (op == "-"){
        return a-b;
    }
    if (op == "*"){
        return a*b;
    }
    if (op == "/"){
        return a/b;
    }
    else {
        throw std::invalid_argument(op);
    }
}

bool Comparaison::apply(double a, double b) const{
    if (comp == "="){
        return a==b;
    }
    if (comp == "!="){
        return a != b;
    }
    if (comp == "<="){
        return a <= b;
    }
    if (comp == ">="){
        return a >= b;
    }
    if (comp == "<"){
        return a < b;
    }
    if (comp == ">"){
        return a > b;
    }
    if (comp == "=|"){
        return (a == b or a == abs (b));
    }
    else {
        throw std::invalid_argument(comp);
    }
}

Contrainte::Contrainte(double c1, int i1, string o, double c2, int i2, string c, double v){
    coef1 = c1;
    var1 = i1;
    coef2 = c2;
    var2 = i2;
    ope = Operation(o);
    comp = Comparaison(c);
    valeur = v;
}

bool Contrainte::satisfaite(const int v1, const int v2) const{
    return(comp.apply(ope.apply(v1*coef1,v2*coef2),valeur));
}

bool CSP::var_satisfait_contraintes(const int var) const {
    for (const int& j : contraintes_par_var[var]) {
        if (!contrainte_satisfiable(j)) {
            return false;
        }
    }
    return true;
}

bool CSP::contrainte_satisfiable(const Contrainte* contrainte, const int val1) const {
    /**
     * @brief Contrainte est-elle satisfiable avec var1 = val1 ?
     */
    const int* pval2 = instanciation[contrainte->var2];
    if (pval2 != nullptr) {
        return contrainte->satisfaite(val1, *pval2);
    }
    else {
        for (auto const &j : domaines[contrainte->var2]) {
            if (contrainte->satisfaite(val1, j)) {
                return true;
            }
        }
    }

    return false;
}

bool CSP::contrainte_satisfiable(const int i) const {
    /**
     * @brief Contrainte d'indice i est-elle satisfiable par instanciatin ?
     */
    const Contrainte contrainte = contraintes[i];
    // Si la variable est instanciée
    const int* pval1 = instanciation[contrainte.var1];
    if (pval1 != nullptr) {
        return contrainte_satisfiable(&contrainte, *pval1);
    }
    else {
        const int* pval2 = instanciation[contrainte.var2];
        if (pval2 != nullptr) {
            for (auto const &i : domaines[contrainte.var1]) {
                if (contrainte.satisfaite(i, *pval2)) {
                    return true;
                }
            }
        }
        else {
            for (auto const &i : domaines[contrainte.var1]) {
                for (auto const &j : domaines[contrainte.var2]) {
                    if (contrainte.satisfaite(i, j)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool CSP::contraintes_satisfiables() const {
    /***
     * Toutes les contraintes sont-elles satisfaites par instanciation ?
     */
    for (auto i = 0; i<contraintes.size(); i++) {
        if (!contrainte_satisfiable(i)) {
            return false;
        }
    }
    return true;
}

bool CSP::backtrack() {
    if (!contraintes_satisfiables()) {
        return false;
    }
    if (nb_instanciee == nb_var) {
        return true;
    }
    int i = 0;
    while (instanciation[i] != nullptr) {
        i++;
    }
    nb_instanciee++;
    for (auto j = 0; j<domaines[i].size(); j++) {
        instanciation[i] = &domaines[i][j];
        if (backtrack()) {
            return true;
        }
    }
    instanciation[i] = nullptr;
    nb_instanciee--;

    return false;
}

vector<int> intersection(vector<int> &v1,vector<int> &v2){
    vector<int> v3;
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_intersection(v1.begin(),v1.end(), v2.begin(),v2.end(), back_inserter(v3));
    return v3;
}

void CSP::arc_consistance(){
    vector<vector<int>> aTester;
    map<pair<int,int>,vector<int>> contraintes_communes; // dictionnaire qui liste les contraintes communes à i et j (i<j)
    for (int i=0; i<nb_var;i++){
        for (int j=i+1; j<nb_var;j++){
            vector<int> communes = intersection(contraintes_par_var[i],contraintes_par_var[j]);
            if (not communes.empty()){
                aTester.push_back({i,j});
                aTester.push_back({j,i});
                contraintes_communes.insert(pair<pair<int,int>,vector<int>>({i,j},communes));
            }
        }
    }
    vector<int> test; // c'est le couple de variable qu'on teste
    vector<int> aSupprimer;// c'est la liste des variables à supprimer du domaine

    bool support; // c'est le booléen qui nous aidera à determiner si une valeur de x à un support
    bool x_var1; // c'est le booléen qui nous permet de savoir si x est la var1 ou non de la contrainte (ce n'est pas forcément symétrique)

    bool empty_var;

    while (not (aTester.empty() or empty_var)){
        test = aTester[aTester.size()-1];
        aTester.pop_back();
        aSupprimer.clear();
        // avec cette première boucle, nous testons si il existe un support pour chaque valeur de la variable x
        for (auto const &vx : domaines[test[0]]){
            for (auto const &vy : domaines[test[1]]){
                support = true;
                for (auto const &c : contraintes_communes[{min(test[0],test[1]),max(test[0],test[1])}]){
                    x_var1 = contraintes[c].var1==test[0];
                    if (x_var1){
                        support = contraintes[c].satisfaite(vx,vy);
                    }
                    else{
                        support = contraintes[c].satisfaite(vy,vx);
                    }
                    if (not support){
                        break;
                    }
                }
                if (support){
                    break; // pas besoin de tester tous les vy si on a déjà un support
                }
            }
            if (not support){ // si l'on a pas de support : on doit supprimer vx
                aSupprimer.push_back(vx);
            }
        }
        // si ce n'est pas le cas, on ajoute des nouvelles contraintes à tester
        // car elles peuvent etre impactées par la suppression de valeur(s) du domaine de x
        if (not aSupprimer.empty()){
            for (int i=0;i<nb_var;i++){
                if (contraintes_communes.find({min(i,test[0]),max(i,test[0])})!=contraintes_communes.end()){
                    if (min(i,test[0])==test[0] and max(i,test[0])!=test[1]){
                        aTester.push_back({max(i,test[0]),test[0]});
                    }
                    if (max(i,test[0])==test[0] and min(i,test[0])!=test[1]){
                        aTester.push_back({min(i,test[0]),test[0]});
                    }
                }
            }
        }
        // on supprime les valeurs sans support du domaine de x
        for (auto const s : aSupprimer){
            domaines[test[0]].erase(remove(domaines[test[0]].begin(), domaines[test[0]].end(), s), domaines[test[0]].end());
        }
        empty_var = domaines[test[0]].empty();
    }
    if (empty_var){
        cout << "Probleme non realisable" << endl;
    }
}

std::vector<int> CSP::solve() {
    if(backtrack()) {
        std::vector<int> valeurs;
        for (auto i = 0; i<nb_var; i++) {
            valeurs.push_back(*instanciation[i]);
        }
        return valeurs;
    }
    return {};
}

Reine::Reine(int n){
    nb_var = n;
    for (int i = 0; i<n; i++) {
        contraintes_par_var.push_back({});
    }
    for (int i = 0; i<n; i++){
        vector<int> domaine_i;
        for (int j = 0; j<n ;j++){
            domaine_i.push_back(j);
            if (i<j){
                // les reines ne doivent pas être sur la même ligne
                contraintes.push_back(Contrainte(1,i,"-",1,j,"!=",0));
                contraintes_par_var[i].push_back(contraintes.size() - 1);
                contraintes_par_var[j].push_back(contraintes.size() - 1);
                // les reines ne doivent pas être sur la même diagonale (montante)
                contraintes.push_back(Contrainte(1,i,"-",1,j,"!=",abs(i-j)));
                contraintes_par_var[i].push_back(contraintes.size() - 1);
                contraintes_par_var[j].push_back(contraintes.size() - 1);
                // les reines ne doivent pas être sur la même diagonale (descendante)
                contraintes.push_back(Contrainte(1,i,"-",1,j,"!=",-abs(i-j)));
                contraintes_par_var[i].push_back(contraintes.size() - 1);
                contraintes_par_var[j].push_back(contraintes.size() - 1);
            }
        }
        domaines.push_back(domaine_i);
    }
    arbre = Arbre_dom(domaines);
    instanciation = std::vector<int*>(nb_var, nullptr);
}
