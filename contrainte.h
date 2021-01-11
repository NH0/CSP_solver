#pragma once
#include <bits/stdc++.h>
#include <utility>
#include <vector>

struct Operation{
    std::string op; // "+","-","/","*"
    Operation(){op = '+';} // constructeur par défaut
    Operation(const std::string& o){op = o;}
    double apply(double a, double b) const;
};

struct Comparaison{
    std::string comp; // "<=",">=","<",">","=","!=","=|"
    Comparaison(){comp = '<';} // constructeur par défaut
    Comparaison(const std::string& c){comp = c;}
    bool apply(double a, double b) const;
};

struct Contrainte{
    int var1, var2;
    std::set<std::pair<int,int>> c;
    Contrainte(); // constructeur par défaut
    Contrainte(const int v1,const int v2){var1 = v1; var2 = v2; c={};}
    Contrainte(const int v1, const std::vector<int> dom1, const int v2, const std::vector<int> dom2);

    void ajoute_relation(const int value1, const int value2);
    void supprime_relation(const int value1, const int value2);
    void ajoute_relations(double coef1, const std::vector<int> dom1, std::string ope, double coef2, const std::vector<int> dom2, std::string compa, double valeur);
    void supprime_relations(double coef1, const std::vector<int> dom1, std::string ope, double coef2, const std::vector<int> dom2, std::string compa, double valeur);

    bool satisfaite(const int v1, const int v2) const;
};
