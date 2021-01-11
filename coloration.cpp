#include "coloration.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

Coloration::Coloration(const string filename, const int k0) {
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
                    for (int i = 0; i<nb_var; i++) {
                        contraintes_par_var.push_back({});
                    }
                    k = k0;
                    for (int i=0; i<nb_var; i++) {
                        vector<int> domaine;
                        for (int j=0; j<k; j++) {
                            domaine.push_back(j);
                        }
                        domaines.push_back(domaine);
                    }
                }
                else if(line.compare(0,1,"e") == 0) {
                    string temp;
                    int e1, e2;
                    stream_s >> temp;
                    stream_s >> e1 >> e2;
                    e1--, e2--; // Nodes in .col start at 1, we count var from 0

                    if (e1 > e2){ // we always want to have the contraint register as (i,j) with i<j
                        int e3 = e2;
                        e2 = e1;
                        e1 = e3;
                    }
                    Contrainte c = Contrainte(e1, domaines[e1],e2, domaines[e2]);
                    c.supprime_relations(1, domaines[e1],"-",2, domaines[e2],"!=",0);
                    contraintes.push_back(c);
                    contraintes_par_var[e1].push_back(contraintes.size() - 1);
                    contraintes_par_var[e2].push_back(contraintes.size() - 1);
                    contraintes_communes.insert(pair<pair<int,int>,int>({e1,e2},contraintes.size() - 1));
                }
            }
            file.close();
            arbre = Arbre_dom(domaines, contraintes, contraintes_par_var,contraintes_communes);
        }
        else {
            cerr << "Could not open file" << endl;
        }
    }  catch (ios_base::failure e) {
        cerr << e.what();
    }
}
