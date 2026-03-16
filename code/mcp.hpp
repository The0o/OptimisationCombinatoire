#pragma once

#include "graph.hpp"

#include <random>
#include <string>
#include <vector>

enum class PivotRule {
    FirstImprovement,
    BestImprovement,
    RandomImprovement
};

struct MCPRunResult {
    std::vector<vertex> clique;
    int iterations = 0;
    double elapsed_ms = 0.0;
};

struct MCPExperimentSummary {
    std::string instance;
    PivotRule rule = PivotRule::FirstImprovement;
    int runs = 0;
    int best_size = 0;
    double average_size = 0.0;
    double average_iterations = 0.0;
    double average_time_ms = 0.0;
};

std::string pivot_rule_name(PivotRule rule);

MCPRunResult local_search_max_clique(
    const Graph &graph,
    PivotRule rule,
    std::mt19937 &rng
);
