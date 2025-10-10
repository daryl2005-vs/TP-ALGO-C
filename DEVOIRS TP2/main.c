#include <stdio.h>
#include <stdlib.h>

/* =============================
   LISTE SIMPLEMENT CHAÎNÉE
   ============================= */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Insertion en tête
Node* insererEnTete(Node* head, int val) {
    Node* nouveau = (Node*)malloc(sizeof(Node));
    nouveau->data = val;
    nouveau->next = head;
    return nouveau;
}

// Suppression de toutes les occurrences
Node* supprimerOccurrences(Node* head, int val) {
    Node *temp = head, *prev = NULL;

    while (temp != NULL) {
        if (temp->data == val) {
            if (prev == NULL) {
                head = temp->next;
                free(temp);
                temp = head;
            } else {
                prev->next = temp->next;
                free(temp);
                temp = prev->next;
            }
        } else {
            prev = temp;
            temp = temp->next;
        }
    }
    return head;
}

// Insertion triée
Node* insererTrie(Node* head, int val) {
    Node* nouveau = (Node*)malloc(sizeof(Node));
    nouveau->data = val;
    nouveau->next = NULL;

    if (!head || val < head->data) {
        nouveau->next = head;
        return nouveau;
    }

    Node* temp = head;
    while (temp->next && temp->next->data < val) {
        temp = temp->next;
    }
    nouveau->next = temp->next;
    temp->next = nouveau;
    return head;
}

// Affichage simple
void afficher(Node* head) {
    while (head) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

/* =============================
   LISTE DOUBLE CHAÎNÉE
   ============================= */
typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

DNode* insererTrieDouble(DNode* head, int val) {
    DNode* nouveau = (DNode*)malloc(sizeof(DNode));
    nouveau->data = val;
    nouveau->prev = nouveau->next = NULL;

    if (!head || val < head->data) {
        nouveau->next = head;
        if (head) head->prev = nouveau;
        return nouveau;
    }

    DNode* temp = head;
    while (temp->next && temp->next->data < val) {
        temp = temp->next;
    }

    nouveau->next = temp->next;
    if (temp->next) temp->next->prev = nouveau;
    temp->next = nouveau;
    nouveau->prev = temp;

    return head;
}

void afficherDouble(DNode* head) {
    while (head) {
        printf("%d <-> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

/* =============================
   LISTE CIRCULAIRE SIMPLE
   ============================= */
typedef struct CNode {
    int data;
    struct CNode* next;
} CNode;

CNode* insererTeteCirc(CNode* last, int val) {
    CNode* nouveau = (CNode*)malloc(sizeof(CNode));
    nouveau->data = val;

    if (!last) {
        nouveau->next = nouveau;
        return nouveau;
    }

    nouveau->next = last->next;
    last->next = nouveau;
    return last;
}

CNode* insererQueueCirc(CNode* last, int val) {
    CNode* nouveau = (CNode*)malloc(sizeof(CNode));
    nouveau->data = val;

    if (!last) {
        nouveau->next = nouveau;
        return nouveau;
    }

    nouveau->next = last->next;
    last->next = nouveau;
    return nouveau;
}

void afficherCirc(CNode* last) {
    if (!last) return;
    CNode* temp = last->next;
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while (temp != last->next);
    printf("(cercle)\n");
}

/* =============================
   LISTE CIRCULAIRE DOUBLE
   ============================= */
typedef struct DCNode {
    int data;
    struct DCNode* prev;
    struct DCNode* next;
} DCNode;

DCNode* insererTeteDoubleCirc(DCNode* last, int val) {
    DCNode* nouveau = (DCNode*)malloc(sizeof(DCNode));
    nouveau->data = val;

    if (!last) {
        nouveau->next = nouveau->prev = nouveau;
        return nouveau;
    }

    DCNode* first = last->next;
    nouveau->next = first;
    nouveau->prev = last;
    first->prev = nouveau;
    last->next = nouveau;
    return last;
}

DCNode* insererQueueDoubleCirc(DCNode* last, int val) {
    DCNode* nouveau = (DCNode*)malloc(sizeof(DCNode));
    nouveau->data = val;

    if (!last) {
        nouveau->next = nouveau->prev = nouveau;
        return nouveau;
    }

    DCNode* first = last->next;
    nouveau->next = first;
    nouveau->prev = last;
    last->next = nouveau;
    first->prev = nouveau;
    return nouveau;
}

void afficherDoubleCirc(DCNode* last) {
    if (!last) return;
    DCNode* temp = last->next;
    do {
        printf("%d <-> ", temp->data);
        temp = temp->next;
    } while (temp != last->next);
    printf("(cercle)\n");
}

/* =============================
   PROGRAMME PRINCIPAL (MENU)
   ============================= */
int main() {
    Node* listeSimple = NULL;
    DNode* listeDouble = NULL;
    CNode* circSimple = NULL;
    DCNode* circDouble = NULL;

    int choix, val, x;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Supprimer occurrences dans une liste simple\n");
        printf("2. Insertion dans une liste simple triée\n");
        printf("3. Insertion dans une liste double triée\n");
        printf("4. Insertion en tete/queue (liste circulaire simple)\n");
        printf("5. Insertion en tete/queue (liste circulaire double)\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Entrez un entier à insérer dans la liste simple : ");
                scanf("%d", &val);
                listeSimple = insererEnTete(listeSimple, val);
                afficher(listeSimple);
                printf("Entrez un entier à supprimer : ");
                scanf("%d", &x);
                listeSimple = supprimerOccurrences(listeSimple, x);
                afficher(listeSimple);
                break;

            case 2:
                printf("Entrez un entier à insérer dans la liste simple triée : ");
                scanf("%d", &val);
                listeSimple = insererTrie(listeSimple, val);
                afficher(listeSimple);
                break;

            case 3:
                printf("Entrez un entier à insérer dans la liste double triée : ");
                scanf("%d", &val);
                listeDouble = insererTrieDouble(listeDouble, val);
                afficherDouble(listeDouble);
                break;

            case 4:
                printf("1. Insérer en tete  2. Insérer en queue : ");
                scanf("%d", &x);
                printf("Valeur à insérer : ");
                scanf("%d", &val);
                if (x == 1) circSimple = insererTeteCirc(circSimple, val);
                else circSimple = insererQueueCirc(circSimple, val);
                afficherCirc(circSimple);
                break;

            case 5:
                printf("1. Inserer en tete  2. Inserer en queue : ");
                scanf("%d", &x);
                printf("Valeur à insérer : ");
                scanf("%d", &val);
                if (x == 1) circDouble = insererTeteDoubleCirc(circDouble, val);
                else circDouble = insererQueueDoubleCirc(circDouble, val);
                afficherDoubleCirc(circDouble);
                break;

            case 0:
                printf("Fin du programme.\n");
                break;

            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 0);

    return 0;
}