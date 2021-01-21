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
//    Coloration* col = new Coloration("../col/gen_barbasi.col");
//    Coloration* col = new Coloration("../col/gen_wheel.col"); cout << "Wheel : " << endl;
//    Coloration* col = new Coloration("../col/gen_star.col"); cout << "Star : " << endl;
//    Coloration* col = new Coloration("../col/gen_small_world.col"); cout << "Small world : " << endl;
//    Coloration* col = new Coloration("../col/gen_random_tree.col"); cout << "Random tree : " << endl;
//    Coloration* col = new Coloration("../col/gen_ladder.col"); cout << "Ladder : " << endl;
//    Coloration* col = new Coloration("../col/gen_karate_club.col"); cout << "Karate club : " << endl;
//    Coloration* col = new Coloration("../col/gen_internet.col"); cout << "Internet : " << endl;
//    Coloration* col = new Coloration("../col/gen_erdos.col"); cout << "Erdos : " << endl;
//    Coloration* col = new Coloration("../col/gen_cycle.col"); cout << "Cycle : " << endl;
//    Coloration* col = new Coloration("../col/gen_complete.col"); cout << "Complete : " << endl;
//    Coloration* col = new Coloration("../col/gen_circular_ladder.col"); cout << "Circular ladder : " << endl;
//    Coloration* col = new Coloration("../col/gen_binomial_10.col"); cout << "Binomial tree : " << endl;
//    Coloration* col = new Coloration("../col/gen_binomial.col"); cout << "Binomial tree 100 : " << endl;
    Coloration* col = new Coloration("../col/gen_line_graph.col"); cout << "Line graph : " << endl;
    col->solve(bt_heuristic_var::linked_to_previous_var, bt_heuristic_val::smallest, true, look_ahead::forward_checking);
    col->display_tree_size();
    cout << "Value : " << col->get_maxcol_val() << endl;
    delete col;
    return 0;
}
