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

bool is_vertex_cover(const Graph& G, const vector<int>& cover) {
    set<int> chosen(cover.begin(), cover.end());

    for (const auto& edge : G.edges) {
        int u = edge.first;
        int v = edge.second;

        if (chosen.count(u) == 0 && chosen.count(v) == 0) {
            return false;
        }
    }

    return true;
}

bool is_matching(const Graph& G, const vector<pair<int, int>>& matching) {
    set<int> used_vertices;
    set<pair<int, int>> seen_edges;

    for (const auto& edge : matching) {
        int u = edge.first;
        int v = edge.second;

        if (!G.has_edge(u, v)) {
            return false;
        }

        pair<int, int> normalized = normalize_edge(u, v);

        if (seen_edges.count(normalized) > 0) {
            return false;
        }

        if (used_vertices.count(u) > 0 || used_vertices.count(v) > 0) {
            return false;
        }

        seen_edges.insert(normalized);
        used_vertices.insert(u);
        used_vertices.insert(v);
    }

    return true;
}

bool is_maximal_matching(const Graph& G, const vector<pair<int, int>>& matching) {
    if (!is_matching(G, matching)) {
        return false;
    }

    set<int> used_vertices;

    for (const auto& edge : matching) {
        used_vertices.insert(edge.first);
        used_vertices.insert(edge.second);
    }

    for (const auto& edge : G.edges) {
        int u = edge.first;
        int v = edge.second;

        if (used_vertices.count(u) == 0 && used_vertices.count(v) == 0) {
            return false;
        }
    }

    return true;
}

static bool generate_cover_of_size(
    const Graph& G,
    int target_size,
    int start,
    vector<int>& chosen,
    ExactVCResult& result
) {
    if (static_cast<int>(chosen.size()) == target_size) {
        result.candidates_checked++;

        set<int> chosen_set(chosen.begin(), chosen.end());
        bool covers_all_edges = true;

        for (const auto& edge : G.edges) {
            result.edge_checks++;

            int u = edge.first;
            int v = edge.second;

            if (chosen_set.count(u) == 0 && chosen_set.count(v) == 0) {
                covers_all_edges = false;
                break;
            }
        }

        if (covers_all_edges) {
            result.cover = chosen;
            return true;
        }

        return false;
    }

    for (int v = start; v < G.n; v++) {
        chosen.push_back(v);

        if (generate_cover_of_size(G, target_size, v + 1, chosen, result)) {
            return true;
        }

        chosen.pop_back();
    }

    return false;
}

ExactVCResult brute_force_min_vertex_cover(const Graph& G) {
    ExactVCResult result;
    result.candidates_checked = 0;
    result.edge_checks = 0;
    result.cover.clear();

    for (int size = 0; size <= G.n; size++) {
        vector<int> chosen;

        if (generate_cover_of_size(G, size, 0, chosen, result)) {
            return result;
        }
    }

    return result;
}

ApproxVCResult approximate_vertex_cover_by_matching(const Graph& G) {
    ApproxVCResult result;
    result.edges_scanned = 0;
    result.cover.clear();
    result.matching.clear();

    set<int> used_vertices;
    set<int> cover_set;

    for (const auto& edge : G.edges) {
        result.edges_scanned++;

        int u = edge.first;
        int v = edge.second;

        if (used_vertices.count(u) == 0 && used_vertices.count(v) == 0) {
            result.matching.push_back(edge);

            used_vertices.insert(u);
            used_vertices.insert(v);

            cover_set.insert(u);
            cover_set.insert(v);
        }
    }

    result.cover = vector<int>(cover_set.begin(), cover_set.end());

    return result;
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

Graph make_star_graph(int leaves) {
    Graph G(leaves + 1);

    G.labels[0] = "center";

    for (int i = 1; i <= leaves; i++) {
        G.labels[i] = "leaf" + to_string(i);
        G.add_edge(0, i);
    }

    return G;
}

Graph make_ladder_graph(int rungs) {
    Graph G(2 * rungs);

    for (int i = 0; i < rungs; i++) {
        G.labels[i] = "top" + to_string(i);
        G.labels[i + rungs] = "bot" + to_string(i);
    }

    for (int i = 0; i < rungs; i++) {
        G.add_edge(i, i + rungs);
    }

    for (int i = 0; i < rungs - 1; i++) {
        G.add_edge(i, i + 1);
        G.add_edge(i + rungs, i + rungs + 1);
    }

    return G;
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

Graph make_sparse_deterministic_graph(int n) {
    Graph G(n);

    for (int i = 0; i < n - 1; i++) {
        G.add_edge(i, i + 1);
    }

    for (int i = 0; i + 3 < n; i += 3) {
        G.add_edge(i, i + 3);
    }

    return G;
}

Graph make_dense_deterministic_graph(int n) {
    Graph G(n);

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if ((u + v) % 3 != 0) {
                G.add_edge(u, v);
            }
        }
    }

    return G;
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

void print_edge_set(const vector<pair<int, int>>& edges, const Graph& G) {
    cout << "{";

    for (int i = 0; i < static_cast<int>(edges.size()); i++) {
        int u = edges[i].first;
        int v = edges[i].second;

        cout << "(" << G.labels[u] << ", " << G.labels[v] << ")";

        if (i + 1 < static_cast<int>(edges.size())) {
            cout << ", ";
        }
    }

    cout << "}";
}