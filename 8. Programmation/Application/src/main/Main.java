/*
 *  Fichier     : main/Main.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe principale
*/

// Déclaration du paquetage :
package main;

// Inclusion des fichiers :
import controller.Controller;
import model.Model;
import view.View;

public abstract class Main
{
    public static void main(String[] args)
    {
        // On instancie les modules :
        final Model model = new Model();
        final View view = new View(model.getGraphe());
        
        // On instancie le contrôleur :
        final Controller controller = new Controller(model, view);
    }
}
