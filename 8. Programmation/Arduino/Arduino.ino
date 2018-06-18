/*
 *  Auteur      : Matthieu Carteron
 *  Langage     : Arduino
 *  Description : Le fichier principal permettant de diriger le robot
*/

// Inclusion de la librairie :
#include "Grove_I2C_Motor_Driver.h"

// Déclaration des constantes :
#define I2C 0x0f      // L'adresse des moteurs "Grove"

#define NB_S 16       // Le nombre de sommets du graphe
#define NB_A 4        // Le nombre maximum d'arêtes par sommets

#define AV_MAX 45     // La vitesse maximale des moteurs en avant (positive)
#define AV_MIN 30     // La vitesse minimale des moteurs en avant (positive)
#define AR_MAX -45    // La vitesse maximale des moteurs en arrière (négative)
#define AR_MIN -30    // La vitesse minimale des moteurs en arrière (négative)

#define CPT_A 7       // Le PIN correspondant au capteur A
#define CPT_B 2       // Le PIN correspondant au capteur B
#define CPT_C 3       // Le PIN correspondant au capteur C
#define CPT_D 6       // Le PIN correspondant au capteur D

#define SIL_DR 100    // Le seuil pour avancer droit après une intersection

// Déclaration de la classe "Point" représentant un point en 2D :
class Point
{
  public :
    Point* arete[NB_A]; // Les adresses des points accessibles (arêtes du graphe)
    Point* parent;      // L'adresse du point précédent dans la construction de chemin
    
    int x;              // Position X du point
    int y;              // Position Y du point

    float h;            // Heuristique du point par rapport au trajet à effectuer
    bool v;             // Si le point a déjà été vérifié

    // Constructeur :
    Point(int x, int y) : x(x), y(y)
    {
      for (register unsigned int i = 0; i < NB_A; i++)
        arete[i] = NULL;
    }
};

// Déclaration de l'énumération "Phase" permettant de décrire les comportements du robot :
enum Phase
{
  PH_ARRET,
  PH_AVANT,
  PH_PIVOT
};

// Déclaration de l'énumération "Direction" permettant de décrire le chemin :
enum Direction
{
  DIR_DROIT,
  DIR_GAUCHE,
  DIR_DROITE,
  DIR_FIN
};

// Déclaration des variables globales :
Point* graphe[NB_S];    // Le graphe consitué de sommets et d'arrêtes (points)
Direction chemin[NB_S]; // Le chemin à parcourir point par point

unsigned int ptCourant; // Point sur lequel le robot se trouve
float angleCourant;     // Angle actuel du robot

Phase phase;            // La phase actuelle du robot

/*
 *  FONCTIONS NATIVES ARDUINO
*/

// Fonction "setup" exécutée au lancement :
void setup()
{
  // On initialise les moteurs :
  Motor.begin(I2C);
  Serial.begin(9600);

  // On charge la carte :
  prepareCarte();

  // On prépare le trajet et on règle la phase initiale :
  if (prepareTrajet())
    phase = PH_AVANT;
  else
    phase = PH_ARRET;

  ptCourant = 0;
  angleCourant = 0.0f;
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  boolean cpt_A;        // Capteur côté droit
  boolean cpt_B;        // Capteur arrière droit
  boolean cpt_C;        // Capteur arrière gauche
  boolean cpt_D;        // Capteur côté gauche

  if (phase != PH_ARRET)
  {
    // On lit les capteurs :
    lectureCapteurs(cpt_A, cpt_B, cpt_C, cpt_D);
  
    // On vérifie les intersections :
    /*if (phase == PH_AVANT)
      intersection(cpt_A, cpt_D);*/

    // On suit la trajectoire :
    trajectoire(cpt_B, cpt_C);
  }
}

/*
 *  FONCTIONS INPUT
*/

// Fonction "lectureCapteurs" permet de lire l'état des capteurs :
void lectureCapteurs(boolean& A, boolean& B, boolean& C, boolean& D)
{
  A = digitalRead(CPT_A) == HIGH ? true : false;
  B = digitalRead(CPT_B) == HIGH ? true : false;
  C = digitalRead(CPT_C) == HIGH ? true : false;
  D = digitalRead(CPT_D) == HIGH ? true : false;
}

/*
 *  FONCTIONS DEPLACEMENT BASIQUE
*/

