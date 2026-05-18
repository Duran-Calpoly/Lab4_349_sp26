#include "algorithms.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

struct NamedFormula {
    string name;
    Formula3CNF formula;
};

long long power_of_two(int exponent) {
    if (exponent >= 62) {
        return -1;
    }

    return 1LL << exponent;
}

void print_measurement_header() {
    cout << left
         << setw(18) << "Formula"
         << right
         << setw(8) << "vars"
         << setw(10) << "clauses"
         << setw(14) << "assignments"
         << setw(12) << "vertices"
         << setw(14) << "tri edges"
         << setw(16) << "contra edges"
         << setw(12) << "edges"
         << setw(10) << "target"
         << setw(10) << "SAT?"
         << setw(10) << "IS?"
         << endl;

    cout << string(134, '-') << endl;
}

void measure_formula(const NamedFormula& named) {
    const Formula3CNF& formula = named.formula;

    long long assignments_checked = 0;
    map<string, bool> satisfying_assignment;

    bool sat_answer = brute_force_3sat(
        formula,
        assignments_checked,
        satisfying_assignment
    );

    ReductionResult reduction = reduce_3sat_to_independent_set(formula);

    long long independent_set_candidates = 0;
    vector<int> witness;

    bool is_answer = brute_force_independent_set(
        reduction.graph,
        reduction.target_independent_set_size,
        independent_set_candidates,
        witness
    );

    long long possible_assignments = power_of_two(reduction.num_variables);

    cout << left
         << setw(18) << named.name
         << right
         << setw(8) << reduction.num_variables
         << setw(10) << reduction.num_clauses;

    if (possible_assignments >= 0) {
        cout << setw(14) << possible_assignments;
    } else {
        cout << setw(14) << "too large";
    }

    cout << setw(12) << reduction.num_vertices
         << setw(14) << reduction.triangle_edges
         << setw(16) << reduction.contradiction_edges
         << setw(12) << reduction.total_edges
         << setw(10) << reduction.target_independent_set_size
         << setw(10) << (sat_answer ? "yes" : "no")
         << setw(10) << (is_answer ? "yes" : "no")
         << endl;
}

void show_formula_evaluation_example() {
    cout << "\nExample 1: Evaluating a 3CNF Formula" << endl;
    cout << string(70, '-') << endl;

    Formula3CNF formula = make_formula_two_clauses();

    cout << "Formula:" << endl;
    cout << formula.to_string() << endl;

    map<string, bool> assignment;
    assignment["x"] = true;
    assignment["y"] = false;
    assignment["w"] = true;
    assignment["z"] = false;

    cout << "\nAssignment:" << endl;
    print_assignment(assignment);
    cout << endl;

    cout << "\nDoes the assignment satisfy the formula? "
         << (evaluate_formula(formula, assignment) ? "yes" : "no")
         << endl;
}

void show_reduction_example() {
    cout << "\nExample 2: 3SAT -> Independent Set Construction" << endl;
    cout << string(70, '-') << endl;

    Formula3CNF formula = make_formula_two_clauses();

    cout << "Formula:" << endl;
    cout << formula.to_string() << endl;

    ReductionResult result = reduce_3sat_to_independent_set(formula);

    cout << "\nReduction summary:" << endl;
    print_reduction_summary(result);

    cout << "\nVertices:" << endl;
    for (int i = 0; i < result.graph.n; i++) {
        cout << setw(4) << i << " : " << result.graph.labels[i] << endl;
    }

    cout << "\nEdges:" << endl;
    for (const auto& edge : result.graph.edges) {
        cout << "  (" << edge.first << ", " << edge.second << ")  "
             << result.graph.labels[edge.first]
             << " -- "
             << result.graph.labels[edge.second]
             << endl;
    }

    cout << "\nWhat to notice:" << endl;
    cout << "- Each clause creates one triangle." << endl;
    cout << "- A triangle prevents choosing two literals from the same clause." << endl;
    cout << "- Contradiction edges prevent choosing both x and not x." << endl;
    cout << "- The target independent set size equals the number of clauses." << endl;
}

