#include "graphs/graphListAdj.hpp"
#include "graphs/graphAdjMatrix.hpp"
#include "graphs/graphHeavy.hpp"

#include <iostream>
#include <chrono>   // Pour mesurer le temps
#include <iomanip>  // Pour formater le tableau (std::setw)
#include "utils.hpp"
#include "mcp_experiment.hpp"

using namespace std;

void print_vector(vector<vertex> vec){
    for (auto v : vec) {
        cout << " " << v;
    }
    cout << endl;
}

int main(int argc , char* argv [])
{
    vector<string> instances = {
        "brock/brock200_1.clq", "brock/brock200_2.clq", "brock/brock200_3.clq", "brock/brock200_4.clq",
        "brock/brock400_1.clq", "brock/brock400_2.clq", "brock/brock400_3.clq", "brock/brock400_4.clq",
        "brock/brock800_1.clq", "brock/brock800_2.clq", "brock/brock800_3.clq", "brock/brock800_4.clq",
        "C/C125.9.clq", "C/C250.9.clq", "C/C500.9.clq", "C/C1000.9.clq", 
        "C/C2000.5.clq", "C/C2000.9.clq", "C/C4000.5.clq",
        "c-fat/c-fat200-1.clq", "c-fat/c-fat200-2.clq", "c-fat/c-fat200-5.clq",
        "c-fat/c-fat500-1.clq", "c-fat/c-fat500-2.clq", "c-fat/c-fat500-5.clq", "c-fat/c-fat500-10.clq",
        "gen/gen200_p0.9_44.clq", "gen/gen200_p0.9_55.clq",
        "gen/gen400_p0.9_55.clq", "gen/gen400_p0.9_65.clq", "gen/gen400_p0.9_75.clq",
        "hamming/hamming6-2.clq", "hamming/hamming6-4.clq",
        "hamming/hamming8-2.clq", "hamming/hamming8-4.clq",
        "hamming/hamming10-2.clq", "hamming/hamming10-4.clq",
        "johnson/johnson8-2-4.clq", "johnson/johnson8-4-4.clq",
        "johnson/johnson16-2-4.clq", "johnson/johnson32-2-4.clq",
        "keller/keller4.clq", "keller/keller5.clq", "keller/keller6.clq",
        "MANN_a/MANN_a9.clq", "MANN_a/MANN_a27.clq", "MANN_a/MANN_a45.clq", "MANN_a/MANN_a81.clq",
        "san/san200_0.7_1.clq", "san/san200_0.7_2.clq", "san/san200_0.9_1.clq", 
        "san/san200_0.9_2.clq", "san/san200_0.9_3.clq", "san/san400_0.5_1.clq", 
        "san/san400_0.7_1.clq", "san/san400_0.7_2.clq", "san/san400_0.7_3.clq", 
        "san/san400_0.9_1.clq", "san/san1000.clq", "san/sanr200_0.7.clq", 
        "san/sanr200_0.9.clq", "san/sanr400_0.5.clq", "san/sanr400_0.7.clq"
    };

    double t_q1 = 0, t_q2 = 0, t_q3 = 0, t_q4 = 0, t_q5 = 0;

    // --- BENCHMARK Q1 ---
    std::cout << "\n=========================================\n";
    std::cout << "Lancement Benchmark Q1" << std::endl;
    auto s1 = std::chrono::high_resolution_clock::now();
    run_benchmark_Q1_discrete(instances, "../code/bench_Q1.csv");
    auto e1 = std::chrono::high_resolution_clock::now();
    t_q1 = std::chrono::duration<double>(e1 - s1).count(); // Temps en secondes

    // --- BENCHMARK Q2 ---
    std::cout << "\n=========================================\n";
    std::cout << "Lancement Benchmark Q2" << std::endl;
    auto s2 = std::chrono::high_resolution_clock::now();
    run_benchmark_Q2_gradient(instances, "../code/bench_Q2.csv");
    auto e2 = std::chrono::high_resolution_clock::now();
    t_q2 = std::chrono::duration<double>(e2 - s2).count();

    // --- BENCHMARK Q3 ---
    std::cout << "\n=========================================\n";
    std::cout << "Lancement Benchmark Q3" << std::endl;
    auto s3 = std::chrono::high_resolution_clock::now();
    run_benchmark_Q3_weighted(instances, "../code/bench_Q3.csv");
    auto e3 = std::chrono::high_resolution_clock::now();
    t_q3 = std::chrono::duration<double>(e3 - s3).count();

    // --- BENCHMARK Q4 ---
    std::cout << "\n=========================================\n";
    std::cout << "Lancement Benchmark Q4 (Hybride)" << std::endl;
    auto s4 = std::chrono::high_resolution_clock::now();
    run_benchmark_Q4_hybrid(instances, "../code/bench_Q4.csv");
    auto e4 = std::chrono::high_resolution_clock::now();
    t_q4 = std::chrono::duration<double>(e4 - s4).count();


    // --- BENCHMARK Q5 ---
    std::cout << "\n=========================================\n";
    std::cout << "Lancement Benchmark Q5 (Recherche Locale ILS/VNS)" << std::endl;
    auto s5 = std::chrono::high_resolution_clock::now();
    run_benchmark_Q5_local_search(instances, "../code/bench_Q5.csv");
    auto e5 = std::chrono::high_resolution_clock::now();
    t_q5 = std::chrono::duration<double>(e5 - s5).count();

    // --- AFFICHAGE DU TABLEAU RECAPITULATIF ---
    std::cout << "\n\n";
    std::cout << "====================================================\n";
    std::cout << "             RESUME DES TEMPS D'EXECUTION           \n";
    std::cout << "----------------------------------------------------\n";
    std::cout << std::left << std::setw(25) << "Benchmark" << "| " << "Temps total (en secondes)\n";
    std::cout << "----------------------------------------------------\n";
    std::cout << std::left << std::setw(25) << "Q1 (Discret)" << "| " << t_q1 << " s\n";
    std::cout << std::left << std::setw(25) << "Q2 (Gradient projete)" << "| " << t_q2 << " s\n";
    std::cout << std::left << std::setw(25) << "Q3 (Pondere)" << "| " << t_q3 << " s\n";
    std::cout << std::left << std::setw(25) << "Q4 (Hybride)" << "| " << t_q4 << " s\n";
    std::cout << std::left << std::setw(25) << "Q5 (ILS/VNS)" << "| " << t_q5 << " s\n";
    std::cout << "====================================================\n";

    return 0;
}