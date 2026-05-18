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

struct NamedTSPInstance {
    string name;
    WeightedGraph graph;
};

struct NamedSetCoverInstance {
    string name;
    SetCoverInstance instance;
};

void print_section_title(const string& title) {
    cout << "\n" << title << endl;
    cout << string(title.size(), '=') << endl;
}

void run_tsp_measurements() {
    print_section_title("Metric TSP: Exact vs. 2-Approximation");

    vector<NamedTSPInstance> instances = {
        {"line n=5", make_line_metric_graph(5)},
        {"circle n=6", make_circle_metric_graph(6)},
        {"grid 2x3", make_grid_metric_graph(2, 3)},
        {"sample n=5", make_sample_metric_graph()}
    };

    cout << left
         << setw(16) << "Instance"
         << right
         << setw(6) << "n"
         << setw(10) << "MST"
         << setw(12) << "OPT"
         << setw(12) << "Approx"
         << setw(10) << "Ratio"
         << setw(16) << "exact us"
         << setw(16) << "approx us"
         << setw(16) << "tours tried"
         << endl;

    cout << string(114, '-') << endl;

    for (const NamedTSPInstance& named : instances) {
        const WeightedGraph& G = named.graph;

        TSPResult exact;
        TSPResult approx;
        MSTResult mst;

        long long exact_time = time_microseconds([&]() {
            exact = brute_force_tsp(G);
        });

        long long approx_time = time_microseconds([&]() {
            mst = prim_mst(G);
            approx = approximate_metric_tsp_double_tree(G);
        });

        double ratio = static_cast<double>(approx.cost) /
                       static_cast<double>(exact.cost);

        cout << left
             << setw(16) << named.name
             << right
             << setw(6) << G.n
             << setw(10) << mst.cost
             << setw(12) << exact.cost
             << setw(12) << approx.cost
             << setw(10) << fixed << setprecision(2) << ratio
             << setw(16) << exact_time
             << setw(16) << approx_time
             << setw(16) << exact.candidates_checked
             << endl;
    }

    cout << "\nRatio means approximate tour cost / optimal tour cost." << endl;
    cout << "For the double-tree metric TSP approximation, the ratio should be at most 2.00." << endl;
}

void run_tsp_scaling() {
    print_section_title("Metric TSP Scaling: Exact Solver vs. Approximation");

    cout << left
         << setw(10) << "n"
         << right
         << setw(16) << "tours tried"
         << setw(16) << "OPT"
         << setw(16) << "Approx"
         << setw(16) << "exact us"
         << setw(16) << "approx us"
         << endl;

    cout << string(90, '-') << endl;

    vector<int> sizes = {4, 5, 6, 7, 8};

    for (int n : sizes) {
        WeightedGraph G = make_circle_metric_graph(n);

        TSPResult exact;
        TSPResult approx;

        long long exact_time = time_microseconds([&]() {
            exact = brute_force_tsp(G);
        });

        long long approx_time = time_microseconds([&]() {
            approx = approximate_metric_tsp_double_tree(G);
        });

        cout << left
             << setw(10) << n
             << right
             << setw(16) << exact.candidates_checked
             << setw(16) << exact.cost
             << setw(16) << approx.cost
             << setw(16) << exact_time
             << setw(16) << approx_time
             << endl;
    }

    cout << "\nBrute-force TSP fixes the starting city and tries all orders of the remaining cities." << endl;
    cout << "That means it tries (n - 1)! candidate tours." << endl;
}

