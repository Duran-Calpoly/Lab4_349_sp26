#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <set>
#include <string>
#include <utility>
#include <vector>

struct WeightedGraph {
    int n;
    std::vector<std::string> labels;
    std::vector<std::vector<int>> weight;

    WeightedGraph();
    explicit WeightedGraph(int num_vertices);

    void set_weight(int u, int v, int w);
    int get_weight(int u, int v) const;
};

struct TSPResult {
    std::vector<int> tour;
    int cost;
    long long candidates_checked;
};

struct MSTResult {
    std::vector<std::pair<int, int>> edges;
    int cost;
};

struct SetCoverInstance {
    int universe_size;
    std::vector<std::set<int>> sets;
    std::vector<std::string> set_names;
};

struct SetCoverResult {
    std::vector<int> chosen_sets;
    int cover_size;
    long long candidates_checked;
};

int tour_cost(const WeightedGraph& G, const std::vector<int>& tour);
bool is_hamiltonian_cycle(const WeightedGraph& G, const std::vector<int>& tour);

TSPResult brute_force_tsp(const WeightedGraph& G);
MSTResult prim_mst(const WeightedGraph& G);
TSPResult approximate_metric_tsp_double_tree(const WeightedGraph& G);

bool satisfies_triangle_inequality(const WeightedGraph& G);

WeightedGraph make_line_metric_graph(int n);
WeightedGraph make_circle_metric_graph(int n);
WeightedGraph make_grid_metric_graph(int rows, int cols);
WeightedGraph make_sample_metric_graph();

bool covers_universe(const SetCoverInstance& instance, const std::vector<int>& chosen_sets);
SetCoverResult brute_force_set_cover(const SetCoverInstance& instance);
SetCoverResult greedy_set_cover(const SetCoverInstance& instance);

SetCoverInstance make_sample_set_cover();
SetCoverInstance make_tricky_set_cover();
SetCoverInstance make_layered_set_cover(int layers);
SetCoverInstance make_dense_set_cover(int universe_size);

void print_tour(const std::vector<int>& tour, const WeightedGraph& G);
void print_mst_edges(const MSTResult& mst, const WeightedGraph& G);
void print_set_cover_solution(const SetCoverInstance& instance, const std::vector<int>& chosen_sets);
void print_set(const std::set<int>& s);

#endif