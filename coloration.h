#ifndef COLORATION_H
#define COLORATION_H

#include "csp.h"
#include <string>

class Coloration : public CSP {
public:
    Coloration(){throw std::invalid_argument("Please provide a filename for the data");};
    Coloration(std::string filename);
};

#endif // COLORATION_H
