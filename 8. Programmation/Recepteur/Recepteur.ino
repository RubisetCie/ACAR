/*
 *  Auteur      : Matthieu Carteron
 *  Langage     : Arduino
 *  Description : Le fichier permettant de recevoir le signal du robot
*/

// Inclusion des librairie :
#include <VirtualWire.h>

// Déclaration des constantes :
#define CPT_REC A0    // Le PIN correspondant au recepteur
#define NB_M 22       // Le nombre de mesures possibles

// Déclaration de la structure "Trame" représentant une trame de données :
struct __attribute__((packed)) Trame
{
  public :
    byte n;             // Le numéro de trame
    byte dep;           // Le point de départ
    byte arr;           // Le point d'arrivée

    float mes;          // La mesure

    unsigned short sum; // La somme de contrôle
};

// Déclaration de la structure "Segment" représentant le segment reçu :
struct __attribute__((packed)) Segment
{
  public :
    char debut[10];     // L'en-tête de 10 octets

    Trame trames[NB_M]; // L'ensemble des trames constituant les données
    
    char fin[8];        // Le signal de fin de 8 octets
};

/*
 *  FONCTIONS NATIVES ARDUINO
*/

// Fonction "setup" exécutée au lancement :
void setup()
{
  // On initialise le récepteur :
  Serial.begin(9600);
  
  vw_set_rx_pin(CPT_REC);
  vw_setup(2000);

  // On démarre l'écoute :
  vw_rx_start();
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  Segment segment;
  uint8_t len = VW_MAX_MESSAGE_LEN;

  byte num;
  byte depart;
  byte arrivee;
  float mesure;
  unsigned short sum;

  // Lorsqu'un message est reçu :
  if (vw_get_message((uint8_t*)&segment, &len))
  {
    Serial.println(segment.debut);
    
    // On vérifie si la transmission nous appartient :
    /*if (strcmp((char*)&buf[0], "ACAR_RKMR") == 0)
      Serial.println("Recieved");
    else
      Serial.println("Not our : ");*/
      
    /*{
      if (strcmp((char*)buf, "TRA_FIN") != 0)
      {
        // Lecture des données :
        num = (unsigned int)buf[0];
        depart = (unsigned int)buf[4];
        arrivee = (unsigned int)buf[8];
        mesure = (float)buf[12];
        sum = (unsigned int)buf[16];

        Serial.println(num);
        Serial.println(depart);
        Serial.println(arrivee);
        Serial.println(mesure);
        Serial.println(sum);
        Serial.println();
      }
      else
        phase = PH_ATTENTE;
    }*/
  }
}
