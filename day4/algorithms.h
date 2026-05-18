#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <map>
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

struct Literal {
    std::string variable;
    bool is_negated;

    Literal();
    Literal(const std::string& var, bool negated);

    std::string to_string() const;
};

struct Clause {
    std::vector<Literal> literals;

    Clause();
    Clause(const Literal& a, const Literal& b, const Literal& c);

    std::string to_string() const;
};

struct Formula3CNF {
    std::vector<Clause> clauses;

    int num_clauses() const;
    std::vector<std::string> variables() const;
    std::string to_string() const;
};

struct SolverStats {
    bool found_solution;
    long long candidates_checked;
    long long primitive_checks;
};

bool is_subset_sum_solution(
    const std::vector<int>& numbers,
    int target,
    const std::vector<int>& indices
);

bool brute_force_subset_sum(
    const std::vector<int>& numbers,
    int target,
    SolverStats& stats,
    std::vector<int>& witness
);

bool is_independent_set(const Graph& G, const std::vector<int>& vertices);
bool is_vertex_cover(const Graph& G, const std::vector<int>& vertices);
bool is_clique(const Graph& G, const std::vector<int>& vertices);

bool brute_force_independent_set(
    const Graph& G,
    int k,
    SolverStats& stats,
    std::vector<int>& witness
);

bool brute_force_vertex_cover(
    const Graph& G,
    int k,
    SolverStats& stats,
    std::vector<int>& witness
);

bool brute_force_clique(
    const Graph& G,
    int k,
    SolverStats& stats,
    std::vector<int>& witness
);

bool evaluate_literal(const Literal& literal, const std::map<std::string, bool>& assignment);
bool evaluate_clause(const Clause& clause, const std::map<std::string, bool>& assignment);
bool evaluate_formula(const Formula3CNF& formula, const std::map<std::string, bool>& assignment);

bool brute_force_3sat(
    const Formula3CNF& formula,
    SolverStats& stats,
    std::map<std::string, bool>& satisfying_assignment
);

Graph make_path_graph(int n);
Graph make_cycle_graph(int n);
Graph make_complete_graph(int n);
Graph make_empty_graph(int n);
Graph make_split_graph(int clique_size, int independent_size);

std::vector<int> make_subset_numbers(int n);
Formula3CNF make_chain_formula(int num_clauses);
Formula3CNF make_unsat_formula_one_variable();

long long two_to_the(int n);
long long combinations_upper_bound(int n, int k);
void print_vertex_set(const std::vector<int>& vertices, const Graph& G);
void print_index_set(const std::vector<int>& indices);

#endif