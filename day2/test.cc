#include "algorithms.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

void test_graph_basics() {
    Graph G(4);

    G.add_edge(0, 1);
    G.add_edge(1, 2);

    assert(G.has_edge(0, 1));
    assert(G.has_edge(1, 0));
    assert(G.has_edge(1, 2));
    assert(!G.has_edge(0, 2));
    assert(G.edge_count() == 2);
}

void test_independent_set_and_vertex_cover_relationship() {
    Graph G = make_sample_graph();

    vector<int> independent = {0, 3}; // A, D
    assert(is_independent_set(G, independent));

    vector<int> complement = set_complement(G.n, independent);
    assert(is_vertex_cover(G, complement));
}

void test_independent_set_to_vertex_cover_reduction() {
    Graph G = make_sample_graph();

    DecisionInstance is_instance{G, 2};
    DecisionInstance vc_instance = reduce_independent_set_to_vertex_cover(is_instance);

    assert(vc_instance.graph.n == G.n);
    assert(vc_instance.graph.edge_count() == G.edge_count());
    assert(vc_instance.k == 3);

    bool is_answer = brute_force_independent_set(is_instance.graph, is_instance.k);
    bool vc_answer = brute_force_vertex_cover(vc_instance.graph, vc_instance.k);

    assert(is_answer == vc_answer);
}

void test_complement_graph() {
    Graph G = make_path_graph(4);
    long long pair_checks = 0;
    long long edges_added = 0;

    Graph H = complement_graph(G, pair_checks, edges_added);

    // Complete graph on 4 vertices has 6 edges.
    // Path graph on 4 vertices has 3 edges.
    // Complement should have 3 edges.
    assert(pair_checks == 6);
    assert(edges_added == 3);
    assert(H.edge_count() == 3);

    assert(H.has_edge(0, 2));
    assert(H.has_edge(0, 3));
    assert(H.has_edge(1, 3));

    assert(!H.has_edge(0, 1));
    assert(!H.has_edge(1, 2));
    assert(!H.has_edge(2, 3));
}

void test_independent_set_to_clique_reduction() {
    Graph G = make_path_graph(4);

    DecisionInstance is_instance{G, 2};

    ReductionStats stats;
    DecisionInstance clique_instance = reduce_independent_set_to_clique(is_instance, stats);

    bool is_answer = brute_force_independent_set(is_instance.graph, is_instance.k);
    bool clique_answer = brute_force_clique(clique_instance.graph, clique_instance.k);

    assert(is_answer == clique_answer);

    vector<int> independent = {0, 2};
    assert(is_independent_set(G, independent));
    assert(is_clique(clique_instance.graph, independent));
}

void test_yes_no_preservation_on_small_graphs() {
    vector<Graph> graphs = {
        make_empty_graph(5),
        make_path_graph(5),
        make_cycle_graph(5),
        make_complete_graph(5),
        make_sample_graph()
    };

    for (const Graph& G : graphs) {
        for (int k = 0; k <= G.n; k++) {
            DecisionInstance is_instance{G, k};

            DecisionInstance vc_instance = reduce_independent_set_to_vertex_cover(is_instance);
            ReductionStats stats;
            DecisionInstance clique_instance = reduce_independent_set_to_clique(is_instance, stats);

            bool is_answer = brute_force_independent_set(G, k);
            bool vc_answer = brute_force_vertex_cover(vc_instance.graph, vc_instance.k);
            bool clique_answer = brute_force_clique(clique_instance.graph, clique_instance.k);

            assert(is_answer == vc_answer);
            assert(is_answer == clique_answer);
        }
    }
}

int main() {
    test_graph_basics();
    test_independent_set_and_vertex_cover_relationship();
    test_independent_set_to_vertex_cover_reduction();
    test_complement_graph();
    test_independent_set_to_clique_reduction();
    test_yes_no_preservation_on_small_graphs();

    cout << "All Day 2 tests passed." << endl;

    return 0;
}