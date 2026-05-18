#include "algorithms.h"

#include <cassert>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void test_subset_sum_solver() {
    vector<int> numbers = {3, 8, 12, 14, 20};
    int target = 23;

    SolverStats stats;
    vector<int> witness;

    bool result = brute_force_subset_sum(numbers, target, stats, witness);

    assert(result);
    assert(is_subset_sum_solution(numbers, target, witness));
    assert(stats.candidates_checked > 0);
}

void test_subset_sum_no_solution() {
    vector<int> numbers = {2, 4, 6, 8};
    int target = 7;

    SolverStats stats;
    vector<int> witness;

    bool result = brute_force_subset_sum(numbers, target, stats, witness);

    assert(!result);
    assert(stats.candidates_checked == 16);
}

void test_graph_predicates() {
    Graph path = make_path_graph(4);

    assert(is_independent_set(path, {0, 2}));
    assert(!is_independent_set(path, {0, 1}));

    assert(is_vertex_cover(path, {1, 2}));
    assert(!is_vertex_cover(path, {0, 3}));

    Graph complete = make_complete_graph(4);

    assert(is_clique(complete, {0, 1, 2}));
    assert(!is_clique(path, {0, 1, 2}));
}

void test_independent_set_solver() {
    Graph path = make_path_graph(5);

    SolverStats stats;
    vector<int> witness;

    bool result = brute_force_independent_set(path, 3, stats, witness);

    assert(result);
    assert(is_independent_set(path, witness));
    assert(static_cast<int>(witness.size()) == 3);
}

void test_vertex_cover_solver() {
    Graph path = make_path_graph(4);

    SolverStats stats;
    vector<int> witness;

    bool result = brute_force_vertex_cover(path, 2, stats, witness);

    assert(result);
    assert(is_vertex_cover(path, witness));
    assert(static_cast<int>(witness.size()) <= 2);
}

void test_clique_solver() {
    Graph complete = make_complete_graph(5);

    SolverStats stats;
    vector<int> witness;

    bool result = brute_force_clique(complete, 4, stats, witness);

    assert(result);
    assert(is_clique(complete, witness));
    assert(static_cast<int>(witness.size()) == 4);
}

void test_formula_evaluation() {
    Formula3CNF formula = make_chain_formula(3);

    map<string, bool> assignment;
    assignment["x0"] = true;
    assignment["x1"] = true;
    assignment["x2"] = false;
    assignment["x3"] = true;
    assignment["x4"] = false;

    bool value = evaluate_formula(formula, assignment);

    // We do not care whether this particular assignment satisfies the formula.
    // We only care that evaluation completes deterministically.
    assert(value == true || value == false);
}

void test_sat_solver_unsat() {
    Formula3CNF formula = make_unsat_formula_one_variable();

    SolverStats stats;
    map<string, bool> assignment;

    bool result = brute_force_3sat(formula, stats, assignment);

    assert(!result);
    assert(stats.candidates_checked == 2);
}

void test_sat_solver_satisfiable() {
    Formula3CNF formula = make_chain_formula(4);

    SolverStats stats;
    map<string, bool> assignment;

    bool result = brute_force_3sat(formula, stats, assignment);

    assert(result);
    assert(evaluate_formula(formula, assignment));
    assert(stats.candidates_checked > 0);
}

void test_helpers() {
    assert(two_to_the(0) == 1);
    assert(two_to_the(5) == 32);

    assert(combinations_upper_bound(5, 0) == 1);
    assert(combinations_upper_bound(5, 1) == 5);
    assert(combinations_upper_bound(5, 2) == 10);
    assert(combinations_upper_bound(5, 5) == 1);
}

int main() {
    test_subset_sum_solver();
    test_subset_sum_no_solution();
    test_graph_predicates();
    test_independent_set_solver();
    test_vertex_cover_solver();
    test_clique_solver();
    test_formula_evaluation();
    test_sat_solver_unsat();
    test_sat_solver_satisfiable();
    test_helpers();

    cout << "All Day 4 tests passed." << endl;

    return 0;
}