#include "algorithms.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

void test_vertex_cover_predicate() {
    Graph G = make_path_graph(4);

    assert(is_vertex_cover(G, {1, 2}));
    assert(!is_vertex_cover(G, {0, 2}));
}

void test_matching_predicate() {
    Graph G = make_path_graph(5);

    vector<pair<int, int>> valid_matching = {{0, 1}, {2, 3}};
    vector<pair<int, int>> invalid_matching = {{0, 1}, {1, 2}};

    assert(is_matching(G, valid_matching));
    assert(!is_matching(G, invalid_matching));
}

void test_vertex_cover_predicate() {
    Graph G = make_path_graph(4);

    assert(is_vertex_cover(G, {1, 2}));
    assert(!is_vertex_cover(G, {0, 3}));
}

void test_exact_vertex_cover_path() {
    Graph G = make_path_graph(4);

    ExactVCResult result = brute_force_min_vertex_cover(G);

    assert(is_vertex_cover(G, result.cover));
    assert(result.cover.size() == 2);
    assert(result.candidates_checked > 0);
}

void test_exact_vertex_cover_star() {
    Graph G = make_star_graph(5);

    ExactVCResult result = brute_force_min_vertex_cover(G);

    assert(is_vertex_cover(G, result.cover));
    assert(result.cover.size() == 1);
}

void test_approx_vertex_cover_returns_cover() {
    Graph G = make_sample_graph();

    ApproxVCResult result = approximate_vertex_cover_by_matching(G);

    assert(is_matching(G, result.matching));
    assert(is_maximal_matching(G, result.matching));
    assert(is_vertex_cover(G, result.cover));
}

void test_approx_ratio_at_most_two_on_examples() {
    vector<Graph> graphs = {
        make_path_graph(6),
        make_cycle_graph(6),
        make_star_graph(6),
        make_sample_graph(),
        make_two_triangles_with_bridge(),
        make_ladder_graph(4)
    };

    for (const Graph& G : graphs) {
        ExactVCResult exact = brute_force_min_vertex_cover(G);
        ApproxVCResult approx = approximate_vertex_cover_by_matching(G);

        assert(is_vertex_cover(G, exact.cover));
        assert(is_vertex_cover(G, approx.cover));

        assert(approx.cover.size() <= 2 * exact.cover.size());
    }
}

void test_graph_generators() {
    Graph path = make_path_graph(5);
    assert(path.n == 5);
    assert(path.edge_count() == 4);

    Graph cycle = make_cycle_graph(5);
    assert(cycle.n == 5);
    assert(cycle.edge_count() == 5);

    Graph complete = make_complete_graph(5);
    assert(complete.n == 5);
    assert(complete.edge_count() == 10);

    Graph star = make_star_graph(5);
    assert(star.n == 6);
    assert(star.edge_count() == 5);

    Graph ladder = make_ladder_graph(4);
    assert(ladder.n == 8);
    assert(ladder.edge_count() == 10);
}

int main() {
    test_vertex_cover_predicate();
    test_matching_predicate();
    test_maximal_matching_predicate();
    test_exact_vertex_cover_path();
    test_exact_vertex_cover_star();
    test_approx_vertex_cover_returns_cover();
    test_approx_ratio_at_most_two_on_examples();
    test_graph_generators();

    cout << "All Day 5 tests passed." << endl;

    return 0;
}