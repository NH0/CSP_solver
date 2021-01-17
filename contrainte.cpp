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
        throw invalid_argument(op);
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
        throw invalid_argument(comp);
    }
}

Contrainte::Contrainte(const int v1, const vector<int> dom1, const int v2, const vector<int> dom2){
    var1 = v1;
    var2 = v2;
    for (auto i : dom1){
        for (auto j : dom2){
            c.insert(pair<int,int>{i,j});
        }
    }
}

void Contrainte::ajoute_relation(const int value1, const int value2){
    c.insert(pair<int,int>(value1,value2)); // les sets n'ajoutent pas si ce n'est pas dedans
}

void Contrainte::supprime_relation(const int value1, const int value2){
    c.erase(pair<int,int>(value1,value2)); // pas de pbs non plus
}

void Contrainte::ajoute_relations(double coef1, const vector<int> dom1, string ope, double coef2, const vector<int> dom2, string compa, double valeur){
    for (auto i : dom1){
        for (auto j : dom2){
            if (Comparaison(compa).apply(Operation(ope).apply(coef1*i,coef2*j),valeur)){
                c.insert(pair<int,int>{i,j});
            }
        }
    }
}

void Contrainte::supprime_relations(double coef1, const vector<int> dom1, string ope, double coef2, const vector<int> dom2, string compa, double valeur){
    for (auto i : dom1){
        for (auto j : dom2){
            if (not Comparaison(compa).apply(Operation(ope).apply(coef1*i,coef2*j),valeur)){
                c.erase(pair<int,int>{i,j});
            }
        }
    }
}

bool Contrainte::satisfaite(const int v1, const int v2) const{
    return(c.find(pair<int,int>(v1,v2)) != c.end());
}
