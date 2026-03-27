#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>
#include "graph.hpp"
#include "utils.hpp"
#include "weightedgraphDefs.hpp"

// ============= QUESTION 1 =============

//1 FIRST IMPROVEMENT : on prend un point au hasard, on regarde le voisin
//qui forme une clique, on le prend
std::vector<vertex> descent_first_improvement(const Graph* g, int seed = 42) {
    std::vector<vertex> clique;
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    std::mt19937 gen(seed);
    std::shuffle(candidates.begin(), candidates.end(), gen);

    for (vertex v : candidates) {
        bool can_add = true;
        for (vertex u : clique) {
            if (!g->is_edge(u, v)) {
                can_add = false;
                break;
            }
        }
        if (can_add) {
            clique.push_back(v);
        }
    }
    return clique;
}

//2 BEST IMPROVEMENT : on prend un point au hasard, on regarde le voisin
//qui a le plus de voisin et qui forme une clique, on le selectionne, on repete
std::vector<vertex> descent_best_improvement_static(const Graph* g) {
    std::vector<vertex> clique;
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    //On trie par ordre decroissant
    std::sort(candidates.begin(), candidates.end(), [g](vertex a, vertex b) {
        return g->degree(a) > g->degree(b);
    });

    for (vertex v : candidates) {
        bool can_add = true;
        for (vertex u : clique) {
            if (!g->is_edge(u, v)) {
                can_add = false;
                break;
            }
        }
        if (can_add) {
            clique.push_back(v);
        }
    }
    return clique;
}

//3 BEST IMPROVEMENT DYNAMIQUE : meme fonctionnement que le static
//sauf que cette fois, au lieu de regarder le sommet qui a le plus d'aretes,
//on prend le sommet qui a le plus d'aretes parmi les sommets encore valides
std::vector<vertex> descent_best_improvement_dynamic(const Graph* g) {
    std::vector<vertex> clique;
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    while (!candidates.empty()) {
        vertex best_v = candidates[0];
        int max_degree_in_candidates = -1;

        //calcul du nombre de sommet amis parmis les sommets pas dans la clique
        for (vertex v : candidates) {
            int degree = 0;
            for (vertex u : candidates) {
                if (v != u && g->is_edge(v, u)) {
                    degree++;
                }
            }
            if (degree > max_degree_in_candidates) {
                max_degree_in_candidates = degree;
                best_v = v;
            }
        }

        clique.push_back(best_v);

        std::vector<vertex> new_candidates;
        for (vertex v : candidates) {
            if (v != best_v && g->is_edge(best_v, v)) {
                new_candidates.push_back(v);
            }
        }
        candidates = std::move(new_candidates);
    }
    return clique;
}

//HILL CLIMBING : on parcourt les sommets qui sont pas dans la clique,
//si on en trouve un, on le rajoute. On fait ça jusqu'a qu'on puisse plus en rajouter
//a ce moment là, on essaye d'enlever un sommet à la condition qu'on peut en rajouter deux derrieres
std::vector<vertex> hill_climbing(const Graph* g, std::vector<vertex> clique) {
    bool improved = true;

    while (improved) {
        improved = false;

        //Etape 1 : chercher un sommet qui n'est pas dans la clique mais qui a une arete avec tout le monde de la clique
        for (vertex v = 0; v < g->nb_vertices(); ++v) {
            if (std::find(clique.begin(), clique.end(), v) != clique.end()) continue;

            bool can_add = true;
            for (vertex u : clique) {
                if (!g->is_edge(u, v)) { can_add = false; break; }
            }

            if (can_add) {
                clique.push_back(v);
                improved = true;
                break;
            }
        }
        
        if (improved) continue;

        //Etape 2, on echange 1 sommet si jamais derriere on peut en rajouter 2
        for (size_t i = 0; i < clique.size(); ++i) {
            //on teste dans l'ordre de la clique, on pourrait pourquoi pas selectionner
            //au hasard (peu de chance que ca soit un meilleur resultat je pense)
            vertex u = clique[i];
            std::vector<vertex> valid_replacements;

            for (vertex v = 0; v < g->nb_vertices(); ++v) {
                if (std::find(clique.begin(), clique.end(), v) != clique.end()) continue;
                //sommet qui n'est pas dans la clique actuelle

                bool valid = true;
                for (size_t j = 0; j < clique.size(); ++j) {
                    if (i != j && !g->is_edge(clique[j], v)) { valid = false; break; }
                    //condition i==j pour checker que u n'est pas pris
                }
                if (valid) valid_replacements.push_back(v);
            }

            //on a une liste de candidats, maintenant il faut checker que ces deux sommets soient amis
            //pas besoin de tester plus que entre les candidats, car a partir du moment ou ils sont candidats,
            //ils sont amis avec tout ceux de la clique actuelle
            bool swap_done = false;
            for (size_t idx1 = 0; idx1 < valid_replacements.size(); ++idx1) {
                for (size_t idx2 = idx1 + 1; idx2 < valid_replacements.size(); ++idx2) {
                    if (g->is_edge(valid_replacements[idx1], valid_replacements[idx2])) {
                        clique.erase(clique.begin() + i);
                        clique.push_back(valid_replacements[idx1]);
                        clique.push_back(valid_replacements[idx2]);
                        improved = true;
                        swap_done = true;
                        break;
                    }
                }
                if (swap_done) break;
            }
            if (swap_done) break;
        }
    }
    return clique;
}


