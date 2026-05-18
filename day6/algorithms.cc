#include "algorithms.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <set>

using namespace std;

WeightedGraph::WeightedGraph() : n(0) {}

WeightedGraph::WeightedGraph(int num_vertices) : n(num_vertices) {
    labels.resize(n);
    weight.assign(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        labels[i] = "v" + to_string(i);
    }
}

void WeightedGraph::set_weight(int u, int v, int w) {
    if (u < 0 || v < 0 || u >= n || v >= n || u == v) {
        return;
    }

    weight[u][v] = w;
    weight[v][u] = w;
}

int WeightedGraph::get_weight(int u, int v) const {
    if (u < 0 || v < 0 || u >= n || v >= n) {
        return numeric_limits<int>::max() / 4;
    }

    return weight[u][v];
}

int tour_cost(const WeightedGraph& G, const vector<int>& tour) {
    if (tour.empty()) {
        return 0;
    }

    int total = 0;

    for (int i = 0; i + 1 < static_cast<int>(tour.size()); i++) {
        total += G.get_weight(tour[i], tour[i + 1]);
    }

    total += G.get_weight(tour.back(), tour.front());

    return total;
}

bool is_hamiltonian_cycle(const WeightedGraph& G, const vector<int>& tour) {
    if (static_cast<int>(tour.size()) != G.n) {
        return false;
    }

    set<int> seen;

    for (int v : tour) {
        if (v < 0 || v >= G.n) {
            return false;
        }

        seen.insert(v);
    }

    return static_cast<int>(seen.size()) == G.n;
}

TSPResult brute_force_tsp(const WeightedGraph& G) {
    TSPResult result;
    result.cost = numeric_limits<int>::max();
    result.candidates_checked = 0;
    result.tour.clear();

    if (G.n == 0) {
        result.cost = 0;
        return result;
    }

    vector<int> vertices;

    for (int v = 1; v < G.n; v++) {
        vertices.push_back(v);
    }

    do {
        vector<int> tour;
        tour.push_back(0);

        for (int v : vertices) {
            tour.push_back(v);
        }

        result.candidates_checked++;

        int cost = tour_cost(G, tour);

        if (cost < result.cost) {
            result.cost = cost;
            result.tour = tour;
        }

    } while (next_permutation(vertices.begin(), vertices.end()));

    return result;
}

MSTResult prim_mst(const WeightedGraph& G) {
    MSTResult result;
    result.cost = 0;
    result.edges.clear();

    if (G.n == 0) {
        return result;
    }

    vector<bool> in_tree(G.n, false);
    vector<int> key(G.n, numeric_limits<int>::max());
    vector<int> parent(G.n, -1);

    key[0] = 0;

    for (int step = 0; step < G.n; step++) {
        int u = -1;

        for (int v = 0; v < G.n; v++) {
            if (!in_tree[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        in_tree[u] = true;

        if (parent[u] != -1) {
            result.edges.push_back({parent[u], u});
            result.cost += G.get_weight(parent[u], u);
        }

        for (int v = 0; v < G.n; v++) {
            if (!in_tree[v] && u != v && G.get_weight(u, v) < key[v]) {
                key[v] = G.get_weight(u, v);
                parent[v] = u;
            }
        }
    }

    return result;
}

static void preorder_dfs(
    int u,
    const vector<vector<int>>& tree_adj,
    vector<bool>& visited,
    vector<int>& order
) {
    visited[u] = true;
    order.push_back(u);

    for (int v : tree_adj[u]) {
        if (!visited[v]) {
            preorder_dfs(v, tree_adj, visited, order);
        }
    }
}

TSPResult approximate_metric_tsp_double_tree(const WeightedGraph& G) {
    TSPResult result;
    result.cost = 0;
    result.candidates_checked = 0;
    result.tour.clear();

    if (G.n == 0) {
        return result;
    }

    MSTResult mst = prim_mst(G);

    vector<vector<int>> tree_adj(G.n);

    for (const auto& edge : mst.edges) {
        int u = edge.first;
        int v = edge.second;

        tree_adj[u].push_back(v);
        tree_adj[v].push_back(u);
    }

    for (int u = 0; u < G.n; u++) {
        sort(tree_adj[u].begin(), tree_adj[u].end());
    }

    vector<bool> visited(G.n, false);
    vector<int> preorder;

    preorder_dfs(0, tree_adj, visited, preorder);

    result.tour = preorder;
    result.cost = tour_cost(G, result.tour);

    return result;
}

bool satisfies_triangle_inequality(const WeightedGraph& G) {
    for (int a = 0; a < G.n; a++) {
        for (int b = 0; b < G.n; b++) {
            for (int c = 0; c < G.n; c++) {
                if (G.get_weight(a, c) > G.get_weight(a, b) + G.get_weight(b, c)) {
                    return false;
                }
            }
        }
    }

    return true;
}

WeightedGraph make_line_metric_graph(int n) {
    WeightedGraph G(n);

    for (int i = 0; i < n; i++) {
        G.labels[i] = "p" + to_string(i);
    }

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            G.set_weight(u, v, abs(u - v));
        }
    }

    return G;
}

WeightedGraph make_circle_metric_graph(int n) {
    WeightedGraph G(n);

    for (int i = 0; i < n; i++) {
        G.labels[i] = "c" + to_string(i);
    }

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            int clockwise = abs(u - v);
            int counter_clockwise = n - clockwise;
            G.set_weight(u, v, min(clockwise, counter_clockwise));
        }
    }

    return G;
}

