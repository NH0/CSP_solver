#include <iostream>
#include <cctype>
#include "csp.h"
#include "coloration.h"
#include "arbre_dom.h"
#include "build_in_problems.h"

using namespace std;

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
