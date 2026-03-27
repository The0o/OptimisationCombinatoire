
#include "graphs/graphListAdj.hpp"
#include "graphs/graphAdjMatrix.hpp"
#include "graphs/graphHeavy.hpp"

#include <iostream>
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

    std::cout << "Benchmark Q1" << std::endl;
    run_benchmark_Q1_discrete(instances, "bench_Q1.csv");

    std::cout << "Benchmark Q2" << std::endl;
    run_benchmark_Q2_gradient(instances, "bench_Q2.csv");

    std::cout << "Benchmark Q3" << std::endl;
    run_benchmark_Q3_weighted(instances, "bench_Q3.csv");

    return 0;
}
