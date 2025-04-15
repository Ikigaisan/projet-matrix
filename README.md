# LEPL1503 - Projet 2025 : Projet matrix

Ce projet consiste à réaliser des opérations sur les matrices et vecteurs.
Cette partie contient une version adaptée aux matrices creuses. 
Certaines opérations ne sont donc pas implémentées, et seule une version séquentielle est disponible.

## Table des matières
- [Lancer le programme](#lancer-le-programme)
- [Arborescence du projet](#arborescence-du-projet)
- [Détails des fichiers](#descritpion-des-dossiers-et-fichiers)
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

## Nettoyage
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
|   ├── approximation.py
│   ├── generator_matrix.c
│   ├── generator_vector.c
├── src (contient les fichiers sources)
|   ├── file.c
|   ├── main.c
|   ├── matrix.c
|   ├── vector.c
├── tests (contient les codes des tests)
|   ├── tests_basic_op.c
|   ├── tests_file.c
│   ├── tests_adv_op.c
├── Makefile
└── README.md
```
## Descritpion des dossiers et fichiers

### headers/

```file.h``` :

Ce fichier header contient les implémentations des fonctions présentes dans le fichier source ```file.c```. Les fonctions void présentes permettent de lire un double, un vecteur, une matrice et un QR ; ```write_double```, ```write_vector```, ```write_matrix``` et ```write_QR``` ainsi que les fonctions permettant de les lire ; ```read_vector```, ```read_matrix``` et ```read_QR```. 

```matrix.h``` :

Ce fichier header contient les implémentations des fonctions présentes dans le fichier source ```matrix.c```.
Les deux premières fonctions permettent d'initialiser et d'imprimer une matrice; ```init_matrix``` et ```print_matrix```. Nous avons ensuite ajouté les fonctions demandées afin de pouvoir réaliser l'addition et la soustraction de deux matrices, la multiplication de deux matrices et d'une matrice et d'un vecteur ainsi que la transposée d'une matrice; ```add_m_m```, ```sub_m_m```, ```mult_m_m```, ```mult_m_v```  et  ```transp```. 

```vector.h``` :

Ce fichier header contient les implémentations des fonctions présentes dans le fichier source ```vector.c```.
Les deux premières fonctions permettent d'initialiser et d'imprimer un vecteur; ```init_vector``` et ```print_vector```. Nous avons ensuite ajouté les fonctions demandées afin de pouvoir réaliser l'addition, la soustraction de deux vecteurs, le produit scalaire de deux vecteurs ainsi que la norme d'un vecteur; ```add_v_v```, ```sub_v_v```, ```dot_prod``` et  ```norm```. 


### help/

```approximation.py``` : 

Le fichier python approximation nous a été fourni afin de tester notre fonction lstsq. 

```generator_matrix.c``` et ```generator_vector.c``` : 

Ces deux codes nous sont fournis pour générer des fichiers contenant des vecteurs et des matrices et ainsi nous permettre d'utiliser les commandes make generator_vector ou make generator_matrix pour obtenir le fichier binaire contenant la matrice.


### src/ 


```file.c``` : 

Le fichier source ```file.c``` contient tous les codes des fonctions implémentées dans ```file.h```. 



```main.c``` :

On va pouvoir run le fichier source ```main.c``` avec différents arguments afin d'effectuer des opérations entre les matrices et le vecteurs présentes dans les fichiers.



```matrix.c``` :

Le fichier source ```matrix.c``` contient tous les codes des fonctions implémentées dans ```matrix.h```. 



```vector.c``` :

Le fichier source ```vector.c``` contient tous les codes des fonctions implémentées dans ```vector.h```. 


### tests/


```tests_basic_op.c``` :

Ce fichier test contient l'ensemble des tests des fonctions basiques: ```add_m_m```, ```add_v_v```, ```sub_m_m```, ```sub_v_v```,  ```mult_m_m```, ```mult_m_v```,  ```transp``` , ```dot_prod``` et  ```norm```. 


```tests_file.c``` :

Ce fichier test contient l'ensemble des tests des fichiers: ```write_double```, ```write_vector```, ```write_matrix```,  ```write_QR```, ```read_vector```, ```read_matrix``` et ```read_QR```. 



```tests_adv_op.c``` :

Ce fichier test contient les tests des 3 fonctions complexes: ```test_lstsq```, ```test_qr``` et ```test_back_sub```. 

Tous ces fichiers tests peuvent être lancés avec la commande make test. 

ATTENTION : Il y a malheureusement un segmentation fault error qui s'est glissé dans les tests et nous travaillons encore à le résoudre. Sans cela nous n'avons donc pas pu tester les fonctions du fichier ```tests_adv_op.c```.



