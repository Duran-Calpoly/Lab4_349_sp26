#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>
#include <vector>
#include <set>
#include <utility>

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

struct DecisionInstance {
    Graph graph;
    int k;
};

struct ReductionStats {
    int original_vertices;
    int original_edges;
    int original_k;

    int transformed_vertices;
    int transformed_edges;
    int transformed_k;

    long long pair_checks;
    long long edges_added;
};

Graph complement_graph(const Graph& G, long long& pair_checks, long long& edges_added);

DecisionInstance reduce_independent_set_to_vertex_cover(const DecisionInstance& instance);

DecisionInstance reduce_independent_set_to_clique(
    const DecisionInstance& instance,
    ReductionStats& stats
);

bool is_independent_set(const Graph& G, const std::vector<int>& vertices);
bool is_vertex_cover(const Graph& G, const std::vector<int>& vertices);
bool is_clique(const Graph& G, const std::vector<int>& vertices);

bool brute_force_independent_set(const Graph& G, int k);
bool brute_force_vertex_cover(const Graph& G, int k);
bool brute_force_clique(const Graph& G, int k);

std::vector<int> set_complement(int n, const std::vector<int>& vertices);

Graph make_path_graph(int n);
Graph make_cycle_graph(int n);
Graph make_complete_graph(int n);
Graph make_empty_graph(int n);
Graph make_sample_graph();

void print_graph_summary(const Graph& G);
void print_instance_summary(const std::string& name, const DecisionInstance& instance);
void print_vertex_set(const std::vector<int>& vertices, const Graph& G);

#endif