/*
 *  Auteur      : Matthieu Carteron
 *  Langage     : Arduino
 *  Description : Le fichier principal permettant de diriger le robot
*/

// Inclusion des librairie :
#include "Grove_I2C_Motor_Driver.h"
#include "VirtualWire.h"

// Déclaration des constantes :
#define I2C 0x0f      // L'adresse des moteurs "Grove"

#define NB_M 22       // Le nombre de mesures possibles
#define NB_T 20       // Le nombre d'octets dans la trame
#define NB_S 16       // Le nombre de sommets du graphe
#define NB_A 4        // Le nombre maximum d'arêtes par sommets

#define AV_MAX 85     // La vitesse maximale des moteurs en avant (positive)
#define AV_MIN 55     // La vitesse minimale des moteurs en avant (positive)
#define AR_MAX -85    // La vitesse maximale des moteurs en arrière (négative)
#define AR_MIN -30    // La vitesse minimale des moteurs en arrière (négative)

#define CPT_A 7       // Le PIN correspondant au capteur A (sécurité gauche)
#define CPT_B 2       // Le PIN correspondant au capteur B (correction gauche)
#define CPT_C 3       // Le PIN correspondant au capteur C (correction droit)
#define CPT_D 6       // Le PIN correspondant au capteur D (sécurité droit)
#define CPT_M A0      // Le PIN correspondant au capteur de distance
#define CPT_EMT 4     // Le PIN correspondant à l'émetteur radio

#define SIL_DRA 285   // Le seuil pour avancer après une intersection
#define SIL_DRR 95    // Le seuil pour tourner après une intersection
#define SIL_DRP 1000  // Le seuil pour pivoter après une intersection
#define SIL_DRI 1300  // Le seuil pour pivoter à l'initialisation
#define SIL_IR 500    // Le seuil d'attente après une intersection
#define SIL_SEC 100   // Le seuil de sécurité pour détecter une intersection supplémentaire
#define SIL_IL 165    // Le seuil de détection d'une intersection en L
#define SIL_IT 50     // Le seuil de détection d'une intersection en T
#define SIL_CPT 4     // Le seuil d'incrémentation du compteur
#define SIL_MES 230   // Le seuil du capteur de distance
#define SIL_TPSP 5000 // Le seuil de validation maximum de mesure
#define SIL_TPSM 100  // Le seuil de validation minimum de mesure
#define VITESSE 0.003 // La vitesse approximative du véhicule (centimètres/intérations)

// Déclaration de la classe "Trame" représentant une trame de données :
class __attribute__((packed)) Trame
{
  public :
    // Permet de chiffrer les trames :
    void encoder()
    {
      // On chiffre les données numériques selon le protocole :
      dep *= 5;    dep *= dep;  dep -= 10;
      arr *= 5;    arr *= arr;  arr -= 10;
      mes *= 5.0f; mes *= mes;  mes -= 10.0f;
    }
  
    byte n;             // Le numéro de trame
    unsigned int dep;   // Le point de départ
    unsigned int arr;   // Le point d'arrivée

    float mes;          // La mesure

    unsigned short sum; // La somme de contrôle
};

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

// Déclaration de la classe "Mesure" représentant un point en 2D :
class Mesure
{
  public :
    Point* arete[NB_A];   // Les adresses des points accessibles (arêtes du graphe)
    Point* parent;        // L'adresse du point précédent dans la construction de chemin
    
    byte depart;          // Point de départ de la mesure
    byte arrivee;         // Point d'arrivée de la mesure

    float d;              // Distance mesurée

    // Constructeur :
    Mesure(byte dep, byte arr, float mes) : depart(dep), arrivee(arr), d(mes)
    {
    }

