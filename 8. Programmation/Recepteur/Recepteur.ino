/*
 *  Auteur      : Matthieu Carteron
 *  Langage     : Arduino
 *  Description : Le fichier permettant de recevoir le signal du robot
*/

// Inclusion des librairie :
#include <VirtualWire.h>

// Déclaration des constantes :
#define CPT_REC 4     // Le PIN correspondant au recepteur
#define NB_M 22       // Le nombre de mesures possibles

// Déclaration de la classe "Trame" représentant une trame de données :
class __attribute__((packed)) Trame
{
  public :
    // Permet de déchiffrer et vérifier une trame :
    bool decoder()
    {
      // On effectue la série d'opérations inverses :
      dep += 10;    dep = sqrt(dep); dep /= 5;
      arr += 10;    arr = sqrt(arr); arr /= 5;
      mes += 10.0f; mes = sqrt(mes); mes /= 5.0f;
    
      // On contrôle la somme :
      return sum == dep + arr;
    }

    byte n;             // Le numéro de trame
    unsigned int dep;   // Le point de départ
    unsigned int arr;   // Le point d'arrivée

    float mes;          // La mesure

    unsigned short sum; // La somme de contrôle
};

// Déclaration de l'énumération "Phase" permettant de décrire les phases de réception :
enum Phase
{
  PH_ATTENTE,
  PH_LECTURE
};

// Déclaration des variables globales :
Phase phase;            // La phase actuelle du robot

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

  // On initialise les variables globales :
  phase = PH_ATTENTE;

  // On démarre l'écoute :
  vw_rx_start();
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  Trame trame;                      // Trame transmise (9 octets)
  char buf[VW_MAX_MESSAGE_LEN];     // Tampon mémoire pour la réception
  uint8_t len = VW_MAX_MESSAGE_LEN;

  // Lorsqu'un message est reçu :
  vw_wait_rx();
  if (vw_get_message((uint8_t*)buf, &len))
  {
    Serial.println(buf);
    
    // On vérifie si la longueur du message est conséquente :
    if (len >= 8)
    {
      // On vérifie si la transmission nous appartient :
      if (strcmp(buf, "ACAR_RKMR") == 0)
        phase = PH_LECTURE;
      // On vérifie si la transmission est terminée :
      else if (strcmp(buf, "TRA_FIN") == 0)
        phase = PH_ATTENTE;
        
      // Si l'on est en lecture, on lit les trames :
      if (phase == PH_LECTURE)
      {
        // On copie le tampon vers la structure :
        memcpy(&trame, buf, len);

        if (trame.decoder())
          Serial.println(trame.mes);
        else
          Serial.println("Somme non-controlee");
      }
    }
  }
}

