#include "algorithms.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

template <typename Func>
long long time_microseconds(Func f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();

    return duration_cast<microseconds>(end - start).count();
}

void print_section_title(const string& title) {
    cout << "\n" << title << endl;
    cout << string(title.size(), '=') << endl;
}

void run_subset_sum_scaling() {
    print_section_title("Subset Sum: Brute-Force Scaling");

    cout << left
         << setw(8) << "n"
         << right
         << setw(16) << "search space"
         << setw(20) << "candidates checked"
         << setw(18) << "primitive checks"
         << setw(14) << "time us"
         << setw(12) << "result"
         << endl;

    cout << string(88, '-') << endl;

    vector<int> sizes = {6, 10, 14, 18, 22};

    for (int n : sizes) {
        vector<int> numbers = make_subset_numbers(n);

        // The sum of odd numbers is even or odd depending on n.
        // This target is deliberately chosen to usually force a large search.
        int target = 1000000;

        SolverStats stats;
        vector<int> witness;

        long long elapsed = time_microseconds([&]() {
            brute_force_subset_sum(numbers, target, stats, witness);
        });

        cout << left
             << setw(8) << n
             << right
             << setw(16) << two_to_the(n)
             << setw(20) << stats.candidates_checked
             << setw(18) << stats.primitive_checks
             << setw(14) << elapsed
             << setw(12) << (stats.found_solution ? "yes" : "no")
             << endl;
    }

    cout << "\nWhat to notice: Subset Sum may require checking up to 2^n subsets." << endl;
}

void run_sat_scaling() {
    print_section_title("3SAT: Brute-Force Scaling");

    cout << left
         << setw(10) << "clauses"
         << right
         << setw(10) << "vars"
         << setw(16) << "assignments"
         << setw(20) << "checked"
         << setw(18) << "clause checks"
         << setw(14) << "time us"
         << setw(12) << "result"
         << endl;

    cout << string(100, '-') << endl;

    vector<int> clause_counts = {3, 5, 7, 9, 11, 13};

    for (int clauses : clause_counts) {
        Formula3CNF formula = make_chain_formula(clauses);
        int vars = static_cast<int>(formula.variables().size());

        SolverStats stats;
        map<string, bool> assignment;

        long long elapsed = time_microseconds([&]() {
            brute_force_3sat(formula, stats, assignment);
        });

        cout << left
             << setw(10) << clauses
             << right
             << setw(10) << vars
             << setw(16) << two_to_the(vars)
             << setw(20) << stats.candidates_checked
             << setw(18) << stats.primitive_checks
             << setw(14) << elapsed
             << setw(12) << (stats.found_solution ? "yes" : "no")
             << endl;
    }

    cout << "\nWhat to notice: each Boolean variable doubles the assignment space." << endl;
}

void run_independent_set_scaling() {
    print_section_title("Independent Set: Brute-Force Scaling");

    cout << left
         << setw(8) << "n"
         << right
         << setw(8) << "k"
         << setw(12) << "edges"
         << setw(18) << "C(n,k)"
         << setw(20) << "checked"
         << setw(18) << "pair checks"
         << setw(14) << "time us"
         << setw(12) << "result"
         << endl;

    cout << string(110, '-') << endl;

    vector<int> sizes = {8, 10, 12, 14, 16, 18};

    for (int n : sizes) {
        Graph G = make_complete_graph(n);

        int k = 2;

        // In a complete graph, no independent set of size 2 exists.
        // This forces the solver to check every pair.
        SolverStats stats;
        vector<int> witness;

        long long elapsed = time_microseconds([&]() {
            brute_force_independent_set(G, k, stats, witness);
        });

        cout << left
             << setw(8) << n
             << right
             << setw(8) << k
             << setw(12) << G.edge_count()
             << setw(18) << combinations_upper_bound(n, k)
             << setw(20) << stats.candidates_checked
             << setw(18) << stats.primitive_checks
             << setw(14) << elapsed
             << setw(12) << (stats.found_solution ? "yes" : "no")
             << endl;
    }

    cout << "\nWhat to notice: the solver checks combinations of vertices." << endl;
}

void run_clique_scaling() {
    print_section_title("Clique: Brute-Force Scaling");

    cout << left
         << setw(8) << "n"
         << right
         << setw(8) << "k"
         << setw(12) << "edges"
         << setw(18) << "C(n,k)"
         << setw(20) << "checked"
         << setw(18) << "pair checks"
         << setw(14) << "time us"
         << setw(12) << "result"
         << endl;

    cout << string(110, '-') << endl;

    vector<int> sizes = {8, 10, 12, 14, 16, 18};

    for (int n : sizes) {
        Graph G = make_empty_graph(n);

        int k = 2;

        // In an empty graph, no clique of size 2 exists.
        // This forces the solver to check every pair.
        SolverStats stats;
        vector<int> witness;

        long long elapsed = time_microseconds([&]() {
            brute_force_clique(G, k, stats, witness);
        });

        cout << left
             << setw(8) << n
             << right
             << setw(8) << k
             << setw(12) << G.edge_count()
             << setw(18) << combinations_upper_bound(n, k)
             << setw(20) << stats.candidates_checked
             << setw(18) << stats.primitive_checks
             << setw(14) << elapsed
             << setw(12) << (stats.found_solution ? "yes" : "no")
             << endl;
    }

    cout << "\nWhat to notice: Clique search also checks combinations of vertices." << endl;
}

