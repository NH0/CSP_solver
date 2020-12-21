#include "coloration.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

Coloration::Coloration(string filename) {
//    contraintes.clear();
    fstream file;
    try {
        file.open(filename.c_str(), ios_base::in);
        if(file.is_open()) {
            string line;
            while(getline(file, line)) {
                istringstream stream_s(line);
                if (line.compare(0,1,"p") == 0) {
                    string temp;
                    stream_s >> temp >> temp;
                    stream_s >> nb_var;
                }
                else if(line.compare(0,1,"e") == 0) {
                    string temp;
                    int e1, e2;
                    stream_s >> temp;
                    stream_s >> e1 >> e2;
                    e1--, e2--; // Nodes in .col start at 1, we count var from 0
                    Contrainte contrainte(1, e1, "-", 1, e2, "!=", 0);
                    contraintes.push_back(contrainte);
                }
            }
            file.close();
            for (int i=0; i<nb_var; i++) {
                vector<int> domaine;
                for (int j=0; j<nb_var; j++) {
                    domaine.push_back(j);
                }
                domaines.push_back(domaine);
            }
            arbre = Arbre_dom(domaines);
        }
        else {
            cerr << "Could not open file" << endl;
        }
    }  catch (ios_base::failure e) {
        cerr << e.what();
    }
}