void show_detailed_tsp_example() {
    print_section_title("Detailed TSP Example");

    WeightedGraph G = make_sample_metric_graph();

    cout << "Graph has " << G.n << " cities." << endl;
    cout << "Triangle inequality holds? "
         << (satisfies_triangle_inequality(G) ? "yes" : "no")
         << endl;

    MSTResult mst = prim_mst(G);
    TSPResult exact = brute_force_tsp(G);
    TSPResult approx = approximate_metric_tsp_double_tree(G);

    cout << "\nMST edges: ";
    print_mst_edges(mst, G);
    cout << endl;
    cout << "MST cost = " << mst.cost << endl;

    cout << "\nOptimal TSP tour: ";
    print_tour(exact.tour, G);
    cout << endl;
    cout << "OPT cost = " << exact.cost << endl;

    cout << "\nApproximate TSP tour: ";
    print_tour(approx.tour, G);
    cout << endl;
    cout << "Approx cost = " << approx.cost << endl;

    cout << "\nReasoning:" << endl;
    cout << "1. Removing one edge from an optimal TSP tour leaves a spanning tree." << endl;
    cout << "2. Therefore MST <= OPT." << endl;
    cout << "3. Doubling the MST costs 2*MST." << endl;
    cout << "4. Shortcutting repeated vertices does not increase cost because of the triangle inequality." << endl;
    cout << "5. Therefore Approx <= 2*OPT." << endl;
}

void run_set_cover_measurements() {
    print_section_title("Set Cover: Exact vs. Greedy");

    vector<NamedSetCoverInstance> instances = {
        {"sample", make_sample_set_cover()},
        {"tricky", make_tricky_set_cover()},
        {"layered 4", make_layered_set_cover(4)},
        {"layered 5", make_layered_set_cover(5)},
        {"dense 8", make_dense_set_cover(8)}
    };

    cout << left
         << setw(16) << "Instance"
         << right
         << setw(10) << "|U|"
         << setw(10) << "#sets"
         << setw(10) << "OPT"
         << setw(10) << "Greedy"
         << setw(10) << "Ratio"
         << setw(16) << "exact us"
         << setw(16) << "greedy us"
         << setw(18) << "exact cand."
         << setw(18) << "greedy scans"
         << endl;

    cout << string(134, '-') << endl;

    for (const NamedSetCoverInstance& named : instances) {
        const SetCoverInstance& instance = named.instance;

        SetCoverResult exact;
        SetCoverResult greedy;

        long long exact_time = time_microseconds([&]() {
            exact = brute_force_set_cover(instance);
        });

        long long greedy_time = time_microseconds([&]() {
            greedy = greedy_set_cover(instance);
        });

        double ratio = static_cast<double>(greedy.cover_size) /
                       static_cast<double>(exact.cover_size);

        cout << left
             << setw(16) << named.name
             << right
             << setw(10) << instance.universe_size
             << setw(10) << instance.sets.size()
             << setw(10) << exact.cover_size
             << setw(10) << greedy.cover_size
             << setw(10) << fixed << setprecision(2) << ratio
             << setw(16) << exact_time
             << setw(16) << greedy_time
             << setw(18) << exact.candidates_checked
             << setw(18) << greedy.candidates_checked
             << endl;
    }

    cout << "\nGreedy Set Cover repeatedly chooses the set that covers the most uncovered elements." << endl;
    cout << "It is not always optimal, but it is fast and has a known approximation guarantee." << endl;
}

void run_set_cover_scaling() {
    print_section_title("Set Cover Scaling: Exact Solver vs. Greedy");

    cout << left
         << setw(14) << "Instance"
         << right
         << setw(10) << "|U|"
         << setw(10) << "#sets"
         << setw(10) << "OPT"
         << setw(10) << "Greedy"
         << setw(16) << "exact us"
         << setw(16) << "greedy us"
         << setw(18) << "exact cand."
         << setw(18) << "greedy scans"
         << endl;

    cout << string(122, '-') << endl;

    vector<int> universe_sizes = {6, 8, 10, 12, 14};

    for (int u : universe_sizes) {
        SetCoverInstance instance = make_dense_set_cover(u);

        SetCoverResult exact;
        SetCoverResult greedy;

        long long exact_time = time_microseconds([&]() {
            exact = brute_force_set_cover(instance);
        });

        long long greedy_time = time_microseconds([&]() {
            greedy = greedy_set_cover(instance);
        });

        cout << left
             << setw(14) << ("dense " + to_string(u))
             << right
             << setw(10) << instance.universe_size
             << setw(10) << instance.sets.size()
             << setw(10) << exact.cover_size
             << setw(10) << greedy.cover_size
             << setw(16) << exact_time
             << setw(16) << greedy_time
             << setw(18) << exact.candidates_checked
             << setw(18) << greedy.candidates_checked
             << endl;
    }

    cout << "\nThe exact Set Cover solver searches subsets of sets." << endl;
    cout << "The greedy solver repeatedly scans the available sets and chooses the best next one." << endl;
}