// Fonction "trajectoire" permet d'exécuter les déplacements en fonction des capteurs B et C :
void trajectoire(boolean R, boolean L)
{
  if (!R && !L)
    avance();
  else if (R && !L)
    tourneDroite();
  else if (L && !R)
    tourneGauche();
  else if (R && L)
    arrete();
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

// Fonction "arrete" permet de s'arrêter :
void arrete()
{
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
}

// Fonction "tourneDroite" permet de tourner vers la droite :
void tourneDroite()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AV_MIN);
  Motor.speed(MOTOR2, AR_MIN);
}

// Fonction "tourneGauche" permet de tourner vers la gauche :
void tourneGauche()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AR_MIN);
  Motor.speed(MOTOR2, AV_MIN);
}

/*
 *  FONCTIONS PATH-FINDING
*/

// Fonction "prepareCarte" permet de charger la carte dans la mémoire :
void prepareCarte()
{
  // Remplit la carte des points (sommets du graphe) :
  graphe[0]  = new Point(0, 0);
  graphe[1]  = new Point(4, 0);
  graphe[2]  = new Point(0, 3);
  graphe[3]  = new Point(12, 0);
  graphe[4]  = new Point(4, 3);
  graphe[5]  = new Point(0, 7);
  graphe[6]  = new Point(15, 0);
  graphe[7]  = new Point(12, 3);
  graphe[8]  = new Point(15, 7);
  graphe[9]  = new Point(0, 12);
  graphe[10] = new Point(17, 0);
  graphe[11] = new Point(17, 7);
  graphe[12] = new Point(4, 12);
  graphe[13] = new Point(4, 9);
  graphe[14] = new Point(15, 12);
  graphe[15] = new Point(15, 9);

  // Créé les accès entre les points (arêtes du graphe) :
  graphe[0]->arete[0] = graphe[1];
  graphe[0]->arete[1] = graphe[2];

  graphe[1]->arete[0] = graphe[0];
  graphe[1]->arete[1] = graphe[3];
  graphe[1]->arete[2] = graphe[4];

  graphe[2]->arete[0] = graphe[0];
  graphe[2]->arete[1] = graphe[4];
  graphe[2]->arete[2] = graphe[5];

  graphe[3]->arete[0] = graphe[1];
  graphe[3]->arete[1] = graphe[6];
  graphe[3]->arete[2] = graphe[7];

  graphe[4]->arete[0] = graphe[1];
  graphe[4]->arete[1] = graphe[2];
  graphe[4]->arete[2] = graphe[7];

  graphe[5]->arete[0] = graphe[2];
  graphe[5]->arete[1] = graphe[8];
  graphe[5]->arete[2] = graphe[9];
    
  graphe[6]->arete[0] = graphe[3];
  graphe[6]->arete[1] = graphe[8];
  graphe[6]->arete[2] = graphe[10];

  graphe[7]->arete[0] = graphe[3];
  graphe[7]->arete[1] = graphe[4];

  graphe[8]->arete[0] = graphe[5];
  graphe[8]->arete[1] = graphe[6];
  graphe[8]->arete[2] = graphe[11];
  graphe[8]->arete[3] = graphe[15];

  graphe[9]->arete[0] = graphe[5];
  graphe[9]->arete[1] = graphe[12];

  graphe[10]->arete[0] = graphe[6];
  graphe[10]->arete[1] = graphe[11];

  graphe[11]->arete[0] = graphe[8];
  graphe[11]->arete[1] = graphe[10];
  graphe[12]->arete[2] = graphe[14];

  graphe[12]->arete[0] = graphe[9];
  graphe[12]->arete[1] = graphe[13];
  graphe[12]->arete[2] = graphe[14];

  graphe[13]->arete[0] = graphe[12];
  graphe[13]->arete[1] = graphe[15];

  graphe[14]->arete[0] = graphe[11];
  graphe[14]->arete[1] = graphe[12];

  graphe[15]->arete[0] = graphe[8];
  graphe[15]->arete[1] = graphe[13];
}

// Fonction "prepareTrajet" permet de construire un chemin par défaut pour la découverte :
bool prepareTrajet()
{
  // On construit le chemin (exemple) :
  chemin[0] = DIR_DROIT;
  chemin[1] = DIR_GAUCHE;
  chemin[2] = DIR_FIN;
  
  return true;
}

// Fonction "preparePoints" permet de vider les parents en vue d'une découverte de chemin :
void preparePoints()
{
  // On vide les données :
  for (register unsigned int i = 0; i < NB_S; i++)
  {
    graphe[i]->parent = NULL;
    graphe[i]->h = 0.0f;
    graphe[i]->v = false;
  }
}

