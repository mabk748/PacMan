#ifndef JEU_H
#define JEU_H

#include <list>
#include <string>
using namespace std;

typedef enum {VIDE, MUR, PACGOMME, FANTOME, PACMAN, POWERUP} Case;
typedef enum {GAUCHE, DROITE, HAUT, BAS} Direction;

class Jeu;

class Fantome
{
    friend class Jeu;

  protected:
    int posX, posY;
    Direction dir;

  public:
    Fantome();
    int getPosX() const;
    int getPosY() const;
};

class Pacgomme
{
    friend class Jeu;

  protected: // Les attributs de la classe pacgomme sont priv�s, seule la classe mere et classe fille peuvent y acc�der
    int posX, posY;
    int typePacgomme;     // 0 pour un pacgomme normale et 1 si c'est un powerup

  public:
    Pacgomme(); // Constructeur par d�faut
    Pacgomme(int, int); // Constructeur � 2 param�tres
    int getPosgX() const;// Getter Accesseur de la coordonn�e d'un pacgomme en x
    int getPosgY() const;// Getter Accesseur de la coordonn�e d'un pacgomme en y

    void setposgX(int);// Setter de la coordonn�e d'un pacgomme en x
    void setposgY(int);// Setter  de la coordonn�e d'un pacgomme en y

    int getTypePg() const;
    void setTypePg(int);

    void positionnerLesPacgommes(Jeu *j);//placer les pacgommes sur le terrain

};

class Pacman
{
    friend class Jeu;

  protected: // Les attributs de la classe pacman sont priv�s, seule la classe mere et classe fille peuvent y acc�der
    int posPacmanX, posPacmanY;
    Direction DirecPacman;

  public:
    Pacman(); // Constructeur par d�faut
    Pacman(int, int); // Constructeur � 2 param�tres
    int getPospX() const;// Getter Accesseur de la coordonn�e d'un pacman en x
    int getPospY() const;// Getter Accesseur de la coordonn�e d'un pacman en y

    void setpospX(int);// Setter de la coordonn�e d'un pacman en x
    void setpospY(int);// Setter  de la coordonn�e d'un pacman en y

    Direction getDirecPacman();
    void setDirecPacman(Direction);

    bool deplacePacman(Direction, const Jeu *j);   // D�place Pacman dans une direction (si la case � atteindre est valide)
};

class Jeu
{
  protected:
    Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    int posPorteX, posPorteY;          // la position de la porte de passage a un niveau superieur
    int posPacmanX, posPacmanY;
    int Niveau;                        // initialisation du niveau, il est incremente avec chaque level up
    int score;                         // initialisation du score du jeu
    int meilleursScore;                // variable qui contient le meilleure score;
    bool controleMouvementEtDirection; //pour le mode de controle du pacman si == true alors le pacman bouge tout seul et l'on controle seulement ses directions, si == non alors on controle le mouvement et la direction
    bool PacgommePowerUpMode;          // variable sui va g�r� l'acces au powerups des pacgommes

  public:
    list<Fantome> fantomes;
    list<Pacgomme> pacgommes;     // Cr�ation d'une liste nomm�e pacgommes contenant des elements de type Pacgomme
    list<Pacman> pacmans;         // Cr�ation d'une liste nomm�e pacman contenant des elements de type Pacman

  public:
    Jeu();
    Jeu(const Jeu &)=delete;
    ~Jeu();

    Jeu &operator=(const Jeu &)=delete;

    bool init(int);                    // l'int represente le nombre de pacmans avec lequel on va initialiser le jeu
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la position de la porte
    int getPorteX() const;
    int getPorteY() const;

    // Retourne la position du Pacman
    int getPacmanX() const;
    int getPacmanY() const;

    bool getMouvementEtDirection();
    void setMouvementEtDirection(bool);

    bool getPacgommePowerMode();
    void setPacgommePowerMode(bool);

    // Retourne le Niveau
    long unsigned int getNiveau() const;
    // imposer le Niveau
    void setNiveau(int);

    // Retourne la case � une position donn�e
    Case getCase(int, int) const;

    // Indique si la case � une position donn�e existe et est vide
    // (Pacman ou les fantomes peuvent l'occuper)
    bool posValide(int, int) const;

    // D�place Pacman dans une direction (si la case � atteindre est valide)
    bool deplacePacman(Direction);

    void ajout(const string &s);
    void suppr(const string &s);

    //Tester les collisions entre le Pacman et les Fantomes
    bool collision();
    void positionnerPorte(); // placer la porte de passage au niveau suivant.
    void positionnnerPowerUps();
    bool mangerPacgomme();
    bool ModePacgommePowerUp();

    int getScore();
    int getMeilleureScore();
    void setScore(int);
    void incrementScore(int);
    void enregestrerMeilleureScore();
    void lireMeilleureScore();
};


#endif // JEU_H
