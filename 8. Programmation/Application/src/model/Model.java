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
import javax.swing.JOptionPane;

public class Model
{
    // Constructeur :
    public Model()
    {   
        try
        {
            // Importation de l'image :
            this.circuit = ImageIO.read(new File("circuit.jpg"));
        }
        catch (final IOException e)
        {
            JOptionPane.showMessageDialog(null, e.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
    }
    
    // Permet de retourner le graphe :
    public Image getCircuit()
    {
        return this.circuit;
    }
    
    // Déclaration des attributs :
    private Image circuit;
}
