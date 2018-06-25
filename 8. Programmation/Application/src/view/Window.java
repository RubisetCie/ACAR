/*
 *  Fichier     : view/Frame.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant la fenêtre principale
*/

// Déclaration du paquetage :
package view;

// Inclusion des fichiers :
import java.awt.Frame;
import java.awt.Button;
import java.awt.TextField;

public class Window extends Frame
{
    // Constructeur :
    public Window()
    {
        // Appel du constructeur supérieur :
        super();
        
        // Création des boutons :
        this.start = new Button("Start");
        this.start.setBounds(740, 50, 150, 40);
        
        this.stop = new Button("Stop");
        this.stop.setBounds(740, 100, 150, 40);
        
        // Création de la zone d'input :
        this.input = new TextField("Order", 12);
        this.input.setBounds(740, 150, 120, 32);
        this.input.setVisible(true);
        
        // Paramétrage de la fenêtre :
        this.setTitle("ACAR");
        this.setResizable(false);
        this.setSize(915, 560);
	this.setLocationRelativeTo(null);
        
        this.add(this.start);
        this.add(this.stop);
    }
    
    // Déclaration des attributs :
    private final Button start;     // Le bouton pour démarrer la machine
    private final Button stop;      // Le bouton pour arrêter la machine
    
    private final TextField input;  // La zone permettant d'entrer un ordre à la machine
}
