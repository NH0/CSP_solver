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

contrainte::contrainte(double c1, string o, double c2, string c, double v){
    coef1 = c1;
    coef2 = c2;
    ope = operation(o);
    comp = comparaison(c);
    valeur = v;
}

bool contrainte::satisfied(int a, int b){
    return comp.apply(ope.apply(a*coef1,b*coef2),valeur);
}
