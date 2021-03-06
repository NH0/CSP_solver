#include "coloration.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

bool find_max_seconds(pair<int,int> const a, pair<int,int> const b) {
    return a.second < b.second;
}

Coloration::Coloration(string const filename) {
//    contraintes.clear();
    fstream file;
    try {
        file.open(filename.c_str(), ios_base::in);
        if(file.is_open()) {
            string line;
            map<int, int> degrees;
            while(getline(file, line)) {
                istringstream stream_s(line);
                if(line.compare(0,1,"e") == 0) {
                    string temp;
                    int e1, e2;
                    stream_s >> temp;
                    stream_s >> e1 >> e2;
                    e1--, e2--; // Nodes in .col start at 1, we count var from 0
                    degrees[e1]++;
                    degrees[e2]++;
                }
            }
            k = max_element(degrees.begin(), degrees.end(), find_max_seconds)->second + 1;
            clog << "k0 = " << k << endl;
            file.close();
            file.open(filename.c_str(), ios_base::in);
            if(file.is_open()) {
                string line;
                map<int, int> degrees;
                while(getline(file, line)) {
                    istringstream stream_s(line);
                    if (line.compare(0,1,"p") == 0) {
                        string temp;
                        stream_s >> temp >> temp;
                        stream_s >> nb_var;
                        for (int i = 0; i<nb_var; i++) {
                            contraintes_par_var.push_back({});
                        }
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
                        degrees[e1]++;
                        degrees[e2]++;

                        if (e1 > e2){ // we always want to have the contraint register as (i,j) with i<j
                            int e3 = e2;
                            e2 = e1;
                            e1 = e3;
                        }
                        Contrainte c = Contrainte(e1, domaines[e1],e2, domaines[e2]);
                        c.supprime_relations(1, domaines[e1],"-",1, domaines[e2],"!=",0);
                        contraintes.push_back(c);
                        contraintes_par_var[e1].push_back(contraintes.size() - 1);
                        contraintes_par_var[e2].push_back(contraintes.size() - 1);
                        contraintes_communes.insert(pair<pair<int,int>,int>({e1,e2},contraintes.size() - 1));
                    }
                }
                file.close();
                arbre = new Arbre_dom(domaines, contraintes, contraintes_par_var, contraintes_communes);

            }
        }
        else {
            cerr << "Could not open file" << endl;
        }
    }  catch (ios_base::failure e) {
        cerr << e.what();
    }
}

Coloration::Coloration(string const filename, int const k0) {
//    contraintes.clear();
    fstream file;
    try {
        file.open(filename.c_str(), ios_base::in);
        if(file.is_open()) {
            string line;
            map<int, int> degrees;
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
                    degrees[e1]++;
                    degrees[e2]++;

                    if (e1 > e2){ // we always want to have the contraint register as (i,j) with i<j
                        int e3 = e2;
                        e2 = e1;
                        e1 = e3;
                    }
                    Contrainte c = Contrainte(e1, domaines[e1],e2, domaines[e2]);
                    c.supprime_relations(1, domaines[e1],"-",1, domaines[e2],"!=",0);
                    contraintes.push_back(c);
                    contraintes_par_var[e1].push_back(contraintes.size() - 1);
                    contraintes_par_var[e2].push_back(contraintes.size() - 1);
                    contraintes_communes.insert(pair<pair<int,int>,int>({e1,e2},contraintes.size() - 1));
                }
            }
            file.close();
            arbre = new Arbre_dom(domaines, contraintes, contraintes_par_var, contraintes_communes);
        }
        else {
            cerr << "Could not open file" << endl;
        }
    }  catch (ios_base::failure e) {
        cerr << e.what();
    }
}

Coloration::Coloration(Coloration* col, int const k0) {
    k = k0;
    nb_var = col->nb_var;
    domaines = col->domaines;
    if (not is_sorted(domaines.begin(), domaines.end())) {
        sort(domaines.begin(), domaines.end());
    }
    for (auto& domi : domaines) {
        auto found = find_if(domi.begin(), domi.end(), [k0](int v) {return v >= k0;});
        if (found != domi.end()) {
            domi.resize(found - domi.begin());
        }
    }
    contraintes = col->contraintes;
    contraintes_par_var = col->contraintes_par_var;
    contraintes_communes = col->contraintes_communes;
    arbre = new Arbre_dom(domaines, contraintes, contraintes_par_var, contraintes_communes);
}

int Coloration::get_maxcol_val() const {
    const vector<int>& solution = arbre->get_solution();
    return *max_element(solution.begin(), solution.end());
}

int Coloration::get_k() const {
    return k;
}

int solve_mincol(string const filename,  bt_heuristic_var hvar, bt_heuristic_val hval, int const k0) {
    int k = k0;
    Coloration* col = new Coloration(filename, k);
    vector<int> sol = col->solve(hvar, hval);
    while (sol.empty()) {
        delete col;
        k += 20;
        Coloration* col = new Coloration(filename, k);
        sol = col->solve(hvar, hval);
    }
    clog << "Works with k = " << k << endl;

    while (not sol.empty()) {
        int max_val = col->get_maxcol_val();
        delete col;
        clog << "New col with k = " << max_val - 1 << endl;
        Coloration* new_col = new Coloration(filename, max_val - 1);
        try {
            sol = new_col->solve(hvar, hval);
        }  catch (runtime_error e) {
            cerr << "Error : " << e.what() << endl;
            return new_col->get_k() + 1;
        }
        col = new_col;
    }
    return col->get_k()+1;
}
