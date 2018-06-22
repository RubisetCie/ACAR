/*
 *  Fichier     : controller/Controller.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe permettant de contrôler les modules "view" et "model"
*/

// Déclaration du paquetage :
package controller;

// Inclusion des fichiers :
import model.Model;
import view.View;

public class Controller
{
    // Constructeur :
    public Controller(final Model model, final View view)
    {
        this.model = model;
        this.view = view;
    }
    
    // Déclaration des attributs :
    private final Model model;  // Référence vers le modèle
    private final View view;    // Référence vers la vue
}
