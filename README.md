# ElegonWorld
ElegonWorld était un serveur privé World of Warcraft 3.3.5 qui visait à revoir complètement l'expérience de jeu offerte aux joueurs. Au sein de ce serveur, il était possible de trouver
un contenu additionnel et innovant au lien avec de nombreux aspects du jeu. Nouvelles quêtes, nouveau champ de bataille, mécanismes de jeu exclusifs, etc.

Cela fait maintenant un bon moment que ce serveur a fermé ses portes, et nous avons aujourd'hui décidé de publier le code source du projet. 

Il est possible que le code présent dans ce dépôt __ne se compile pas__, nous n'avons pas vérifié. Néanmoins, deux dossiers sont particulièrement intéressants pour vous:
* src/server/scripts/custom
* src/server/game/Scripting

Ils contiennent l'ensemble des scripts que nous avons ajouté au jeu de base. Il est fort probable que vous ne soyez pas en mesure de les utiliser directement. Par exemple, certains font appel à des créatures
dont les informations sont censées être stockées dans la base de données, mais nous n'avons pas été en mesure de récupérer la base de données complète. 

Ci-dessous, vous retrouverez une courte présentation des scripts disponibles.

# Scripts 
## Mastodonte
__Fichiers__: Scripting>MastoHandler.h, .cpp et Custom>script\_ffa\_mastodonte.cpp

Il s'agit d'un événement qui se lance toutes les X minutes. Durant cet événement, tous les joueurs qui se trouvent dans une zone définie sont groupés ensemble à l'exception d'un: le mastodonte.
Le but des joueurs groupés est de tuer le mastodonte (en un temps limité), joueur dont les caractéristiques sont augmentées en fonction du nombre de joueurs qu'il a à affronter.
Le gagnant (le mastodonte ou les personnes du groupe qui l'attaque) reçoit différents récompenses.

## Sponsorship
__Fichiers__: Scripting>SponsorshipHandler.h, .cpp et Custom>script\_creature\_parrainage.cpp

Il s'agit d'une fonctionnalité qui propose aux joueurs de parrainer de nouveaux venus sur le serveur. Les membres d'un lien de parrainage disposent de différents avantages lorsqu'ils jouent ensemble.

## Failles
__Fichiers__: Scripting>RiftHandler.h, .cpp et Custom>script\_rift\_creature.cpp, script\_npc\_info\_rift.cpp, script\_rift\_creature.cpp, script\_run\_rift.cpp
Le système de _failles_ proposé ici puise ses fondements dans le système de failles présent dans un jeu appelé Rift. A différents endroits à travers le monde, une faille peut apparaître. Quand c'est le cas, les joueurs
sont avertis de la zone dans laquelle une faille s'ouvre, et doivent alors s'y rendre pour essayer de la trouver.
Lorsqu'ils approchent une faille, les joueurs sont automatiquement groupés et doivent tuer les créatures qui en sortent (d'une sorte de portail), suivi d'un mini boss. S'ils y parviennent, la faille se referme et une autre faille s'ouvre ailleurs.

## Séries d'éliminations
Le système de séries d'éliminations qui était présent sur ElegonWorld est disponible sur [ce dépôt Github](https://github.com/AmaVic/Maelstrom-Scripts), proposant des scripts additionnels. 

## Système de loterie
__Fichiers__: Custom>Lottery\_System.cpp

Un système de loterie où les joueurs peuvent acheter un billet de loterie et ont une chance d'obtenir de l'argent s'ils ont le ticket gagnant.

## Autres scripts
D'autres scripts sont également disponibles: champ de bataille inspiré de League of Legends, un système de potager, un pnj qui présente le serveur, une implémentation de la recherche de groupe de raid, un système de classement PvP, etc.
Nous vous laissons le plaisir et la curiosité de fouiller tous les fichiers qui vous semblent intéressants.

# Avertissement
Nous pensons qu'il sera difficile de simplement copier-coller les scripts pour qu'ils fonctionnent. Nous espérons que certains d'entre vous seront capables d'extraire les parties pertinentes du code pour les utiliser au sein de leur serveur.

Bon amusement,
L'équipe Elegon World. 
