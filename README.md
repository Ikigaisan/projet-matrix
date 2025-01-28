# LEPL1503 - Projet 2025

Ce projet consiste à réaliser des opérations sur les matrices et vecteurs.
Cette partie contient une version adaptée aux matrices creuses. 
Certaines opérations ne sont donc pas implémentées, et seule une version séquentielle est disponible.

## Lancer le programme

Pour compiler le projet, entrez la commande

```sh
make
```

L'exécutable est alors généré à la racine du projet. Pour l'utiliser, entrez la commande

```sh
./main [-v] [-f output_stream] name_op input_file_A [input_file_B]
```

Les arguments entre crochets sont optionnels.
Les opérations disponibles sont : ```add_m_m```, ```sub_m_m```, ```mult_m_v```, ```transp``` et ```back_sub```.

La commande ```./main -h``` permet d'avoir des explications sur les différents paramètres.

Les fichiers d'entrée et de sortie sont au format binaire. Pour un vecteur, le fichier prend le même format que celui indiqué dans l'énoncé du projet.
Pour une matrice, il est différent afin d'être adapté aux matrices creuses. Celui-ci contient :

- Le nombre de lignes (`m`), de colonnes (`n`) et de valeurs non nulles (`nnz`) dans la matrice. Ces trois valeurs sont des `uint64_t` stockées en big-endian.
- Les nnz valeurs non nulles de la matrice. Ce sont des double.
- Les nnz colonnes de ces valeurs. Ce sont des `uint64_t`.
- Les m+1 index de débuts de lignes, comme voulu par le format CSR. Ce sont des `uint64_t`. Par exemple, si la matrice contient 3 lignes et a 2 valeurs non nulles, une à la ligne 0 et une à la ligne 2, on pourra lire [0 1 1 2].

Pour lancer les tests, tapez la commande

```sh
make test
```

Enfin, pour nettoyer le projet en senlevant les fichiers objets et les exécutables, utilisez la commande

```sh
make clean
```

## Arborescence du projet

```
./
├── headers (contient les fichiers headers)
│   ├── file.h
│   ├── matrix.h
│   ├── vector.h
├── help (contient les codes pour générer des vecteurs et matrices)
│   ├── generator_matrix.c
│   ├── generator_vector.c
├── src (contient les fichiers sources)
|   ├── file.c
|   ├── main.c
|   ├── matrix.c
|   ├── vector.c
├── tests (contient les codes des tests)
|   ├── tests_matrix.c
|   ├── tests_matrix.h
│   ├── tests.c
├── Makefile
└── README.md
```