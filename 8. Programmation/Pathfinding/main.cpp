#include <cstdlib>
#include <cstdio>

// Le point :
struct Point
{
    // Constructeur :
    Point(int n, int x, int y) : num(n), x(x), y(y)
    {
        acces[0] = NULL;
        acces[1] = NULL;
        acces[2] = NULL;
    }

    Point* acces[3];
    Point* parent;

    unsigned int num;

    int x;
    int y;
};

Point* points[13];
Point* chemin[13];
unsigned int cheminS;

void prepareCarte();
void preparePoints();
void libereCarte();

void trouverChemin(Point* start, Point* target);
void construireChemin(Point* start, Point* pt);

int main()
{
    unsigned int s, e;

    // On prépare la carte :
    prepareCarte();

    // On entre le point de départ et d'arrivée :
    printf("Veuillez entrer le point de depart : ");
    scanf("%u", &s);

    printf("Veuillez entrer le point d'arrivée : ");
    scanf("%u", &e);

    // On lance la recherche de chemin :
    trouverChemin(points[s], points[e]);

    // On affiche le chemin :
    printf("\nLe chemin pour y acceder est :\n");
    printf("1 - Pt %d\n", points[s]->num);

    for (register unsigned int i = 0; i < cheminS; i++)
        printf("%d - Pt %d\n", i + 2, chemin[i]->num);

    // On libère la mémoire :
    libereCarte();

    return 0;
}

void prepareCarte()
{
    // Remplit la carte des points :
    points[0]  = new Point(0, 0, 0);
    points[1]  = new Point(1, -1, 0);
    points[2]  = new Point(2, -2, 0);
    points[3]  = new Point(3, -2, 1);
    points[4]  = new Point(4, -2, 2);
    points[5]  = new Point(5, -1, 2);
    points[6]  = new Point(6, -1, 1);
    points[7]  = new Point(7, 1, 2);
    points[8]  = new Point(8, 1, 1);
    points[9]  = new Point(9, 2, 2);
    points[10] = new Point(10, 2, 1);
    points[11] = new Point(11, 2, 0);
    points[12] = new Point(12, 1, 0);

    // Créé les accès entre les points :
    points[0]->acces[0] = points[1];
    points[0]->acces[1] = points[12];

    points[1]->acces[0] = points[2];
    points[1]->acces[1] = points[6];

    points[2]->acces[0] = points[3];

    points[3]->acces[0] = points[4];
    points[3]->acces[1] = points[6];

    points[4]->acces[0] = points[5];

    points[5]->acces[0] = points[6];
    points[5]->acces[1] = points[7];

    points[6]->acces[0] = points[1];
    points[6]->acces[1] = points[5];
    points[6]->acces[2] = points[8];

    points[7]->acces[0] = points[8];
    points[7]->acces[1] = points[9];

    points[8]->acces[0] = points[7];
    points[8]->acces[1] = points[10];
    points[8]->acces[2] = points[12];

    points[9]->acces[0] = points[10];

    points[10]->acces[0] = points[8];
    points[10]->acces[1] = points[11];

    points[11]->acces[0] = points[12];

    points[12]->acces[0] = points[1];
}

void preparePoints()
{
    // On vide les parents :
    for (register unsigned int i = 0; i < 12; i++)
        points[i]->parent = NULL;
}

void libereCarte()
{
    // Libère les points :
    for (register unsigned int i = 0; i < 12; i++)
        delete points[i];
}

void trouverChemin(Point* start, Point* target)
{
    // On déclare les variables locales :
    Point* ferme[12];     // Points vérifiés
    Point* ouvert[12];    // Points à vérifier

    Point* courant;       // Le point étudié en cours
    Point* accessible;    // Le point accessible par le point courant

    unsigned int fermeS = 0;
    unsigned int ouvertS = 1;
    unsigned int lecteur = 0;

    bool dedans;

    register unsigned int i;
    register unsigned int j;

    // On ajoute le point de départ :
    ouvert[0] = start;

    // On prépare les données :
    preparePoints();

    // Tant que la liste des points ouvert n'est pas vide :
    while (ouvertS > 0)
    {
        // On récupère le dernier point :
        courant = ouvert[lecteur];
        lecteur++;

        // Si le point en cours est l'arrivée, on arrête la recherche :
        if (courant == target)
        {
            construireChemin(start, courant);
            break;
        }

        // On teste tout les accessibles :
        for (i = 0; i < 3; i++)
        {
            // On récupère le point accessible :
            accessible = courant->acces[i];

            if (accessible == NULL)
                break;

            // On vérifie si le point ne se trouve pas déjà dans la liste "fermé" :
            dedans = false;
            for (j = 0; j < fermeS; j++)
            {
                if (ferme[j] == accessible)
                    dedans = true;
            }

            if (dedans)
                continue;

            // On configure le parent pour la reconstruction du chemin :
            accessible->parent = courant;

            // On vérifie si le point ne se trouve pas déjà dans la liste "ouvert" :
            dedans = false;
            for (j = 0; j < ouvertS; j++)
            {
                if (ouvert[j] == accessible)
                    dedans = true;
            }

            // On ajoute le point dans la liste "ouvert" :
            if (!dedans)
            {
                ouvert[ouvertS] = accessible;
                ouvertS++;

                //printf("+ : %u\n", ouvertS);
            }
        }

        // On ajoute le point dans la liste "fermé" :
        ferme[fermeS] = courant;
        fermeS++;
    }
}

void construireChemin(Point* start, Point* pt)
{
    // Tableau temporaire stockant le chemin dans le sens inverse :
    Point* temp[12];
    cheminS = 0;

    // On construit le chemin à partir des points parents :
    do
    {
        temp[cheminS] = pt;

        pt = pt->parent;
        cheminS++;
    }
    while (pt != NULL && pt != start);

    // On remplit la table chemin dans le sens inverse :
    for (register unsigned int i = 0; i < cheminS; i++)
        chemin[i] = temp[cheminS - i - 1];
}
