# Question 1 - MCP non pondere

## Protocole

Le binaire `mcp_experiments` compare trois descentes discretes sur des solutions faisables
(des cliques) :

- `first` : premiere amelioration trouvee dans le voisinage
- `best` : meilleure amelioration du voisinage
- `random` : ordre aleatoire des mouvements, puis premiere amelioration

Voisinage utilise :

- ajout d'un sommet compatible avec toute la clique courante
- echange `1-1` : insertion d'un sommet exterieur en retirant au plus un sommet en conflit
- apres chaque mouvement, completion gloutonne jusqu'a obtenir une clique maximale

Chaque methode est executee 30 fois par instance avec des graines `0..29`.

Commande utilisee :

```bash
./build/mcp_experiments --runs 30 > results/question1_mcp_unweighted.csv
```

## Tendances observees

- `best` est souvent legerement meilleur en qualite moyenne, mais son cout en temps est nettement plus eleve.
- `first` est le meilleur compromis temps/qualite sur la plupart des instances.
- `random` peut depasser `first` et `best` sur certaines instances plus irregulieres (`gen200_p0.9_44`, `MANN_a27`) grace a la diversification.
- Sur les instances tres structurees (`johnson16-2-4`, `c-fat200-2`), les trois descentes sont pratiquement equivalentes.

## Exemples de resultats

- `C125.9.clq` : meilleur resultat trouve `33` pour les trois methodes, avec un temps moyen de `2.24 ms` a `3.70 ms`.
- `gen200_p0.9_44.clq` : `random` trouve la meilleure clique (`37`) et la meilleure moyenne (`34.80`).
- `MANN_a27.clq` : meme meilleure clique (`121`) pour les trois variantes, mais `best` coute environ `212.63 ms` contre `115.57 ms` pour `first`.

## Conclusion

Pour la question 1, les experiences suggerent de recommander `first` comme methode par defaut :
la perte de qualite face a `best` est faible, tandis que le gain en temps est souvent important.
Si l'on accepte un peu d'aleatoire, `random` est une bonne alternative pour ameliorer ponctuellement
la qualite sur des instances difficiles sans payer le cout complet de `best`.
