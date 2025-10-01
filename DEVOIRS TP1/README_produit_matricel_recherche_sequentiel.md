README

Ce dossier contient deux programmes en langage C ainsi qu'un fichier explicatif.

Programme 1  Recherche séquentielle dans un tableau

But
Le programme permet de rechercher un élément dans un tableau d entiers en utilisant la méthode de recherche séquentielle.

Fonctionnement
1. Le programme commence par demander à l utilisateur la taille du tableau.
2. Ensuite il demande de remplir le tableau en saisissant chaque valeur.
3. L utilisateur entre la valeur à rechercher.
4. Le programme parcourt le tableau élément par élément en utilisant une boucle for.
5. Si la valeur recherchée est trouvée le programme affiche la position où elle a été trouvée.
6. Si la valeur n existe pas dans le tableau le programme affiche un message disant que la valeur n a pas été trouvée.

Raisons des choix
- La boucle for est utilisée pour parcourir les cases du tableau une par une.
- La condition if est utilisée pour comparer chaque élément du tableau avec la valeur recherchée.
- L utilisation d un compteur ou d un indicateur permet de savoir si la valeur a été trouvée ou non.

Programme 2  Produit matriciel

But
Le programme calcule le produit de deux matrices de dimensions compatibles.

Fonctionnement
1. Les dimensions des matrices sont fixées dans le programme pour simplifier.
2. Les deux matrices sont déclarées et initialisées avec des valeurs.
3. Une troisième matrice résultat est déclarée.
4. Le programme utilise une triple boucle imbriquée pour calculer le produit matriciel.
   - La première boucle parcourt les lignes de la première matrice.
   - La deuxième boucle parcourt les colonnes de la deuxième matrice.
   - La troisième boucle parcourt les colonnes de la première matrice et les lignes de la deuxième matrice pour effectuer la somme des produits.
5. Le programme affiche ensuite la matrice résultat sous forme de tableau.

Raisons des choix
- Trois boucles imbriquées sont nécessaires car chaque élément de la matrice résultat est une combinaison d une ligne de la première matrice et d une colonne de la deuxième matrice.
- La mise à zéro de la matrice résultat avant le calcul permet de ne pas avoir de valeurs aléatoires.
- L affichage est fait ligne par ligne pour respecter la structure d une matrice.