void show_satisfying_assignment_to_independent_set_example() {
    cout << "\nExample 3: Satisfying Assignment and Independent Set" << endl;
    cout << string(70, '-') << endl;

    Formula3CNF formula = make_formula_four_clauses();

    cout << "Formula:" << endl;
    cout << formula.to_string() << endl;

    long long assignments_checked = 0;
    map<string, bool> satisfying_assignment;

    bool sat_answer = brute_force_3sat(
        formula,
        assignments_checked,
        satisfying_assignment
    );

    cout << "\nSAT solver result: " << (sat_answer ? "satisfiable" : "unsatisfiable") << endl;
    cout << "Assignments checked before stopping: " << assignments_checked << endl;

    if (sat_answer) {
        cout << "One satisfying assignment:" << endl;
        print_assignment(satisfying_assignment);
        cout << endl;
    }

    ReductionResult result = reduce_3sat_to_independent_set(formula);

    long long candidates_checked = 0;
    vector<int> witness;

    bool is_answer = brute_force_independent_set(
        result.graph,
        result.target_independent_set_size,
        candidates_checked,
        witness
    );

    cout << "\nIndependent Set solver result: "
         << (is_answer ? "yes" : "no")
         << endl;

    cout << "Independent-set candidates checked before stopping: "
         << candidates_checked
         << endl;

    if (is_answer) {
        cout << "One independent set of size "
             << result.target_independent_set_size
             << ":"
             << endl;

        print_vertex_set(witness, result.graph);
        cout << endl;
    }

    cout << "\nThe SAT answer and Independent Set answer should match." << endl;
}

void show_growth_table() {
    cout << "\nGrowth Table: Formula Size vs. Graph Size" << endl;
    cout << string(70, '-') << endl;

    cout << left
         << setw(10) << "clauses"
         << right
         << setw(12) << "variables"
         << setw(12) << "vertices"
         << setw(14) << "tri edges"
         << setw(16) << "contra edges"
         << setw(12) << "edges"
         << setw(12) << "target"
         << endl;

    cout << string(88, '-') << endl;

    vector<int> clause_counts = {1, 2, 4, 8, 12, 16, 24};

    for (int m : clause_counts) {
        Formula3CNF formula = make_formula_chain(m);
        ReductionResult result = reduce_3sat_to_independent_set(formula);

        cout << left
             << setw(10) << m
             << right
             << setw(12) << result.num_variables
             << setw(12) << result.num_vertices
             << setw(14) << result.triangle_edges
             << setw(16) << result.contradiction_edges
             << setw(12) << result.total_edges
             << setw(12) << result.target_independent_set_size
             << endl;
    }

    cout << "\nNotice that the number of vertices is exactly 3 times the number of clauses." << endl;
    cout << "The construction is polynomial in the size of the formula." << endl;
}

int main() {
    cout << "Lab 4 Day 3: SAT, 3SAT, and 3SAT -> Independent Set" << endl;
    cout << string(78, '=') << endl;

    vector<NamedFormula> formulas = {
        {"sat small", make_formula_satisfiable_small()},
        {"unsat small", make_formula_unsatisfiable_small()},
        {"two clauses", make_formula_two_clauses()},
        {"four clauses", make_formula_four_clauses()},
        {"chain 6", make_formula_chain(6)}
    };

    cout << "\nReduction Measurements" << endl;
    cout << string(78, '-') << endl;

    print_measurement_header();

    for (const NamedFormula& named : formulas) {
        measure_formula(named);
    }

    show_formula_evaluation_example();
    show_reduction_example();
    show_satisfying_assignment_to_independent_set_example();
    show_growth_table();

    cout << "\nWhat to notice:" << endl;
    cout << "1. SAT asks whether some truth assignment satisfies all clauses." << endl;
    cout << "2. A 3SAT formula with m clauses becomes a graph with 3m vertices." << endl;
    cout << "3. Each clause becomes a triangle, forcing at most one chosen literal per clause." << endl;
    cout << "4. Contradiction edges prevent choosing both x and not x." << endl;
    cout << "5. The target independent set size is the number of clauses." << endl;
    cout << "6. The original formula is satisfiable iff the graph has an independent set of that size." << endl;

    return 0;
}