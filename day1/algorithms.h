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

struct SolverStats {
    bool found_solution;
    long long candidates_checked;
    long long primitive_checks;
};

struct VerifierStats {
    bool accepted;
    long long certificate_items_checked;
    long long primitive_checks;
};

bool verify_subset_sum(
    const std::vector<int>& numbers,
    int target,
    const std::vector<int>& certificate,
    VerifierStats& stats
);

bool solve_subset_sum_bruteforce(
    const std::vector<int>& numbers,
    int target,
    SolverStats& stats,
    std::vector<int>& witness
);

bool verify_clique(
    const Graph& G,
    int k,
    const std::vector<int>& certificate,
    VerifierStats& stats
);

bool solve_clique_bruteforce(
    const Graph& G,
    int k,
    SolverStats& stats,
    std::vector<int>& witness
);

bool verify_vertex_cover(
    const Graph& G,
    int k,
    const std::vector<int>& certificate,
    VerifierStats& stats
);

bool solve_vertex_cover_bruteforce(
    const Graph& G,
    int k,
    SolverStats& stats,
    std::vector<int>& witness
);

Graph make_path_graph(int n);
Graph make_cycle_graph(int n);
Graph make_complete_graph(int n);
Graph make_empty_graph(int n);
Graph make_sample_graph();
Graph make_two_triangles_with_bridge();

std::vector<int> make_subset_sum_numbers(int n);
void print_vertex_set(const std::vector<int>& vertices, const Graph& G);
void print_index_set(const std::vector<int>& indices);
long long two_to_the(int n);
long long choose_count(int n, int k);

#endif