void run_vertex_cover_scaling() {
    print_section_title("Vertex Cover: Brute-Force Scaling");

    cout << left
         << setw(8) << "n"
         << right
         << setw(8) << "k"
         << setw(12) << "edges"
         << setw(20) << "subsets <= k"
         << setw(20) << "checked"
         << setw(18) << "edge checks"
         << setw(14) << "time us"
         << setw(12) << "result"
         << endl;

    cout << string(112, '-') << endl;

    vector<int> sizes = {8, 10, 12, 14, 16};

    for (int n : sizes) {
        Graph G = make_cycle_graph(n);

        // A cycle on n vertices needs about n/2 vertices in a minimum cover.
        // Use k slightly too small to force many failed candidates.
        int k = (n / 2) - 1;

        long long subsets_up_to_k = 0;
        for (int size = 0; size <= k; size++) {
            subsets_up_to_k += combinations_upper_bound(n, size);
        }

        SolverStats stats;
        vector<int> witness;

        long long elapsed = time_microseconds([&]() {
            brute_force_vertex_cover(G, k, stats, witness);
        });

        cout << left
             << setw(8) << n
             << right
             << setw(8) << k
             << setw(12) << G.edge_count()
             << setw(20) << subsets_up_to_k
             << setw(20) << stats.candidates_checked
             << setw(18) << stats.primitive_checks
             << setw(14) << elapsed
             << setw(12) << (stats.found_solution ? "yes" : "no")
             << endl;
    }

    cout << "\nWhat to notice: Vertex Cover checks all subsets of size at most k." << endl;
}

void run_detailed_example() {
    print_section_title("Detailed Example: Same Theme, Different Search Spaces");

    cout << "Each solver is correct on small instances, but each explores a large search space." << endl;
    cout << endl;

    vector<int> numbers = {3, 8, 12, 14, 20};
    int target = 23;

    SolverStats subset_stats;
    vector<int> subset_witness;

    brute_force_subset_sum(numbers, target, subset_stats, subset_witness);

    cout << "Subset Sum instance:" << endl;
    cout << "numbers = {3, 8, 12, 14, 20}, target = 23" << endl;
    cout << "found solution? " << (subset_stats.found_solution ? "yes" : "no") << endl;
    cout << "witness indices = ";
    print_index_set(subset_witness);
    cout << endl;
    cout << "candidates checked = " << subset_stats.candidates_checked << endl;

    Graph G = make_path_graph(5);

    SolverStats is_stats;
    vector<int> is_witness;

    brute_force_independent_set(G, 3, is_stats, is_witness);

    cout << "\nIndependent Set instance:" << endl;
    cout << "path graph with 5 vertices, k = 3" << endl;
    cout << "found solution? " << (is_stats.found_solution ? "yes" : "no") << endl;
    cout << "witness = ";
    print_vertex_set(is_witness, G);
    cout << endl;
    cout << "candidates checked = " << is_stats.candidates_checked << endl;

    Formula3CNF formula = make_chain_formula(4);

    SolverStats sat_stats;
    map<string, bool> assignment;

    brute_force_3sat(formula, sat_stats, assignment);

    cout << "\n3SAT instance:" << endl;
    cout << formula.to_string() << endl;
    cout << "found satisfying assignment? " << (sat_stats.found_solution ? "yes" : "no") << endl;
    cout << "assignments checked = " << sat_stats.candidates_checked << endl;
}

int main() {
    cout << "Lab 4 Day 4: NP-Complete Problems and Brute-Force Scaling" << endl;
    cout << string(76, '=') << endl;

    run_subset_sum_scaling();
    run_sat_scaling();
    run_independent_set_scaling();
    run_clique_scaling();
    run_vertex_cover_scaling();
    run_detailed_example();

    cout << "\nWhat to notice:" << endl;
    cout << "1. These brute-force solvers are correct, but they scale poorly." << endl;
    cout << "2. Subset Sum and SAT naturally expose a 2^n search space." << endl;
    cout << "3. Clique, Independent Set, and Vertex Cover search over vertex subsets." << endl;
    cout << "4. NP-complete does not mean impossible; small cases are solvable." << endl;
    cout << "5. NP-complete means that efficient exact algorithms are unlikely in general." << endl;
    cout << "6. This motivates approximation algorithms, heuristics, and special-case algorithms." << endl;

    return 0;
}