#include "algorithms.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

static pair<int, int> normalize_edge(int u, int v) {
    if (u > v) {
        swap(u, v);
    }
    return {u, v};
}

Graph::Graph() : n(0) {}

Graph::Graph(int num_vertices) : n(num_vertices) {
    labels.resize(n);

    for (int i = 0; i < n; i++) {
        labels[i] = "v" + to_string(i);
    }
}

void Graph::add_edge(int u, int v) {
    if (u == v) {
        return;
    }

    if (u < 0 || v < 0 || u >= n || v >= n) {
        return;
    }

    edges.insert(normalize_edge(u, v));
}

bool Graph::has_edge(int u, int v) const {
    if (u == v) {
        return false;
    }

    if (u < 0 || v < 0 || u >= n || v >= n) {
        return false;
    }

    return edges.count(normalize_edge(u, v)) > 0;
}

int Graph::edge_count() const {
    return static_cast<int>(edges.size());
}

Literal::Literal() : variable(""), is_negated(false) {}

Literal::Literal(const string& var, bool negated)
    : variable(var), is_negated(negated) {}

string Literal::to_string() const {
    if (is_negated) {
        return "not " + variable;
    }

    return variable;
}

Clause::Clause() {}

Clause::Clause(const Literal& a, const Literal& b, const Literal& c) {
    literals.push_back(a);
    literals.push_back(b);
    literals.push_back(c);
}

string Clause::to_string() const {
    stringstream ss;
    ss << "(";

    for (int i = 0; i < static_cast<int>(literals.size()); i++) {
        ss << literals[i].to_string();

        if (i + 1 < static_cast<int>(literals.size())) {
            ss << " OR ";
        }
    }

    ss << ")";
    return ss.str();
}

int Formula3CNF::num_clauses() const {
    return static_cast<int>(clauses.size());
}

vector<string> Formula3CNF::variables() const {
    set<string> vars;

    for (const Clause& clause : clauses) {
        for (const Literal& literal : clause.literals) {
            vars.insert(literal.variable);
        }
    }

    return vector<string>(vars.begin(), vars.end());
}

string Formula3CNF::to_string() const {
    stringstream ss;

    for (int i = 0; i < static_cast<int>(clauses.size()); i++) {
        ss << clauses[i].to_string();

        if (i + 1 < static_cast<int>(clauses.size())) {
            ss << " AND ";
        }
    }

    return ss.str();
}

bool is_subset_sum_solution(
    const vector<int>& numbers,
    int target,
    const vector<int>& indices
) {
    int total = 0;

    for (int index : indices) {
        if (index < 0 || index >= static_cast<int>(numbers.size())) {
            return false;
        }

        total += numbers[index];
    }

    return total == target;
}

bool brute_force_subset_sum(
    const vector<int>& numbers,
    int target,
    SolverStats& stats,
    vector<int>& witness
) {
    int n = static_cast<int>(numbers.size());

    stats = {false, 0, 0};
    witness.clear();

    if (n >= 62) {
        return false;
    }

    long long total_subsets = 1LL << n;

    for (long long mask = 0; mask < total_subsets; mask++) {
        stats.candidates_checked++;

        int sum = 0;
        vector<int> candidate;

        for (int i = 0; i < n; i++) {
            stats.primitive_checks++;

            if ((mask >> i) & 1LL) {
                sum += numbers[i];
                candidate.push_back(i);
            }
        }

        if (sum == target) {
            stats.found_solution = true;
            witness = candidate;
            return true;
        }
    }

    return false;
}

bool is_independent_set(const Graph& G, const vector<int>& vertices) {
    for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(vertices.size()); j++) {
            if (G.has_edge(vertices[i], vertices[j])) {
                return false;
            }
        }
    }

    return true;
}

bool is_vertex_cover(const Graph& G, const vector<int>& vertices) {
    set<int> chosen(vertices.begin(), vertices.end());

    for (const auto& edge : G.edges) {
        int u = edge.first;
        int v = edge.second;

        if (chosen.count(u) == 0 && chosen.count(v) == 0) {
            return false;
        }
    }

    return true;
}

