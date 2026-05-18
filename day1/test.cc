#include "algorithms.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

void test_subset_sum_verifier_valid() {
    vector<int> numbers = {3, 8, 12, 14, 20};
    int target = 23;
    vector<int> certificate = {0, 4}; // 3 + 20

    VerifierStats stats;
    bool result = verify_subset_sum(numbers, target, certificate, stats);

    assert(result);
    assert(stats.accepted);
    assert(stats.certificate_items_checked == 2);
}

void test_subset_sum_verifier_invalid() {
    vector<int> numbers = {3, 8, 12, 14, 20};
    int target = 23;
    vector<int> certificate = {1, 2}; // 8 + 12 = 20

    VerifierStats stats;
    bool result = verify_subset_sum(numbers, target, certificate, stats);

    assert(!result);
    assert(!stats.accepted);
}

void test_subset_sum_solver() {
    vector<int> numbers = {3, 8, 12, 14, 20};
    int target = 23;

    SolverStats stats;
    vector<int> witness;

    bool result = solve_subset_sum_bruteforce(numbers, target, stats, witness);

    assert(result);

    VerifierStats verifier_stats;
    assert(verify_subset_sum(numbers, target, witness, verifier_stats));
    assert(stats.candidates_checked > 0);
}

void test_clique_verifier_valid() {
    Graph G = make_sample_graph();

    // A, B, C form a triangle.
    vector<int> certificate = {0, 1, 2};

    VerifierStats stats;
    bool result = verify_clique(G, 3, certificate, stats);

    assert(result);
    assert(stats.accepted);
}

void test_clique_verifier_invalid() {
    Graph G = make_sample_graph();

    // A and D are not adjacent.
    vector<int> certificate = {0, 3};

    VerifierStats stats;
    bool result = verify_clique(G, 2, certificate, stats);

    assert(!result);
    assert(!stats.accepted);
}

void test_clique_solver() {
    Graph G = make_sample_graph();

    SolverStats stats;
    vector<int> witness;

    bool result = solve_clique_bruteforce(G, 3, stats, witness);

    assert(result);

    VerifierStats verifier_stats;
    assert(verify_clique(G, 3, witness, verifier_stats));
}

void test_vertex_cover_verifier_valid() {
    Graph G = make_sample_graph();

    // B, C, E covers all edges:
    // A-B, A-C, B-C, C-D, D-E.
    vector<int> certificate = {1, 2, 4};

    VerifierStats stats;
    bool result = verify_vertex_cover(G, 3, certificate, stats);

    assert(result);
    assert(stats.accepted);
}

void test_vertex_cover_verifier_invalid() {
    Graph G = make_sample_graph();

    // A, C does not cover D-E.
    vector<int> certificate = {0, 2};

    VerifierStats stats;
    bool result = verify_vertex_cover(G, 2, certificate, stats);

    assert(!result);
    assert(!stats.accepted);
}

void test_vertex_cover_solver() {
    Graph G = make_sample_graph();

    SolverStats stats;
    vector<int> witness;

    bool result = solve_vertex_cover_bruteforce(G, 3, stats, witness);

    assert(result);

    VerifierStats verifier_stats;
    assert(verify_vertex_cover(G, 3, witness, verifier_stats));
}

void test_graph_helpers() {
    Graph path = make_path_graph(5);
    assert(path.n == 5);
    assert(path.edge_count() == 4);

    Graph cycle = make_cycle_graph(5);
    assert(cycle.n == 5);
    assert(cycle.edge_count() == 5);

    Graph complete = make_complete_graph(5);
    assert(complete.n == 5);
    assert(complete.edge_count() == 10);

    Graph empty = make_empty_graph(5);
    assert(empty.n == 5);
    assert(empty.edge_count() == 0);
}

void test_count_helpers() {
    assert(two_to_the(0) == 1);
    assert(two_to_the(5) == 32);

    assert(choose_count(5, 0) == 1);
    assert(choose_count(5, 1) == 5);
    assert(choose_count(5, 2) == 10);
    assert(choose_count(5, 5) == 1);
}

int main() {
    test_subset_sum_verifier_valid();
    test_subset_sum_verifier_invalid();
    test_subset_sum_solver();

    test_clique_verifier_valid();
    test_clique_verifier_invalid();
    test_clique_solver();

    test_vertex_cover_verifier_valid();
    test_vertex_cover_verifier_invalid();
    test_vertex_cover_solver();

    test_graph_helpers();
    test_count_helpers();

    cout << "All Day 1 tests passed." << endl;

    return 0;
}