// ============= QUESTION 2 =============

//Cas de base : gradient à pas fixe  -> juste une etape 
std::vector<double> projected_gradient_step_fixed(const Graph* g, std::vector<double> x, double alpha) {
    int n = g->nb_vertices();
    std::vector<double> grad(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g->is_edge(i, j)) grad[i] += x[j];
            //si il y a une arete entre les deux, on ajoute la poids du sommet j
            //qui je rappelle a ete donne au hasard au premier tour, et a partir du
            //deuxieme le poids change grace a CETTE fonction actuelle
        }
    }
    std::vector<double> y(n);
    for (int i = 0; i < n; ++i) y[i] = x[i] + alpha * grad[i];
    //justement, la boucle qui change les poids de chaque sommet
    //(on vient de bouger sur le graphique)
    return project_onto_simplex(y);
}

//Cas adaptatif cette fois, avec valeur alpha qui bouge
std::vector<double> projected_gradient_step_adaptive(const Graph* g, std::vector<double> x, double& alpha) {
    int n = g->nb_vertices();
    
    auto compute_score = [&](const std::vector<double>& vec) {
        double s = 0;
        for(int i=0; i<n; ++i)
            for(int j=0; j<n; ++j)
                if(g->is_edge(i, j)) s += vec[i]*vec[j];
        return s;
    };

    double current_score = compute_score(x);
    
    while (alpha > 1e-9) {
        //on limite a quel point on peut baisser sinon c'est infini
        std::vector<double> x_new = projected_gradient_step_fixed(g, x, alpha);
        //on rappelle la fonction non adaptative mais avec le alpha modifie du coup
        if (compute_score(x_new) >= current_score) {
            alpha *= 1.1; 
            //par contre si le score est pire, on augmente la taille du pas
            //A ADAPTER
            return x_new;
        }
        alpha *= 0.5;
        //si on trouve un score plus interessant, on reduit la taille du pas
        //A ADAPTER
    }
    return x;
}

//Fonction multi start : celle qui lance vraiment les tests de la question 2
std::vector<vertex> multi_start_gradient(const Graph* g, int num_runs, bool adaptive) {
    std::vector<vertex> best_clique;
    std::mt19937 gen(12345);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int run = 0; run < num_runs; ++run) {
        //nombre de fois qu'on lance, on peut lancer 1 fois normalement,
        //mais aussi plusieurs fois pour un multi_start
        //A ADAPTER
        int n = g->nb_vertices();
        std::vector<double> x(n);
        double sum = 0;
        
        for(int i=0; i<n; ++i) { x[i] = dist(gen); sum += x[i]; }
        for(int i=0; i<n; ++i) x[i] /= sum;
        //on donne un poids totalement au hasard, et on met le total a 1

        double alpha = 0.05; //alpha de base, A ADAPTER
        
        //Boucle de 100, A ADAPTER
        for (int iter = 0; iter < 100; ++iter) {
            if (adaptive) {
                x = projected_gradient_step_adaptive(g, x, alpha);
            } else {
                x = projected_gradient_step_fixed(g, x, 0.05);
            }
        }

        std::vector<vertex> current_clique = extract_clique(g, x);
        if (current_clique.size() > best_clique.size()) {
            best_clique = current_clique;
        }
    }
    return best_clique;
}

