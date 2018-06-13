/*  Auteur      : Matthieu Carteron
    Langage     : Arduino
    Description : Le fichier principal permettant de contrôler le robot
*/

// Inclusion de la librairie :
#include "Grove_I2C_Motor_Driver.h"

// Déclaration des constantes :
#define I2C_ADDRESS 0x0f

#define AV_MAX -100   // La vitesse maximale des moteurs en avant (négative)
#define AV_MIN -70    // La vitesse minimale des moteurs en avant (négative)
#define AR_MAX 100    // La vitesse maximale des moteurs en arrière (positive)
#define AR_MIN 70     // La vitesse minimale des moteurs en arrière (positive)

#define CPT_A 5       // Le PIN correspondant au capteur A
#define CPT_B 2       // Le PIN correspondant au capteur B
#define CPT_C 3       // Le PIN correspondant au capteur C
#define CPT_D 6       // Le PIN correspondant au capteur D

#define SEUIL_COR 250 // Le seuil de correction de trajectoire
#define SEUIL_DET 20  // Le seuil de détection du capteur de distance

// Déclaration de la structure "Point" représentant un point en 2D :
struct Point
{
  int x;
  int y;

  int heuristique;

  // Constructeur :
  Point(int x, int y, int h) : x(x), y(y), heuristique(h)
  {
  }
};

// Déclaration des variables :
Point* points[12];     // Les points atteignables et leur coordonnées

Point* pointCourant;  // Pointeur vers le point sur lequel le robot se trouve
Point* pointDest;     // Pointeur vers le point sur lequel le robot doit aller

// Fonction "setup" exécutée au lancement :
void setup()
{
  // On initialise les moteurs :
  Motor.begin(I2C_ADDRESS);
  //Serial.begin(9600);

  // On commence à avancer :
  avance();
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  boolean cpt_A;        // Capteur côté droit
  boolean cpt_B;        // Capteur arrière droit
  boolean cpt_C;        // Capteur arrière gauche
  boolean cpt_D;        // Capteur côté gauche

  // On lit les capteurs :
  lectureCapteurs(cpt_A, cpt_B, cpt_C, cpt_D);
  
  // On corrige la trajectoire si les capteurs B ou C détectent du noir :
  if (cpt_B == HIGH && cpt_C == LOW)
    tourneDroite(SEUIL_COR);
  else if (cpt_C == HIGH && cpt_B == LOW)
    tourneGauche(SEUIL_COR);
  else if (cpt_B == HIGH && cpt_C == HIGH)
    arrete();
}

// Fonction "lectureCapteurs" permet de lire l'état des capteurs :
void lectureCapteurs(boolean& A, boolean& B, boolean& C, boolean& D)
{
  A = digitalRead(CPT_A);
  B = digitalRead(CPT_B);
  C = digitalRead(CPT_C);
  D = digitalRead(CPT_D);
}

// Fonction "avance" permet de se déplacer tout droit :
void avance()
{
  Motor.speed(MOTOR1, AV_MIN);
  Motor.speed(MOTOR2, AV_MIN); 
}

// Fonction "recule" permet de se déplacer en marche-arrière :
void recule()
{
  Motor.speed(MOTOR1, AR_MIN);
  Motor.speed(MOTOR2, AR_MIN); 
}

// Fonction "arrete" permet de s'arrêter :
void arrete()
{
  Motor.speed(MOTOR1, 0);
  Motor.speed(MOTOR2, 0); 
}

// Fonction "tourneDroite" permet de tourner d'un certain angle vers la droite :
void tourneDroite(unsigned long del)
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AV_MAX);
  Motor.speed(MOTOR2, AR_MAX);
  
  // On avance de nouveau après le délai :
  delay(del);
  avance();
}

// Fonction "tourneGauche" permet de tourner d'un certain angle vers la gauche :
void tourneGauche(unsigned long del)
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AR_MAX);
  Motor.speed(MOTOR2, AV_MAX);

  // On avance de nouveau après le délai :
  delay(del);
  avance();
}

// Fonction "preparerCarte" permet de charger la carte dans la mémoire :
void preparerCarte()
{
  points[0]  = new Point(0, 0, 0);
  points[1]  = new Point(-1, 0, 0);
  points[2]  = new Point(-2, 0, 0);
  points[3]  = new Point(-2, 1, 0);
  points[4]  = new Point(-2, 2, 0);
  points[5]  = new Point(-1, 2, 0);
  points[6]  = new Point(-1, 1, 0);
  points[7]  = new Point(1, 2, 0);
  points[8]  = new Point(1, 1, 0);
  points[9]  = new Point(2, 2, 0);
  points[10] = new Point(2, 1, 0);
  points[11] = new Point(2, 0, 0);
  points[12] = new Point(1, 0, 0);
}

// Fonction "pointSuivant" permet de se diriger vers le prochain point :
void pointSuivant(unsigned int pt)
{
}

// Fonction "comparePoints" compare deux heuristiques de points différents :
int comparePoints(const Point& pA, const Point& pB)
{
  if (pA.heuristique < pB.heuristique)
    return -1;
  else if (pA.heuristique > pB.heuristique)
    return 1;
  else
    return 0;
}

