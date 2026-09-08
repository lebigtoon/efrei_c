# TP Structures de données — Analyse

Trois structures en C pour stocker une grande collection d'entiers, comparées
en théorie et par des mesures, puis choix d'une structure selon le contexte.

---

## 1. Compilation & exécution

```sh
make          # compile ./tests et ./benchmark
make test     # lance les tests de correction
make bench    # lance le benchmark (partie 2)
make clean
```

Compilé avec `gcc -std=c11 -O2 -Wall -Wextra -Wpedantic`, sans warning.

## 2. Organisation du projet

| Fichier | Rôle |
|---|---|
| `dynamic_array.{h,c}` | Tableau dynamique |
| `linked_list.{h,c}`   | Liste simplement chaînée (`head` + `size`) |
| `hash_table.{h,c}`    | Table de hachage, `TABLE_SIZE = 10007`, collisions par chaînage |
| `tests.c`             | Tests de correction |
| `benchmark.c`         | Mesures de performance |

Correspondance avec les opérations demandées :

| Demandé | Dynamic Array | Linked List | Hash Table |
|---|---|---|---|
| insert_front / insert_back | `da_insert_front` / `da_insert_back` | `ll_insert_front` / `ll_insert_back` | — |
| find / get(index)          | `da_find` / `da_get` | `ll_find` / `ll_get` | — |
| remove_front               | `da_remove_front` | `ll_remove_front` | — |
| free_collection            | `da_free` | `ll_free` | `ht_free` |
| hash_insert / hash_contains | — | — | `hash_insert` / `hash_contains` |

Toute la mémoire allouée est libérée par les fonctions `*_free`, appelées à la
fin de chaque test et de chaque phase du benchmark.

Choix d'implémentation :
- **Tableau dynamique** : capacité initiale 8, doublée quand elle est pleine.
- **Liste chaînée** : conforme au sujet, seulement `head` (pas de pointeur vers
  la fin) → `insert_back` doit parcourir toute la liste.
- **Table de hachage** : insertion en tête de bucket, sans suppression des
  doublons. `hash_good = value % TABLE_SIZE` suppose des valeurs ≥ 0.

---

## 3. Partie 1 — les trois structures

Voir les fichiers `.c` / `.h`. `make test` vérifie chaque opération : insertions
aux deux bouts, `get` valide et hors bornes, `find` présent et absent,
`remove_front` jusqu'au vide, collisions dans la table de hachage, et `hash_bad`.

---

## 4. Partie 2 — Analyse & Benchmark

### 4.1 Complexités théoriques

| Opération | Dynamic Array | Linked List | Gagnant prévu |
|---|---|---|---|
| `get(index)`     | O(1) | O(n) | Dynamic Array |
| `find(value)`    | O(n) | O(n) | Dynamic Array (léger avantage) |
| `insert_front`   | O(n) | O(1) | Linked List |
| `insert_back`    | O(1) amorti | O(n) | Dynamic Array |
| `remove_front`   | O(n) | O(1) | Linked List |
| Parcours complet | O(n) | O(n) | Dynamic Array (léger avantage) |

Le tableau décale ses éléments (`memmove`) pour insérer/supprimer en tête → O(n).
La liste le fait en O(1) mais ne connaît pas sa fin → `insert_back` en O(n).
Quand la complexité est identique, le tableau est un peu plus rapide car ses
éléments sont rangés côte à côte en mémoire.

### 4.2 Protocole de mesure

- Tailles testées : **1 000, 10 000, 100 000, 1 000 000** éléments.
- Horloge monotone imposée : `clock_gettime(CLOCK_MONOTONIC, ...)`.
- Chaque opération est répétée un grand nombre de fois, puis on divise le temps
  total par le nombre de répétitions.
- Opérations mesurées : accès à l'élément n/2, recherche d'une valeur absente,
  insertion en début, insertion en fin, parcours complet avec somme, et coût de
  construction.

### 4.3 Résultats mesurés

