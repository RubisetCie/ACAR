/*
 *  Auteur      : Matthieu Carteron
 *  Langage     : Arduino
 *  Description : Le fichier permettant de recevoir le signal du robot
*/

// Inclusion des librairie :
#include <VirtualWire.h>

// Déclaration des constantes :
#define CPT_REC 3     // Le PIN correspondant au recepteur

/*
 *  FONCTIONS NATIVES ARDUINO
*/

// Fonction "setup" exécutée au lancement :
void setup()
{
  // On initialise le récepteur :
  vw_set_rx_pin(CPT_REC);
  vw_setup(2000);

  // On démarre la réception :
  vw_rx_start();
}

// Fonction "loop" exécutée continuellement :
void loop()
{
  // On déclare les variables locales :
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  // Non-bloquant :
  if (vw_get_message(buf, &buflen))
  {
    // Message with a good checksum received, dump HEX
    for(register int i = 0; i < buflen; ++i)
    {    
    }
  }
}
