#include "algorithms.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

struct NamedGraph {
    string name;
    Graph graph;
};

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

void print_measurement_header() {
    cout << left
         << setw(24) << "Graph"
         << right
         << setw(6) << "n"
         << setw(8) << "m"
         << setw(10) << "OPT"
         << setw(12) << "Approx"
         << setw(10) << "Ratio"
         << setw(12) << "|M|"
         << setw(16) << "exact us"
         << setw(16) << "approx us"
         << setw(18) << "exact cand."
         << endl;

    cout << string(132, '-') << endl;
}

void measure_graph(const NamedGraph& named) {
    const Graph& G = named.graph;

    ExactVCResult exact;
    ApproxVCResult approx;

    long long exact_time = time_microseconds([&]() {
        exact = brute_force_min_vertex_cover(G);
    });

    long long approx_time = time_microseconds([&]() {
        approx = approximate_vertex_cover_by_matching(G);
    });

    double ratio = 0.0;

    if (!exact.cover.empty()) {
        ratio = static_cast<double>(approx.cover.size()) /
                static_cast<double>(exact.cover.size());
    }

    cout << left
         << setw(24) << named.name
         << right
         << setw(6) << G.n
         << setw(8) << G.edge_count()
         << setw(10) << exact.cover.size()
         << setw(12) << approx.cover.size()
         << setw(10) << fixed << setprecision(2) << ratio
         << setw(12) << approx.matching.size()
         << setw(16) << exact_time
         << setw(16) << approx_time
         << setw(18) << exact.candidates_checked
         << endl;
}

void run_main_measurements() {
    print_section_title("Vertex Cover Approximation Measurements");

    vector<NamedGraph> graphs = {
        {"path n=8", make_path_graph(8)},
        {"cycle n=8", make_cycle_graph(8)},
        {"star leaves=8", make_star_graph(8)},
        {"sample graph", make_sample_graph()},
        {"two triangles", make_two_triangles_with_bridge()},
        {"ladder rungs=4", make_ladder_graph(4)},
        {"sparse n=10", make_sparse_deterministic_graph(10)},
        {"dense n=10", make_dense_deterministic_graph(10)}
    };

    print_measurement_header();

    for (const NamedGraph& named : graphs) {
        measure_graph(named);
    }

    cout << "\nRatio means approximate cover size / optimal cover size." << endl;
    cout << "For a 2-approximation, the ratio should be at most 2.00." << endl;
}

void run_scaling_measurements() {
    print_section_title("Runtime Scaling: Exact vs. Approximate");

    cout << left
         << setw(12) << "Graph"
         << right
         << setw(6) << "n"
         << setw(8) << "m"
         << setw(10) << "OPT"
         << setw(12) << "Approx"
         << setw(16) << "exact us"
         << setw(16) << "approx us"
         << setw(18) << "exact cand."
         << endl;

    cout << string(98, '-') << endl;

    vector<int> sizes = {6, 8, 10, 12, 14, 16};

    for (int n : sizes) {
        Graph G = make_cycle_graph(n);

        ExactVCResult exact;
        ApproxVCResult approx;

        long long exact_time = time_microseconds([&]() {
            exact = brute_force_min_vertex_cover(G);
        });

        long long approx_time = time_microseconds([&]() {
            approx = approximate_vertex_cover_by_matching(G);
        });

        cout << left
             << setw(12) << "cycle"
             << right
             << setw(6) << G.n
             << setw(8) << G.edge_count()
             << setw(10) << exact.cover.size()
             << setw(12) << approx.cover.size()
             << setw(16) << exact_time
             << setw(16) << approx_time
             << setw(18) << exact.candidates_checked
             << endl;
    }

    cout << "\nThe exact solver checks subsets until it finds a minimum cover." << endl;
    cout << "The approximation algorithm scans edges once to build a maximal matching." << endl;
}

