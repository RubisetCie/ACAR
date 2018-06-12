/*  Auteur      : Matthieu Carteron
    Langage     : Arduino
    Description : Le fichier principal permettant de contrôler le robot
*/

// Déclaration de la structure "Point" représentant un point en 2D :
struct Point
{
  int x;
  int y;
}

// Déclaration des variables :
Point points[12];     // Les points atteignables et leur coordonnées
Point* pointCourant;  // Pointeur vers le point sur lequel le robot se trouve

// Fonction "setup" exécutée au lancement :
void setup()
{
}

// Fonction "loop" exécutée continuellement :
void loop()
{
}

// Fonction "pointSuivant" permet de se diriger vers le prochain point :
void pointSuivant(unsigned int pt)
{
}