// ============= QUESTION 3 =============

//FIRST IMPROVEMENT WEIGHTED : c'est exactement la meme fonction qua sans ponderee
//je la cree juste pour simplifier les tests plus tard
std::vector<vertex> descent_first_improvement_weighted(const Graph* g, int seed = 42) {
    std::vector<vertex> clique;
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    std::mt19937 gen(seed);
    std::shuffle(candidates.begin(), candidates.end(), gen);

    for (vertex v : candidates) {
        bool can_add = true;
        for (vertex u : clique) {
            if (!g->is_edge(u, v)) {
                can_add = false;
                break;
            }
        }
        if (can_add) {
            clique.push_back(v);
        }
    }
    return clique;
}

//BEST IMPROVEMENT AVEC POIDS
std::vector<vertex> descent_best_improvement_static_weighted(const Graph* g) {
    std::vector<vertex> clique;    
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    //on change juste le tri
    std::sort(candidates.begin(), candidates.end(), [g](vertex a, vertex b) {
        return getVertexWeight(*g, a) > getVertexWeight(*g, b); 
    });

    for (vertex v : candidates) {
        bool can_add = true;
        for (vertex u : clique) {
            if (!g->is_edge(u, v)) {
                can_add = false;
                break;
            }
        }
        
        if (can_add) {
            clique.push_back(v);
        }
    }
    return clique;
}

//BEST IMPROVEMENT STATIQUE AVEC POIDS HYBRIDE
//on va mixer le poids des aretes + le nombre de voisin pour faire la decision
std::vector<vertex> descent_best_improvement_static_weighted_hybrid(const Graph* g) {
    std::vector<vertex> clique;
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    //on a juste a change le tri ducoup
    std::sort(candidates.begin(), candidates.end(), [g](vertex a, vertex b) {
        double weight_a = getVertexWeight(*g, a);
        double weight_b = getVertexWeight(*g, b);
        
        double degree_a = g->degree(a);
        double degree_b = g->degree(b);
        
        //pour le score je multiple le poids et le nombre de voisin, jsp si c'est le plus efficace
        double score_a = weight_a * degree_a;
        double score_b = weight_b * degree_b;
        
        return score_a > score_b; 
    });

    for (vertex v : candidates) {
        bool can_add = true;
        for (vertex u : clique) {
            if (!g->is_edge(u, v)) {
                can_add = false;
                break;
            }
        }
        if (can_add) {
            clique.push_back(v);
        }
    }
    return clique; 
}

//3 BEST IMPROVEMENT DYNAMIQUE HYBRIDE
//Hybride parce que l'algo de base est gere grace au nombres d'aretes pour chaque sommet
//or si on garde ce fonctionnement, c'est exactement le meme algo (on change pas les aretes, on rajoute juste un poids)
//donc on va utiliser ce poids EN PLUS des aretes pour realiser l'algo
std::vector<vertex> descent_best_improvement_dynamic_weighted_hybrid(const Graph* g) {
    std::vector<vertex> clique;
    std::vector<vertex> candidates(g->nb_vertices());
    std::iota(candidates.begin(), candidates.end(), 0);

    while (!candidates.empty()) {
        vertex best_v = candidates[0];
        double max_score_in_candidates = -1.0;

        //calcul du nombre de sommet amis parmis les sommets pas dans la clique
        for (vertex v : candidates) {
            int degree = 0;
            for (vertex u : candidates) {
                if (v != u && g->is_edge(v, u)) {
                    degree++;
                }
            }
            
            double score = (double)degree * getVertexWeight(*g, v);
            //pour calculer le socre, on multiplie le nombre de sommets amis pas dans le clique
            //MULTIPLIE par leur poids
            
            if (score > max_score_in_candidates) {
                max_score_in_candidates = score;
                best_v = v;
            }
        }

        clique.push_back(best_v);

        std::vector<vertex> new_candidates;
        for (vertex v : candidates) {
            if (v != best_v && g->is_edge(best_v, v)) {
                new_candidates.push_back(v);
            }
        }
        candidates = std::move(new_candidates);
    }
    return clique;
}

