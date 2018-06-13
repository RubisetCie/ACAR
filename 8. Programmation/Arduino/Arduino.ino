/*  Auteur      : Matthieu Carteron
    Langage     : Arduino
    Description : Le fichier principal permettant de contrôler le robot
*/

// Inclusion de la librairie :
#include "Grove_I2C_Motor_Driver.h"

// Déclaration des constantes :
#define I2C_ADDRESS 0x0f

#define AV_MAX 100    // La vitesse maximale des moteurs en avant
#define AV_MIN 50     // La vitesse minimale des moteurs en avant
#define AR_MAX -100   // La vitesse maximale des moteurs en arrière
#define AR_MIN -50    // La vitesse minimale des moteurs en arrière

#define CPT_A 3       // Le PIN correspondant au capteur A
#define CPT_B 4       // Le PIN correspondant au capteur B
#define CPT_C 5       // Le PIN correspondant au capteur C
#define CPT_D 6       // Le PIN correspondant au capteur D

#define SEUIL_COR 150 // Le seuil de correction de trajectoire
#define SEUIL_DET 20  // Le seuil de détection du capteur de distance

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
  // On initialise les moteurs et le moniteur série :
  Motor.begin(I2C_ADDRESS);
  Serial.begin(9600);

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
  if (cpt_B == HIGH)
    tourneDroite(SEUIL_COR);
  else if (cpt_C == HIGH)
    tourneGauche(SEUIL_COR);
}

// Fonction "lectureCapteurs" permet de lire l'état des capteurs :
void lectureCapteurs(const boolean& A, const boolean& B, const boolean& C, const boolean& D)
{
  A = digitalRead(CPT_A);
  B = digitalRead(CPT_B);
  C = digitalRead(CPT_C);
  D = digitalRead(CPT_D);
}

// Fonction "avance" permet de se déplacer tout droit :
void avance()
{
  Motor.speed(MOTOR1, AV_MAX);
  Motor.speed(MOTOR2, AV_MAX); 
}

// Fonction "recule" permet de se déplacer en marche-arrière :
void recule()
{
  Motor.speed(MOTOR1, AR_MAX);
  Motor.speed(MOTOR2, AR_MAX); 
}

// Fonction "tourneDroite" permet de tourner d'un certain angle vers la droite :
void tourneDroite(unsigned int del)
{
  // On règle les vitesses des moteurs :
  Motor.speed(MOTOR1, 0);
  Motor.speed(MOTOR2, AV_MAX);
  
  // On avance de nouveau après le délai :
  delay(del);
  avance();
}

// Fonction "tourneGauche" permet de tourner d'un certain angle vers la gauche :
void tourneGauche(unsigned int del)
{
  // On règle les vitesses des moteurs :
  Motor.speed(MOTOR1, AV_MAX);
  Motor.speed(MOTOR2, 0);

  // On avance de nouveau après le délai :
  delay(del);
  avance();
}

// Fonction "pointSuivant" permet de se diriger vers le prochain point :
void pointSuivant(unsigned int pt)
{
}

