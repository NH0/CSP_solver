#include <iostream>
#include <cctype>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"
#include "build_in_problems.h"

using namespace std;

int main(){

    vector<vector<int>> weights;
    vector<int> values;
    vector<int> Wmax;

    knapsack_problems(8,values,weights,Wmax);

    vector<int> sol = solve_max_knapsack(values,weights,Wmax);

    for (int s=0 ;s < sol.size();s++){
        if (s == sol.size() -1){
            cout << "Solution : " << value_criteria(sol[s],values);
        }
        else{
            cout << sol[s] << endl;
        }

    }
    cout << endl;

//    cout << "Min number of colorations : " << solve_mincol("../thib.col", 10) + 1 << endl;
//    cout << "Min number of colorations : " << solve_mincol("../fpsol2.i.2.col", 40) << endl;
//
//    Reine r = Reine(8);
//    r.solve(bt_heuristic_var::smallest_domain, bt_heuristic_val::smallest, true, look_ahead::maintain_arc_consistency);
//    r.display_tree_size();
//    r.display_solution();


    return 0;
}
