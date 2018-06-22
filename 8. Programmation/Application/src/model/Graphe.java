/*
 *  Fichier     : model/Graphe.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant le graphe
*/

// Déclaration du paquetage :
package model;

// Inclusion des fichiers :
import java.awt.Graphics2D;

public class Graphe
{
    // Constructeur :
    public Graphe()
    {
        // Création du tableau de segment :
        this.segments = new Segment[1];
        
        // On construit les lignes :
        this.segments[0] = new Segment(16, 16, 64, 64);
    }
    
    // Permet de dessiner le graphe :
    public void dessiner(final Graphics2D g2)
    {
        for (Segment s : segments)
            g2.drawLine(s.x1, s.y1, s.x2, s.y2);
    }
    
    // Déclaration des attributs :
    private final Segment segments[];
}
