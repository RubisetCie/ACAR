/*
 *  Fichier     : view/View.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant l'interface applicative
*/

// Déclaration du paquetage :
package view;

import java.applet.Applet;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

public class View extends Applet implements WindowListener
{
    // Constructeur :
    public View(final Image circuit)
    {
        // Création de la fenêtre :
        this.frame = new Window();
        this.frame.addWindowListener(this);
        this.frame.add("Center", this);
        this.frame.setVisible(true);
        
        this.circuit = circuit;
    }
    
    @Override
    public void paint(final Graphics g)
    {
        // On convertis en "Graphics2D" :
        final Graphics2D g2 = (Graphics2D)g;
        
        // On paramètre le dessin de la ligne :
        g2.setStroke(new BasicStroke(10));
        g2.setColor(Color.BLACK);
        
        // On dessine le graphe :
        g2.drawImage(this.circuit, 16, 16, this);
    }
    
    @Override
    public void windowOpened(WindowEvent e)
    {
    }

    @Override
    public void windowClosing(WindowEvent e)
    {
        // Ferme l'application si on clique sur la croix en haut à droite :
        System.exit(0);
    }

    @Override
    public void windowClosed(WindowEvent e)
    {
    }

    @Override
    public void windowIconified(WindowEvent e)
    {
    }

    @Override
    public void windowDeiconified(WindowEvent e)
    {
    }

    @Override
    public void windowActivated(WindowEvent e)
    {
    }

    @Override
    public void windowDeactivated(WindowEvent e)
    {
    }
    
    // Déclaration des attributs :
    private final Window frame;     // La fenêtre principale
    private final Image circuit;    // L'image représentant le circuit
}
