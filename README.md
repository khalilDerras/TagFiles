# TagFiles
Tagging Files System in Linux

Marquer un fichier avec un tag :

./main -t file tag

● Effacer un tag d'un fichier :

./main -t file tag

● Lister les tags d'un fichier :

./main -l file

● Lister les fichiers possédant une combinaison de tags :

./main -re répertoire

Et taper la combinaison des tags voulu dans une nouvelle ligne.

● Définir un lien entre deux tags :

./main -f tagPere tagFils

● Spécifier un lien hiérarchique entre deux tags :

./main -lh tag1 tag2