bool is_clique(const Graph& G, const vector<int>& vertices) {
    for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(vertices.size()); j++) {
            if (!G.has_edge(vertices[i], vertices[j])) {
                return false;
            }
        }
    }

    return true;
}

static bool choose_for_graph_problem(
    const Graph& G,
    int k,
    int start,
    vector<int>& chosen,
    SolverStats& stats,
    vector<int>& witness,
    bool (*predicate)(const Graph&, const vector<int>&)
) {
    if (static_cast<int>(chosen.size()) == k) {
        stats.candidates_checked++;

        if (predicate(G, chosen)) {
            stats.found_solution = true;
            witness = chosen;
            return true;
        }

        return false;
    }

    for (int v = start; v < G.n; v++) {
        chosen.push_back(v);

        if (choose_for_graph_problem(G, k, v + 1, chosen, stats, witness, predicate)) {
            return true;
        }

        chosen.pop_back();
    }

    return false;
}

bool brute_force_independent_set(
    const Graph& G,
    int k,
    SolverStats& stats,
    vector<int>& witness
) {
    stats = {false, 0, 0};
    witness.clear();

    if (k < 0 || k > G.n) {
        return false;
    }

    if (k == 0) {
        stats.found_solution = true;
        return true;
    }

    vector<int> chosen;
    bool result = choose_for_graph_problem(
        G,
        k,
        0,
        chosen,
        stats,
        witness,
        is_independent_set
    );

    stats.primitive_checks = stats.candidates_checked * combinations_upper_bound(k, 2);
    return result;
}

bool brute_force_clique(
    const Graph& G,
    int k,
    SolverStats& stats,
    vector<int>& witness
) {
    stats = {false, 0, 0};
    witness.clear();

    if (k < 0 || k > G.n) {
        return false;
    }

    if (k == 0) {
        stats.found_solution = true;
        return true;
    }

    vector<int> chosen;
    bool result = choose_for_graph_problem(
        G,
        k,
        0,
        chosen,
        stats,
        witness,
        is_clique
    );

    stats.primitive_checks = stats.candidates_checked * combinations_upper_bound(k, 2);
    return result;
}

bool brute_force_vertex_cover(
    const Graph& G,
    int k,
    SolverStats& stats,
    vector<int>& witness
) {
    stats = {false, 0, 0};
    witness.clear();

    if (k < 0) {
        return false;
    }

    if (k >= G.n) {
        stats.found_solution = true;

        for (int i = 0; i < G.n; i++) {
            witness.push_back(i);
        }

        return true;
    }

    for (int size = 0; size <= k; size++) {
        vector<int> chosen;

        SolverStats size_stats = {false, 0, 0};
        vector<int> size_witness;

        bool result = choose_for_graph_problem(
            G,
            size,
            0,
            chosen,
            size_stats,
            size_witness,
            is_vertex_cover
        );

        stats.candidates_checked += size_stats.candidates_checked;

        if (result) {
            stats.found_solution = true;
            witness = size_witness;
            stats.primitive_checks = stats.candidates_checked * G.edge_count();
            return true;
        }
    }

    stats.primitive_checks = stats.candidates_checked * G.edge_count();
    return false;
}

bool evaluate_literal(const Literal& literal, const map<string, bool>& assignment) {
    auto it = assignment.find(literal.variable);

    if (it == assignment.end()) {
        return false;
    }

    bool value = it->second;

    if (literal.is_negated) {
        return !value;
    }

    return value;
}

bool evaluate_clause(const Clause& clause, const map<string, bool>& assignment) {
    for (const Literal& literal : clause.literals) {
        if (evaluate_literal(literal, assignment)) {
            return true;
        }
    }

    return false;
}

bool evaluate_formula(const Formula3CNF& formula, const map<string, bool>& assignment) {
    for (const Clause& clause : formula.clauses) {
        if (!evaluate_clause(clause, assignment)) {
            return false;
        }
    }

    return true;
}

