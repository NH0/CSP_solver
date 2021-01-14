#ifndef COLORATION_H
#define COLORATION_H

#include "csp.h"
#include <string>

class Coloration : public CSP {
private:
    int k;
public:
    Coloration(){throw std::invalid_argument("Please provide a filename for the data");};
    Coloration(std::string const filename, int const k);
    Coloration(Coloration* col, int const k);

    int get_maxcol_val() const;
    int get_k() const;
};

int solve_mincol(string const filename, int const k0);

#endif // COLORATION_H
