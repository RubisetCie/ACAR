#include <cstdlib>
#include <cstdio>
#include <cmath>

// Le point :
struct Point
{
    // Constructeur :
    Point(unsigned int n, unsigned int x, unsigned int y) : num(n), x(x), y(y)
    {
        arete[0] = NULL;
        arete[1] = NULL;
        arete[2] = NULL;
        arete[3] = NULL;
    }

    Point* arete[4];
    Point* parent;

    float heuristique;

    unsigned int num;

    unsigned int x;
    unsigned int y;

    bool verifie;
};

Point* graphe[16];
Point* chemin[16];
unsigned int cheminS;

void prepareCarte();
void prepareDonnees();
void libereCarte();

bool trouverChemin(Point* depart, Point* destination);
void construireChemin(Point* depart, Point* pt);

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
    trouverChemin(graphe[s-1], graphe[e-1]);

    // On affiche le chemin :
    printf("\nLe chemin pour y acceder est :\n");
    printf("1 - Pt %d\n", graphe[s-1]->num);

    for (register unsigned int i = 0; i < cheminS; i++)
        printf("%d - Pt %d\n", i + 2, chemin[i]->num);

    // On libère la mémoire :
    libereCarte();

    return 0;
}

void prepareCarte()
{
    // Remplit la carte des points :
    graphe[0]  = new Point(1, 0, 0);
    graphe[1]  = new Point(2, 4, 0);
    graphe[2]  = new Point(3, 0, 3);
    graphe[3]  = new Point(4, 12, 0);
    graphe[4]  = new Point(5, 4, 3);
    graphe[5]  = new Point(6, 0, 7);
    graphe[6]  = new Point(7, 15, 0);
    graphe[7]  = new Point(8, 12, 3);
    graphe[8]  = new Point(9, 15, 7);
    graphe[9]  = new Point(10, 0, 12);
    graphe[10] = new Point(11, 17, 0);
    graphe[11] = new Point(12, 17, 7);
    graphe[12] = new Point(13, 4, 12);
    graphe[13] = new Point(14, 4, 9);
    graphe[14] = new Point(15, 15, 12);
    graphe[15] = new Point(16, 15, 9);

    // Créé les accès entre les points :
    graphe[0]->arete[0] = graphe[1];
    graphe[0]->arete[1] = graphe[2];

    graphe[1]->arete[0] = graphe[0];
    graphe[1]->arete[1] = graphe[3];
    graphe[1]->arete[2] = graphe[4];

    graphe[2]->arete[0] = graphe[0];
    graphe[2]->arete[1] = graphe[4];
    graphe[2]->arete[2] = graphe[5];

    graphe[3]->arete[0] = graphe[1];
    graphe[3]->arete[1] = graphe[6];
    graphe[3]->arete[2] = graphe[7];

    graphe[4]->arete[0] = graphe[1];
    graphe[4]->arete[1] = graphe[2];
    graphe[4]->arete[2] = graphe[7];

    graphe[5]->arete[0] = graphe[2];
    graphe[5]->arete[1] = graphe[8];
    graphe[5]->arete[2] = graphe[9];

    graphe[6]->arete[0] = graphe[3];
    graphe[6]->arete[1] = graphe[8];
    graphe[6]->arete[2] = graphe[10];

    graphe[7]->arete[0] = graphe[3];
    graphe[7]->arete[1] = graphe[4];

    graphe[8]->arete[0] = graphe[5];
    graphe[8]->arete[1] = graphe[6];
    graphe[8]->arete[2] = graphe[11];
    graphe[8]->arete[3] = graphe[15];

    graphe[9]->arete[0] = graphe[5];
    graphe[9]->arete[1] = graphe[12];

    graphe[10]->arete[0] = graphe[6];
    graphe[10]->arete[1] = graphe[11];

    graphe[11]->arete[0] = graphe[8];
    graphe[11]->arete[1] = graphe[10];
    graphe[12]->arete[2] = graphe[14];

    graphe[12]->arete[0] = graphe[9];
    graphe[12]->arete[1] = graphe[13];
    graphe[12]->arete[2] = graphe[14];

    graphe[13]->arete[0] = graphe[12];
    graphe[13]->arete[1] = graphe[15];

    graphe[14]->arete[0] = graphe[11];
    graphe[14]->arete[1] = graphe[12];

    graphe[15]->arete[0] = graphe[8];
    graphe[15]->arete[1] = graphe[13];
}

void prepareDonnees()
{
    // On vide les données avant découverte de chemin :
    for (register unsigned int i = 0; i < 16; i++)
    {
        graphe[i]->parent = NULL;
        graphe[i]->heuristique = 0.0f;
        graphe[i]->verifie = false;
    }
}

void libereCarte()
{
    // Libère les graphe :
    for (register unsigned int i = 0; i < 16; i++)
        delete graphe[i];
}

bool trouverChemin(Point* depart, Point* destination)
{
    // On déclare les variables locales :
    Point* verif[16];       // graphe à vérifier

    Point* courant;         // Le point étudié en cours
    Point* accessible;      // Le point accessible par le point courant

    unsigned int taille = 1;
    unsigned int lecteur = 0;
    float distance;

    // Variables temporaires :
    bool dedans;
    unsigned int x, y;
    register unsigned int i, j;

    // On ajoute le point de départ :
    verif[0] = depart;

    // On prépare les données :
    prepareDonnees();

    // Tant que la liste des graphe ouvert n'est pas vide :
    while (taille > 0)
    {
        // On récupère le dernier point :
        courant = verif[lecteur];
        lecteur++;

        // Si le point en cours est l'arrivée, on arrête la recherche :
        if (courant == destination)
        {
            construireChemin(depart, courant);
            return true;
        }

        // On calcule l'heuristique (distance entre le point courant et la destination) :
        x = (courant->x - destination->x);
        y = (courant->y - destination->y);
        distance = sqrt(x * x + y * y);

        // On teste tout les accessibles :
        for (i = 0; i < 4; i++)
        {
            // On récupère le point accessible :
            accessible = courant->arete[i];

            if (accessible == NULL)
                break;

            // On vérifie si le point a été vérifié :
            if (accessible->verifie)
                continue;

            // On compare les distances pour éviter les détours importants :
            x = (accessible->x - destination->x);
            y = (accessible->y - destination->y);

            if (distance <= sqrt(x * x + y * y))
                continue;

            // On configure le parent pour la reconstruction du chemin :
            accessible->parent = courant;

            // On vérifie si le point ne se trouve pas déjà dans la liste "ouvert" :
            dedans = false;
            for (j = 0; j < taille; j++)
            {
                if (verif[j] == accessible)
                    dedans = true;
            }

            // On ajoute le point dans la liste "ouvert" :
            if (!dedans)
            {
                verif[taille] = accessible;
                taille++;
            }
        }

        // On marque le point comme vérifié :
        courant->verifie = true;
    }

    return false;
}

void construireChemin(Point* start, Point* pt)
{
    // Tableau temporaire stockant le chemin dans le sens inverse :
    Point* temp[16];
    cheminS = 0;

    // On construit le chemin à partir des graphe parents :
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