WeightedGraph make_grid_metric_graph(int rows, int cols) {
    int n = rows * cols;
    WeightedGraph G(n);

    vector<pair<int, int>> points;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int index = r * cols + c;
            G.labels[index] = "(" + to_string(r) + "," + to_string(c) + ")";
            points.push_back({r, c});
        }
    }

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            int dist = abs(points[u].first - points[v].first)
                     + abs(points[u].second - points[v].second);
            G.set_weight(u, v, dist);
        }
    }

    return G;
}

WeightedGraph make_sample_metric_graph() {
    WeightedGraph G(5);

    G.labels = {"A", "B", "C", "D", "E"};

    vector<pair<int, int>> points = {
        {0, 0},
        {2, 0},
        {4, 0},
        {1, 3},
        {3, 3}
    };

    for (int u = 0; u < G.n; u++) {
        for (int v = u + 1; v < G.n; v++) {
            int dist = abs(points[u].first - points[v].first)
                     + abs(points[u].second - points[v].second);
            G.set_weight(u, v, dist);
        }
    }

    return G;
}

bool covers_universe(const SetCoverInstance& instance, const vector<int>& chosen_sets) {
    set<int> covered;

    for (int index : chosen_sets) {
        if (index < 0 || index >= static_cast<int>(instance.sets.size())) {
            return false;
        }

        for (int element : instance.sets[index]) {
            covered.insert(element);
        }
    }

    for (int element = 0; element < instance.universe_size; element++) {
        if (covered.count(element) == 0) {
            return false;
        }
    }

    return true;
}

SetCoverResult brute_force_set_cover(const SetCoverInstance& instance) {
    SetCoverResult result;
    result.cover_size = 0;
    result.candidates_checked = 0;
    result.chosen_sets.clear();

    int m = static_cast<int>(instance.sets.size());

    if (m >= 62) {
        return result;
    }

    for (int size = 0; size <= m; size++) {
        long long total_masks = 1LL << m;

        for (long long mask = 0; mask < total_masks; mask++) {
            if (__builtin_popcountll(mask) != size) {
                continue;
            }

            result.candidates_checked++;

            vector<int> candidate;

            for (int i = 0; i < m; i++) {
                if ((mask >> i) & 1LL) {
                    candidate.push_back(i);
                }
            }

            if (covers_universe(instance, candidate)) {
                result.chosen_sets = candidate;
                result.cover_size = static_cast<int>(candidate.size());
                return result;
            }
        }
    }

    return result;
}

SetCoverResult greedy_set_cover(const SetCoverInstance& instance) {
    SetCoverResult result;
    result.cover_size = 0;
    result.candidates_checked = 0;
    result.chosen_sets.clear();

    set<int> covered;
    set<int> chosen;

    while (static_cast<int>(covered.size()) < instance.universe_size) {
        int best_set = -1;
        int best_gain = -1;

        for (int i = 0; i < static_cast<int>(instance.sets.size()); i++) {
            if (chosen.count(i) > 0) {
                continue;
            }

            result.candidates_checked++;

            int gain = 0;

            for (int element : instance.sets[i]) {
                if (covered.count(element) == 0) {
                    gain++;
                }
            }

            if (gain > best_gain) {
                best_gain = gain;
                best_set = i;
            }
        }

        if (best_set == -1 || best_gain == 0) {
            break;
        }

        chosen.insert(best_set);
        result.chosen_sets.push_back(best_set);

        for (int element : instance.sets[best_set]) {
            covered.insert(element);
        }
    }

    result.cover_size = static_cast<int>(result.chosen_sets.size());
    return result;
}

