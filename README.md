# MBot-sumo-challenge

Programme du MBot pour gagner des combats de sumo.

> Ce petit tutoriel aidera l'équipe à développer les programmes.

## Le robot

Le MBot héberge de nombreux capteurs et actionneurs sur sa carte mère et sous forme de modules (capteur à ultrasons, module suiveur de ligne…). C'est en réalité une carte Arduino personnalisée !

## Le programme

Le programme est écrit en C++, un langage de programmation couramment utilisé dans le domaine (notamment pour les Arduino). La logique est semblable à celle du Python, c'est principalement la syntaxe qui diffère. Par exemple, le C++ utilise des accolades `{}` pour délimiter les fonctions et des point virgules `;` à la fin des lignes.

> Une fonction est un bloc d'instructions, des lignes de code s'exécutant de haut en bas. Une fonction peut être exécutée en l'appelant depuis un autre endroit du programme.

De nombreuses fonctions sont disponibles pour contrôler facilement les divers composants du robot. Leur fonctionnement est expliqué plus loin dans ce guide.

Tous les fichiers sont commentés pour faciliter la compréhension du programme. La documentation des fonctions est affichée lorsqu'elles sont pointées par la souris.

Le programme est réparti en plusieurs fichiers ayant différentes fonctions. Le fichier principal est `main.cpp`, il contient les deux fonctions fondamentales d'un programme Arduino :

- Le `void setup()` qui s'exécute une seule fois lors du démarrage du MBot. Cette fonction sert à initialiser tous les composants du robot.
- Le `void loop()` qui s'exécute en boucle après le `void setup()`. Cette fonction exécute toutes les actions du robot.

Votre zone de travail est ici : vous pouvez mettre des choses dans les deux fonctions selon vos besoins.

Le projet contient aussi un fichier `functions.cpp` : c'est ici que certaines fonctions sont écrites pour nous simplifier la vie !

> Chaque fichier `.cpp` a un fichier `.hpp` associé (par exemple, `main.hpp`). Ils servent a expliquer, en quelque sorte, ce que fait le fichier `hpp` au reste du programme. Il n'est pas nécessaire de les modifier.

### Utilisations des capteurs et des actionneurs

#### Le module suiveur de ligne

Il permet de détecter le noir et le blanc.

Son utilisation est simple : la fonction `onBoardLineFinder.readSensors()` renvoie un des éléments du tableau :

| Valeur renvoyée | Correspondance                                                                               |
|-----------------|----------------------------------------------------------------------------------------------|
| S1_IN_S2_IN     | Les deux capteurs sont sur la couleur noire.                                                 |
| S1_IN_S2_OUT    | Le capteur gauche est sur la couleur noire. Le capteur droit n'est pas sur la couleur noire. |
| S1_OUT_S2_IN    | Le capteur gauche n'est pas sur la couleur noire. Le capteur droit est sur la couleur noire. |
| S1_OUT_S2_OUT   | Les deux capteurs ne sont pas sur la couleur noire.                                          |

Exemple d'utilisation :

```cpp
// Un `switch` permet de choisir une option parmis celles disponibles, c'est plus court que des `if … else`. Pour chaque possibilité, un code couleur est associé.
switch (onBoardLineFinder.readSensors())
{
case S1_IN_S2_IN:
    setLED(255, 255, 0);
    break;
case S1_IN_S2_OUT:
    setLeftLED(255, 255, 0);
    setRightLED(0, 0, 0);
    break;
case S1_OUT_S2_IN:
    setLeftLED(0, 0, 0);
    setRightLED(255, 255, 0);
    break;
case S1_OUT_S2_OUT:
    setLED(0, 0, 0);
    break;
default:
    break;
}
```

#### Le capteur d'ultrasons

Il permet de détecter un objet à l'avant du robot, et de donner sa distance.

Pour l'utiliser, deux méthodes existent :

- La fonction `onBoardUltrasonicSensor.distanceCm()` renvoie la distance.
- La fonction `onBoardUltrasonicSensor.distanceCm(int maximimDistance)` renvoie la distance avec une valeur de `maximumDistance` au maximum.

> À vérifier : il est possible que la mesure ne marche que tout les 100ms.

Exemple d'utilisation :

```cpp
// On émet un son dont la fréquence dépend de la distance avec l'objet détecté.
onBoardBuzzer.tone(onBoardUltrasonicSensor.distanceCm(), 100);
```

#### Le module infrarouge

Ce module permet de communiquer via les ondes infrarouges. Cela permet d'utiliser la télécommande infrarouge et de communiquer entre les robots.

Les touches de la télécommande vont de `BUTTON_A` à `BUTTON_F`, de `BUTTON_0` à `BUTTON_9`. Il y a aussi les quatre flèches et `BUTTON_SETTINGS`.

Exemple d'utilisation :

```cpp
// Si la touche A est pressés :
if (onBoardInfraredSensor.keyPressed(BUTTON_A))
{
    // On envoye un message à l'ordinateur.
    Serial.println("Touche A pressée.");
}

// Si la touche B est pressée :
if (onBoardInfraredSensor.keyPressed(BUTTON_B))
{
    // On envoye un message à l'ordinateur.
    Serial.println("Touche B pressée.");
}
```

