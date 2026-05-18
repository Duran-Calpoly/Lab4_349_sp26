#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <set>
#include <string>
#include <utility>
#include <vector>

struct Graph {
    int n;
    std::vector<std::string> labels;
    std::set<std::pair<int, int>> edges;

    Graph();
    explicit Graph(int num_vertices);

    void add_edge(int u, int v);
    bool has_edge(int u, int v) const;
    int edge_count() const;
};

struct ExactVCResult {
    std::vector<int> cover;
    long long candidates_checked;
    long long edge_checks;
};

struct ApproxVCResult {
    std::vector<int> cover;
    std::vector<std::pair<int, int>> matching;
    long long edges_scanned;
};

bool is_vertex_cover(const Graph& G, const std::vector<int>& cover);
bool is_matching(const Graph& G, const std::vector<std::pair<int, int>>& matching);
bool is_maximal_matching(const Graph& G, const std::vector<std::pair<int, int>>& matching);

ExactVCResult brute_force_min_vertex_cover(const Graph& G);
ApproxVCResult approximate_vertex_cover_by_matching(const Graph& G);

Graph make_path_graph(int n);
Graph make_cycle_graph(int n);
Graph make_complete_graph(int n);
Graph make_star_graph(int leaves);
Graph make_ladder_graph(int rungs);
Graph make_sample_graph();
Graph make_two_triangles_with_bridge();
Graph make_sparse_deterministic_graph(int n);
Graph make_dense_deterministic_graph(int n);

void print_vertex_set(const std::vector<int>& vertices, const Graph& G);
void print_edge_set(const std::vector<std::pair<int, int>>& edges, const Graph& G);

#endif