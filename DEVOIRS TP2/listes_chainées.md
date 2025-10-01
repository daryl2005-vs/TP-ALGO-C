# Opérations sur les Listes Chaînées

## 1. Lire un élément, vérifier et supprimer toutes ses occurrences (Liste Simplement Chaînée)

### Algorithme
```pseudo
Définir la structure Node :
    data : entier
    next : pointeur vers Node

Fonction supprimerOccurrences(head : pointeur vers Node, val : entier) :
    Si head est NULL, retourner (liste vide)
    
    // Supprimer les occurrences en tête si nécessaire
    Tant que head != NULL et head->data == val :
        temp = head
        head = head->next
        Libérer temp
    
    current = head
    Tant que current != NULL et current->next != NULL :
        Si current->next->data == val :
            temp = current->next
            current->next = current->next->next
            Libérer temp
        Sinon :
            current = current->next
    
    Retourner head (nouvelle tête si modifiée)
```

### Code C
```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void append(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct Node* last = *head;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = newNode;
}

void printList(struct Node* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

struct Node* supprimerOccurrences(struct Node* head, int val) {
    if (head == NULL) return head;

    while (head != NULL && head->data == val) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }

    struct Node* current = head;
    while (current != NULL && current->next != NULL) {
        if (current->next->data == val) {
            struct Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
        } else {
            current = current->next;
        }
    }
    return head;
}

int main() {
    struct Node* head = NULL;
    append(&head, 1);
    append(&head, 2);
    append(&head, 2);
    append(&head, 3);
    append(&head, 2);

    printf("Liste initiale: ");
    printList(head);

    int val = 2;
    head = supprimerOccurrences(head, val);

    printf("Liste après suppression de %d: ", val);
    printList(head);

    return 0;
}
```

## 2. Insertion d'un élément dans une liste chaînée triée

### a. Liste Simplement Chaînée

#### Algorithme
```pseudo
Définir la structure Node :
    data : entier
    next : pointeur vers Node

Fonction insererTriee(head : pointeur vers Node, val : entier) :
    newNode = créer Node avec data = val
    
    Si head == NULL ou head->data >= val :  // Insertion en tête
        newNode->next = head
        Retourner newNode comme nouvelle tête
    
    current = head
    Tant que current->next != NULL et current->next->data < val :
        current = current->next
    
    newNode->next = current->next
    current->next = newNode
    
    Retourner head
```

#### Code C
```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void printList(struct Node* head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

struct Node* insererTriee(struct Node* head, int val) {
    struct Node* newNode = createNode(val);

    if (head == NULL || head->data >= val) {
        newNode->next = head;
        return newNode;
    }

    struct Node* current = head;
    while (current->next != NULL && current->next->data < val) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

int main() {
    struct Node* head = NULL;
    head = insererTriee(head, 3);
    head = insererTriee(head, 1);
    head = insererTriee(head, 2);
    head = insererTriee(head, 5);
    head = insererTriee(head, 4);

    printf("Liste triée (singly): ");
    printList(head);

    return 0;
}
```

### b. Liste Doublement Chaînée

#### Algorithme
```pseudo
Définir la structure Node :
    data : entier
    prev : pointeur vers Node
    next : pointeur vers Node

Fonction insererTriee(head : pointeur vers Node, val : entier) :
    newNode = créer Node avec data = val, prev = NULL, next = NULL
    
    Si head == NULL ou head->data >= val :  // Insertion en tête
        newNode->next = head
        Si head != NULL : head->prev = newNode
        Retourner newNode
    
    current = head
    Tant que current->next != NULL et current->next->data < val :
        current = current->next
    
    newNode->next = current->next
    newNode->prev = current
    Si current->next != NULL : current->next->prev = newNode
    current->next = newNode
    
    Retourner head
```

#### Code C
```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void printList(struct Node* head) {
    while (head != NULL) {
        printf("%d <-> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

struct Node* insererTriee(struct Node* head, int val) {
    struct Node* newNode = createNode(val);

    if (head == NULL || head->data >= val) {
        newNode->next = head;
        if (head != NULL) head->prev = newNode;
        return newNode;
    }

    struct Node* current = head;
    while (current->next != NULL && current->next->data < val) {
        current = current->next;
    }

    newNode->next = current->next;
    newNode->prev = current;
    if (current->next != NULL) current->next->prev = newNode;
    current->next = newNode;

    return head;
}

int main() {
    struct Node* head = NULL;
    head = insererTriee(head, 3);
    head = insererTriee(head, 1);
    head = insererTriee(head, 2);
    head = insererTriee(head, 5);
    head = insererTriee(head, 4);

    printf("Liste triée (doubly): ");
    printList(head);

    return 0;
}
```

## 3. Insertion dans une liste chaînée circulaire

### a. Liste Simplement Chaînée Circulaire