    // Méthode "envoyer" afin d'envoyer la mesure via le protocole "ACAR_RKMR" :
    void envoyer(Trame* trame, unsigned int n)
    {
      trame->n = n;                           // On remplit le numéro de trame
      trame->dep = depart;                    // On remplit le numéro du point de départ
      trame->arr = arrivee;                   // On remplit le numéro du point d'arrivée
      trame->mes = d;                         // On remplit la mesure
      trame->sum = depart + arrivee;          // On remplit la somme de contrôle

      // On chiffre la trame :
      trame->encoder();
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
Mesure* mesures[NB_M];    // Les mesures prises successivement
Point* graphe[NB_S];      // Le graphe consitué de sommets et d'arrêtes (points)
Direction chemin[NB_S];   // Le chemin à parcourir point par point

unsigned char ptCourant;  // Point sur lequel le robot se trouve
unsigned char mCourante;  // Mesure courante effectuée
unsigned int cpL;         // Compteur pour détecter une intersection en L
unsigned int cpI;         // Compteur pour détecter une intersection non-détectée
unsigned long temps;      // Temps durant lequel la mesure est effectuée
float angleCourant;       // Angle actuel du robot

Phase phase;              // La phase actuelle du robot

/*
 *  FONCTIONS NATIVES ARDUINO
*/

// Fonction "setup" exécutée au lancement :
void setup()
{
  // On initialise les moteurs et l'émetteur :
  Serial.begin(9600);
  Motor.begin(I2C);
  
  vw_set_tx_pin(CPT_EMT);
  vw_setup(2000);

  // On charge la carte :
  prepareCarte();

  // On prépare le trajet et on règle la phase initiale :
  if (prepareTrajet())
  {
    switch (chemin[0])
    {
      case DIR_DROIT  : phase = PH_AVANT; break;
      case DIR_GAUCHE : phase = PH_PIVOT; pivotGauche(); delay(SIL_DRI); break;
      case DIR_DROITE : phase = PH_PIVOT; pivotDroite(); delay(SIL_DRI); break;
    }
  }
  else
    phase = PH_ARRET;

  // On initialise les variables globales :
  cpL = 0;
  cpI = 0;
  temps = 0;
  angleCourant = PI * 2.0f;

  mesures[0] = new Mesure(0, 1, 20.0f);
  mesures[1] = new Mesure(4, 6, 3.14f);
  mCourante = 2;

  emettre();
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  boolean cpt_A;        // Capteur côté gauche
  boolean cpt_B;        // Capteur avant gauche
  boolean cpt_C;        // Capteur avant droit
  boolean cpt_D;        // Capteur côté droit
  
  int cpt_M;            // Capteur de distance

  /*if (phase != PH_ARRET)
  {
    // On lit les capteurs :
    lectureCapteurs(cpt_A, cpt_B, cpt_C, cpt_D, cpt_M);
  
    // On effectue les mouvements en fonction de la phase du véhicule :
    switch (phase)
    {
      case PH_AVANT :
        trajectoire(cpt_B, cpt_C, cpt_D, cpt_A);  // Corrige la trajectoire
        mesurer(cpt_M);                           // Effectue les mesures
        break;
      case PH_PIVOT : pivot(cpt_B, cpt_C); break;
    }

    // On remet progressivement les compteurs à zéro :
    if (cpL > 0)
      cpL--;

    if (cpI > 0)
      cpI--;
  }*/
}

/*
 *  FONCTIONS INPUT
*/

// Fonction "lectureCapteurs" permet de lire l'état des capteurs :
inline void lectureCapteurs(boolean& A, boolean& B, boolean& C, boolean& D, int& M)
{
  // Capteurs TOR :
  A = digitalRead(CPT_A) == HIGH ? true : false;
  B = digitalRead(CPT_B) == HIGH ? true : false;
  C = digitalRead(CPT_C) == HIGH ? true : false;
  D = digitalRead(CPT_D) == HIGH ? true : false;

  // Capteur "analogique" :
  M = analogRead(CPT_M);
}

/*
 *  FONCTIONS DEPLACEMENT BASIQUE
*/

// Fonction "trajectoire" permet d'exécuter les déplacements en fonction des capteurs B et C :
inline void trajectoire(boolean R, boolean L, boolean RS, boolean LS)
{
  if (!R && !L)
    avance();
  else if (R && !L)
  {
    pivotDroite();
    
    // On détecte si le véhicule se trouve à une intersection en L :
    cpL += SIL_CPT;
    if (cpL > SIL_IL)
      pointSuivant();
  }
  else if (L && !R)
  {
    pivotGauche();

    // On détecte si le véhicule se trouve à une intersection en L :
    cpL += SIL_CPT;
    if (cpL > SIL_IL)
      pointSuivant();
  }
  else if (R && L)
  {
    // On teste une intersection :
    if (cpL < SIL_IT)
      intersection(false);
  }

  if (cpI == 0)
  {
    // On teste une intersection supplémentaire :
    if (RS || LS)
      intersection(true);
  }
  else
  {
    // On rectifie une mauvaise manoeuvre :
    if (chemin[ptCourant] == DIR_DROITE && RS)
    {
      pivotDroite();
      delay(SIL_DRP);
    }
    else if (chemin[ptCourant] == DIR_GAUCHE && LS)
    {
      pivotGauche();
      delay(SIL_DRP);
    }
  }
}

// Fonction "pivot" permet de pivoter en fonction des capteurs B et C :
void pivot(boolean R, boolean L)
{
  if (!R && !L)
  {
    // Le robot est de nouveau sur une ligne droite :
    phase = PH_AVANT;
    avance();
  }
  else
  {
    // On fait tourner le robot :
    switch (chemin[ptCourant])
    {
      case DIR_GAUCHE : tourneGauche(); break;
      case DIR_DROITE : tourneDroite(); break;
    }
  }
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
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
}

// Fonction "tourneDroite" permet de tourner vers la droite :
void tourneDroite()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AV_MAX);
  Motor.speed(MOTOR2, AR_MIN);
}

// Fonction "tourneGauche" permet de tourner vers la gauche :
void tourneGauche()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AR_MIN);
  Motor.speed(MOTOR2, AV_MAX);
}

// Fonction "pivotDroite" permet de pivoter vers la droite :
void pivotDroite()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AV_MAX);
  Motor.speed(MOTOR2, AR_MAX);
}

