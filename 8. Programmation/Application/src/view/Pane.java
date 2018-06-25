/*
 *  Fichier     : view/Panel.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant la toile de fond de la fenêtre
*/

// Déclaration du paquetage :
package view;

// Inclusion des fichiers :
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Panel;

public class Pane extends Panel
{
    // Constructeur :
    public Pane(final Image circuit)
    {
        this.circuit = circuit;
        this.setBackground(Color.magenta);
    }
    
    // Permet de dessiner dans la fenêtre :
    /*@Override
    public void paint(final Graphics g)
    {
        // On convertis en "Graphics2D" :
        final Graphics2D g2 = (Graphics2D)g;
        
        // On paramètre le dessin de la ligne :
        g2.setStroke(new BasicStroke(10));
        g2.setColor(Color.BLACK);
        
        // On dessine le graphe :
        g2.drawImage(this.circuit, 16, 16, this);
    }*/
    
    // Déclaration des attributs :
    private final Image circuit;
}
