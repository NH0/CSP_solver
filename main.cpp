#include <iostream>
#include <cctype>
#include <unistd.h>
#include <ios>
#include <fstream>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"
#include "build_in_problems.h"

using namespace std;

void get_ram_usage(long& ram_usage, unsigned long& swap_usage) {
    long rss;
    unsigned long vsize;
    ifstream proc_stat("/proc/self/stat", ios_base::in);
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    proc_stat >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
            >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
            >> utime >> stime >> cutime >> cstime >> priority >> nice
            >> O >> itrealvalue >> starttime >> vsize >> rss;
    ram_usage = rss * sysconf(_SC_PAGE_SIZE) / 1024;
    swap_usage = vsize / 1024.0;
}
int main(){

//    vector<vector<int>> weights;
//    vector<int> values;
//    vector<int> Wmax;

//    knapsack_problems(8,values,weights,Wmax);

//    vector<int> sol = solve_max_knapsack(values,weights,Wmax,bt_heuristic_var::random,bt_heuristic_val::largest,look_ahead::forward_checking);

//    vector<bt_heuristic_val> hval = {bt_heuristic_val::most_supported,
//                                     bt_heuristic_val::least_supported,
//                                     bt_heuristic_val::most_filtration,
//                                     bt_heuristic_val::fewest_filtration,
//                                     bt_heuristic_val::largest,
//                                     bt_heuristic_val::smallest};

//    for (auto h : hval){
//        Reine* r = new Reine(20);
//        cout << "####################### nouveau Hvar" << endl;

//        r->solve(bt_heuristic_var::random,h, false, look_ahead::maintain_arc_consistency);
//        r->display_tree_size();

//        r = new Reine(20);
//        r->solve(bt_heuristic_var::random,h, false, look_ahead::forward_checking);
//        r->display_tree_size();

//        r = new Reine(20);
//        r->solve(bt_heuristic_var::smallest_domain,h, false, look_ahead::maintain_arc_consistency);
//        r->display_tree_size();

//        r = new Reine(20);
//        r->solve(bt_heuristic_var::smallest_domain,h, false, look_ahead::forward_checking);
//        r->display_tree_size();
//    }
    Reine* r = new Reine(20);

    r->solve(bt_heuristic_var::least_constraints,bt_heuristic_val::most_filtration, false, look_ahead::forward_checking);
    r->display_tree_size();


//    r = new Reine(50);
//    r->solve(bt_heuristic_var::smallest_domain,bt_heuristic_val::random, false, look_ahead::forward_checking);
//    r->display_tree_size();

//    r = new Reine(50);
//    r->solve(bt_heuristic_var::most_constraints,bt_heuristic_val::random, false, look_ahead::forward_checking);
//    r->display_tree_size();
    return 0;
}