`make bench` (les valeurs varient d'environ ±20 % d'un run à l'autre) :

```
========================  n = 1000  ========================
operation                             array      linked list
construction (total)               0.004 ms         0.021 ms
get(n/2)  [par op]                   1.5 ns         426.2 ns
find(absent)  [par op]             259.3 ns         934.0 ns
insert_front  [par op]             113.8 ns          41.7 ns
insert_back   [par op]               1.6 ns        3444.7 ns
parcours complet [total]             232 ns          1027 ns

========================  n = 10000  ========================
operation                             array      linked list
construction (total)               0.017 ms         0.100 ms
get(n/2)  [par op]                   1.4 ns        4626.3 ns
find(absent)  [par op]            2280.9 ns        9355.7 ns
insert_front  [par op]             417.1 ns          10.4 ns
insert_back   [par op]               1.6 ns        9672.8 ns
parcours complet [total]            2312 ns         10512 ns

========================  n = 100000  ========================
operation                             array      linked list
construction (total)               0.169 ms         1.043 ms
get(n/2)  [par op]                   1.1 ns       46469.4 ns
find(absent)  [par op]           23823.2 ns       92032.4 ns
insert_front  [par op]            4713.4 ns          13.0 ns
insert_back   [par op]               1.7 ns       93969.6 ns
parcours complet [total]           23062 ns       104484 ns

========================  n = 1000000  ========================
operation                             array      linked list
construction (total)               1.671 ms        12.129 ms
get(n/2)  [par op]                   1.1 ns      474488.9 ns
find(absent)  [par op]          230381.2 ns      942987.2 ns
insert_front  [par op]           60637.2 ns          38.1 ns
insert_back   [par op]               1.7 ns     1471935.0 ns
parcours complet [total]          231984 ns      1060245 ns

================  hash : bonne vs mauvaise fonction  ================
hash_good    n=20000     build =     0.225 ms   contains =        2.7 ns/op
hash_good    n=200000    build =     2.182 ms   contains =        9.8 ns/op
hash_bad     n=20000     build =     0.216 ms   contains =    14332.3 ns/op
```

Unités : `ns` = nanoseconde (milliardième de seconde), `ms` = milliseconde.
`[par op]` = temps d'une opération ; `[total]` = durée d'un parcours entier.

### 4.4 Interprétation

- **`get(n/2)`** — tableau : ~1 ns quelle que soit la taille (accès direct).
  Liste : 426 → 4 600 → 46 500 → 474 500 ns, soit ×10 quand n est ×10 → O(n).
  À n = 1 000 000, le tableau est ~400 000× plus rapide.
- **`insert_front`** — l'inverse : liste ~10–40 ns (constant), tableau
  114 → 60 600 ns (il décale tout le contenu).
- **`insert_back`** — tableau ~1,7 ns (constant), liste 3 400 → 1 472 000 ns
  (elle parcourt tout pour trouver la fin).
- **`find` et parcours complet** — les deux structures sont en O(n) (temps ×10
  quand n ×10). Le tableau reste ~4× plus rapide (0,23 ms contre 1 ms à
  n = 1 000 000) : ses éléments sont côte à côte en mémoire, le processeur les lit
  plus vite que les maillons d'une liste dispersés un peu partout.
- **Construction** — tableau 1,7 ms contre liste 12 ms à n = 1 000 000 : le
  tableau fait une grosse allocation qu'il agrandit quelques fois, la liste fait
  1 000 000 de petites allocations.

### 4.5 Bonne vs mauvaise fonction de hachage

| | `hash_good` | `hash_bad` |
|---|---|---|
| Code | `return value % TABLE_SIZE;` | `return 0;` |
| Effet | Valeurs réparties dans les 10007 cases | Toutes les valeurs dans la même case |
| `hash_contains` | quasi constant | O(n) : la table redevient une liste chaînée |

Mesuré à n = 20 000 : **2,7 ns/op** avec `hash_good`, **14 332 ns/op** avec
`hash_bad` (~5 000× plus lent). `hash_good` reste rapide même à n = 200 000
(9,8 ns/op).

Conclusion : une table de hachage n'est rapide que si sa fonction répartit bien
les valeurs. Avec une mauvaise fonction, on paie la mémoire des cases sans rien
y gagner.

---

## 5. Partie 3 — Décision technique

### Situation A — collection rarement modifiée, 1 000 000 de recherches

| Critère | Dynamic Array | Linked List | Hash Table |
|---|---|---|---|
| Temps d'une recherche | O(n) ≈ 0,23 ms | O(n) ≈ 0,94 ms | quasi constant (~quelques ns) |
| Temps des 1 000 000 recherches | ~64 heures | ~11 jours | moins d'une seconde |
| Coût de construction (n = 1e6) | ~1,7 ms | ~12 ms | ~15 ms |
| Mémoire (n = 1e6) | ~4 Mo | ~16 Mo | ~16 Mo + 80 Ko |
| Simplicité | la plus simple | simple | moyenne |

**Décision : Hash Table.** Le million de recherches domine tout le reste :
passer de O(n) à un temps quasi constant fait tomber le total de ~64 heures à
moins d'une seconde. Le surcoût mémoire et de code est négligeable devant ce
gain, et la construction n'est payée qu'une fois.

*Alternative :* un tableau **trié** avec recherche dichotomique donne
O(log n) par recherche (~20 comparaisons) pour ~4 Mo seulement. Bon compromis si
la mémoire compte, un peu plus lent que la table de hachage.

### Situation B — mémoire très limitée, seulement 10 recherches

**Décision : on change pour le Dynamic Array.** Ce qui décidait en A (le nombre
de recherches) a disparu : 10 recherches en O(n) coûtent ~2 ms, c'est
négligeable. Il n'y a donc plus de raison de payer la mémoire d'une table de
hachage ou d'une liste chaînée.

Le tableau dynamique est le plus compact (4 octets par élément, sans pointeur),
le plus rapide à construire et le plus simple.

On accepte une recherche O(n) parce qu'elle n'a lieu que 10 fois, en échange de
la plus faible empreinte mémoire. Le bon choix dépend de l'usage, pas seulement
de la complexité théorique.
