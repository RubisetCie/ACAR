/*
 *  Fichier     : view/View.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant l'interface applicative
*/

// Déclaration du paquetage :
package view;

import java.awt.Image;

public class View
{
    // Constructeur :
    public View(final Image circuit)
    {
        // Création de la fenêtre :
        this.frame = new Frame(circuit);
    }
    
    // Déclaration des attributs :
    private final Frame frame;
}
