# environnement_de_conception_de_capteurs_sans_fil

## Objectifs

L'objectif de notre projet est celui d'améliorer le processus de conception et de maintenance des systèmes embarqués. Il s 'agit ici de fournir aux developpeurs des outils facilitant la remontée en abstraction, ainsi les developpeurs peuvent se concentrer sur ce que doit faire l’application, sur l’assemblage de « briques de bases » (capteur, radio, algorithme, machine à état) plutôt que sur les détails d’implémentations.

## démarche

le matériel utilisé:

- un microcontrolleur Arduino uno R3
- un capteur de temnperature et d'humidité DHT11
- un microcontrolleur ESP3286 comme module wifi
- une platine d'expérimentation pour effectuer les differents cablages

nous avons commencé par écrire un simple programme qui récupere les données d'un capteur de temperature et d'humidité, et qui les stockent dans la memoire EEPROM de l'arduino uno R3.<br>
[tempAndHumi.ino](tempAndHumi.ino)<br>
l'étude de ce programme nous a permis de comprendre comment les données sont récupérées au niveau des capteurs, et comment celles-ci sont stockées.
Par la suite nous avons developpé un programme qui permets d'heberger un serveur sur le microcontrolleur. Sur ce server nous avons stocké les données de temperature et d'humidité rélevées par le capteur DHT11. Ces données peuvent etre récupérées par n'importe quel client qui se connecte à notre server.<br>
[webServer.ino](webServer.ino)<br>
Par la suite nous avons essayé de définir un metamodèle qui pourrait nous permettre de modéliser le précédent programme Cf. résultats.
Après avoir developpé et testé le serveur, nous avons constaté que celui restait actif meme en l'abscence de connection d'un client.
A ce point nous avons été confrontés à deux problématiques:

- Avancer sur le développement du Metamodèle précédement obtenu
- Trouver un moyen d'économiser de l'énergie en mettant en veille le serveur, et en le reactivant que lorsqu'on reçoit une demande de connection de la part d'un client.

Nous avons choisis de nous orienter vers la deuxieme problematique.
Pour ce faire nous avons fait le choix d'utiliser la librairie Sleep.h. L'idée ici est celle de mettre le microcontrolleur dans un état de sommeil profond, et d'utiliser une interruption matériel pour le réactiver chaque fois qu'on reçoit une demande de connection de la part d 'un client.
La mise en place de notre solution s'est revelée plus compliquée que prévu. Nous nous somme rendu compte que çela nécéssitait une connaissance beaucoup plus approfondie du fonctionnement du microcontrolleur.

## les résultats

### le métamodèle

Le métamodèle obtenu consiste en une division des differents composantes en 2 type de blocs:<br>

BLOCS MATéRIEL

- Microcontrolleur
- Module wifi
- Capteurs
- Communication serie
- Comunicatinos analogique
- Communication numerique

BLOCS LOLIGIELS

- Verifier la presence du bloc wifi
- Se connecter au wifi
- Demmarer le serveur
- Lire les données des capteurs et les stocker en mémoire
- Recuperer les données en mémoire
- Traiter les requettes du client
