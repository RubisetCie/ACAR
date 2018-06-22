/*
 *  Fichier     : model/Line.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant un segment
*/

// Déclaration du paquetage :
package model;

public class Segment
{
    // Constructeur :
    public Segment(final int x1, final int x2, final int y1, final int y2)
    {
        this.x1 = x1;
        this.y1 = x2;
        this.x2 = y1;
        this.y2 = y2;
    }
    
    // Déclaration des attributs :
    public final int x1;
    public final int y1;
    public final int x2;
    public final int y2;
}
