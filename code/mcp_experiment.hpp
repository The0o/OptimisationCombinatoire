#pragma once

#include "graph.hpp"

void run_mcp_experiments(const Graph* g);
void run_benchmark_Q1_discrete(const std::vector<std::string>& instances, const std::string& output_filename);
void run_benchmark_Q2_gradient(const std::vector<std::string>& instances, const std::string& output_filename);
void run_benchmark_Q3_weighted(const std::vector<std::string>& instances, const std::string& output_filename);
void run_benchmark_Q4_hybrid(const std::vector<std::string>& instances, const std::string& output_filename);
void run_benchmark_Q5_local_search(const std::vector<std::string>& instances, const std::string& output_filename);