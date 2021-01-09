#ifndef CONTRAINTE_H
#define CONTRAINTE_H
#include <string>

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
    // coef1 * var1 ope coef2 * var2 comp valeur
    double coef1;
    int var1; // indice de la variable 1
    double coef2;
    int var2; // indice de la variable 2
    Operation ope;
    Comparaison comp;
    double valeur;
    Contrainte(){coef1 = 0, coef2 = 0; var1 = 0; var2 = 0; ope = Operation(); comp = Comparaison(); valeur = 0;} // constructeur par défaut
    Contrainte(double c1, int i1, std::string o, double c2, int i2, std::string c, double v); // attention on utilise des string
    bool satisfaite(const int v1, const int v2) const;
};

#endif // CONTRAINTE_H
