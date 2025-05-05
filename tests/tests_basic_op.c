// Inclusion des bibliothèques nécessaires
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <float.h>

// Inclusion des en-têtes de modules personnalisés
#include "../headers/matrix.h"
#include "../headers/vector.h"
#include "../headers/file.h"

// Test de l'addition de deux vecteurs
void test_add_v_v(void){
    uint64_t m = 200;
    vector *v = init_vector(m);
    vector *w = init_vector(m);

    // Remplissage des vecteurs v et w avec des valeurs aléatoires
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = (double)rand() / 2.0;
        w->values[i] = (double)rand() / 2.0;
    }

    vector *z = init_vector(m);
    add_v_v(v, w, z);  // z = v + w

    // Vérification du résultat
    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(z->values[i], v->values[i] + w->values[i], 1e-3);
    }

    // Libération de la mémoire
    free_vector(v);
    free_vector(w);
    free_vector(z);
}

// Test de l'addition de deux matrices
void test_add_m_m(void) {
    uint64_t m = 100, n = 50;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(m, n);

    // Remplissage des matrices A et B avec des valeurs aléatoires
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
            B->values[i][j] = (double)rand() / 2.0;
        }
    }

    matrix *C = init_matrix(m, n);
    add_m_m(A, B, C);  // C = A + B

    // Vérification des valeurs
    for (uint64_t i = 0; i < m; i++) {
        for (uint64_t j = 0; j < n; j++) {
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j],
                                   A->values[i][j] + B->values[i][j], 1e-3);
        }
    }
    
    // Libération de la mémoire allouée pour les matrices
    for (uint64_t i = 0; i < m; i++) {
        free(A->values[i]);
        free(B->values[i]);
        free(C->values[i]);
    }
    free(A->values);
    free(B->values);
    free(C->values);
    
    free(A);
    free(B);
    free(C);
}

// Test de la multiplication matrice x vecteur
void test_mult_m_v(void) {
    uint64_t m = 100, n = 50;
    matrix *A = init_matrix(m, n);
    vector *B = init_vector(n);

    // Initialisation de la matrice A et du vecteur B
    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++)
            A->values[i][j] = (double)rand() / 2.0;
    
    for (uint64_t i = 0; i < n; i++)
        B->values[i] = (double)rand() / 2.0;

    vector *C = init_vector(m);
    mult_m_v(A, B, C);  // C = A * B

    // Vérification du résultat
    for (uint64_t i = 0; i < m; i++) {
        double res = 0;
        for (uint64_t j = 0; j < n; j++) {
            res += A->values[i][j] * B->values[j];
        }
        CU_ASSERT_DOUBLE_EQUAL(C->values[i], res, 1e-3);
    }

    free_matrix(A);
    free_vector(B);
    free_vector(C);
}

// Test du calcul de la norme d'un vecteur
void test_norm(void) {
    uint64_t m = 5;
    vector *v = init_vector(m);
    double values[] = {3.0, 4.0, 0.0, 0.0, 0.0};

    // Attribution de valeurs spécifiques pour un test de norme classique
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = values[i];
    }

    double expected_norm = sqrt(3.0 * 3.0 + 4.0 * 4.0);
    double computed_norm;
    norm(v, &computed_norm);  // Calcul de la norme

    CU_ASSERT_DOUBLE_EQUAL(computed_norm, expected_norm, 1e-3);

    free_vector(v);
}

// Test de la soustraction de deux vecteurs
void test_sub_v_v(void) {
    uint64_t m = 200;
    vector *v = init_vector(m);
    vector *w = init_vector(m);

    // Remplissage avec des valeurs aléatoires
    for (uint64_t i = 0; i < m; i++) {
        v->values[i] = (double)rand() / 2.0;
        w->values[i] = (double)rand() / 2.0;
    }

    vector *z = init_vector(m);
    sub_v_v(v, w, z);  // z = v - w

    // Vérification du résultat
    for (uint64_t i = 0; i < m; i++) {
        CU_ASSERT_DOUBLE_EQUAL(z->values[i], v->values[i] - w->values[i], 1e-3);
    }

    free_vector(v);
    free_vector(w);
    free_vector(z);
}