/*
HILL CLIMBING PONDERE
l'algo va se passer en deux etapes :
ETAPE 1 :
maintenant qu'on a des poids, au lieu d'essayer de changer 1 sommet contre 2, on va
essayer de changer 1 sommet contre 1, mais uniquement si l'arete de ce sommet a un plus gros poids
que celui qu'on va enlever
On continue ca jusqu'a que ça bloque
Quand ca bloque, on passe a l'etape 2 :
on reprend le fonctionnement de l'ancien algo hill climbing
en essayant de changer 1 sommet deja present dans la clique contre 2 autres qui ne sont pas dans la clique
*/
std::vector<vertex> hill_climbing_weighted(const Graph* g, std::vector<vertex> clique) {
    bool improved = true;

    while (improved) {
        improved = false;

        //ETAPE 1 : ajout simple (pareil que l'algo de base)
        for (vertex v = 0; v < g->nb_vertices(); ++v) {
            if (std::find(clique.begin(), clique.end(), v) != clique.end()) continue;

            bool can_add = true;
            for (vertex u : clique) {
                if (!g->is_edge(u, v)) { can_add = false; break; }
            }

            if (can_add) {
                clique.push_back(v);
                improved = true;
                break;
            }
        }
        if (improved) continue;

        //Etape 2 : echange 1 contre 1
        for (size_t i = 0; i < clique.size(); ++i) {
            vertex u = clique[i];
            double weight_u = getVertexWeight(*g, u);
            //le poids de u, il va falloir trouver mieux que u
            bool swap_done = false;

            for (vertex v = 0; v < g->nb_vertices(); ++v) {
                if (std::find(clique.begin(), clique.end(), v) != clique.end()) continue;
                
                double weight_v = getVertexWeight(*g, v);
                //le poids de v
                
                if (weight_v <= weight_u) continue; 
                //si le poids de v est meilleur que u

                bool valid = true;
                for (size_t j = 0; j < clique.size(); ++j) {
                    if (i != j && !g->is_edge(clique[j], v)) { valid = false; break; }
                }
                //il faut quand meme verifier que v est un arete avec tout ceux de la clique actuelle,
                //sinon c'est plus une clique
                
                if (valid) {
                    clique.erase(clique.begin() + i);
                    clique.push_back(v);
                    improved = true;
                    swap_done = true;
                    break;
                }
            }
            if (swap_done) break;
        }
        if (improved) continue;

        //ETAPE 3 : on echange 1 contre 2 (pareil que l'algo de base sauf qu'on checke les poids par contre)
        for (size_t i = 0; i < clique.size(); ++i) {
            vertex u = clique[i];
            double weight_u = getVertexWeight(*g, u);
            std::vector<vertex> valid_replacements;

            for (vertex v = 0; v < g->nb_vertices(); ++v) {
                if (std::find(clique.begin(), clique.end(), v) != clique.end()) continue;
                //sommet qui n'est pas dans la clique actuelle

                bool valid = true;
                for (size_t j = 0; j < clique.size(); ++j) {
                    if (i != j && !g->is_edge(clique[j], v)) { valid = false; break; }
                    //condition i==j pour checker que u n'est pas pris
                }
                if (valid) valid_replacements.push_back(v);
            }

            bool swap_done = false;
            for (size_t idx1 = 0; idx1 < valid_replacements.size(); ++idx1) {
                for (size_t idx2 = idx1 + 1; idx2 < valid_replacements.size(); ++idx2) {
                    vertex v1 = valid_replacements[idx1];
                    vertex v2 = valid_replacements[idx2];
                    if (g->is_edge(v1, v2) && (getVertexWeight(*g, v1) + getVertexWeight(*g, v2) > weight_u)) {
                        clique.erase(clique.begin() + i);
                        clique.push_back(v1);
                        clique.push_back(v2);
                        improved = true;
                        swap_done = true;
                        break;
                    }
                }
                if (swap_done) break;
            }
            if (swap_done) break;
        }
    }
    return clique;
}

//Fonction principale pour lancer les experiences
void run_mcp_experiments(const Graph* g) {}