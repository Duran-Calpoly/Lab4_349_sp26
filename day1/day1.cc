#include "algorithms.h"

#include <chrono>
#include <iomanip>
#include <iostream>
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

void run_subset_sum_example() {
    print_section_title("Subset Sum: Solver vs. Verifier");

    vector<int> numbers = {3, 8, 12, 14, 20};
    int target = 23;

    vector<int> valid_certificate = {0, 4};   // 3 + 20 = 23
    vector<int> invalid_certificate = {1, 2}; // 8 + 12 = 20

    SolverStats solver_stats;
    vector<int> witness;

    long long solver_time = time_microseconds([&]() {
        solve_subset_sum_bruteforce(numbers, target, solver_stats, witness);
    });

    VerifierStats valid_stats;
    long long valid_time = time_microseconds([&]() {
        verify_subset_sum(numbers, target, valid_certificate, valid_stats);
    });

    VerifierStats invalid_stats;
    long long invalid_time = time_microseconds([&]() {
        verify_subset_sum(numbers, target, invalid_certificate, invalid_stats);
    });

    cout << "Instance: numbers = {3, 8, 12, 14, 20}, target = 23" << endl;
    cout << "Search space size: 2^n = " << two_to_the(numbers.size()) << " subsets" << endl;

    cout << "\nBrute-force solver:" << endl;
    cout << "  found solution? " << (solver_stats.found_solution ? "yes" : "no") << endl;
    cout << "  witness indices: ";
    print_index_set(witness);
    cout << endl;
    cout << "  candidates checked: " << solver_stats.candidates_checked << endl;
    cout << "  primitive checks: " << solver_stats.primitive_checks << endl;
    cout << "  time: " << solver_time << " microseconds" << endl;

    cout << "\nVerifier on valid certificate {0, 4}:" << endl;
    cout << "  accepted? " << (valid_stats.accepted ? "yes" : "no") << endl;
    cout << "  certificate items checked: " << valid_stats.certificate_items_checked << endl;
    cout << "  primitive checks: " << valid_stats.primitive_checks << endl;
    cout << "  time: " << valid_time << " microseconds" << endl;

    cout << "\nVerifier on invalid certificate {1, 2}:" << endl;
    cout << "  accepted? " << (invalid_stats.accepted ? "yes" : "no") << endl;
    cout << "  certificate items checked: " << invalid_stats.certificate_items_checked << endl;
    cout << "  primitive checks: " << invalid_stats.primitive_checks << endl;
    cout << "  time: " << invalid_time << " microseconds" << endl;
}

void run_clique_example() {
    print_section_title("Clique: Solver vs. Verifier");

    Graph G = make_sample_graph();
    int k = 3;

    vector<int> valid_certificate = {0, 1, 2}; // A, B, C
    vector<int> invalid_certificate = {0, 2, 3}; // A, C, D

    SolverStats solver_stats;
    vector<int> witness;

    long long solver_time = time_microseconds([&]() {
        solve_clique_bruteforce(G, k, solver_stats, witness);
    });

    VerifierStats valid_stats;
    long long valid_time = time_microseconds([&]() {
        verify_clique(G, k, valid_certificate, valid_stats);
    });

    VerifierStats invalid_stats;
    long long invalid_time = time_microseconds([&]() {
        verify_clique(G, k, invalid_certificate, invalid_stats);
    });

    cout << "Instance: sample graph with " << G.n << " vertices and "
         << G.edge_count() << " edges, k = " << k << endl;

    cout << "Candidate cliques of size k: C(n,k) = "
         << choose_count(G.n, k)
         << endl;

    cout << "\nBrute-force solver:" << endl;
    cout << "  found solution? " << (solver_stats.found_solution ? "yes" : "no") << endl;
    cout << "  witness: ";
    print_vertex_set(witness, G);
    cout << endl;
    cout << "  candidates checked: " << solver_stats.candidates_checked << endl;
    cout << "  primitive checks: " << solver_stats.primitive_checks << endl;
    cout << "  time: " << solver_time << " microseconds" << endl;

    cout << "\nVerifier on valid certificate {A, B, C}:" << endl;
    cout << "  accepted? " << (valid_stats.accepted ? "yes" : "no") << endl;
    cout << "  certificate items checked: " << valid_stats.certificate_items_checked << endl;
    cout << "  primitive checks: " << valid_stats.primitive_checks << endl;
    cout << "  time: " << valid_time << " microseconds" << endl;

    cout << "\nVerifier on invalid certificate {A, C, D}:" << endl;
    cout << "  accepted? " << (invalid_stats.accepted ? "yes" : "no") << endl;
    cout << "  certificate items checked: " << invalid_stats.certificate_items_checked << endl;
    cout << "  primitive checks: " << invalid_stats.primitive_checks << endl;
    cout << "  time: " << invalid_time << " microseconds" << endl;
}