// Fonction "intersection" permet de réagir en fonction de l'intersection :
void intersection()
{
  // On réagit en fonction de la prochaine étape du chemin :
  switch (chemin[ptCourant+1])
  {
    case DIR_DROIT  : phase = PH_AVANT; avance(); delay(SIL_DR); break;
    case DIR_GAUCHE : phase = PH_PIVOT; break;
    case DIR_DROITE : phase = PH_PIVOT; break;
    case DIR_FIN    : phase = PH_ARRET; arrete(); break;
  }

  // On incrémente le point courant :
  ptCourant++;
}

// Fonction "trouverChemin" permet de découvrir le chemin de "dep" vers "dest" :
bool trouverChemin(Point* dep, Point* dest)
{
  // On déclare les variables locales :
  Point* ouvert[NB_S];      // Les points à vérifier dans la recherche (se remplissant peu à peu)

  Point* courant;           // Le point étudié en cours
  Point* accessible;        // Le point accessible par le point courant étudié en cours

  unsigned int ouverts = 1; // La taille de la liste "ouvert"
  unsigned int lecteur = 0; // Le curseur de lecture de la liste "ouvert"
  float distance;           // La distance entre le point en cours et la destination

  // Variables temporaires :
  bool dedans;
  unsigned int x, y;
  register unsigned int i, j;

  // On ajoute le point de départ :
  ouvert[0] = dep;

  // On prépare les données :
  preparePoints();

  // Tant que la liste des graphe ouvert n'est pas vide :
  while (ouverts > 0)
  {
    // On récupère le dernier point :
    courant = ouvert[lecteur];
    lecteur++;

    // Si le point en cours est l'arrivée, on arrête la recherche :
    if (courant == dest)
    {
      construireChemin(dep, courant);
      return true;
    }

    // On calcule l'heuristique (distance entre le point courant et la destination) :
    x = (courant->x - dest->x);
    y = (courant->y - dest->y);
    distance = sqrt(x * x + y * y);

    // On teste tous les points accessibles :
    for (i = 0; i < NB_A; i++)
    {
      // On récupère le point accessible :
      accessible = courant->arete[i];

      // Si le point n'a plus d'autres accessibles :
      if (accessible == NULL)
        break;

      // On vérifie si le point a été vérifié :
      if (accessible->v)
        continue;

      // On compare les heuristiques (distances) pour éviter les détours importants :
      x = (accessible->x - dest->x);
      y = (accessible->y - dest->y);
      if (distance <= sqrt(x * x + y * y))
        continue;

      // On configure le parent pour la reconstruction du chemin :
      accessible->parent = courant;

      // On vérifie si le point ne se trouve pas déjà dans la liste des chemins ouverts :
      dedans = false;
      for (j = 0; j < ouverts; j++)
      {
        if (ouvert[j] == accessible)
          dedans = true;
      }

      // Sinon, on l'ajoute :
      if (!dedans)
      {
        ouvert[ouverts] = accessible;
        ouverts++;
      }
    }

    // On marque le point comme vérifié :
    courant->v = true;
  }

  // La découverte a échouée, point inaccessible :
  return false;
}

// Fonction "construireChemin" permet de reconstruire un chemin à partir des parents :
void construireChemin(Point* dep, Point* pt)
{
  // On déclare les variables locales :
  Point* sommet[NB_S];      // Les points successifs du chemin inverse
  Point* a = dep;           // Le point actuel
  Point* b;                 // Le point suivant
  float angle;              // L'angle calculé
  unsigned int sommets = 0; // Le nombre de sommets dans le chemin

  // On construit le chemin en remontant les parents :
  do
  {
    sommet[sommets] = pt;

    pt = pt->parent;
    sommets++;
  }
  while (pt != NULL && pt != dep);

  // On détermines les mouvements simplifiés du robot :
  for (register unsigned int i = 0; i < sommets; i++)
  {
    // Détermination de l'angle à prendre :
    b = sommet[sommets - i - 1];
    angle = angleCourant - atan2(a->y - b->y, a->x - b->x);

    // On compare afin de déterminer la direction à prendre :
    if (angle > 45.0)
      chemin[i] = DIR_DROITE;
    else if (angle < -45.0)
      chemin[i] = DIR_GAUCHE;
    else
      chemin[i] = DIR_DROIT;

    // On assigne en vue des prochains calculs :
    angleCourant = angle;
    a = b;
  }

  // On insère un NULL à la fin si possible :
  if (sommets < NB_S)
    chemin[sommets] = NULL;
}

