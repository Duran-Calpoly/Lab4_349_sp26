#include "algorithms.h"

#include <cassert>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void test_literal_and_clause_evaluation() {
    map<string, bool> assignment;
    assignment["x"] = true;
    assignment["y"] = false;
    assignment["z"] = false;

    Literal x("x", false);
    Literal not_y("y", true);

    assert(evaluate_literal(x, assignment));
    assert(evaluate_literal(not_y, assignment));

    Clause clause(
        Literal("x", false),
        Literal("y", false),
        Literal("z", false)
    );

    assert(evaluate_clause(clause, assignment));
}

void test_formula_evaluation() {
    Formula3CNF formula = make_formula_satisfiable_small();

    map<string, bool> assignment;
    assignment["x"] = true;
    assignment["y"] = false;
    assignment["z"] = false;

    assert(evaluate_formula(formula, assignment));
}

void test_brute_force_3sat_satisfiable() {
    Formula3CNF formula = make_formula_satisfiable_small();

    long long assignments_checked = 0;
    map<string, bool> satisfying_assignment;

    bool result = brute_force_3sat(
        formula,
        assignments_checked,
        satisfying_assignment
    );

    assert(result);
    assert(assignments_checked >= 1);
    assert(evaluate_formula(formula, satisfying_assignment));
}

void test_brute_force_3sat_unsatisfiable() {
    Formula3CNF formula = make_formula_unsatisfiable_small();

    long long assignments_checked = 0;
    map<string, bool> satisfying_assignment;

    bool result = brute_force_3sat(
        formula,
        assignments_checked,
        satisfying_assignment
    );

    assert(!result);
    assert(assignments_checked == 2);
}

void test_3sat_to_independent_set_size() {
    Formula3CNF formula = make_formula_four_clauses();

    ReductionResult result = reduce_3sat_to_independent_set(formula);

    assert(result.num_clauses == 4);
    assert(result.num_vertices == 12);
    assert(result.target_independent_set_size == 4);
    assert(result.triangle_edges == 12);
    assert(result.total_edges >= result.triangle_edges);
}

void test_triangle_blocks_prevent_two_choices_from_same_clause() {
    Formula3CNF formula = make_formula_two_clauses();

    ReductionResult result = reduce_3sat_to_independent_set(formula);

    Graph G = result.graph;

    // First clause vertices are 0, 1, 2.
    assert(G.has_edge(0, 1));
    assert(G.has_edge(0, 2));
    assert(G.has_edge(1, 2));

    vector<int> invalid_choice = {0, 1};
    assert(!is_independent_set(G, invalid_choice));
}

void test_contradiction_edges() {
    Formula3CNF formula = make_formula_two_clauses();

    ReductionResult result = reduce_3sat_to_independent_set(formula);

    Graph G = result.graph;

    // Formula:
    // C1: x OR y OR w
    // C2: not x OR z OR not w
    //
    // Vertices:
    // 0 = C1:x
    // 2 = C1:w
    // 3 = C2:not x
    // 5 = C2:not w
    assert(G.has_edge(0, 3));
    assert(G.has_edge(2, 5));
}

void test_reduction_preserves_satisfiability_for_satisfiable_formula() {
    Formula3CNF formula = make_formula_four_clauses();

    long long assignments_checked = 0;
    map<string, bool> satisfying_assignment;

    bool sat_answer = brute_force_3sat(
        formula,
        assignments_checked,
        satisfying_assignment
    );

    ReductionResult reduction = reduce_3sat_to_independent_set(formula);

    long long candidates_checked = 0;
    vector<int> witness;

    bool is_answer = brute_force_independent_set(
        reduction.graph,
        reduction.target_independent_set_size,
        candidates_checked,
        witness
    );

    assert(sat_answer == is_answer);
}

void test_reduction_preserves_unsatisfiability_for_unsat_formula() {
    Formula3CNF formula = make_formula_unsatisfiable_small();

    long long assignments_checked = 0;
    map<string, bool> satisfying_assignment;

    bool sat_answer = brute_force_3sat(
        formula,
        assignments_checked,
        satisfying_assignment
    );

    ReductionResult reduction = reduce_3sat_to_independent_set(formula);

    long long candidates_checked = 0;
    vector<int> witness;

    bool is_answer = brute_force_independent_set(
        reduction.graph,
        reduction.target_independent_set_size,
        candidates_checked,
        witness
    );

    assert(sat_answer == is_answer);
}

int main() {
    test_literal_and_clause_evaluation();
    test_formula_evaluation();
    test_brute_force_3sat_satisfiable();
    test_brute_force_3sat_unsatisfiable();
    test_3sat_to_independent_set_size();
    test_triangle_blocks_prevent_two_choices_from_same_clause();
    test_contradiction_edges();
    test_reduction_preserves_satisfiability_for_satisfiable_formula();
    test_reduction_preserves_unsatisfiability_for_unsat_formula();

    cout << "All Day 3 tests passed." << endl;

    return 0;
}