void run_vertex_cover_example() {
    print_section_title("Vertex Cover: Solver vs. Verifier");

    Graph G = make_sample_graph();
    int k = 3;

    vector<int> valid_certificate = {1, 2, 4}; // B, C, E
    vector<int> invalid_certificate = {0, 2};  // A, C misses D-E

    SolverStats solver_stats;
    vector<int> witness;

    long long solver_time = time_microseconds([&]() {
        solve_vertex_cover_bruteforce(G, k, solver_stats, witness);
    });

    VerifierStats valid_stats;
    long long valid_time = time_microseconds([&]() {
        verify_vertex_cover(G, k, valid_certificate, valid_stats);
    });

    VerifierStats invalid_stats;
    long long invalid_time = time_microseconds([&]() {
        verify_vertex_cover(G, k, invalid_certificate, invalid_stats);
    });

    cout << "Instance: sample graph with " << G.n << " vertices and "
         << G.edge_count() << " edges, k = " << k << endl;

    cout << "Candidate covers of size at most k: ";
    long long candidates = 0;
    for (int size = 0; size <= k; size++) {
        candidates += choose_count(G.n, size);
    }
    cout << candidates << endl;

    cout << "\nBrute-force solver:" << endl;
    cout << "  found solution? " << (solver_stats.found_solution ? "yes" : "no") << endl;
    cout << "  witness: ";
    print_vertex_set(witness, G);
    cout << endl;
    cout << "  candidates checked: " << solver_stats.candidates_checked << endl;
    cout << "  primitive checks: " << solver_stats.primitive_checks << endl;
    cout << "  time: " << solver_time << " microseconds" << endl;

    cout << "\nVerifier on valid certificate {B, C, E}:" << endl;
    cout << "  accepted? " << (valid_stats.accepted ? "yes" : "no") << endl;
    cout << "  certificate items checked: " << valid_stats.certificate_items_checked << endl;
    cout << "  primitive checks: " << valid_stats.primitive_checks << endl;
    cout << "  time: " << valid_time << " microseconds" << endl;

    cout << "\nVerifier on invalid certificate {A, C}:" << endl;
    cout << "  accepted? " << (invalid_stats.accepted ? "yes" : "no") << endl;
    cout << "  certificate items checked: " << invalid_stats.certificate_items_checked << endl;
    cout << "  primitive checks: " << invalid_stats.primitive_checks << endl;
    cout << "  time: " << invalid_time << " microseconds" << endl;
}

void run_subset_sum_scaling_table() {
    print_section_title("Scaling Table: Subset Sum Solver vs. Verifier");

    cout << left
         << setw(8) << "n"
         << right
         << setw(16) << "2^n subsets"
         << setw(20) << "solver checked"
         << setw(18) << "solver ops"
         << setw(18) << "verifier ops"
         << setw(16) << "solver us"
         << setw(16) << "verifier us"
         << endl;

    cout << string(112, '-') << endl;

    vector<int> sizes = {6, 10, 14, 18, 22};

    for (int n : sizes) {
        vector<int> numbers = make_subset_sum_numbers(n);

        // Deliberately impossible target so the solver must check all subsets.
        int target = 1000000;

        SolverStats solver_stats;
        vector<int> witness;

        long long solver_time = time_microseconds([&]() {
            solve_subset_sum_bruteforce(numbers, target, solver_stats, witness);
        });

        // A short invalid certificate.
        vector<int> certificate = {0, 1};

        VerifierStats verifier_stats;
        long long verifier_time = time_microseconds([&]() {
            verify_subset_sum(numbers, target, certificate, verifier_stats);
        });

        cout << left
             << setw(8) << n
             << right
             << setw(16) << two_to_the(n)
             << setw(20) << solver_stats.candidates_checked
             << setw(18) << solver_stats.primitive_checks
             << setw(18) << verifier_stats.primitive_checks
             << setw(16) << solver_time
             << setw(16) << verifier_time
             << endl;
    }

    cout << "\nNotice: the solver checks many possible subsets." << endl;
    cout << "The verifier only checks the proposed certificate." << endl;
}

