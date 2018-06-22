/*
 *  Fichier     : view/Frame.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant la fenêtre principale
*/

// Déclaration du paquetage :
package view;

// Inclusion des fichiers :
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
        this.panel = new Panel(circuit);
        
        // Création des boutons :
        this.go = new JButton();
        this.go.setText("Yolo");
        //this.go.setVisible(true);
        
        // Paramétrage de la fenêtre :
        this.setTitle("ACAR");
        this.setResizable(false);
        this.setSize(960, 560);
	this.setLocationRelativeTo(null);
	this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	this.setContentPane(this.panel);
	this.setBackground(Color.WHITE);
        
        this.add(this.go, BorderLayout.WEST);
	
	this.setVisible(true);
    }
    
    // Déclaration des attributs :
    private final Panel panel;      // La toile de fond
    
    private final JButton go;       // Le bouton pour aller à un point
}
