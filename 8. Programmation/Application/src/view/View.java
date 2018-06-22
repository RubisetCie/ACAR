/*
 *  Fichier     : view/View.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant l'interface applicative
*/

// Déclaration du paquetage :
package view;

import model.Graphe;

public class View
{
    // Constructeur :
    public View(final Graphe graphe)
    {
        // Création de la fenêtre :
        this.frame = new Frame(graphe);
    }
    
    // Déclaration des attributs :
    private final Frame frame;
}
