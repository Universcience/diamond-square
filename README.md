# `diamond-square`

Générateur procédural de terrain en 3D utilisant l'algorithme diamant-carré, développé dans le cadre de l'exposé **Informatique et autres sciences** du Palais de la Découverte.

## Description

Ce logiciel permet de générer un terrain aléatoire en utilisant l'algorithme [diamant-carré(en)](http://en.wikipedia.org/wiki/Diamond-square_algorithm).

Le programme C contenu dans `diamond-square.c` génère dans un premier temps une carte d'altitude sous la forme d'une [image PGM(en)](https://en.wikipedia.org/wiki/Portable_graymap#PGM_example).

Une vue 3D du paysage ainsi généré est ensuite produite à l'aide du logiciel de lancer de rayon [POV-Ray(en)](https://en.wikipedia.org/wiki/POV-Ray).

# Déploiement

Le programme fourni est conçu pour un système GNU/Linux ([Debian(fr)](https://www.debian.org/index.fr.html)), mais peut être aisément porté sur d'autres systèmes.

Le fichier `Makefile` fourni permet de compiler le programme C (cible `diamond-square`) ainsi que de générer un paysage et son rendu en 3D automatiquement (cible `render`).

## Paquetages nécessaires (Debian Stretch)
- `gcc`
- `make`
- `povray`

## Usage
```
git clone https://github.com/universcience/diamond-square
cd diamond-square
make render
display results/render-final.png
```

# Licence

Ceci est un [logiciel libre(fr)](https://www.gnu.org/philosophy/free-sw.fr.html), distribué sous les termes de la licence GNU GPL Affero. Les termes exacts de cette licence sont disponibles dans le fichier `LICENSE`.

En résumé, vous êtes libres de télécharger ce logiciel, de l'utiliser et de le modifier sans restrictions, ainsi que de le redistribuer sous les mêmes termes, avec ou sans modifications. 
