#include "algorithms.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

void test_weighted_graph_basics() {
    WeightedGraph G(3);

    G.set_weight(0, 1, 5);
    G.set_weight(1, 2, 7);
    G.set_weight(0, 2, 10);

    assert(G.get_weight(0, 1) == 5);
    assert(G.get_weight(1, 0) == 5);
    assert(G.get_weight(1, 2) == 7);
    assert(G.get_weight(0, 2) == 10);
}

void test_tour_cost_and_hamiltonian_cycle() {
    WeightedGraph G = make_line_metric_graph(4);

    vector<int> tour = {0, 1, 2, 3};

    assert(is_hamiltonian_cycle(G, tour));
    assert(tour_cost(G, tour) == 6);

    vector<int> invalid = {0, 1, 1, 3};
    assert(!is_hamiltonian_cycle(G, invalid));
}

void test_triangle_inequality() {
    WeightedGraph G = make_grid_metric_graph(2, 3);
    assert(satisfies_triangle_inequality(G));
}

void test_brute_force_tsp() {
    WeightedGraph G = make_line_metric_graph(4);

    TSPResult result = brute_force_tsp(G);

    assert(is_hamiltonian_cycle(G, result.tour));
    assert(result.cost == 6);
    assert(result.candidates_checked == 6);
}

void test_mst() {
    WeightedGraph G = make_line_metric_graph(5);

    MSTResult mst = prim_mst(G);

    assert(mst.edges.size() == 4);
    assert(mst.cost == 4);
}

void test_approximate_tsp() {
    WeightedGraph G = make_sample_metric_graph();

    TSPResult exact = brute_force_tsp(G);
    TSPResult approx = approximate_metric_tsp_double_tree(G);

    assert(is_hamiltonian_cycle(G, approx.tour));
    assert(approx.cost <= 2 * exact.cost);
}

void test_set_cover_predicate() {
    SetCoverInstance instance = make_sample_set_cover();

    assert(covers_universe(instance, {0, 2, 3}));
    assert(!covers_universe(instance, {0, 1}));
}

void test_brute_force_set_cover() {
    SetCoverInstance instance = make_sample_set_cover();

    SetCoverResult exact = brute_force_set_cover(instance);

    assert(covers_universe(instance, exact.chosen_sets));
    assert(exact.cover_size > 0);
}

void test_greedy_set_cover() {
    SetCoverInstance instance = make_sample_set_cover();

    SetCoverResult greedy = greedy_set_cover(instance);

    assert(covers_universe(instance, greedy.chosen_sets));
    assert(greedy.cover_size > 0);
}

void test_set_cover_comparison() {
    vector<SetCoverInstance> instances = {
        make_sample_set_cover(),
        make_tricky_set_cover(),
        make_layered_set_cover(4),
        make_dense_set_cover(8)
    };

    for (const SetCoverInstance& instance : instances) {
        SetCoverResult exact = brute_force_set_cover(instance);
        SetCoverResult greedy = greedy_set_cover(instance);

        assert(covers_universe(instance, exact.chosen_sets));
        assert(covers_universe(instance, greedy.chosen_sets));

        assert(greedy.cover_size >= exact.cover_size);
    }
}

int main() {
    test_weighted_graph_basics();
    test_tour_cost_and_hamiltonian_cycle();
    test_triangle_inequality();
    test_brute_force_tsp();
    test_mst();
    test_approximate_tsp();
    test_set_cover_predicate();
    test_brute_force_set_cover();
    test_greedy_set_cover();
    test_set_cover_comparison();

    cout << "All Day 6 tests passed." << endl;

    return 0;
}