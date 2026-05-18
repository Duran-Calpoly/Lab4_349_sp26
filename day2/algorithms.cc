#include "algorithms.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

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

Graph complement_graph(const Graph& G, long long& pair_checks, long long& edges_added) {
    Graph H(G.n);
    H.labels = G.labels;

    pair_checks = 0;
    edges_added = 0;

    for (int u = 0; u < G.n; u++) {
        for (int v = u + 1; v < G.n; v++) {
            pair_checks++;

            if (!G.has_edge(u, v)) {
                H.add_edge(u, v);
                edges_added++;
            }
        }
    }

    return H;
}

DecisionInstance reduce_independent_set_to_vertex_cover(const DecisionInstance& instance) {
    DecisionInstance reduced;
    reduced.graph = instance.graph;
    reduced.k = instance.graph.n - instance.k;
    return reduced;
}

DecisionInstance reduce_independent_set_to_clique(
    const DecisionInstance& instance,
    ReductionStats& stats
) {
    long long pair_checks = 0;
    long long edges_added = 0;

    Graph H = complement_graph(instance.graph, pair_checks, edges_added);

    DecisionInstance reduced;
    reduced.graph = H;
    reduced.k = instance.k;

    stats.original_vertices = instance.graph.n;
    stats.original_edges = instance.graph.edge_count();
    stats.original_k = instance.k;

    stats.transformed_vertices = reduced.graph.n;
    stats.transformed_edges = reduced.graph.edge_count();
    stats.transformed_k = reduced.k;

    stats.pair_checks = pair_checks;
    stats.edges_added = edges_added;

    return reduced;
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

static bool brute_force_choose(
    const Graph& G,
    int k,
    int start,
    vector<int>& chosen,
    bool (*predicate)(const Graph&, const vector<int>&)
) {
    if (static_cast<int>(chosen.size()) == k) {
        return predicate(G, chosen);
    }

    for (int v = start; v < G.n; v++) {
        chosen.push_back(v);

        if (brute_force_choose(G, k, v + 1, chosen, predicate)) {
            return true;
        }

        chosen.pop_back();
    }

    return false;
}

bool brute_force_independent_set(const Graph& G, int k) {
    if (k < 0) {
        return false;
    }
    if (k == 0) {
        return true;
    }
    if (k > G.n) {
        return false;
    }

    vector<int> chosen;
    return brute_force_choose(G, k, 0, chosen, is_independent_set);
}

bool brute_force_clique(const Graph& G, int k) {
    if (k < 0) {
        return false;
    }
    if (k == 0) {
        return true;
    }
    if (k > G.n) {
        return false;
    }

    vector<int> chosen;
    return brute_force_choose(G, k, 0, chosen, is_clique);
}

bool brute_force_vertex_cover(const Graph& G, int k) {
    if (k < 0) {
        return false;
    }
    if (k >= G.n) {
        return true;
    }

    for (int size = 0; size <= k; size++) {
        vector<int> chosen;

        if (brute_force_choose(G, size, 0, chosen, is_vertex_cover)) {
            return true;
        }
    }

    return false;
}

vector<int> set_complement(int n, const vector<int>& vertices) {
    set<int> chosen(vertices.begin(), vertices.end());
    vector<int> result;

    for (int v = 0; v < n; v++) {
        if (chosen.count(v) == 0) {
            result.push_back(v);
        }
    }

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

void print_graph_summary(const Graph& G) {
    cout << "vertices = " << G.n
         << ", edges = " << G.edge_count()
         << endl;
}

void print_instance_summary(const string& name, const DecisionInstance& instance) {
    cout << name << ": ";
    print_graph_summary(instance.graph);
    cout << "k = " << instance.k << endl;
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