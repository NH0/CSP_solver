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
    Coloration* col = new Coloration("../col/gen_barbasi.col", 20);
    col->solve(bt_heuristic_var::linked_to_previous_var, bt_heuristic_val::smallest, true, look_ahead::forward_checking);
    col->display_tree_size();
    delete col;
    return 0;
}
