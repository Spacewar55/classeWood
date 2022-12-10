Le but de ce projet est de réaliser un système atmosphérique connecté pour le four de séchage d'une entreprise.
Le fonctionnement de ce sysème est simple, l'ouvrier place le bois dans le four de chauffage et clique le bouton Démarrer.
Si le four est à la bonne température, le four se lance sinon ce dernier attend d'arriver à la température adéquate.

Pour réaliser ce projet, nous avons utiliser un esp32, un senseur de température ainsi qu'un écran oled pour l'affichage de différentes infomrations en temps réel.

Pour gérer la liste des bois ainsi que leur caractéristiques, on récupère ces différentes données grâce à une API.

A l'heure actuelle, il est possible de choisir un bois dans la liste. Quant à l'écran oled, ce dernier affiche toutes les vues d'initilisation du sytème ainsi que les vues 
adéquates en fonction de la température retournée par le senseur de température.
