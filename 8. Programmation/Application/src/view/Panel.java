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
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import javax.swing.JPanel;

public class Panel extends JPanel
{
    // Constructeur :
    public Panel(final Image circuit)
    {
        this.circuit = circuit;
    }
    
    // Permet de dessiner dans la fenêtre :
    @Override
    public void paintComponent(final Graphics g)
    {
        // On convertis en "Graphics2D" :
        final Graphics2D g2 = (Graphics2D)g;
        
        // On paramètre le dessin de la ligne :
        g2.setStroke(new BasicStroke(10));
        g2.setColor(Color.BLACK);
        
        // On dessine le graphe :
        g2.drawImage(this.circuit, 16, 16, this);
    }
    
    // Déclaration des attributs :
    private final Image circuit;
}
