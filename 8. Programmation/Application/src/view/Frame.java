/*
 *  Fichier     : view/Frame.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant la fenêtre principale
*/

// Déclaration du paquetage :
package view;

// Inclusion des fichiers :
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Image;
import javax.swing.JButton;
import javax.swing.JFrame;

public class Frame extends JFrame
{
    // Constructeur :
    public Frame(final Image circuit)
    {
        // Appel du constructeur supérieur :
        super();
        
        // Création de la toile :
        this.image = new Panel(circuit);
        this.image.setLayout(new BorderLayout());
        
        // Création des boutons :
        this.start = new JButton("Start");
        this.stop = new JButton("Stop");
        
        // Paramétrage de la fenêtre :
        this.setTitle("ACAR");
        this.setResizable(false);
        this.setSize(960, 560);
	this.setLocationRelativeTo(null);
	this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	this.setContentPane(this.panel);
	this.setBackground(Color.WHITE);
        
        this.panel.add(this.start, BorderLayout.EAST);
        this.panel.add(this.stop, BorderLayout.EAST);
        	
	this.setVisible(true);
    }
    
    // Déclaration des attributs :
    private final Panel image;      // La toile de fond
    
    private final Toolbar toolbar;  // La barre de commande
    
    private final JButton start;    // Le bouton pour démarrer la machine
    private final JButton stop;     // Le bouton pour arrêter la machine
}
