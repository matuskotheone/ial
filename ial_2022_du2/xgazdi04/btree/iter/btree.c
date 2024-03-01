/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL)
    {
        if (tree->key > key)
        {
            tree = tree->left;
        }
        else if(tree->key < key)
        {
            tree = tree->right;
        }
        else 
        {
            *value = tree->value;
            return 1;
        }
    }
    return 0;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if(tree == NULL)
        return;
    bst_node_t* new = malloc(sizeof(bst_node_t));
    new->key = key;
    new->value = value;
    new->left = NULL;
    new->right= NULL;

    if (*tree == NULL)
    {
        *tree = new;
        return;
    }

    bst_node_t* temp = *tree; 
    while(temp != NULL)
    {
        if (temp->key > key)
        {
            if(temp->left == NULL)
            {
                temp->left = new;
                return;
            }
            temp = temp->left;
        }
        else if(temp->key < key)
        {
            if(temp->right == NULL)
            {
                temp->right = new;
                return;
            }
            temp = temp->right;
        }
        else 
        {

            temp->value = value;
            free(new);
            return;
        }
    }
}









    /*
    */



    /*
}

 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if(tree == NULL)
        return;
    if(target == NULL)
        return;
    bst_node_t* temp = *tree;
    bst_node_t* prev = NULL;
    while (temp->right != NULL)
    {
        prev = temp;
        temp = temp->right;
    }
    (void)prev;
    target->key = temp->key;
    target->value = temp->value;
    bst_delete(tree, temp->key);
    /*
    if (temp->right == NULL && temp->left == NULL)
    {
        target->value = temp->value;
        target->key = temp->key;
        prev->right = NULL;
        free(temp);
        temp = NULL;
    }
    else 
    {
        prev->right = temp->left;
        target->value = temp->value;
        target->key = temp->key;
        prev->right = NULL;
        free(temp);
        temp = NULL;
    }
    */
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    if(tree == NULL)
        return;
    bst_node_t* curr = *tree;
    bst_node_t* prev = NULL;
    bool last_side; // 0 lava 1 prava
    while (curr != NULL)
    {
        if (curr->key == key)
        {
            if (curr->left == NULL && curr->right == NULL)
            {
                if (last_side == 0)
                {
                    prev->left = NULL;
                }
                else {
                    prev->right = NULL;
                }
                free(curr);
                return;
            }
            if (curr->left == NULL && curr->right != NULL)
            {
                if (last_side == 0)
                {
                    prev->left = curr->right;
                }
                else {
                    prev->right = curr->right;
                }
                free(curr);
                curr = NULL;
                return;
            }
            if (curr->left != NULL && curr->right == NULL)
            {
                if (last_side == 0)
                {
                    prev->left = curr->left;
                }
                else {
                    prev->right = curr->left;
                }
                free(curr);
                curr = NULL;
                return;
            }
            if (curr->left != NULL && curr->right != NULL)
            {
                bst_replace_by_rightmost(curr, &curr->left);
                return;
            }

        }
        if(curr->key < key)
        {
            prev = curr;
            curr = curr->right;
            last_side = 1;
        }
        else{
            prev = curr;
            curr = curr->left;
            last_side = 0;
        }
    }

}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */

void bst_dispose(bst_node_t **tree) {
    if(tree == NULL)
        return;

    stack_bst_t stack;
    stack_bst_init(&stack);
    
    bst_node_t *curr = NULL;
    stack_bst_push(&stack, *tree);

    while (stack_bst_empty(&stack))
    {
        curr = stack_bst_pop(&stack);
        if(curr->left !=  NULL)
        {
            stack_bst_push(&stack, curr->left);
        }
        if(curr->right !=  NULL)
        {
            stack_bst_push(&stack, curr->right);
        }
        free(curr);
        curr = NULL;
    }
    *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while(tree != NULL) 
    {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
    if(stack == NULL)
    {
        return;
    }
    stack_bst_init(stack);

    bst_leftmost_preorder(tree, stack);
    while(!stack_bst_empty(stack)) 
    {
        bst_node_t *tmp = stack_bst_top(stack);
        stack_bst_pop(stack);
        bst_leftmost_preorder(tmp->right, stack);
    }
    free(stack);

}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while(tree != NULL) 
    {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {

    stack_bst_t *stack = malloc(sizeof(stack_bst_t));
        if (stack == NULL) {
            return;
        }
        stack_bst_init(stack);

        bst_leftmost_inorder(tree, stack);

        while (!stack_bst_empty(stack)) {
            tree = stack_bst_top(stack);
            stack_bst_pop(stack);
            bst_print_node(tree);
            bst_leftmost_inorder(tree->right, stack);
        }
        free(stack);





}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    stack_bst_t *node_stack = malloc(sizeof(stack_bst_t));
    if (node_stack == NULL) {
        return;
    }
    stack_bst_init(node_stack);
    stack_bool_t *bool_stack = malloc(sizeof(stack_bool_t));
    if (bool_stack == NULL) {
        return;
    }
    stack_bool_init(bool_stack);

    bool isFromLeft;

    bst_leftmost_postorder(tree, node_stack, bool_stack);

    while (!stack_bst_empty(node_stack)) {
        tree = stack_bst_top(node_stack);
        isFromLeft = stack_bool_top(bool_stack);
        stack_bool_pop(bool_stack);
        if (isFromLeft) {
            stack_bool_push(bool_stack, false);
            bst_leftmost_postorder(tree->right, node_stack, bool_stack);
        }
        else {
            stack_bst_pop(node_stack);
            bst_print_node(tree);
        }
    }

    free(node_stack);
    free(bool_stack);
}