// Test de la soustraction de deux matrices
void test_sub_m_m(void) {
    uint64_t m = 100, n = 50;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(m, n);

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
            B->values[i][j] = (double)rand() / 2.0;
        }

    matrix *C = init_matrix(m, n);
    sub_m_m(A, B, C);  // C = A - B

    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++) {
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j],
                                   A->values[i][j] - B->values[i][j], 1e-5);
        }

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// Test du produit scalaire de deux vecteurs
void test_dot_prod(void){
    uint64_t m = 100;
    vector *x = init_vector(m);
    vector *y = init_vector(m);

    for (uint64_t i = 0; i < m; i++) {
        x->values[i] = (double)rand() / 2.0; 
        y->values[i] = (double)rand() / 2.0; 
    }

    double result = 0.0;
    dot_prod(x, y, &result);  // Produit scalaire

    double expected_result = 0.0;
    for (uint64_t i = 0; i < m; i++) {
        expected_result += x->values[i] * y->values[i];
    }

    CU_ASSERT_DOUBLE_EQUAL(result, expected_result, 1e-6);

    free_vector(x);
    free_vector(y);
}

// Test de la multiplication de deux matrices
void test_mult_m_m(void){
    uint64_t m = 100, n = 50, p = 70;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(n, p);

    // Initialisation des matrices avec des valeurs aléatoires
    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++)
            A->values[i][j] = (double)rand() / 2.0;

    for (uint64_t i = 0; i < n; i++)
        for (uint64_t j = 0; j < p; j++)
            B->values[i][j] = (double)rand() / 2.0;

    matrix *C = init_matrix(m, p);
    mult_m_m(A, B, C);  // C = A * B

    // Vérification du résultat
    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < p; j++) {
            double expected = 0;
            for (uint64_t k = 0; k < n; k++) {
                expected += A->values[i][k] * B->values[k][j];
            }
            CU_ASSERT_DOUBLE_EQUAL(C->values[i][j], expected, 1e-6);
        }

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

// Test de la transposition d'une matrice
void test_transp(void){
    uint64_t m = 100, n = 50;
    matrix *A = init_matrix(m, n);
    matrix *B = init_matrix(n, m);

    // B est la transposée attendue de A
    for (uint64_t i = 0; i < m; i++)
        for (uint64_t j = 0; j < n; j++) {
            A->values[i][j] = (double)rand() / 2.0;
            B->values[j][i] = A->values[i][j];
        }

    transp(A);  // Transpose la matrice A

    // Vérifie que la transposition est correcte
    for (uint64_t i = 0; i < n; i++)
        for (uint64_t j = 0; j < m; j++)
            CU_ASSERT_DOUBLE_EQUAL(A->values[i][j], B->values[i][j], 1e-3);

    free_matrix(A);
    free_matrix(B);
}

// Fonction principale exécutant tous les tests
int main(int argc, char **argv) {
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    CU_pSuite test_basic_op = CU_add_suite("test_basic_op", NULL, NULL);
    if (test_basic_op == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Ajout de tous les tests à la suite
    if ((CU_add_test(test_basic_op, "add_v_v", test_add_v_v) == NULL) ||
        (CU_add_test(test_basic_op, "add_m_m", test_add_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "norm", test_norm) == NULL)|| 
        (CU_add_test(test_basic_op, "sub_m_m", test_sub_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "sub_v_v", test_sub_v_v) == NULL) || 
        (CU_add_test(test_basic_op, "dot_prod", test_dot_prod) == NULL) ||
        (CU_add_test(test_basic_op, "mult_m_m", test_mult_m_m) == NULL) ||
        (CU_add_test(test_basic_op, "mult_m_v", test_mult_m_v) == NULL) ||
        (CU_add_test(test_basic_op, "transp", test_transp) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Exécution des tests en mode verbeux
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}
