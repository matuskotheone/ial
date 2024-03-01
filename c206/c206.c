/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    DLLElementPtr curr = list->firstElement;
    DLLElementPtr next = NULL;

    // postupne uvolneie vsetkych prvkov z pamate
    while(curr != NULL)
    {
        next = curr->nextElement;
        free(curr);
        curr = next;
    }
    // nastavenie pointrov na NULL 
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    DLLElementPtr curr = malloc(sizeof(struct DLLElement)); 
        
    if(curr == NULL) // kontorla malloc
    {
        DLL_Error();
        return;
    }

    curr->nextElement = list->firstElement;
    curr-> previousElement = NULL;
    curr->data = data;

    // ak bol list prazdny nastavia sa pointre first a last na novo vytvoreny prvok
    if(list->lastElement == NULL)
    {
        list->lastElement = curr;
        list->firstElement = curr;
        return;
    }
    
    list->firstElement->previousElement = curr;
    list->firstElement = curr;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    DLLElementPtr curr = malloc(sizeof(struct DLLElement));
    if(curr == NULL)//kontrola malloc
    {
        DLL_Error();
        return;
    }

    curr->data = data;
    curr->nextElement = NULL;
    curr->previousElement = list->lastElement;
    // ak bol list prazdny nastavia sa pointre first a last na novo vytvoreny prvok
    if(list->firstElement == NULL)
    {
        list->firstElement = curr;
        list->lastElement = curr;
        return;
    }
    list->lastElement->nextElement = curr;
    list->lastElement = curr;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    // kontorla ci nie je list prazdny
    if(list->firstElement == NULL)
    {
        DLL_Error();
        return;
    }
    *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    // kontorla ci nie je list pprazdny 
    if(list->lastElement == NULL)
    {
        DLL_Error();
        return;
    }
    *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    // kontorla ci je list prazdny 
    if(list->firstElement == NULL)
        return;
    // ak je prvy element aj aktivny nastavi sa list ako neaktivny 
    if(list->firstElement == list->activeElement)
    {
        list->activeElement = NULL;
    }
    // ak ma iba jeden prvok 
    if(list->firstElement == list->lastElement)
    {
        free(list->firstElement);
        list->firstElement = NULL;
        list->lastElement = NULL;
        return;
    }
    // nastavenie dat
    DLLElementPtr first = list->firstElement;
    list->firstElement=list->firstElement->nextElement;
    list->firstElement->previousElement = NULL;
    free(first);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    // kontrola ci je list prazdny 
    if(list->lastElement == NULL)
        return;
    // ak je posledny element aktivny nastavy sa list ako neaktivny
    if(list->lastElement == list->activeElement)
    {
        list->activeElement = NULL;
    }
    // ak ma list iba 1 prvok prevedie sa to takto 
    if(list->firstElement == list->lastElement)
    {
        free(list->firstElement);
        list->firstElement = NULL;
        list->lastElement = NULL;
        return;
    }
    // inak takto
    DLLElementPtr last = list->lastElement;
    list->lastElement = list->lastElement->previousElement;
    list->lastElement->nextElement = NULL;
    free(last);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    // kontrola ci je aktivny a ci nie je posledny
    if(list->activeElement == NULL || list->activeElement->nextElement == NULL)
        return; 
    // ak je predposledny pouzije sa funkcia delete last
    if(list->activeElement == list->lastElement->previousElement)
    {
        DLL_DeleteLast(list);
        return;
    }
    // nastavenie dat
    DLLElementPtr todelete = list->activeElement->nextElement;
    list->activeElement->nextElement = todelete->nextElement;
    todelete->nextElement->previousElement = todelete->previousElement;
    free(todelete);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    // kontrola ci je aktivny a ci nie je prvy 
    if(list->activeElement == NULL || list->activeElement->previousElement == NULL)
    {
        return; 
    }

    // kontrola ci je aktivny element druhy ak ano pouzijeme funkciu DeleteFirst
    if(list->activeElement == list->firstElement->nextElement)
    {
        DLL_DeleteFirst(list);
        return;
    }
    

    // nastavenie dat
    DLLElementPtr todelete = list->activeElement->previousElement;
    list->activeElement->previousElement = todelete->previousElement;
    todelete->previousElement->nextElement = todelete->nextElement;
    free(todelete);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    // kontrola ci je prazdny 
    if(list->activeElement == NULL)
        return; 

    DLLElementPtr new = malloc(sizeof(struct DLLElement));

    if(new == NULL) //kontrola malloc 
    {
        DLL_Error();
        return;
    }
    // kontrola ci je active element aj posledny 
    if(list->activeElement == list->lastElement)
    {
        free(new);
        DLL_InsertLast(list,data);
        return;
    }

    // nastavenie dat
    new->nextElement = list->activeElement->nextElement; 
    new->previousElement = list->activeElement; 
    new->data = data; 
    list->activeElement->nextElement->previousElement = new;
    list->activeElement->nextElement = new;
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    // kontorla ci je list aktivny 
    if(list->activeElement == NULL)
        return; 

    DLLElementPtr new = malloc(sizeof(struct DLLElement));

    if(new == NULL)//kontrola malloc
    {
        DLL_Error();
        return;
    }

    // kontrola ci je active element aj prvy element
    if(list->activeElement == list->firstElement)
    {
        free(new);
        DLL_InsertFirst(list,data);
        return;
    }
    // nastavenei dat
    new->nextElement = list->activeElement; 
    new->previousElement = list->activeElement->previousElement; 
    list->activeElement->previousElement-> nextElement= new;
    new->data = data; 
    list->activeElement->previousElement = new;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    // kontorla ci je list aktivny 
    if(list->activeElement == NULL)
    {
        DLL_Error();
        return;
    }

    *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    // kontorla ci je list aktivny 
    if(list->activeElement == NULL)
    {
        return;
    }
    list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    // kontorla ci je list aktivny 
    if(list->activeElement == NULL)
    {
        return;
    }
    list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    // kontorla ci je list aktivny 
    if(list->activeElement == NULL)
    {
        return;
    }
    list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement == NULL ? 0:1;
}

/* Konec c206.c */
