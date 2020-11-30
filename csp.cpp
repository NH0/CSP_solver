#include "csp.h"

double operation::apply(double a, double b){
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
}

bool comparaison::apply(double a, double b){
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
}

contrainte::contrainte(double c1, int i1, string o, double c2, int i2, string c, double v){
    coef1 = c1;
    var1 = i1;
    coef2 = c2;
    var2 = i2;
    ope = operation(o);
    comp = comparaison(c);
    valeur = v;
}

bool contrainte::satisfaite(int v1, int v2){
    return(comp.apply(ope.apply(v1*coef1,v2*coef2),valeur));
}

reine::reine(int n){
    nb_var = n;
    for (int i = 0; i<n; i++){
        vector<int> dom;
        for (int j = 0; j<n ;j++){
            dom.push_back(j);
            if (i<j){
                contrainte c = contrainte(1,i,"-",1,j,"!=",0);
                contraintes.push_back(c);
            }
        }
        domaines.push_back(dom);
    }
}