#### Algorithme
```pseudo
Définir la structure Node :
    data : entier
    next : pointeur vers Node

// Insertion en tête
Fonction insererTete(head : pointeur vers Node, val : entier) :
    newNode = créer Node avec data = val
    Si head == NULL :
        newNode->next = newNode
        Retourner newNode
    current = head
    Tant que current->next != head : current = current->next
    newNode->next = head
    current->next = newNode
    Retourner newNode

// Insertion en queue
Fonction insererQueue(head : pointeur vers Node, val : entier) :
    newNode = créer Node avec data = val
    Si head == NULL :
        newNode->next = newNode
        Retourner newNode
    current = head
    Tant que current->next != head : current = current->next
    current->next = newNode
    newNode->next = head
    Retourner head

// Insertion au milieu (après une valeur existante 'afterVal')
Fonction insererMilieu(head : pointeur vers Node, val : entier, afterVal : entier) :
    newNode = créer Node avec data = val
    Si head == NULL : Retourner NULL
    current = head
    Faire :
        Si current->data == afterVal :
            newNode->next = current->next
            current->next = newNode
            Retourner head
        current = current->next
    Tant que current != head
    Retourner insererQueue(head, val)
```

#### Code C
```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void printList(struct Node* head) {
    if (head == NULL) return;
    struct Node* current = head;
    do {
        printf("%d -> ", current->data);
        current = current->next;
    } while (current != head);
    printf("(back to head)\n");
}

struct Node* insererTete(struct Node* head, int val) {
    struct Node* newNode = createNode(val);
    if (head == NULL) {
        newNode->next = newNode;
        return newNode;
    }
    struct Node* current = head;
    while (current->next != head) current = current->next;
    newNode->next = head;
    current->next = newNode;
    return newNode;
}

struct Node* insererQueue(struct Node* head, int val) {
    struct Node* newNode = createNode(val);
    if (head == NULL) {
        newNode->next = newNode;
        return newNode;
    }
    struct Node* current = head;
    while (current->next != head) current = current->next;
    current->next = newNode;
    newNode->next = head;
    return head;
}

struct Node* insererMilieu(struct Node* head, int val, int afterVal) {
    struct Node* newNode = createNode(val);
    if (head == NULL) return NULL;
    struct Node* current = head;
    do {
        if (current->data == afterVal) {
            newNode->next = current->next;
            current->next = newNode;
            return head;
        }
        current = current->next;
    } while (current != head);
    return insererQueue(head, val);
}

int main() {
    struct Node* head = NULL;
    head = insererTete(head, 1);
    head = insererQueue(head, 3);
    head = insererMilieu(head, 2, 1);
    head = insererTete(head, 0);
    head = insererQueue(head, 4);

    printf("Liste circulaire singly: ");
    printList(head);

    return 0;
}
```

### b. Liste Doublement Chaînée Circulaire

#### Algorithme
```pseudo
Définir la structure Node :
    data : entier
    prev : pointeur vers Node
    next : pointeur vers Node

// Insertion en tête
Fonction insererTete(head : pointeur vers Node, val : entier) :
    newNode = créer Node avec data = val
    Si head == NULL :
        newNode->next = newNode
        newNode->prev = newNode
        Retourner newNode
    newNode->next = head
    newNode->prev = head->prev
    head->prev->next = newNode
    head->prev = newNode
    Retourner newNode

// Insertion en queue
Fonction insererQueue(head : pointeur vers Node, val : entier) :
    newNode = créer Node avec data = val
    Si head == NULL :
        newNode->next = newNode
        newNode->prev = newNode
        Retourner newNode
    last = head->prev
    newNode->next = head
    newNode->prev = last
    last->next = newNode
    head->prev = newNode
    Retourner head

// Insertion au milieu (après une valeur 'afterVal')
Fonction insererMilieu(head : pointeur vers Node, val : entier, afterVal : entier) :
    newNode = créer Node avec data = val
    Si head == NULL : Retourner NULL
    current = head
    Faire :
        Si current->data == afterVal :
            newNode->next = current->next
            newNode->prev = current
            current->next->prev = newNode
            current->next = newNode
            Retourner head
        current = current->next
    Tant que current != head
    Retourner insererQueue(head, val)
```

#### Code C
```c
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void printList(struct Node* head) {
    if (head == NULL) return;
    struct Node* current = head;
    do {
        printf("%d <-> ", current->data);
        current = current->next;
    } while (current != head);
    printf("(back to head)\n");
}

struct Node* insererTete(struct Node* head, int val) {
    struct Node* newNode = createNode(val);
    if (head == NULL) {
        newNode->next = newNode;
        newNode->prev = newNode;
        return newNode;
    }
    newNode->next = head;
    newNode->prev = head->prev;
    head->prev->next = newNode;
    head->prev = newNode;
    return newNode;
}

struct Node* insererQueue(struct Node* head, int val) {
    struct Node* newNode = createNode(val);
    if (head == NULL) {
        newNode->next = newNode;
        newNode->prev = newNode;
        return newNode;
    }
    struct Node* last = head->prev;
    newNode->next = head;
    newNode->prev = last;
    last->next = newNode;
    head->prev = newNode;
    return head;
}

struct Node* insererMilieu(struct Node* head, int val, int afterVal) {
    struct Node* newNode = createNode(val);
    if (head == NULL) return NULL;
    struct Node* current = head;
    do {
        if (current->data == afterVal) {
            newNode->next = current->next;
            newNode->prev = current;
            current->next->prev = newNode;
            current->next = newNode;
            return head;
        }
        current = current->next;
    } while (current != head);
    return insererQueue(head, val);
}

int main() {
    struct Node* head = NULL;
    head = insererTete(head, 1);
    head = insererQueue(head, 3);
    head = insererMilieu(head, 2, 1);
    head = insererTete(head, 0);
    head = insererQueue(head, 4);

    printf("Liste circulaire doubly: ");
    printList(head);

    return 0;
}
```