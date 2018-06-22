/*
 *  Auteur      : Matthieu Carteron
 *  Langage     : Arduino
 *  Description : Le fichier permettant de recevoir le signal du robot
*/

// Inclusion des librairie :
#include <VirtualWire.h>

// Déclaration des constantes :
#define CPT_REC A0    // Le PIN correspondant au recepteur

// Déclaration de l'énumération "Phase" permettant de décrire la phase de réception :
enum Phase
{
  PH_ATTENTE,
  PH_LECTURE
};

// Déclaration des variables globales :
Phase phase;

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

  // On initialise la phase :
  phase = PH_ATTENTE;

  // On démarre l'écoute :
  vw_rx_start();
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  unsigned int num;
  unsigned int depart;
  unsigned int arrivee;
  unsigned int sum;
  float mesure;

  // Lorsqu'un message est reçu :
  if (vw_get_message(buf, &buflen))
  {
    if (phase == PH_ATTENTE)
    {
      // On vérifie si la transmission nous appartient :
      /*if (strcmp((char*)buf, "ACAR_RKMR") == 0)
        phase = PH_LECTURE;*/
      float* angle = (float*)buf;
      
      Serial.println(*angle);
    }
    else
    {
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
    }
  }
}
