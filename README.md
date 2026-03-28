# Optimisation Combinatoire : Maximum Clique Problem (MCP & WMCP)

Ce projet implémente et compare plusieurs algorithmes de recherche locale pour résoudre le problème de la clique maximum non pondérée (MCP) et pondérée (WMCP) dans des graphes.

## Prérequis
Pour compiler et exécuter ce projet, vous aurez besoin de :
- Un compilateur C++ compatible C++11 ou supérieur (GCC, MSVC, Clang).
- **CMake** (version 3.10 ou supérieure).

## Compilation

Ouvrez un terminal à la racine du projet et exécutez les commandes suivantes :

```bash
# 1. Créer le dossier de build (s'il n'existe pas déjà)
mkdir build
cd build

# 2. Générer les fichiers de configuration CMake
cmake ..

# 3. Compiler le projet
cmake --build .
# (Ou utilisez 'mingw32-make' / 'make' selon votre environnement)
```

# Lancer les différentes expérimentations 
Par défaut, les expérimentations des questions `1`, `2`, `3` et `4` seront toutes exécutées.
Pour éviter qu'une expérimentation ne s'exécute, il suffit de commenter la ligne de code qui lance son exécution dans le fichier `/code/main.cpp`.

```bash
#Exemple pour la question 3

// run_benchmark_Q3_weighted(instances, "../code/bench_Q3.csv");
```
# Récupération des résultats
Les résulats des différentes expérimentations sont stockées dans les fichiers `.csv`

`bench_Q1.csv` : résultat des algorithmes non pondérés
- FI : First Improvement
- FI_SUMMARY : Temps d'exécution moyen
- HC : Hill Climber
- HC_SUMMARY : Temps d'exécution moyen
- Best_Static : Best Improvement Static
- Best_Dynamic : Best Improvement Dynamic

`bench_Q2.csv` : résultat des algorithmes non pondérés
- FI : First Improvement
- FI_SUMMARY : Temps d'exécution moyen
- HC : Hill Climber
- HC_SUMMARY : Temps d'exécution moyen
- Best_Static : Best Improvement Static
- Best_Dynamic : Best Improvement Dynamic

`bench_Q3.csv` : résultat des algorithmes non pondérés
- FI : First Improvement
- FI_SUMMARY : Temps d'exécution moyen
- HC : Hill Climber
- HC_SUMMARY : Temps d'exécution moyen
- Best_Static : Best Improvement Static
- Best_Dynamic : Best Improvement Dynamic

`bench_Q4.csv` : résultat des algorithmes non pondérés
- HC_Hybrid_Steepest : Hill Climber Hybrid non pondérés
- WMPC : pondéré