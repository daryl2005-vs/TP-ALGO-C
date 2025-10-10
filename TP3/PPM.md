# Application de Manipulation d'Images PPM

## Table des matières

- [Introduction](#introduction)
- [Description du sujet](#description-du-sujet)
- [Prérequis](#prérequis)
- [Construction du projet](#construction-du-projet)
- [Utilisation](#utilisation)
  - [Démarrage de l'application](#démarrage-de-lapplication)
  - [Commandes disponibles](#commandes-disponibles)
- [Détails d'implémentation](#détails-dimplémentation)
- [Contribution](#contribution)
- [Licence](#licence)

## Introduction

Ce projet est réalisé dans le cadre du cours **INF231 : Technique de Conception d'Algorithmes et Structures de Données** à l'Université de Yaoundé 1, Département d'Informatique, pour l'année académique 2025-2026. Il s'agit de la fiche de TP n°1 intitulée **Traitement d'images avec les fichiers et tableaux**, visant à développer une application de manipulation d'images au format PPM (Portable PixMap).

## Description du sujet

L'objectif est de créer une application permettant de manipuler des images PPM, un format d'image non compressé qui stocke les pixels avec leurs coordonnées RGB (rouge, vert, bleu) variant de 0 à 255. Le format PPM P3 (utilisé ici) se compose :

- D'une en-tête avec quatre valeurs :
  1. La version (ex. "P3")
  2. La largeur de l'image en pixels
  3. La hauteur de l'image en pixels
  4. La valeur maximale de chaque composante de couleur
- Des commentaires (lignes commençant par `#`, ignorées)
- Les données des pixels, énumérées ligne par ligne de gauche à droite et de haut en bas, avec les valeurs RGB séparées par des espaces ou des sauts de ligne.

Exemple d'image P3 :
```
# exemple d'image
4 # carré de 4 sur 4 pixels
255
3 8 25  0 0 0  0 0 0  15 0 15
0 0 0  0 15 7  0 0 0  255 0 18
0 0 0  0 0 0  0 15 7  0 0 0
15 0 15  0 0 0  0 0 0  66 0 4
```

## Prérequis

- Compilateur C (par exemple `gcc`)
- Compréhension de base des entrées/sorties de fichiers et des tableaux en C

## Construction du projet

Compilez le code source avec la commande suivante :

```
gcc tps1.c -o ppmviewer
```

Assurez-vous que tous les fichiers nécessaires (par exemple `tps1.c`) sont dans le répertoire de travail.

## Utilisation

### Démarrage de l'application

Exécutez l'exécutable compilé :

```
./ppmviewer
```

L'application affichera une invite, par exemple `ppmviewer >`, indiquant qu'elle est prête à recevoir des commandes.

### Commandes disponibles

- `size <nom_fichier>` : Affiche les dimensions de l'image.
- `gris <nom_fichier>` : Convertit l'image en niveaux de gris et la sauvegarde sous `image_gris.ppm`.
- `neg <nom_fichier_entree> <nom_fichier_sortie>` : Crée un négatif de l'image et le sauvegarde sous `nom_fichier_sortie`.
- `cut <nom_fichier_entree> <l1> <l2> <c1> <c2> <nom_fichier_sortie>` : Découpe une portion de l'image définie par les coordonnées `(l1, c1)` à `(l2, c2)` et la sauvegarde sous `nom_fichier_sortie`.
- `dom <R/V/B> <valeur> <nom_fichier>` : Assombrit ou éclaircit les pixels ayant une dominante de couleur spécifiée (R, V ou B) avec `valeur` (positive pour éclaircir, négative pour assombrir) et sauvegarde sous `image_modifiee.ppm`.
- `fil <nom_fichier>` : Applique un filtre médian et sauvegarde sous `image_filtre.ppm`.
- `quit` : Quitte l'application.

## Détails d'implémentation

Le programme implémente les fonctions suivantes :

- **Chargement et sauvegarde** : `load_ppm` lit un fichier PPM en analysant l'en-tête et les données des pixels, tandis que `save_ppm` écrit l'image modifiée dans un fichier.
- **Manipulation d'images** :
  - `convert_to_grayscale` : Calcule la moyenne des valeurs RGB pour créer une image en niveaux de gris.
  - `create_negative` : Inverse chaque valeur RGB (maxval - valeur originale).
  - `crop_image` : Extrait une région rectangulaire selon les coordonnées fournies.
  - `darken_lighten` : Ajuste l'intensité des pixels en fonction de la couleur dominante.
  - `apply_median_filter` : Applique un filtre médian 3x3 pour réduire le bruit.
- **Utilitaire** : `clamp` garantit que les valeurs restent dans la plage 0 à maxval.

La structure `Image` stocke la largeur, la hauteur, la valeur maximale et les données des pixels sous forme d'un tableau de structures `Pixel` (r, g, b).

## Contribution

Ce projet a été développé dans le cadre du TP n°1 d'INF231.
