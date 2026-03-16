#include "graphs/graphListAdj.hpp"
#include "mcp.hpp"
#include "utils.hpp"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace {

struct Options {
    int runs = 30;
    std::vector<std::string> instances;
};

Options parse_options(int argc, char *argv[]) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--runs") {
            if (i + 1 >= argc) {
                throw std::runtime_error("missing value after --runs");
            }
            options.runs = std::stoi(argv[++i]);
            continue;
        }

        options.instances.push_back(arg);
    }

    if (options.instances.empty()) {
        options.instances = {
            "C125.9.clq",
            "brock/brock200_1.clq",
            "gen/gen200_p0.9_44.clq",
            "san/san200_0.7_1.clq",
            "san/sanr200_0.7.clq",
            "c-fat/c-fat200-2.clq",
            "johnson/johnson16-2-4.clq",
            "hamming/hamming8-4.clq",
            "MANN_a/MANN_a27.clq"
        };
    }

    return options;
}

MCPExperimentSummary run_summary(
    const std::string &instance,
    PivotRule rule,
    int runs
) {
    std::string filename = instance;
    GraphHeavy graph(filename);

    int best_size = 0;
    double total_size = 0.0;
    double total_iterations = 0.0;
    double total_time_ms = 0.0;

    for (int seed = 0; seed < runs; ++seed) {
        std::mt19937 rng(seed);
        MCPRunResult result = local_search_max_clique(graph, rule, rng);
        if (!is_clique(graph, result.clique)) {
            throw std::runtime_error("invalid clique returned on " + instance);
        }
        const int size = static_cast<int>(result.clique.size());
        best_size = std::max(best_size, size);
        total_size += size;
        total_iterations += result.iterations;
        total_time_ms += result.elapsed_ms;
    }

    MCPExperimentSummary summary;
    summary.instance = instance;
    summary.rule = rule;
    summary.runs = runs;
    summary.best_size = best_size;
    summary.average_size = total_size / runs;
    summary.average_iterations = total_iterations / runs;
    summary.average_time_ms = total_time_ms / runs;
    return summary;
}

void print_summary_header() {
    std::cout
        << "instance,n,m,rule,runs,best_size,avg_size,avg_iterations,avg_time_ms\n";
}

void print_summary_row(const MCPExperimentSummary &summary) {
    std::string filename = summary.instance;
    GraphHeavy graph(filename);

    std::cout
        << summary.instance << ","
        << graph.nb_vertices() << ","
        << graph.nb_edges() << ","
        << pivot_rule_name(summary.rule) << ","
        << summary.runs << ","
        << summary.best_size << ","
        << std::fixed << std::setprecision(2)
        << summary.average_size << ","
        << summary.average_iterations << ","
        << summary.average_time_ms << "\n";
}

}  // namespace

int main(int argc, char *argv[]) {
    try {
        const Options options = parse_options(argc, argv);
        const std::vector<PivotRule> rules = {
            PivotRule::FirstImprovement,
            PivotRule::BestImprovement,
            PivotRule::RandomImprovement
        };

        print_summary_header();
        for (const std::string &instance : options.instances) {
            for (PivotRule rule : rules) {
                print_summary_row(run_summary(instance, rule, options.runs));
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
