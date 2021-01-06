#ifndef COLORATION_H
#define COLORATION_H

#include "csp.h"
#include <string>

class Coloration : public CSP {
public:
    Coloration(){throw std::invalid_argument("Please provide a filename for the data");};
    Coloration(const std::string filename, const int k);
};

#endif // COLORATION_H
