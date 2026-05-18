#include "algorithms.h"

#include <algorithm>
#include <iostream>
#include <set>

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

bool verify_subset_sum(
    const vector<int>& numbers,
    int target,
    const vector<int>& certificate,
    VerifierStats& stats
) {
    stats.accepted = false;
    stats.certificate_items_checked = 0;
    stats.primitive_checks = 0;

    set<int> seen_indices;
    int total = 0;

    for (int index : certificate) {
        stats.certificate_items_checked++;
        stats.primitive_checks++;

        if (index < 0 || index >= static_cast<int>(numbers.size())) {
            return false;
        }

        if (seen_indices.count(index) > 0) {
            return false;
        }

        seen_indices.insert(index);
        total += numbers[index];
    }

    stats.primitive_checks++;

    stats.accepted = (total == target);
    return stats.accepted;
}

bool solve_subset_sum_bruteforce(
    const vector<int>& numbers,
    int target,
    SolverStats& stats,
    vector<int>& witness
) {
    stats.found_solution = false;
    stats.candidates_checked = 0;
    stats.primitive_checks = 0;
    witness.clear();

    int n = static_cast<int>(numbers.size());

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

        stats.primitive_checks++;

        if (sum == target) {
            stats.found_solution = true;
            witness = candidate;
            return true;
        }
    }

    return false;
}

bool verify_clique(
    const Graph& G,
    int k,
    const vector<int>& certificate,
    VerifierStats& stats
) {
    stats.accepted = false;
    stats.certificate_items_checked = 0;
    stats.primitive_checks = 0;

    if (static_cast<int>(certificate.size()) < k) {
        return false;
    }

    set<int> seen_vertices;

    for (int v : certificate) {
        stats.certificate_items_checked++;
        stats.primitive_checks++;

        if (v < 0 || v >= G.n) {
            return false;
        }

        if (seen_vertices.count(v) > 0) {
            return false;
        }

        seen_vertices.insert(v);
    }

    for (int i = 0; i < static_cast<int>(certificate.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(certificate.size()); j++) {
            stats.primitive_checks++;

            if (!G.has_edge(certificate[i], certificate[j])) {
                return false;
            }
        }
    }

    stats.accepted = true;
    return true;
}

static bool choose_clique_candidate(
    const Graph& G,
    int k,
    int start,
    vector<int>& chosen,
    SolverStats& stats,
    vector<int>& witness
) {
    if (static_cast<int>(chosen.size()) == k) {
        stats.candidates_checked++;

        VerifierStats verifier_stats;
        bool valid = verify_clique(G, k, chosen, verifier_stats);
        stats.primitive_checks += verifier_stats.primitive_checks;

        if (valid) {
            stats.found_solution = true;
            witness = chosen;
            return true;
        }

        return false;
    }

    for (int v = start; v < G.n; v++) {
        chosen.push_back(v);

        if (choose_clique_candidate(G, k, v + 1, chosen, stats, witness)) {
            return true;
        }

        chosen.pop_back();
    }

    return false;
}

bool solve_clique_bruteforce(
    const Graph& G,
    int k,
    SolverStats& stats,
    vector<int>& witness
) {
    stats.found_solution = false;
    stats.candidates_checked = 0;
    stats.primitive_checks = 0;
    witness.clear();

    if (k < 0 || k > G.n) {
        return false;
    }

    if (k == 0) {
        stats.found_solution = true;
        return true;
    }

    vector<int> chosen;
    return choose_clique_candidate(G, k, 0, chosen, stats, witness);
}

bool verify_vertex_cover(
    const Graph& G,
    int k,
    const vector<int>& certificate,
    VerifierStats& stats
) {
    stats.accepted = false;
    stats.certificate_items_checked = 0;
    stats.primitive_checks = 0;

    if (static_cast<int>(certificate.size()) > k) {
        return false;
    }

    set<int> chosen;

    for (int v : certificate) {
        stats.certificate_items_checked++;
        stats.primitive_checks++;

        if (v < 0 || v >= G.n) {
            return false;
        }

        if (chosen.count(v) > 0) {
            return false;
        }

        chosen.insert(v);
    }

    for (const auto& edge : G.edges) {
        int u = edge.first;
        int v = edge.second;

        stats.primitive_checks++;

        if (chosen.count(u) == 0 && chosen.count(v) == 0) {
            return false;
        }
    }

    stats.accepted = true;
    return true;
}

static bool choose_vertex_cover_candidate(
    const Graph& G,
    int target_size,
    int start,
    vector<int>& chosen,
    SolverStats& stats,
    vector<int>& witness
) {
    if (static_cast<int>(chosen.size()) == target_size) {
        stats.candidates_checked++;

        VerifierStats verifier_stats;
        bool valid = verify_vertex_cover(G, target_size, chosen, verifier_stats);
        stats.primitive_checks += verifier_stats.primitive_checks;

        if (valid) {
            stats.found_solution = true;
            witness = chosen;
            return true;
        }

        return false;
    }

    for (int v = start; v < G.n; v++) {
        chosen.push_back(v);

        if (choose_vertex_cover_candidate(G, target_size, v + 1, chosen, stats, witness)) {
            return true;
        }

        chosen.pop_back();
    }

    return false;
}

bool solve_vertex_cover_bruteforce(
    const Graph& G,
    int k,
    SolverStats& stats,
    vector<int>& witness
) {
    stats.found_solution = false;
    stats.candidates_checked = 0;
    stats.primitive_checks = 0;
    witness.clear();

    if (k < 0) {
        return false;
    }

    for (int size = 0; size <= k && size <= G.n; size++) {
        vector<int> chosen;

        if (choose_vertex_cover_candidate(G, size, 0, chosen, stats, witness)) {
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

Graph make_sample_graph() {
    Graph G(5);

    G.labels = {"A", "B", "C", "D", "E"};

    G.add_edge(0, 1); // A-B
    G.add_edge(0, 2); // A-C
    G.add_edge(1, 2); // B-C
    G.add_edge(2, 3); // C-D
    G.add_edge(3, 4); // D-E

    return G;
}

Graph make_two_triangles_with_bridge() {
    Graph G(6);

    G.labels = {"A", "B", "C", "D", "E", "F"};

    G.add_edge(0, 1);
    G.add_edge(0, 2);
    G.add_edge(1, 2);

    G.add_edge(3, 4);
    G.add_edge(3, 5);
    G.add_edge(4, 5);

    G.add_edge(2, 3);

    return G;
}

vector<int> make_subset_sum_numbers(int n) {
    vector<int> numbers;

    for (int i = 1; i <= n; i++) {
        numbers.push_back(2 * i + 1);
    }

    return numbers;
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

long long two_to_the(int n) {
    if (n >= 62) {
        return -1;
    }

    return 1LL << n;
}

long long choose_count(int n, int k) {
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