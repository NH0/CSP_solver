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

    int solve_mincol();
};

#endif // COLORATION_H
