#include <VirtualWire.h> //On inclus la librairie VirtualWire
#include <VirtualWire.Config.h>

int RF_RX_PIN = ...;

void setup()
{
  Serial.begin(9600); //Inialisation du port série 
  Serial.println("Début communication");

  vw_set_rx_pin(RF_RX_PIN);  // Configurer la broche série
  vw_setup(2000); //Initialisation de la librairie VirtualWire à 2000 bauds
  vw_rx_start(); // Activation de la partie réception de la librairie
}

void loop()
{
  uint8_t msgRecu[VW_MAX_MESSAGE_LEN]; // Tableau qui va contenir le message recu
  uint8_t longueurMsg = VW_MAX_MESSAG_LEN; // Taille maximum de notre tableau

  if(vw_wait_rx_max(200)) //si un message est récu dans les 200ms qui viennent
     {
      if(vw_get_message(msgRecu, &longueurMsg)) // On copie le message, qur'il soit corrompu ou non
         {
          Serial.println("On recoit : ");
          for (byte i = 0; i < longueurMsg; i++) //S'il n'est pas corrompu on l'affche via Serial
            {
              Serial.println(msgRecu[i]);
              Serial.println (" ");
         }
     }
  }
  
}