void show_detailed_star_example() {
    print_section_title("Detailed Example: Star Graph");

    Graph G = make_star_graph(6);

    ExactVCResult exact = brute_force_min_vertex_cover(G);
    ApproxVCResult approx = approximate_vertex_cover_by_matching(G);

    cout << "A star graph has one center connected to many leaves." << endl;
    cout << "The optimal vertex cover is just the center." << endl;

    cout << "\nOptimal cover: ";
    print_vertex_set(exact.cover, G);
    cout << "  size = " << exact.cover.size() << endl;

    cout << "Matching chosen by approximation: ";
    print_edge_set(approx.matching, G);
    cout << "  size = " << approx.matching.size() << endl;

    cout << "Approximate cover: ";
    print_vertex_set(approx.cover, G);
    cout << "  size = " << approx.cover.size() << endl;

    double ratio = static_cast<double>(approx.cover.size()) /
                   static_cast<double>(exact.cover.size());

    cout << "Approximation ratio on this graph: "
         << fixed << setprecision(2) << ratio
         << endl;

    cout << "\nWhat to notice:" << endl;
    cout << "- The approximation is valid because it returns a vertex cover." << endl;
    cout << "- It is not always optimal." << endl;
    cout << "- The ratio is still at most 2." << endl;
}

void show_detailed_matching_argument() {
    print_section_title("Why the Matching Argument Works");

    Graph G = make_sample_graph();

    ApproxVCResult approx = approximate_vertex_cover_by_matching(G);
    ExactVCResult exact = brute_force_min_vertex_cover(G);

    cout << "Graph: sample graph" << endl;
    cout << "n = " << G.n << ", m = " << G.edge_count() << endl;

    cout << "\nMaximal matching M: ";
    print_edge_set(approx.matching, G);
    cout << endl;

    cout << "Approximate cover S = all endpoints of M: ";
    print_vertex_set(approx.cover, G);
    cout << endl;

    cout << "\nMeasurements:" << endl;
    cout << "|M| = " << approx.matching.size() << endl;
    cout << "|S| = " << approx.cover.size() << endl;
    cout << "OPT = " << exact.cover.size() << endl;

    cout << "\nReasoning:" << endl;
    cout << "1. Every vertex cover must cover each edge in the matching." << endl;
    cout << "2. Matching edges do not share endpoints." << endl;
    cout << "3. Therefore OPT >= |M|." << endl;
    cout << "4. The algorithm returns both endpoints of each matching edge, so |S| = 2|M|." << endl;
    cout << "5. Since OPT >= |M|, we get |S| <= 2OPT." << endl;
}

void show_maximal_vs_maximum() {
    print_section_title("Maximal Matching vs. Maximum Matching");

    Graph G(4);
    G.labels = {"A", "B", "C", "D"};

    G.add_edge(0, 1); // A-B
    G.add_edge(1, 2); // B-C
    G.add_edge(2, 3); // C-D

    vector<pair<int, int>> maximal_not_maximum = {{1, 2}};
    vector<pair<int, int>> maximum = {{0, 1}, {2, 3}};

    cout << "Path graph: A - B - C - D" << endl;

    cout << "\nMatching 1: ";
    print_edge_set(maximal_not_maximum, G);
    cout << endl;
    cout << "Is matching? " << (is_matching(G, maximal_not_maximum) ? "yes" : "no") << endl;
    cout << "Is maximal? " << (is_maximal_matching(G, maximal_not_maximum) ? "yes" : "no") << endl;
    cout << "Size: " << maximal_not_maximum.size() << endl;

    cout << "\nMatching 2: ";
    print_edge_set(maximum, G);
    cout << endl;
    cout << "Is matching? " << (is_matching(G, maximum) ? "yes" : "no") << endl;
    cout << "Is maximal? " << (is_maximal_matching(G, maximum) ? "yes" : "no") << endl;
    cout << "Size: " << maximum.size() << endl;

    cout << "\nWhat to notice:" << endl;
    cout << "Maximal means no more edges can be added." << endl;
    cout << "Maximum means largest possible." << endl;
    cout << "The approximation algorithm only needs a maximal matching." << endl;
}

int main() {
    cout << "Lab 4 Day 5: Approximation Algorithms -- Vertex Cover" << endl;
    cout << string(72, '=') << endl;

    run_main_measurements();
    run_scaling_measurements();
    show_detailed_star_example();
    show_detailed_matching_argument();
    show_maximal_vs_maximum();

    cout << "\nWhat to notice:" << endl;
    cout << "1. Exact minimum Vertex Cover is expensive because it searches subsets." << endl;
    cout << "2. The approximation algorithm is fast because it greedily builds a maximal matching." << endl;
    cout << "3. Returning both endpoints of every matched edge always gives a vertex cover." << endl;
    cout << "4. Any vertex cover must include at least one endpoint from each matching edge." << endl;
    cout << "5. Therefore the approximate cover has size at most 2 times OPT." << endl;

    return 0;
}