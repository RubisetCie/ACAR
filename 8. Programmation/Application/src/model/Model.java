/*
 *  Fichier     : model/Model.java
 *  Auteur      : Matthieu Carteron
 *  Langage     : Java
 *  Description : La classe constituant les données internes de l'application
*/

// Déclaration du paquetage :
package model;

public class Model
{
    // Constructeur :
    public Model()
    {
        // Création du graphe :
        this.graphe = new Graphe();
    }
    
    // Permet de retourner le graphe :
    public final Graphe getGraphe()
    {
        return this.graphe;
    }
    
    // Déclaration des attributs :
    private final Graphe graphe;
}
