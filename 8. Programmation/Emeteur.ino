#include <VirtualWire.h> //On inclus la librairie VirtualWire
#include <VirtualWire_Config.h>

 const char *msg = "Bonjour"; // Tableau qui contient notre message

 int RF_TX_PIN = ...;

 void setup()
 {
   Serial.begin(9600); //Initialisation du port série 
   Serial.println("Debut communication");// Petit message pour tester l'interface

   vw_set_tx_pin(RF_TX_PIN); //Configurer La broche de transmission
   vw_setup(2000); //Initialisation de la librairie VirtualWire à 2000 bauds
 }


void loop()
{
  Serial.print("Envoie..."); //On signal le début de l'envoie
  vw_send((uint8_t *)msg, strlen(msg)); //on envoie le message
  vw_wait_tx(); // On attend la fin de l'envoie
  Serial.println("Done !"); Signal la fin de l'envoie
  delay(1000); //On attend 1s pour éviter que deux messages se superpose
  
}