bool brute_force_3sat(
    const Formula3CNF& formula,
    SolverStats& stats,
    map<string, bool>& satisfying_assignment
) {
    vector<string> vars = formula.variables();
    int n = static_cast<int>(vars.size());

    stats = {false, 0, 0};
    satisfying_assignment.clear();

    if (n >= 62) {
        return false;
    }

    long long total_assignments = 1LL << n;

    for (long long mask = 0; mask < total_assignments; mask++) {
        stats.candidates_checked++;

        map<string, bool> assignment;

        for (int i = 0; i < n; i++) {
            assignment[vars[i]] = ((mask >> i) & 1LL) != 0;
        }

        stats.primitive_checks += formula.num_clauses();

        if (evaluate_formula(formula, assignment)) {
            stats.found_solution = true;
            satisfying_assignment = assignment;
            return true;
        }
    }

    return false;
}

Graph make_path_graph(int n) {
    Graph G(n);

    for (int i = 0; i < n - 1; i++) {
        G.add_edge(i, i + 1);
    }

    return G;
}

Graph make_cycle_graph(int n) {
    Graph G = make_path_graph(n);

    if (n >= 3) {
        G.add_edge(0, n - 1);
    }

    return G;
}

Graph make_complete_graph(int n) {
    Graph G(n);

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            G.add_edge(u, v);
        }
    }

    return G;
}

Graph make_empty_graph(int n) {
    return Graph(n);
}

Graph make_split_graph(int clique_size, int independent_size) {
    Graph G(clique_size + independent_size);

    for (int u = 0; u < clique_size; u++) {
        for (int v = u + 1; v < clique_size; v++) {
            G.add_edge(u, v);
        }
    }

    // Connect every clique vertex to every independent-side vertex.
    for (int u = 0; u < clique_size; u++) {
        for (int v = clique_size; v < G.n; v++) {
            G.add_edge(u, v);
        }
    }

    return G;
}

vector<int> make_subset_numbers(int n) {
    vector<int> numbers;

    for (int i = 1; i <= n; i++) {
        numbers.push_back(2 * i + 1);
    }

    return numbers;
}

Formula3CNF make_chain_formula(int num_clauses) {
    Formula3CNF formula;

    for (int i = 0; i < num_clauses; i++) {
        string a = "x" + to_string(i);
        string b = "x" + to_string(i + 1);
        string c = "x" + to_string(i + 2);

        bool neg_a = (i % 2 == 1);
        bool neg_b = (i % 3 == 1);
        bool neg_c = (i % 4 == 2);

        formula.clauses.push_back(Clause(
            Literal(a, neg_a),
            Literal(b, neg_b),
            Literal(c, neg_c)
        ));
    }

    return formula;
}

Formula3CNF make_unsat_formula_one_variable() {
    Formula3CNF formula;

    formula.clauses.push_back(Clause(
        Literal("x", false),
        Literal("x", false),
        Literal("x", false)
    ));

    formula.clauses.push_back(Clause(
        Literal("x", true),
        Literal("x", true),
        Literal("x", true)
    ));

    return formula;
}

long long two_to_the(int n) {
    if (n >= 62) {
        return -1;
    }

    return 1LL << n;
}

long long combinations_upper_bound(int n, int k) {
    if (k < 0 || k > n) {
        return 0;
    }

    if (k > n - k) {
        k = n - k;
    }

    long long result = 1;

    for (int i = 1; i <= k; i++) {
        result = result * (n - k + i) / i;
    }

    return result;
}

void print_vertex_set(const vector<int>& vertices, const Graph& G) {
    cout << "{";

    for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
        int v = vertices[i];

        if (v >= 0 && v < G.n) {
            cout << G.labels[v];
        } else {
            cout << v;
        }

        if (i + 1 < static_cast<int>(vertices.size())) {
            cout << ", ";
        }
    }

    cout << "}";
}

void print_index_set(const vector<int>& indices) {
    cout << "{";

    for (int i = 0; i < static_cast<int>(indices.size()); i++) {
        cout << indices[i];

        if (i + 1 < static_cast<int>(indices.size())) {
            cout << ", ";
        }
    }

    cout << "}";
}