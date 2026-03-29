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
Par défaut, les expérimentations des questions `1`, `2`, `3`, `4` et `5` seront toutes exécutées.
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

`bench_Q2.csv` : résultat des algorithmes gradient projetés
- adaptatif : alpha adaptatif
- fixe : alpha fixe

`bench_Q3.csv` : résultat des algorithmes pondérés
- FI : First Improvement
- FI_SUMMARY : Temps d'exécution moyen
- HC : Hill Climber
- HC_SUMMARY : Temps d'exécution moyen
- Best_Static : Best Improvement Static
- Best_Dynamic : Best Improvement Dynamic

`bench_Q4.csv` : résultat des hill climbers hybrides
- `HC_Hybrid_Steepest` : Hill Climber hybride MCP (non pondéré)
- `HC_Hybrid_Weight_Steepest` : Hill Climber hybride WMCP (pondéré)

`bench_Q5.csv` : résultat des heuristiques de recherche locale ILS/VNS
- Colonnes : `Instance;Probleme;Algorithme;Score;Temps_ms;Statut`
- `Probleme` : `MCP` (taille clique) ou `WMCP` (poids clique)
- `ILS_k1` à `ILS_k4` : Iterated Local Search avec perturbation de taille k
- `VNS_kmax5` : Variable Neighborhood Search avec k variant de 1 à 5
- Budget temps : 10 secondes par algorithme par instance

# Régénérer les graphiques Q5

Après avoir obtenu `bench_Q5.csv`, les graphiques peuvent être régénérés avec :

```bash
cd /chemin/vers/projet
python3 code/gen_charts_q5.py
```

Prérequis Python : `pip install matplotlib`

