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

// Déclaration de la classe "Point" représentant un point en 2D :
class Point
{
  public :
    int x;            // Position X du point
    int y;            // Position Y du point

    Point* acces[3];

    // Constructeur :
    Point(int x, int y) : x(x), y(y)
    {
      acces[0] = NULL;
      acces[1] = NULL;
      acces[2] = NULL;
    }
};

// Déclaration des variables :
Point* points[12];    // Les points atteignables et leur coordonnées

Point* pointCourant;  // Pointeur vers le point sur lequel le robot se trouve
Point* pointDest;     // Pointeur vers le point sur lequel le robot doit aller

// Fonction "setup" exécutée au lancement :
void setup()
{
  // On initialise les moteurs :
  Motor.begin(I2C_ADDRESS);
  //Serial.begin(9600);

  // On charge la carte :
  prepareCarte();
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
  if (cpt_B == LOW && cpt_C == LOW)
    avance();
  else if (cpt_B == HIGH && cpt_C == LOW)
    tourneDroite();
  else if (cpt_C == HIGH && cpt_B == LOW)
    tourneGauche();
  else
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
void tourneDroite()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AV_MAX);
  Motor.speed(MOTOR2, AR_MAX);
}

// Fonction "tourneGauche" permet de tourner d'un certain angle vers la gauche :
void tourneGauche()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AR_MAX);
  Motor.speed(MOTOR2, AV_MAX);
}

// Fonction "preparerCarte" permet de charger la carte dans la mémoire :
void prepareCarte()
{
  // Remplit la carte des points :
  points[0]  = new Point(0, 0);
  points[1]  = new Point(-1, 0);
  points[2]  = new Point(-2, 0);
  points[3]  = new Point(-2, 1);
  points[4]  = new Point(-2, 2);
  points[5]  = new Point(-1, 2);
  points[6]  = new Point(-1, 1);
  points[7]  = new Point(1, 2);
  points[8]  = new Point(1, 1);
  points[9]  = new Point(2, 2);
  points[10] = new Point(2, 1);
  points[11] = new Point(2, 0);
  points[12] = new Point(1, 0);

  // Créé les accès entre les points :
  points[1]->acces[0] = points[2];
  points[1]->acces[1] = points[6];

  points[2]->acces[0] = points[3];
  
  points[3]->acces[0] = points[4];
  points[3]->acces[1] = points[6];

  points[4]->acces[0] = points[5];

  points[5]->acces[0] = points[6];
  points[5]->acces[1] = points[7];

  points[6]->acces[0] = points[1];
  points[6]->acces[1] = points[5];
  points[6]->acces[2] = points[8];

  points[7]->acces[0] = points[8];
  points[7]->acces[1] = points[9];

  points[8]->acces[0] = points[7];
  points[8]->acces[1] = points[10];
  points[8]->acces[2] = points[12];

  points[9]->acces[0] = points[10];

  points[10]->acces[0] = points[8];
  points[10]->acces[1] = points[11];

  points[11]->acces[0] = points[12];

  points[12]->acces[0] = points[1];
}

// Fonction "pointSuivant" permet de se diriger vers le prochain point :
void pointSuivant(const Point* pt)
{
}

// Fonction "trouverChemin" permet de trouver le chemin de "start" vers "target" :
void trouverChemin(const Point* start, const Point* target)
{
  // On déclare les variables locales :
  const Point* ferme[12];
  const Point* ouvert[12];
  const Point* chemin[12];
  
  const Point* courant;
  const Point* accessible;
  const Point* existant;

  unsigned int fermeS = 0;
  unsigned int ouvertS = 1;
  unsigned int cheminS = 0;

  register unsigned int i;
  register unsigned int j;
  
  // On ajoute le point de départ :
  ouvert[0] = start;

  // Tant que la liste des points ouvert n'est pas vide :
  while (ouvertS > 0)
  {
    // On récupère le dernier point :
    courant = ouvert[ouvertS-1];

    // On compare le point courant avec la destination :
    if (courant == target)
    {
      //node_goal.parentNode = node_current.parentNode ;
      break;
    }

    for (i = 0; i < 3; i++)
    {
      // On récupère le point accessible :
      accessible = courant->acces[i];
      
      // On cherche si l'index du point successeur est dans la liste "ouvert" :
      unsigned int indexO = -1;
      for (j = 0; j < ouvertS; j++)
      {
        if (ouvert[j] == accessible)
        {
          indexO = j;
          break;
        }
      }

      // Si le successeur est dans la liste ouverte, on laisse celui-ci :
      /*if (indexO > -1)
      {
        existant = ouvert[index];
      }*/

      // On cherche si l'index du point successeur est dans la liste "fermé" :
      unsigned int indexF = -1;
      for (j = 0; j < fermeS; j++)
      {
        if (ouvert[j] == accessible)
        {
          indexF = j;
          break;
        }
      }

      // On ajoute le point dans la liste "ouvert" :
      ouvert[ouvertS-1] = accessible;
      ouvertS++;
    }

    // On ajoute le point dans la liste "fermé" :
    ferme[fermeS-1] = courant;
    fermeS++;
  }
}

// Fonction "trouverAccessibles" permet de récupérer les points accessibles à partir d'un autre :
void trouverAccessibles(const Point** acc, const Point* pt)
{
  
}

