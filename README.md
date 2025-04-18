# LEPL1503 - Projet 2025 : Projet matrix
Groupe L2

Membres : Aymeric Grymonpré, Clara Hoffait, Emma Benmeridja, Lisa Fredon, Marthe Thiry, Oscar De Vleeschouwer 

Environnement dans lequel le projet d'execute : Rasberry Pi

Packages recquis : <errno.h>, <getopt.h>, <stdbool.h>, <stdint.h>, <stdio.h>, <stdlib.h>, <string.h>, <inttypes.h>, <math.h>, <float.h>, <time.h>, <sys/mman.h>, <fcntl.h>, <unistd.h>, <endian.h>, <errno.h>, <stdint.h>

Ce projet consiste à réaliser des opérations sur les matrices et vecteurs.

## Table des matières 
- [Lancer le programme](#lancer-le-programme)
- [Arborescence du projet](#arborescence-du-projet)
- [Détails des fichiers](#description-des-dossiers-et-fichiers)
- [Tests](#tests)
- [Nettoyage](#nettoyage)

## Lancer le programme

### Compilation

```sh
make
```
L'exécutable est alors généré à la racine du projet.

### Utilisation

```sh
./main [-v] [-f output_stream] name_op input_file_A [input_file_B]
```

Les arguments entre crochets sont optionnels.
Les opérations disponibles sont : 
- ```add_m_m```
-  ```sub_m_m```
-  ```mult_m_v```
-  ```transp```
- ```back_sub```.

La commande ```./main -h``` permet d'avoir des explications sur les différents paramètres.

### Format des fichiers
Les fichiers d'entrée et de sortie sont au format binaire.

#### Doubles
Un fichier ne contenant que la valeur `double`directement en format binaire.

#### Vecteurs
1. **Taille `m` du fichier** (uint64_t, big endian)
2. **Valeurs contenues dans le vecteur** dans l'ordre

#### Matrices
1. **Nombre de lignes `m`** (`uint64_t`, big endian)
2. **Nombre de colonnes `n`** (`uint64_t`, big endian)
3. **Ligne des matrices** :
    - Chaque ligne commence par l'indice de ligne (`uint64_t`, big endian)
    - Ensuite les `n` **valeurs** (`double`) contenues dans la ligne `m`

#### Décomposition QR
1. **Nombre de ligne `m` de la matrice `Q`**(`uint64_t`, big endian)
2. **Nombre de colonnes `n`de la matrice `Q`** (`uint64_t`, big endian)
3. **Valeurs de la matrice `Q`**(`double`)
4. **Valeurs de la matrice `R`**(au format `nxn`,`double`)

## Tests
Pour lancer les tests, tapez la commande

```sh
make test
```

### Tests mémoire
Commande pour effectuer les tests Valgrind : tests sur la gestion de mémoire et la détection de fuites de mémoire sur le fichier main.
 ```sh
make valgrind
```

Commande pour effectuer les tests Valgrind : tests sur les tests unitaires.
```sh
make valgrindtest
```
Ces commandes sont compatibles uniquement avec un ordinateur Windows.

### Comparaison monothreading et multithreading
Pour lancer la comparaison entre le monothreading et le multithreading, tapez les commande 

```sh
make compare
```
```sh
./compare [nombre-de-threads-souhaités]
```

## Nettoyage
```sh
make clean
```
## Arborescence du projet

```
./
├── headers (contient les fichiers headers)
│   ├── file.h
|   ├── matrix_threads.h
│   ├── matrix.h
|   ├── vector_threads.h
│   ├── vector.h
├── help (contient les codes pour générer des vecteurs et matrices)
|   ├── approximation.py
│   ├── generator_matrix.c
│   ├── generator_vector.c
├── objects (vide mais contient les fichiers compilé lors du build)
├── src (contient les fichiers sources)
|   ├── file.c
|   ├── main.c
|   ├── matrix_threads.c
|   ├── matrix.c
|   ├── vector_threads.c
|   ├── vector.c
├── tests (contient les codes des tests)
|   ├── compare.c
|   ├── tests_basic_op.c
|   ├── tests_file.c
│   ├── tests_adv_op.c
├── Makefile
└── README.md
```
## Description des dossiers et fichiers

### headers/

- ```file.h``` : Fonctions pour lire ou écrire des double, vecteurs, matrices et QR.

- ```matrix.h``` / ```vector.h``` : Fonctions pour créer, afficher et manipuler des matrices et des vecteurs.

<<<<<<< README.md
- ```vector.h``` : Fonctions pour créer, afficher et manipuler des vecteurs.
=======
- ```matrix_threads.h``` / ```vector_threads.h``` : Fonctions pour manipuler les matrices et vecteurs avec des threads.
>>>>>>> README.md



### help/

- ```approximation.py``` : Script pour tester lstsq.

- ```generator_matrix.c``` / ```generator_vector.c``` : Générateurs de matrices et de vecteurs aléatoires sous le nom de 
    - `matrix_A.bin`
    - `matrix_B.bin`
    - `vector_A.bin`
    - `vector_B.bin`

### src/ 


- ```file.c``` : Implémentation des I/O (input/output) en format binaire.

- ```main.c``` : Fichier principal prenant en charge les arguments et excécute les opérations demandées.

- ```matrix.c``` / ```vector.c``` : Implémentation des opérations sur les matrices et vecteurs.

- ```matrix_threads.c```/ ```vector_threads.c``` : Implémentation des opérations sur les matrices utilisant le multithreading.


### tests/

- ```compare.c``` : Comparaison des fonctions de base entre le monothreading et le mutlithreading.

- ```tests_adv_op.c``` : Test de `lstsq`, `qr`, `back_sub`.

- ```tests_basic_op.c``` : Test des opérations de base (`add`, `sub`, `mult`, etc.).

- ```tests_file.c``` : Test de la lecture et écriture des fichiers binaires.





