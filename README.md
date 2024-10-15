# TP1 Cryptographie

Dans ce TP, nous avons étudié et mis en œuvre le chiffrement de Vigenère, un chiffrement polyalphabétique basé sur une clé. Le principe consiste à associer chaque lettre du texte à chiffrer à une lettre de la clé (répétée si nécessaire) pour décaler les lettres du texte en fonction de la valeur de la lettre correspondante de la clé. Nous avons aussi abordé le déchiffrement (retrouver le texte clair) ainsi que la cryptanalyse du chiffre de Vigenère repose sur deux étapes principales : identifier la période (la longueur de la clé) et trouver la clé elle-même. 

## Membres de l'équipe.

 + Nouhaila Jabbar
 + Rim Sadiqi

### Détails des fichiers

* Squelette de Code/ : Ce répertoire contient le code source principal :

     + main_cipher.cpp : Ce fichier implémente un chiffrement et un déchiffrement de texte basé sur l'algorithme de Vigenère, en utilisant une clé fournie qui est agrandie si nécessaire. Le texte est d'abord normalisé en majuscules, puis chiffré ou déchiffré en fonction de la clé.
 
     + main_cryptanalysis.cpp : Ce fichier effectue une cryptanalyse du chiffre de Vigenère. Il calcule l'indice de coïncidence pour estimer la longueur de la clé, puis utilise un test chi-carré pour déterminer la clé exacte en comparant les fréquences des lettres du texte chiffré avec celles de la langue anglaise ou française. Enfin, il reconstitue la clé utilisée pour déchiffrer le texte.
 



