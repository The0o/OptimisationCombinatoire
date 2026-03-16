#include "mcp.hpp"

#include <algorithm>
#include <chrono>
#include <numeric>

namespace {

struct Move {
    vertex inserted = -1;
    vertex removed = -1;
    std::vector<vertex> clique;
};

bool contains_vertex(const std::vector<vertex> &clique, vertex v) {
    return std::find(clique.begin(), clique.end(), v) != clique.end();
}

std::vector<vertex> feasible_candidates(
    const Graph &graph,
    const std::vector<vertex> &clique
) {
    std::vector<vertex> candidates;
    candidates.reserve(graph.nb_vertices());

    for (vertex v = 0; v < graph.nb_vertices(); ++v) {
        if (contains_vertex(clique, v)) {
            continue;
        }

        bool feasible = true;
        for (vertex u : clique) {
            if (!graph.is_edge(u, v)) {
                feasible = false;
                break;
            }
        }

        if (feasible) {
            candidates.push_back(v);
        }
    }

    return candidates;
}

void greedy_completion(
    const Graph &graph,
    std::vector<vertex> &clique,
    std::mt19937 &rng
) {
    while (true) {
        std::vector<vertex> candidates = feasible_candidates(graph, clique);
        if (candidates.empty()) {
            break;
        }

        std::shuffle(candidates.begin(), candidates.end(), rng);
        std::stable_sort(
            candidates.begin(),
            candidates.end(),
            [&graph](vertex lhs, vertex rhs) {
                return graph.degree(lhs) > graph.degree(rhs);
            }
        );
        clique.push_back(candidates.front());
    }

    std::sort(clique.begin(), clique.end());
}

std::vector<vertex> random_initial_clique(
    const Graph &graph,
    std::mt19937 &rng
) {
    std::vector<vertex> clique;
    std::uniform_int_distribution<vertex> dist(0, graph.nb_vertices() - 1);
    clique.push_back(dist(rng));
    greedy_completion(graph, clique, rng);
    return clique;
}

Move evaluate_move(
    const Graph &graph,
    const std::vector<vertex> &current,
    vertex inserted,
    vertex removed,
    std::mt19937 &rng
) {
    Move move;
    move.inserted = inserted;
    move.removed = removed;

    move.clique.reserve(current.size() + 1);
    for (vertex v : current) {
        if (v != removed) {
            move.clique.push_back(v);
        }
    }
    move.clique.push_back(inserted);
    greedy_completion(graph, move.clique, rng);
    return move;
}

std::vector<Move> improving_moves(
    const Graph &graph,
    const std::vector<vertex> &current,
    std::mt19937 &rng
) {
    std::vector<Move> moves;

    for (vertex candidate = 0; candidate < graph.nb_vertices(); ++candidate) {
        if (contains_vertex(current, candidate)) {
            continue;
        }

        std::vector<vertex> conflicts;
        for (vertex v : current) {
            if (!graph.is_edge(v, candidate)) {
                conflicts.push_back(v);
                if (conflicts.size() > 1) {
                    break;
                }
            }
        }

        if (conflicts.empty()) {
            Move move = evaluate_move(graph, current, candidate, -1, rng);
            if (static_cast<int>(move.clique.size()) > static_cast<int>(current.size())) {
                moves.push_back(std::move(move));
            }
        } else if (conflicts.size() == 1) {
            Move move = evaluate_move(graph, current, candidate, conflicts.front(), rng);
            if (static_cast<int>(move.clique.size()) > static_cast<int>(current.size())) {
                moves.push_back(std::move(move));
            }
        }
    }

    return moves;
}

bool improving_move_for_candidate(
    const Graph &graph,
    const std::vector<vertex> &current,
    vertex candidate,
    std::mt19937 &rng,
    Move &move
) {
    std::vector<vertex> conflicts;
    for (vertex v : current) {
        if (!graph.is_edge(v, candidate)) {
            conflicts.push_back(v);
            if (conflicts.size() > 1) {
                return false;
            }
        }
    }

    if (conflicts.empty()) {
        move = evaluate_move(graph, current, candidate, -1, rng);
        return static_cast<int>(move.clique.size()) > static_cast<int>(current.size());
    }

    move = evaluate_move(graph, current, candidate, conflicts.front(), rng);
    return static_cast<int>(move.clique.size()) > static_cast<int>(current.size());
}

bool select_move(
    const Graph &graph,
    const std::vector<vertex> &current,
    PivotRule rule,
    std::mt19937 &rng,
    Move &selected
) {
    std::vector<vertex> candidates(graph.nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);
    candidates.erase(
        std::remove_if(
            candidates.begin(),
            candidates.end(),
            [&current](vertex v) { return contains_vertex(current, v); }
        ),
        candidates.end()
    );

    if (rule == PivotRule::RandomImprovement) {
        std::shuffle(candidates.begin(), candidates.end(), rng);
    }

    if (rule == PivotRule::BestImprovement) {
        std::vector<Move> moves = improving_moves(graph, current, rng);
        if (moves.empty()) {
            return false;
        }

        auto best_it = std::max_element(
            moves.begin(),
            moves.end(),
            [](const Move &lhs, const Move &rhs) {
                return lhs.clique.size() < rhs.clique.size();
            }
        );
        selected = std::move(*best_it);
        return true;
    }

    for (vertex candidate : candidates) {
        Move move;
        if (improving_move_for_candidate(graph, current, candidate, rng, move)) {
            selected = std::move(move);
            return true;
        }
    }

    return false;
}

}  // namespace

std::string pivot_rule_name(PivotRule rule) {
    switch (rule) {
        case PivotRule::FirstImprovement:
            return "first";
        case PivotRule::BestImprovement:
            return "best";
        case PivotRule::RandomImprovement:
            return "random";
    }

    return "unknown";
}

MCPRunResult local_search_max_clique(
    const Graph &graph,
    PivotRule rule,
    std::mt19937 &rng
) {
    const auto start = std::chrono::steady_clock::now();
    std::vector<vertex> current = random_initial_clique(graph, rng);
    int iterations = 0;

    while (true) {
        ++iterations;
        Move selected;
        if (!select_move(graph, current, rule, rng, selected)) {
            break;
        }
        current = std::move(selected.clique);
    }

    const auto end = std::chrono::steady_clock::now();
    const double elapsed_ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    MCPRunResult result;
    result.clique = std::move(current);
    result.iterations = iterations;
    result.elapsed_ms = elapsed_ms;
    return result;
}