void show_detailed_set_cover_example() {
    print_section_title("Detailed Set Cover Example");

    SetCoverInstance instance = make_tricky_set_cover();

    cout << "Universe U = {0, 1, 2, 3, 4, 5, 6, 7}" << endl;
    cout << "\nAvailable sets:" << endl;

    for (int i = 0; i < static_cast<int>(instance.sets.size()); i++) {
        cout << "  " << instance.set_names[i] << " = ";
        print_set(instance.sets[i]);
        cout << endl;
    }

    SetCoverResult exact = brute_force_set_cover(instance);
    SetCoverResult greedy = greedy_set_cover(instance);

    cout << "\nOptimal set cover: ";
    print_set_cover_solution(instance, exact.chosen_sets);
    cout << "  size = " << exact.cover_size << endl;

    cout << "Greedy set cover:  ";
    print_set_cover_solution(instance, greedy.chosen_sets);
    cout << "  size = " << greedy.cover_size << endl;

    cout << "\nGreedy is making a local choice:" << endl;
    cout << "At each step, it chooses the set covering the largest number of currently uncovered elements." << endl;
    cout << "That local choice is fast, but it does not always produce the global optimum." << endl;
}

void show_compare_approximation_styles() {
    print_section_title("Comparing Approximation Styles");

    cout << "Vertex Cover approximation:" << endl;
    cout << "- Uses a maximal matching." << endl;
    cout << "- Matching size gives a lower bound on OPT." << endl;
    cout << "- Returning both endpoints gives a 2-approximation." << endl;

    cout << "\nMetric TSP approximation:" << endl;
    cout << "- Uses an MST." << endl;
    cout << "- MST cost gives a lower bound on OPT." << endl;
    cout << "- Doubling plus shortcutting gives a 2-approximation." << endl;

    cout << "\nSet Cover approximation:" << endl;
    cout << "- Uses a greedy rule." << endl;
    cout << "- Repeatedly covers as many new elements as possible." << endl;
    cout << "- The guarantee is different from the 2-approximation arguments above." << endl;

    cout << "\nMain point:" << endl;
    cout << "Approximation algorithms are problem-specific." << endl;
    cout << "Each proof depends on a structure that relates the algorithm's solution to OPT." << endl;
}

int main() {
    cout << "Lab 4 Day 6: Approximation Algorithms -- TSP and Set Cover" << endl;
    cout << string(76, '=') << endl;

    run_tsp_measurements();
    run_tsp_scaling();
    show_detailed_tsp_example();

    run_set_cover_measurements();
    run_set_cover_scaling();
    show_detailed_set_cover_example();

    show_compare_approximation_styles();

    cout << "\nWhat to notice:" << endl;
    cout << "1. Exact TSP becomes expensive because it checks factorially many tours." << endl;
    cout << "2. The metric TSP approximation uses MST as a lower bound." << endl;
    cout << "3. The triangle inequality makes shortcutting safe." << endl;
    cout << "4. Exact Set Cover searches subsets of sets." << endl;
    cout << "5. Greedy Set Cover is fast but not always optimal." << endl;
    cout << "6. Approximation algorithms trade exactness for speed and guarantees." << endl;

    return 0;
}