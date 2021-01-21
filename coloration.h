#ifndef COLORATION_H
#define COLORATION_H

#include "csp.h"
#include <string>

class Coloration : public CSP {
private:
    int k;
public:
    Coloration(){throw std::invalid_argument("Please provide a filename for the data");};
    Coloration(std::string const filename);
    Coloration(std::string const filename, int const k0);
    Coloration(Coloration* col, int const k);

    int get_maxcol_val() const;
    int get_k() const;
};

int solve_mincol(string const filename,
                 bt_heuristic_var hvar = bt_heuristic_var::linked_to_previous_var,
                 bt_heuristic_val hval = bt_heuristic_val::smallest,
                 int const k0 = 5);

#endif // COLORATION_H