void run_graph_scaling_table() {
    print_section_title("Scaling Table: Clique and Vertex Cover Verification");

    cout << left
         << setw(12) << "Problem"
         << right
         << setw(8) << "n"
         << setw(8) << "k"
         << setw(16) << "search space"
         << setw(20) << "solver checked"
         << setw(18) << "solver ops"
         << setw(18) << "verifier ops"
         << endl;

    cout << string(100, '-') << endl;

    vector<int> sizes = {6, 8, 10, 12};

    for (int n : sizes) {
        Graph empty = make_empty_graph(n);
        int clique_k = 3;
        vector<int> clique_certificate = {0, 1, 2};

        SolverStats clique_solver_stats;
        vector<int> clique_witness;

        solve_clique_bruteforce(empty, clique_k, clique_solver_stats, clique_witness);

        VerifierStats clique_verifier_stats;
        verify_clique(empty, clique_k, clique_certificate, clique_verifier_stats);

        cout << left
             << setw(12) << "Clique"
             << right
             << setw(8) << n
             << setw(8) << clique_k
             << setw(16) << choose_count(n, clique_k)
             << setw(20) << clique_solver_stats.candidates_checked
             << setw(18) << clique_solver_stats.primitive_checks
             << setw(18) << clique_verifier_stats.primitive_checks
             << endl;

        Graph cycle = make_cycle_graph(n);
        int vc_k = (n / 2) - 1;
        vector<int> vc_certificate;

        for (int i = 0; i < vc_k; i++) {
            vc_certificate.push_back(i);
        }

        long long vc_search_space = 0;
        for (int size = 0; size <= vc_k; size++) {
            vc_search_space += choose_count(n, size);
        }

        SolverStats vc_solver_stats;
        vector<int> vc_witness;

        solve_vertex_cover_bruteforce(cycle, vc_k, vc_solver_stats, vc_witness);

        VerifierStats vc_verifier_stats;
        verify_vertex_cover(cycle, vc_k, vc_certificate, vc_verifier_stats);

        cout << left
             << setw(12) << "V. Cover"
             << right
             << setw(8) << n
             << setw(8) << vc_k
             << setw(16) << vc_search_space
             << setw(20) << vc_solver_stats.candidates_checked
             << setw(18) << vc_solver_stats.primitive_checks
             << setw(18) << vc_verifier_stats.primitive_checks
             << endl;
    }

    cout << "\nNotice: graph solvers search over combinations of vertices." << endl;
    cout << "The verifier checks only the proposed vertex set." << endl;
}

void print_big_picture_summary() {
    print_section_title("What to Notice");

    cout << "1. A solver tries to find an answer from scratch." << endl;
    cout << "2. A verifier checks a proposed certificate." << endl;
    cout << "3. For Subset Sum, the certificate is a set of chosen indices." << endl;
    cout << "4. For Clique, the certificate is a set of vertices." << endl;
    cout << "5. For Vertex Cover, the certificate is also a set of vertices." << endl;
    cout << "6. The brute-force solver may examine exponentially many candidates." << endl;
    cout << "7. The verifier is much cheaper because it does not search." << endl;
    cout << "8. This is the main intuition behind NP: yes-solutions can be checked efficiently." << endl;
}

int main() {
    cout << "Lab 4 Day 1: Decision Problems and Verifiers" << endl;
    cout << string(70, '=') << endl;

    run_subset_sum_example();
    run_clique_example();
    run_vertex_cover_example();

    run_subset_sum_scaling_table();
    run_graph_scaling_table();

    print_big_picture_summary();

    return 0;
}