// Fonction "pivotGauche" permet de pivoter vers la gauche :
void pivotGauche()
{
  // On inverse les vitesses des moteurs :
  Motor.speed(MOTOR1, AR_MAX);
  Motor.speed(MOTOR2, AV_MAX);
}

/*
 *  FONCTIONS DE MESURE
*/

// Fonction "mesurer" permet de prendre les mesures grâce au capteur de distance :
void mesurer(int cpt_M)
{
  // On teste si les mesures ne sont pas pleines :
  if (mCourante >= NB_M)
    return;
      
  if (cpt_M > SIL_MES)
  {
    // On incrémente le chronomètre pour mesurer le temps :
    temps++;
  }
  else
  {
    // On teste si le temps est valide :
    if (temps < SIL_TPSM || temps > SIL_TPSP)
      return;

    // On calcule la distance :
    mesures[mCourante] = new Mesure(ptCourant, ptCourant + 1, temps * VITESSE);
    mCourante++;

    // On remet le chronomètre à zéro :
    temps = 0;
  }
}

/*
 *  FONCTIONS DE COMMUNICATION
*/

// Fonction "emettre" permet d'émettre les mesures par liaison radio :
void emettre()
{
  // On déclare les variables locales :
  Trame trame;              // Trame à transmettre (9 octets)
  
  // On envoie l'en-tête :
  vw_send((uint8_t*)"ACAR_RKMR", 10);
  vw_wait_tx();

  // On envoie successivement chacune des trames :
  for (register unsigned int i = 0; i < mCourante-1; i++)
  {
    mesures[i]->envoyer(&trame, i);

    vw_send((uint8_t*)&trame, sizeof(Trame));
    vw_wait_tx();
  }

  // On envoie le signal de fin de transmission :
  vw_send((uint8_t*)"TRA_FIN", 8);
  vw_wait_tx();
}

/*
 *  FONCTIONS PATH-FINDING
*/

// Fonction "prepareCarte" permet de charger la carte dans la mémoire :
inline void prepareCarte()
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
  // On met le point courant et la mesure courante à zéro :
  ptCourant = 0;
  mCourante = 0;
  
  // On construit le chemin (exemple) :
  chemin[0] = DIR_DROIT;
  chemin[1] = DIR_FIN;

  return true;

  // On construit le chemin :
  return trouverChemin(graphe[0], graphe[8]);
}

// Fonction "preparePoints" permet de vider les parents en vue d'une découverte de chemin :
inline void preparePoints()
{
  // On vide les données :
  for (register unsigned int i = 0; i < NB_S; i++)
  {
    graphe[i]->parent = NULL;
    graphe[i]->h = 0.0f;
    graphe[i]->v = false;
  }

  // On remet le point courant à zéro :
  ptCourant = 0;
}

// Fonction "intersection" permet de réagir en fonction de l'intersection :
void intersection(boolean sec)
{
  // On arrête le véhicule un moment :
  arrete();

  delay(SIL_IR);
    
  // On réagit en fonction de la prochaine étape du chemin :
  if (sec)
  {
    switch (chemin[ptCourant+1])
    {
      case DIR_DROIT  : phase = PH_AVANT; avance(); break;
      case DIR_GAUCHE : phase = PH_PIVOT; pivotGauche(); delay(SIL_DRP); break;
      case DIR_DROITE : phase = PH_PIVOT; pivotDroite(); delay(SIL_DRP); break;
      case DIR_FIN    : phase = PH_ARRET; emettre(); arrete(); break;
    }
  }
  else
  {
    switch (chemin[ptCourant+1])
    {
      case DIR_DROIT  : phase = PH_AVANT; avance(); delay(SIL_DRA); break;
      case DIR_GAUCHE : phase = PH_PIVOT; tourneGauche(); delay(SIL_DRR); break;
      case DIR_DROITE : phase = PH_PIVOT; tourneDroite(); delay(SIL_DRR); break;
      case DIR_FIN    : phase = PH_ARRET; emettre(); arrete(); break;
    }
  }

  // On incrémente le point courant :
  ptCourant++;

  // On initialise les compteurs :
  cpL = 0;
  cpI = SIL_SEC;
  temps = 0;
}

// Fonction "pointSuivant" permet de passer au point suivant :
void pointSuivant()
{
  // Si le circuit est terminé :
  if (chemin[ptCourant+1] == DIR_FIN)
  {
    phase = PH_ARRET;

    emettre();
    arrete();
  }

  // On incrémente le point courant :
  ptCourant++;

  // On remet les compteurs à zéro :
  cpL = 0;
  temps = 0;
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
  float delta;              // La différence d'angle
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
    angle = atan2(a->y - b->y, a->x - b->x);
    delta = angle - angleCourant;

    // On compare afin de déterminer la direction à prendre :
    if (delta > 1.0)
      chemin[i] = DIR_DROITE;
    else if (delta < -1.0)
      chemin[i] = DIR_GAUCHE;
    else
      chemin[i] = DIR_DROIT;

    // On assigne en vue des prochains calculs :
    angleCourant = angle;
    a = b;
  }

  // On insère la fin du parcours si possible :
  chemin[sommets] = DIR_FIN;
}

