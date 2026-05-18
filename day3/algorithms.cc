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
    long long& assignments_checked,
    map<string, bool>& satisfying_assignment
) {
    vector<string> vars = formula.variables();
    int n = static_cast<int>(vars.size());

    assignments_checked = 0;

    if (n >= 62) {
        return false;
    }

    long long total_assignments = 1LL << n;

    for (long long mask = 0; mask < total_assignments; mask++) {
        assignments_checked++;

        map<string, bool> assignment;

        for (int i = 0; i < n; i++) {
            bool value = ((mask >> i) & 1LL) != 0;
            assignment[vars[i]] = value;
        }

        if (evaluate_formula(formula, assignment)) {
            satisfying_assignment = assignment;
            return true;
        }
    }

    satisfying_assignment.clear();
    return false;
}

static bool contradictory(const Literal& a, const Literal& b) {
    return a.variable == b.variable && a.is_negated != b.is_negated;
}

ReductionResult reduce_3sat_to_independent_set(const Formula3CNF& formula) {
    int m = formula.num_clauses();
    int num_vertices = 3 * m;

    Graph G(num_vertices);

    vector<Literal> vertex_literals(num_vertices);

    int vertex_index = 0;

    for (int clause_index = 0; clause_index < m; clause_index++) {
        const Clause& clause = formula.clauses[clause_index];

        for (int j = 0; j < 3; j++) {
            int v = vertex_index++;

            vertex_literals[v] = clause.literals[j];

            G.labels[v] = "C" + to_string(clause_index + 1)
                        + ":" + clause.literals[j].to_string();
        }
    }

    int triangle_edges = 0;

    for (int clause_index = 0; clause_index < m; clause_index++) {
        int base = 3 * clause_index;

        G.add_edge(base, base + 1);
        G.add_edge(base, base + 2);
        G.add_edge(base + 1, base + 2);

        triangle_edges += 3;
    }

    int contradiction_edges = 0;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) {
            int clause_i = i / 3;
            int clause_j = j / 3;

            if (clause_i == clause_j) {
                continue;
            }

            if (contradictory(vertex_literals[i], vertex_literals[j])) {
                int before = G.edge_count();
                G.add_edge(i, j);
                int after = G.edge_count();

                if (after > before) {
                    contradiction_edges++;
                }
            }
        }
    }

    ReductionResult result;
    result.graph = G;
    result.target_independent_set_size = m;
    result.num_variables = static_cast<int>(formula.variables().size());
    result.num_clauses = m;
    result.num_vertices = num_vertices;
    result.triangle_edges = triangle_edges;
    result.contradiction_edges = contradiction_edges;
    result.total_edges = G.edge_count();

    return result;
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

static bool choose_independent_set(
    const Graph& G,
    int k,
    int start,
    vector<int>& chosen,
    long long& candidates_checked,
    vector<int>& witness
) {
    if (static_cast<int>(chosen.size()) == k) {
        candidates_checked++;

        if (is_independent_set(G, chosen)) {
            witness = chosen;
            return true;
        }

        return false;
    }

    for (int v = start; v < G.n; v++) {
        chosen.push_back(v);

        if (choose_independent_set(G, k, v + 1, chosen, candidates_checked, witness)) {
            return true;
        }

        chosen.pop_back();
    }

    return false;
}

bool brute_force_independent_set(
    const Graph& G,
    int k,
    long long& candidates_checked,
    vector<int>& witness
) {
    candidates_checked = 0;
    witness.clear();

    if (k < 0 || k > G.n) {
        return false;
    }

    if (k == 0) {
        return true;
    }

    vector<int> chosen;
    return choose_independent_set(G, k, 0, chosen, candidates_checked, witness);
}

Formula3CNF make_formula_satisfiable_small() {
    Formula3CNF formula;

    formula.clauses.push_back(Clause(
        Literal("x", false),
        Literal("y", false),
        Literal("z", false)
    ));

    formula.clauses.push_back(Clause(
        Literal("x", true),
        Literal("y", false),
        Literal("z", true)
    ));

    return formula;
}

Formula3CNF make_formula_unsatisfiable_small() {
    Formula3CNF formula;

    // This encodes all combinations that force x to be both true and false
    // when only x is the meaningful variable.
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

Formula3CNF make_formula_two_clauses() {
    Formula3CNF formula;

    formula.clauses.push_back(Clause(
        Literal("x", false),
        Literal("y", false),
        Literal("w", false)
    ));

    formula.clauses.push_back(Clause(
        Literal("x", true),
        Literal("z", false),
        Literal("w", true)
    ));

    return formula;
}

Formula3CNF make_formula_four_clauses() {
    Formula3CNF formula;

    formula.clauses.push_back(Clause(
        Literal("x", false),
        Literal("y", false),
        Literal("w", false)
    ));

    formula.clauses.push_back(Clause(
        Literal("x", true),
        Literal("z", false),
        Literal("w", true)
    ));

    formula.clauses.push_back(Clause(
        Literal("y", true),
        Literal("v", false),
        Literal("z", true)
    ));

    formula.clauses.push_back(Clause(
        Literal("x", true),
        Literal("v", false),
        Literal("z", false)
    ));

    return formula;
}

Formula3CNF make_formula_chain(int num_clauses) {
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

void print_assignment(const map<string, bool>& assignment) {
    cout << "{";

    int count = 0;

    for (const auto& entry : assignment) {
        cout << entry.first << " = " << (entry.second ? "true" : "false");

        count++;

        if (count < static_cast<int>(assignment.size())) {
            cout << ", ";
        }
    }

    cout << "}";
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

void print_reduction_summary(const ReductionResult& result) {
    cout << "variables: " << result.num_variables << endl;
    cout << "clauses: " << result.num_clauses << endl;
    cout << "graph vertices: " << result.num_vertices << endl;
    cout << "triangle edges: " << result.triangle_edges << endl;
    cout << "contradiction edges: " << result.contradiction_edges << endl;
    cout << "total graph edges: " << result.total_edges << endl;
    cout << "target independent set size: "
         << result.target_independent_set_size
         << endl;
}