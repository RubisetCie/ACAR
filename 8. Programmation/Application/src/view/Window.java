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
import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.Label;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

public class Window extends Frame implements ActionListener
{
    // Constructeur :
    public Window()
    {
        // Appel du constructeur supérieur :
        super();
        
        // Création des boutons :
        this.start = new Button("Start");
        this.start.setBounds(740, 50, 150, 40);
        this.start.addActionListener(this);
            
        this.stop = new Button("Stop");
        this.stop.setBounds(740, 100, 150, 40);
        this.stop.addActionListener(this);
        this.stop.setEnabled(false);
        
        this.go = new Button("Go to point");
        this.go.setBounds(740, 300, 150, 40);
        this.go.setActionCommand("Go");
        this.go.addActionListener(this);
        
        this.find = new Button("Calculate path");
        this.find.setBounds(740, 350, 150, 40);
        this.find.setActionCommand("Find");
        this.find.addActionListener(this);
        
        this.tright = new Button("Turn right");
        this.tright.setBounds(740, 400, 150, 40);
        this.tright.setActionCommand("Right");
        this.tright.addActionListener(this);
        
        this.tleft = new Button("Turn left");
        this.tleft.setBounds(740, 450, 150, 40);
        this.tleft.setActionCommand("Left");
        this.tleft.addActionListener(this);
        
        this.forward = new Button("Forward");
        this.forward.setBounds(740, 500, 150, 40);
        this.forward.addActionListener(this);
            
        // Création de la zone d'input :
        this.label = new Label("Entrez une commande :");
        this.label.setBounds(740, 180, 150, 16);
        
        this.input = new TextField("goto(2)");
        this.input.setBounds(740, 200, 150, 32);
        
        Font consola;
        try
        {
            consola = Font.createFont(Font.TRUETYPE_FONT, new File("consola.ttf"));
            this.input.setFont(consola.deriveFont(20.0f));
        }
        catch (final FontFormatException | IOException e)
        {
            e.printStackTrace();
        }
        
        // Paramétrage de la fenêtre :
        this.setTitle("ACAR");
        this.setResizable(false);
        this.setSize(915, 560);
        this.setLocationRelativeTo(null);
            
        // Ajout des composants :
        this.add(this.start);
        this.add(this.stop);
        this.add(this.go);
        this.add(this.find);
        this.add(this.tright);
        this.add(this.tleft);
        this.add(this.forward);
        
        this.add(this.label);
        this.add(this.input);
    }
    
    @Override
    public void actionPerformed(ActionEvent e)
    {
        // On compare les actions demandées :
        switch (e.getActionCommand())
        {
            case "Start"    :
                start.setEnabled(false);
                stop.setEnabled(true);
                go.setEnabled(false);
                find.setEnabled(false);
                tright.setEnabled(false);
                tleft.setEnabled(false);
                forward.setEnabled(false);
                
                input.setEditable(false);
                
                break;
            case "Stop"     :
                start.setEnabled(true);
                stop.setEnabled(false);
                go.setEnabled(true);
                find.setEnabled(true);
                tright.setEnabled(true);
                tleft.setEnabled(true);
                forward.setEnabled(true);
                
                input.setEditable(true);
                
                break;
            
            case "Go"       : input.setText("goto(A)"); break;
            case "Find"     : input.setText("find(A, B)"); break;
            case "Right"    : input.setText("right()"); break;
            case "Left"     : input.setText("left()"); break;
            case "Forward"  : input.setText("forward()"); break;
        }
    }
    
    // Déclaration des attributs :
    private final Button start;     // Le bouton pour démarrer la machine
    private final Button stop;      // Le bouton pour arrêter la machine
    private final Button go;        // Le bouton pour aller vers le point
    private final Button find;      // Le bouton pour trouver un chemin du point A vers B
    private final Button tright;    // Le bouton pour ordonner un tour vers la droite
    private final Button tleft;     // Le bouton pour ordonner un tour vers la gauche
    private final Button forward;   // Le bouton pour ordonner d'aller vers l'avant
    
    private final Label label;      // Le label indiquant la zone d'ordre
    
    private final TextField input;  // La zone permettant d'entrer un ordre à la machine
}
