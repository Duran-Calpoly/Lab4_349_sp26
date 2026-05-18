#include "algorithms.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

struct NamedInstance {
    string name;
    DecisionInstance instance;
};

void print_reduction_table_header() {
    cout << left
         << setw(18) << "Graph"
         << right
         << setw(6) << "n"
         << setw(8) << "m"
         << setw(8) << "k"
         << setw(10) << "VC k'"
         << setw(12) << "comp m"
         << setw(14) << "pair checks"
         << setw(14) << "edges added"
         << setw(12) << "IS yes"
         << setw(12) << "VC yes"
         << setw(12) << "CLQ yes"
         << endl;

    cout << string(126, '-') << endl;
}

void measure_instance(const NamedInstance& named) {
    const DecisionInstance& is_instance = named.instance;

    auto vc_start = high_resolution_clock::now();
    DecisionInstance vc_instance = reduce_independent_set_to_vertex_cover(is_instance);
    auto vc_end = high_resolution_clock::now();

    ReductionStats stats;
    auto clique_start = high_resolution_clock::now();
    DecisionInstance clique_instance = reduce_independent_set_to_clique(is_instance, stats);
    auto clique_end = high_resolution_clock::now();

    bool is_answer = brute_force_independent_set(is_instance.graph, is_instance.k);
    bool vc_answer = brute_force_vertex_cover(vc_instance.graph, vc_instance.k);
    bool clique_answer = brute_force_clique(clique_instance.graph, clique_instance.k);

    auto vc_time_ns = duration_cast<nanoseconds>(vc_end - vc_start).count();
    auto clique_time_ns = duration_cast<nanoseconds>(clique_end - clique_start).count();

    cout << left
         << setw(18) << named.name
         << right
         << setw(6) << is_instance.graph.n
         << setw(8) << is_instance.graph.edge_count()
         << setw(8) << is_instance.k
         << setw(10) << vc_instance.k
         << setw(12) << clique_instance.graph.edge_count()
         << setw(14) << stats.pair_checks
         << setw(14) << stats.edges_added
         << setw(12) << (is_answer ? "yes" : "no")
         << setw(12) << (vc_answer ? "yes" : "no")
         << setw(12) << (clique_answer ? "yes" : "no")
         << endl;

    // These are intentionally not in the main table because they are usually tiny.
    // They are printed only if students want to inspect timing details.
    (void) vc_time_ns;
    (void) clique_time_ns;
}

void show_detailed_example() {
    cout << "\nDetailed Example: Independent Set -> Vertex Cover" << endl;
    cout << string(60, '-') << endl;

    Graph G = make_sample_graph();
    DecisionInstance is_instance{G, 2};

    cout << "Original Independent Set instance:" << endl;
    print_instance_summary("IS", is_instance);

    DecisionInstance vc_instance = reduce_independent_set_to_vertex_cover(is_instance);

    cout << "\nReduced Vertex Cover instance:" << endl;
    print_instance_summary("VC", vc_instance);

    vector<int> independent = {0, 3}; // A, D
    vector<int> cover = set_complement(G.n, independent);

    cout << "\nExample independent set S = ";
    print_vertex_set(independent, G);
    cout << endl;

    cout << "Complement V - S = ";
    print_vertex_set(cover, G);
    cout << endl;

    cout << "Is S independent? "
         << (is_independent_set(G, independent) ? "yes" : "no")
         << endl;

    cout << "Is V - S a vertex cover? "
         << (is_vertex_cover(G, cover) ? "yes" : "no")
         << endl;
}

void show_complement_example() {
    cout << "\nDetailed Example: Independent Set -> Clique" << endl;
    cout << string(60, '-') << endl;

    Graph G = make_path_graph(4);
    G.labels = {"A", "B", "C", "D"};

    DecisionInstance is_instance{G, 2};

    ReductionStats stats;
    DecisionInstance clique_instance = reduce_independent_set_to_clique(is_instance, stats);

    cout << "Original graph G:" << endl;
    cout << "Edges in G:" << endl;
    for (const auto& edge : G.edges) {
        cout << "  (" << G.labels[edge.first] << ", " << G.labels[edge.second] << ")" << endl;
    }

    cout << "\nComplement graph complement(G):" << endl;
    cout << "Edges in complement(G):" << endl;
    for (const auto& edge : clique_instance.graph.edges) {
        cout << "  (" << clique_instance.graph.labels[edge.first]
             << ", " << clique_instance.graph.labels[edge.second] << ")" << endl;
    }

    vector<int> independent = {0, 2}; // A, C

    cout << "\nExample independent set in G: ";
    print_vertex_set(independent, G);
    cout << endl;

    cout << "Same vertices form a clique in complement(G)? "
         << (is_clique(clique_instance.graph, independent) ? "yes" : "no")
         << endl;

    cout << "\nComplement construction checked "
         << stats.pair_checks
         << " vertex pairs and added "
         << stats.edges_added
         << " complement edges."
         << endl;
}

void show_scaling_for_complement_construction() {
    cout << "\nComplement Graph Construction Scaling" << endl;
    cout << string(60, '-') << endl;

    cout << left
         << setw(8) << "n"
         << right
         << setw(14) << "pairs"
         << setw(18) << "path edges"
         << setw(18) << "comp edges"
         << endl;

    cout << string(58, '-') << endl;

    vector<int> sizes = {5, 10, 20, 40, 80, 160};

    for (int n : sizes) {
        Graph G = make_path_graph(n);

        long long pair_checks = 0;
        long long edges_added = 0;
        Graph H = complement_graph(G, pair_checks, edges_added);

        cout << left
             << setw(8) << n
             << right
             << setw(14) << pair_checks
             << setw(18) << G.edge_count()
             << setw(18) << H.edge_count()
             << endl;
    }

    cout << "\nNotice that the complement construction checks all vertex pairs." << endl;
    cout << "The number of possible pairs is n(n - 1) / 2, which is O(n^2)." << endl;
}

int main() {
    cout << "Lab 4 Day 2: Reductions" << endl;
    cout << "Independent Set -> Vertex Cover / Clique" << endl;
    cout << string(70, '=') << endl;

    vector<NamedInstance> instances = {
        {"empty n=6 k=4", {make_empty_graph(6), 4}},
        {"path n=6 k=3", {make_path_graph(6), 3}},
        {"cycle n=6 k=3", {make_cycle_graph(6), 3}},
        {"complete n=6 k=2", {make_complete_graph(6), 2}},
        {"sample k=2", {make_sample_graph(), 2}},
        {"sample k=3", {make_sample_graph(), 3}}
    };

    cout << "\nReduction Measurements" << endl;
    cout << string(70, '-') << endl;
    print_reduction_table_header();

    for (const NamedInstance& named : instances) {
        measure_instance(named);
    }

    show_detailed_example();
    show_complement_example();
    show_scaling_for_complement_construction();

    cout << "\nWhat to notice:" << endl;
    cout << "1. Independent Set -> Vertex Cover keeps the same graph." << endl;
    cout << "2. The Vertex Cover parameter changes from k to |V| - k." << endl;
    cout << "3. Independent Set -> Clique keeps k the same but builds complement(G)." << endl;
    cout << "4. The yes/no answers match across the original and reduced instances." << endl;
    cout << "5. Building complement(G) takes O(n^2) pair checks, which is polynomial time." << endl;

    return 0;
}