/*
 *  Fichier     : model/Model.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant les données internes de l'application
*/

// Déclaration du paquetage :
package model;

import java.awt.Image;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Model
{
    // Constructeur :
    public Model()
    {   
        try
        {
            // Importation des images :
            this.circuit = ImageIO.read(new File("circuit.jpg"));
            this.voiture = ImageIO.read(new File("car.png"));
        }
        catch (final IOException e)
        {
            e.printStackTrace();
        }
    }
    
    // Permet de retourner le graphe :
    public Image getCircuit()
    {
        return this.circuit;
    }
    
    // Permet de retourner le véhicule :
    public Image getVoiture()
    {
        return this.voiture;
    }
    
    // Déclaration des attributs :
    private Image circuit;  // L'image représentant le circuit
    private Image voiture;  // L'image représentant le véhicule
}