#### Les roues

Le robot peut se déplacer grâce à la fonction `void moveMBot(int direction, int speed)`.

La direction :

| Valeur dans le programme | Correspondance                       |
|--------------------------|--------------------------------------|
| FORWARD                  | Avancer                              |
| BACKWARD                 | Reculer                              |
| LEFT                     | Tourner à gauche (sens anti-horaire) |
| RIGHT                    | Tourner à droite (sens horaire)      |

La vitesse est une valeur de `0` à `255`. Il est aussi possible de renseigner une valeur négative pour effectuer le déplacement inverse.

Exemple d'utilisation :

```cpp
// Déplacement avant à la vitesse maximale.
moveMBot(FORWARD, 255);

// On attend une seconde.
delay(1000);

// Arrêt du mouvement (la vitesse étant nulle).
moveMBot(FORWARD, 0);
```

#### Les DEL RVB

Le robot est équipé de deux DEL RVB sur l'avant de la carte mère.

Le contrôle se fait via trois fonctions donc les paramètres sont identiques : il faut préciser une valeur d'intensité pour chaque composante du cercle chromatique : le rouge, le vert et le bleu.

![Image du cercle chromatique](https://lh3.googleusercontent.com/proxy/i_xDK_vClI1pSmS7aN7AY14Yd8deIACOkagTgRVf7QYbOSZgFMp3Bb0UGoxnVatW1pPVgbzKLP6zxeTvamt9AaplFWKGHdzqbjmQFCYgVWlY9YhmCo2Xn7WOGVKJy7qPafvxuIpCKpnCHjP3yEUU-DtPzf_xvI8bXN2SY-ecxA)

Les valeurs vont de 0 (éteint) à 255 (pleine luminosité). Il est ainsi possible d'avoir 16 581 375 nuances !

Vous pouvez utiliser un site tel que [celui-ci](https://www.google.com/search?client=firefox-b-e&q=s%C3%A9lecteur+de+couleur) pour trouver la couleur parfaite. Prenez la valeur RGB.

Les fonctions sont :

- `setLeftLED(int r, int g, int b)` pour contrôler la DEL gauche.
- `setRightLED(int r, int g, int b)` pour contrôler la DEL droite.
- `setLED(int r, int g, int b)` pour contrôler les deux DEL.

Exemple d'utilisation :

```cpp
// Un magnifique violet !
setLeftLED(255, 0, 200);
```

#### Le buzzer

Sans-doute le composant le plus important : il permet de marquer la défaite de l'adversaire par une musique de la victoire.

Une fonction est disponible : `onBoardBuzzer.tone(int frequency, int duration);`. Attention ! La fonction est bloquante : le programme d'après ne s'exécute pas tant que le son n'est pas terminé.

Exemple d'utilisation (notamment à essayer sur les MBot de la concurrence) :

```cpp
// Boucle qui s'exécute à l'infini.
while (1)
{
    // Génération d'un bruit dont la fréquence augmente lentement.
    for (int i = 0; i < 9000; i++)
    {
        onBoardBuzzer.tone(i, 1);
    }

    // Génration d'un bruit dont la fréquence diminue lentement.
    for (int i = 9000; i > 0; i--)
    {
        onBoardBuzzer.tone(i, 1);
    }
}
```

#### Communication avec l'ordinateur

Le MBot peut envoyer des messages à l'ordinateur grâce aux fonctions suivantes :

- `Serial.print()` envoie un message sans retour à la ligne à la fin.
- `Serial.println()` envoie un message avec un retour à la ligne à la fin.

Exemple d'utilisation :

```cpp
Serial.println("Hello World!");

Serial.print("Bonjour ");
Serial.println("Monde !");
```

Ce qui produit la sortie :

```out
Hello World!
Bonjour Monde !
```

#### Bouton embarqué

Le MBot embarque un bouton sur sa carte mère.

La fonction `buttonPressed()` renvoie `true` si le bouton est actuellement pressé.

Exemple d'utilisation :

```cpp
// Si le bouton est pressé.
if (buttonPressed())
{
    // On attend qu'il soit relaché.
    while (buttonPressed())
        delay(1);

    // Puis on attend un peu par précaution !
    delay(50);

    // Enfin, on effectue une action :
    setLeftLED(255, 0, 200);
}
```

## Visual Studo Code

C'est le logiciel utilisé pour la programmation. Toutes les fonctionnalités de développement y sont intégrées : édition du code, compilation et envoi.

Pour compiler puis téléverser le programme, cliquer sur le bouton en haut à droite nommé `upload` (c'est une flèche vers la droite).

Pour lancer la communication en direct avec le MBot, cliquer sur l'icône de prise électrique en haut à droite.

## GitHub

GitHub permet de développer un programme en équipe : c'est notre cas ! Chaque ordinateur possède une copie complète du programme qu'il peut modifier. Par la suite, il faut l'envoyer dans le stockage en ligne qui est [ici](https://github.com/zetiti10/MBot-sumo-challenge). Il est possible de consulter en ligne l'historique de toutes les modifications effectuées.

Il existe un système de branche pour avoir plusieurs versions indépendantes du programme en parallèle.