SetCoverInstance make_sample_set_cover() {
    SetCoverInstance instance;
    instance.universe_size = 6;

    instance.sets = {
        {0, 1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {0, 5},
        {1, 3, 5}
    };

    instance.set_names = {"A", "B", "C", "D", "E", "F"};

    return instance;
}

SetCoverInstance make_tricky_set_cover() {
    SetCoverInstance instance;
    instance.universe_size = 8;

    instance.sets = {
        {0, 1, 2, 3},
        {0, 1, 4},
        {2, 3, 5},
        {4, 5, 6, 7},
        {6},
        {7}
    };

    instance.set_names = {"BigEarly", "LeftMix", "RightMix", "Tail", "Only6", "Only7"};

    return instance;
}

SetCoverInstance make_layered_set_cover(int layers) {
    SetCoverInstance instance;
    instance.universe_size = 2 * layers;

    for (int i = 0; i < layers; i++) {
        set<int> local_pair = {2 * i, 2 * i + 1};
        instance.sets.push_back(local_pair);
        instance.set_names.push_back("pair" + to_string(i));
    }

    set<int> evens;
    set<int> odds;

    for (int i = 0; i < layers; i++) {
        evens.insert(2 * i);
        odds.insert(2 * i + 1);
    }

    instance.sets.push_back(evens);
    instance.set_names.push_back("all_evens");

    instance.sets.push_back(odds);
    instance.set_names.push_back("all_odds");

    return instance;
}

SetCoverInstance make_dense_set_cover(int universe_size) {
    SetCoverInstance instance;
    instance.universe_size = universe_size;

    set<int> first_half;
    set<int> second_half;
    set<int> evens;
    set<int> odds;

    for (int i = 0; i < universe_size; i++) {
        if (i < universe_size / 2) {
            first_half.insert(i);
        } else {
            second_half.insert(i);
        }

        if (i % 2 == 0) {
            evens.insert(i);
        } else {
            odds.insert(i);
        }
    }

    instance.sets.push_back(first_half);
    instance.set_names.push_back("first_half");

    instance.sets.push_back(second_half);
    instance.set_names.push_back("second_half");

    instance.sets.push_back(evens);
    instance.set_names.push_back("evens");

    instance.sets.push_back(odds);
    instance.set_names.push_back("odds");

    for (int i = 0; i < universe_size; i++) {
        instance.sets.push_back({i});
        instance.set_names.push_back("single_" + to_string(i));
    }

    return instance;
}

void print_tour(const vector<int>& tour, const WeightedGraph& G) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(tour.size()); i++) {
        int v = tour[i];

        if (v >= 0 && v < G.n) {
            cout << G.labels[v];
        } else {
            cout << v;
        }

        if (i + 1 < static_cast<int>(tour.size())) {
            cout << " -> ";
        }
    }

    if (!tour.empty()) {
        cout << " -> " << G.labels[tour[0]];
    }

    cout << "]";
}

void print_mst_edges(const MSTResult& mst, const WeightedGraph& G) {
    cout << "{";

    for (int i = 0; i < static_cast<int>(mst.edges.size()); i++) {
        int u = mst.edges[i].first;
        int v = mst.edges[i].second;

        cout << "(" << G.labels[u] << ", " << G.labels[v] << ")";

        if (i + 1 < static_cast<int>(mst.edges.size())) {
            cout << ", ";
        }
    }

    cout << "}";
}

void print_set_cover_solution(const SetCoverInstance& instance, const vector<int>& chosen_sets) {
    cout << "{";

    for (int i = 0; i < static_cast<int>(chosen_sets.size()); i++) {
        int index = chosen_sets[i];

        if (index >= 0 && index < static_cast<int>(instance.set_names.size())) {
            cout << instance.set_names[index];
        } else {
            cout << "S" << index;
        }

        if (i + 1 < static_cast<int>(chosen_sets.size())) {
            cout << ", ";
        }
    }

    cout << "}";
}

void print_set(const set<int>& s) {
    cout << "{";

    int count = 0;

    for (int element : s) {
        cout << element;
        count++;

        if (count < static_cast<int>(s.size())) {
            cout << ", ";
        }
    }

    cout << "}";
}