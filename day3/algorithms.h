#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

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

struct ReductionResult {
    Graph graph;
    int target_independent_set_size;

    int num_variables;
    int num_clauses;
    int num_vertices;
    int triangle_edges;
    int contradiction_edges;
    int total_edges;
};

bool evaluate_literal(const Literal& literal, const std::map<std::string, bool>& assignment);
bool evaluate_clause(const Clause& clause, const std::map<std::string, bool>& assignment);
bool evaluate_formula(const Formula3CNF& formula, const std::map<std::string, bool>& assignment);

bool brute_force_3sat(
    const Formula3CNF& formula,
    long long& assignments_checked,
    std::map<std::string, bool>& satisfying_assignment
);

ReductionResult reduce_3sat_to_independent_set(const Formula3CNF& formula);

bool is_independent_set(const Graph& G, const std::vector<int>& vertices);

bool brute_force_independent_set(
    const Graph& G,
    int k,
    long long& candidates_checked,
    std::vector<int>& witness
);

Formula3CNF make_formula_satisfiable_small();
Formula3CNF make_formula_unsatisfiable_small();
Formula3CNF make_formula_two_clauses();
Formula3CNF make_formula_four_clauses();
Formula3CNF make_formula_chain(int num_clauses);

void print_assignment(const std::map<std::string, bool>& assignment);
void print_vertex_set(const std::vector<int>& vertices, const Graph& G);
void print_reduction_summary(const ReductionResult& result);

#endif