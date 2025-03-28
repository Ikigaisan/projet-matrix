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
## Descritpion de l'arborescence du projet

HEADERS

```file.h``` :

Ce fichier header contient les implémentations des fonctions présentes dans le fichier source ```file.c```. Les fonctions void présentes permettent de lire un double, un vecteur, une matrice et un QR ; ```write_double```, ```write_vector```, ```write_matrix``` et ```write_QR``` ainsi que les fonctions permettant de les lire ; ```read_vector```, ```read_matrix``` et ```read_QR```. 

```matrix.h``` :

Ce fichier header contient les implémentations des fonctions présentes dans le fichier source ```matrix.c```.
Les deux premières fonctions permettent d'initialiser et d'imprimer une matrice; ```init_matrix``` et ```print_matrix```. Nous avons ensuite ajouté les fonctions demandées afin de pouvoir réaliser l'addition et la soustraction de deux matrices, la multiplication de deux matrices et d'une matrice et d'un vecteur ainsi que la transposée d'une matrice; ```add_m_m```, ```sub_m_m```, ```mult_m_m```, ```mult_m_v```  et  ```transp```. 

```vector.h``` :

Ce fichier header contient les implémentations des fonctions présentes dans le fichier source ```vector.c```.
Les deux premières fonctions permettent d'initialiser et d'imprimer un vecteur; ```init_vector``` et ```print_vector```. Nous avons ensuite ajouté les fonctions demandées afin de pouvoir réaliser l'addition, la soustraction de deux vecteurs, le produit scalaire de deux vecteurs ainsi que la norme d'un vecteur; ```add_v_v```, ```sub_v_v```, ```dot_prod``` et  ```norm```. 


HELP 

```approximation.py``` : 

Le fichier python approximation nous a été fourni afin de tester notre fonction lstsq. 

```generator_matrix.c``` et ```generator_vector.c``` : 

Ces deux codes nous sont fournis pour générer des fichiers contenant des vecteurs et des matrices et ainsi nous permettre d'utiliser les commandes make generator_vector ou make generator_matrix pour obtenir le fichier binaire contenant la matrice.


SRC 


```file.c``` : 

Le fichier source ```file.c``` contient tous les codes des fonctions implémentées dans ```file.h```. 



```main.c``` :

On va pouvoir run le fichier source ```main.c``` avec différents arguments afin d'effectuer des opérations entre les matrices et le vecteurs présentes dans les fichiers.



```matrix.c``` :

Le fichier source ```matrix.c``` contient tous les codes des fonctions implémentées dans ```matrix.h```. 



```vector.c``` :

Le fichier source ```vector.c``` contient tous les codes des fonctions implémentées dans ```vector.h```. 


TESTS


```tests_basic_op.c``` :

Ce fichier test contient l'ensemble des tests des fonctions basiques: ```add_m_m```, ```add_v_v```, ```sub_m_m```, ```sub_v_v```,  ```mult_m_m```, ```mult_m_v```,  ```transp``` , ```dot_prod``` et  ```norm```. 


```tests_file.c``` :

Ce fichier test contient l'ensemble des tests des fichiers: ```write_double```, ```write_vector```, ```write_matrix```,  ```write_QR```, ```read_vector```, ```read_matrix``` et ```read_QR```. 



```tests_adv_op.c``` :

Ce fichier test contient les tests des 3 fonctions complexes: ```test_lstsq```, ```test_qr``` et ```test_back_sub```. 

Tous ces fichiers tests peuvent être lancés avec la commande make test. 



MAKEFILE 

Le fichier Makefile est lancé en même temps que les tests lors de l'exéctution de la commande make test. 





















