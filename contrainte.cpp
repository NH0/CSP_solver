#include <stdexcept>
#include "contrainte.h"

using namespace std;

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
