#include <iostream>
#include <cassert>
#include "jeu.h"
#include <fstream>
#include <vector>

using namespace std;

Fantome::Fantome()
{
    posX = 0; posY = 0;
    dir = BAS;
}

int Fantome::getPosX() const
{
    return posX;
}

int Fantome::getPosY() const
{
    return posY;
}

Jeu::Jeu()
{
    terrain = NULL;
    largeur = 0; hauteur = 0;
    posPacmanX = 0; posPacmanY = 0;
    lireMeilleureScore();      // on lit la valeur de la varaible lors de la construction du jeu
    Niveau=1;                  // le jeu commence au niveau 1
    PacgommePowerUpMode=false; // pas de PacGommes PowerUps au début
    score=0;               // on initialise le score
}

Jeu::~Jeu()
{
    if (terrain!=NULL)
        delete[] terrain;
}

bool Jeu::init(int i)//à revoir
{
    int x, y;
    list<Fantome>::iterator itFantome;

    const char terrain_defaut[15][21] = {
        "####################",
        "#........##........#",
        "#.#####..##...####.#",
        "#........##........#",
        "#..................#",
        "#......#....#......#",
        "#......#...##......#",
        "#####..#....#..#####",
        "#......##...#......#",
        "#......#....#......#",
        "#..................#",
        "#..................#",
        "#.....#......#.....#",
        "#.....#......#.....#",
        "####################"
    };

    const char terrain_defaut2[15][21] = {
            "#########.##########",
            "#.......#.#........#",
            "#.#####......#####.#",
            "#......#...##......#",
            "#......#....#......#",
            "###....###..##...###",
            ".......#...##.......",
            "###...##....#....###",
            "#......##...#......#",
            "#..#...#....#...#..#",
            "#..#............#..#",
            "#..#..###..###..#..#",
            "#..####......####..#",
            "#.......#.#........#",
            "#########.##########"
        };

    largeur = 20;
    hauteur = 15;

    terrain = new Case[largeur*hauteur];

    for(y=0;y<hauteur;++y)
        for(x=0;x<largeur;++x)
            {
            if (Niveau%2==1)
                {
                if (terrain_defaut[y][x]=='#')
                    terrain[y*largeur+x] = MUR;
                else
                        terrain[y*largeur+x] = VIDE;
            }else{
                    if (terrain_defaut2[y][x]=='#')
                        terrain[y*largeur+x] = MUR;
                    else
                        terrain[y*largeur+x] = VIDE;
                }
            }

    fantomes.resize(2+Niveau); // le nombre de fantomes du niveau

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        do {
            x = rand()%largeur;
            y = rand()%hauteur;
        } while (terrain[y*largeur+x]!=VIDE);

        itFantome->posX = x;
        itFantome->posY = y;
        itFantome->dir = (Direction)(rand()%4);
    }

    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);

    const char terrain_PacGomme[15][21] = {
            "####################",
            "#p*******##*******p#",
            "#*#####.*##*.#####*#",
            "#********##********#",
            "#******************#",
            "#.....*#....#...***#",
            "###..*##...##...*###",
            "*******#....#...****",
            "###...*##...##..*###",
            "#p....*#....#...***#",
            "#******************#",
            "#*...***....***...*#",
            "#*...*#*....*#*...*#",
            "#*****#******#****p#",
            "####################"
        };

    const  char terrain_PacGomme2[15][21] = {  // le terrain pour le niveau mode(2)=0 pour pouvoir alterner entre les deux terrains selon les Niveaux
            "#########*##########",
            "#*******#*#********#",
            "#p#####..*...#####p#",
            "#*....*#.*.##******#",
            "#***..*#.*.*#**....#",
            "###*..*###**##*..###",
            "*******#.*.##*******",
            "###*.*##.***#*..*###",
            "#***.**##*.*#*..**p#",
            "#.*#..*#.*.*#*.*#*.#",
            "#*.#..****.***.*#.*#",
            "#.*#..###*.###**#*.#",
            "#p.####*****.####.*#",
            "#*******#*#********#",
            "#########*##########"
        };


    for(y=0;y<hauteur;++y)
    {
        for(x=0;x<largeur;++x)
        {
            if (Niveau%2==1)
                {
               if (terrain_PacGomme[y][x]=='*'){
                   terrain[y*largeur+x] = PACGOMME;
                   }
               else if(terrain_PacGomme[y][x]=='p'){
                    terrain[y*largeur+x] = POWERUP;
                    }
                }
            else
                {
                if (terrain_PacGomme2[y][x]=='*'){
                   terrain[y*largeur+x] = PACGOMME;
                   }
                else if(terrain_PacGomme2[y][x]=='p'){
                    terrain[y*largeur+x] = POWERUP;
                }
                }
        }
    }

    positionnerPorte(); // pour détérminer la position de la porte

    Pacgomme pg;
    pg.positionnerLesPacgommes(this);//placer les pacgommes sur le terrain

    //placer les pacmans
      pacmans.resize(i);
      list<Pacman>::iterator itPacman;

        for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
        {
            do {
                x = rand()%largeur;
                y = rand()%hauteur;
            } while (terrain[y*largeur+x]!=VIDE || terrain[y*largeur+x]==FANTOME ||(y<(hauteur-1)/2));

            itPacman->posPacmanX = x;
            itPacman->posPacmanY = y;
        }

    return true;
}

void Jeu::evolue()//à revoir
{
    int testX, testY;
    list<Fantome>::iterator itFantome;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
    {
        testX = itFantome->posX + depX[itFantome->dir];
        testY = itFantome->posY + depY[itFantome->dir];

        if (terrain[testY*largeur+testX]==VIDE || terrain[testY*largeur+testX]==PACGOMME || terrain[testY*largeur+testX]==POWERUP)
        {
            itFantome->posX = testX;
            itFantome->posY = testY;
        }
        else
            // Changement de direction
            itFantome->dir = (Direction)(rand()%4);
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

long unsigned int Jeu::getNiveau() const
{
    return Niveau;
}

void Jeu::setNiveau(int i)
{
    Niveau=i;
}
int Jeu::getScore()   //gerer de Score
{
    return score;
}

void Jeu::setScore(int i)
{
    score=i;
}

bool Jeu::getMouvementEtDirection()
{
    return controleMouvementEtDirection;
}

bool Jeu::getPacgommePowerMode()
{
    return PacgommePowerUpMode;
}

void Jeu::setPacgommePowerMode(bool b)
{
    PacgommePowerUpMode=b;
}

void Jeu::setMouvementEtDirection(bool b)
{
    controleMouvementEtDirection=b;
}

int Jeu::getMeilleureScore()
{
    return meilleursScore;
}

void Jeu::incrementScore(int i) //incrementer le SCORE
{
    score=score+i;
}

void Jeu::enregestrerMeilleureScore()                 // on enregistre la valeur pour la recuperer pour la prochaine fois quand on ouvre le jeu
{
    string filename(":/MeilleurScore/meilleurScore.txt");
    fstream file_out;

    file_out.open(filename,ios_base::out);
    if (!file_out.is_open()) {
        cout << "failed to open " << filename << '\n';
    } else {
        file_out << to_string(meilleursScore) << endl;
        cout << "Done Writing!" << endl;
    }
}

void Jeu::lireMeilleureScore()                         //lecture du meilleur score
{
    string filename(":/MeilleurScore/meilleurScore.txt");
    vector<string> lines;
    string line;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< filename << "'" << endl;
        }

    while (getline(input_file, line)){
        lines.push_back(line);
    }

    for (const auto &i : lines)
        cout << i << endl;

    input_file.close();
}

Case Jeu::getCase(int x, int y) const
{
    assert(x>=0 && x<largeur && y>=0 && y<hauteur);
    return terrain[y*largeur+x];
}

bool Jeu::posValide(int x, int y) const
{
    return (x>=0 && x<largeur && y>=0 && y<hauteur && terrain[y*largeur+x]==VIDE) || terrain[y*largeur+x]==PACGOMME || terrain[y*largeur+x]==POWERUP|| terrain[y*largeur+x]==PACMAN || terrain[y*largeur+x]==FANTOME;
}

bool Pacman::deplacePacman(Direction dir,const Jeu *j) // on passe au Jeu un parametre pour pouvoir utiliser la fonction "posValide"
{
    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};
    int testX, testY;

    testX = posPacmanX + depX[dir];
    testY = posPacmanY + depY[dir];

    if (testX==j->getNbCasesX())    // gestion des déplacements dans les frontiaires
        {
            posPacmanX = 0;
        }
    else if (testX<0)
        {
            posPacmanX = j->getNbCasesX()-1;
        }
    else if (testY==j->getNbCasesY())
        {
            posPacmanY = 0;
        }
    else if (testY<0)
        {
            posPacmanY = j->getNbCasesY()-1;
        }
    else if (j->posValide(testX, testY))
        {

            posPacmanX = testX;
            posPacmanY = testY;

            return true;
        }
    else
        {
            return false;
        }
}

void Jeu::ajout(const string &s) // passe en parametre un code string pour déterminer quel element a ajouter
{
    int x , y;
   if (s=="fantome")
   {
    Fantome fantome;
     if(fantomes.size()<20)
    {
    do {
            x = rand()%largeur;
            y = rand()%((hauteur-1)/2);  //placer les fantomes dans la moitie superieure du terrain

        } while (terrain[y*largeur+x]!=VIDE);

        fantome.posX=x;
        fantome.posY=y;
        fantomes.push_back(fantome);
    }

   }else if (s=="pacman")
   {
    Pacman pacman;
     if(pacmans.size()<2)
    {
    do {
            x = rand()%largeur;
            y = rand()%hauteur;

        } while (terrain[y*largeur+x]!=VIDE || terrain[y*largeur+x]==FANTOME ||(y<(hauteur-1)/2));

        pacman.posPacmanX=x;
        pacman.posPacmanY=y;
        pacmans.push_back(pacman);
    }
   }
}

void Jeu::suppr(const string &s)
{
    if(s=="fantome")
    {
        if (fantomes.size()!=1)
          fantomes.pop_back();

    }else if (s=="pacman")
    {
        if (pacmans.size()!=1)
          pacmans.pop_back();
    }
}

bool Jeu::collision()
{
    list<Fantome>::iterator itFantome;
    list<Pacman>::iterator itPacman;

    for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
        for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
             if (itPacman -> posPacmanX==itFantome -> posX && itPacman -> posPacmanY == itFantome ->posY)
                {             //la position de chaque fantome va etre comparée a la position de chaque pacman
                 score=0;     // s'il y a collision on perd le score collecté
                 return true; // si la position d'un fantome coincide avec celle du pacman --> collision
                }
    return false;
}

bool Jeu::mangerPacgomme ()   // methode qui gere l'interaction des pacmans avec les pacgommes
{
    list<Pacgomme>::const_iterator it;
    list<Pacman>::const_iterator itPacman;
    bool b=false;
    for(itPacman=pacmans.begin();itPacman!=pacmans.end();itPacman++)
        for(it=pacgommes.begin();it!=pacgommes.end();it++)
            {
                if((it->getPosgX()==itPacman->getPospX()) && (it->getPosgY()==itPacman->getPospY()))
                {
                    pacgommes.erase(it);
                    incrementScore(Niveau*Niveau);  //l'incrementation du score dépend du niveau, plus le niveau et grand plus l'incrementation du score est grande
                    if(score>meilleursScore)
                    {
                        meilleursScore=score;
                    }

                    if(it->getTypePg()==1){
                        PacgommePowerUpMode=true;
                        b= true;
                    }
                    break;
                }
            }

         return b;   //if (pacgommes.size()==0) // si tous les Gommes ont ete mangées on retourne 0
              //  return 0;
            //else
              //  return 1;
}

bool Jeu::ModePacgommePowerUp()   //gere la collision avec les fantomes dans le modes PoxerUp PacGomme
{
    list<Fantome>::iterator itFantome;
    list<Pacman>::iterator itPacman;
    bool b= false;
    for (itPacman=pacmans.begin(); itPacman!=pacmans.end(); itPacman++)
        for (itFantome=fantomes.begin(); itFantome!=fantomes.end(); itFantome++)
             if (itPacman -> posPacmanX==itFantome -> posX && itPacman -> posPacmanY == itFantome ->posY)
                {             //la position de chaque fantome va etre comparée a la position de chaque pacman
                    fantomes.erase(itFantome);
                    incrementScore(Niveau*500);
                    b=true;                   // si le pacman mange un fantome en retourne un true pour initialiser le timer
                    break;
                }

    return b;
}

//pour la classe pacgomme

Pacgomme::Pacgomme() // Constructeur par défaut
{
    posX = 0; posY = 0;
}


Pacgomme::Pacgomme(int x, int y) // Constructeur à 3 paramètres
{
    posX = x; posY = y;
}

int Pacgomme::getPosgX() const // Getter Accesseur de la coordonée d'un fantome en x
{
    return posX;
}

int Pacgomme::getPosgY() const // Getter Accesseur de la coordonnée d'un fantome en y
{
    return posY;
}


void Pacgomme::setposgX(int x) // Setter de la coordonnée d'un fantome en x
{
    posX=x;
}


void Pacgomme::setposgY(int y) // Setter  de la coordonnée d'un fantome en y
{
    posY=y;
}

int Pacgomme::getTypePg() const
{
    return typePacgomme;
}

void Pacgomme::setTypePg(int i)
{
    typePacgomme=i;
}

//fonction pour positionner les pacgommes
void Pacgomme::positionnerLesPacgommes( Jeu *j)
{
    Pacgomme pg;
     for(int y=0; y<j->getNbCasesY();y++)
     {
         for(int x=0; x<j->getNbCasesX();x++)
        {
            if (j->getCase(x,y)==PACGOMME)
            {
                pg.setposgX(x);
                pg.setposgY(y);
                pg.typePacgomme=0;         // pacgomme normal
                j->pacgommes.push_back(pg);
            }else if(j->getCase(x,y)==POWERUP){

                pg.setposgX(x);
                pg.setposgY(y);
                pg.typePacgomme=1;         // pacgomme Power up
                j->pacgommes.push_back(pg);
            }
        }
     }
}
//fonction pour positionner la porte de passage au niveau suivant
void Jeu::positionnerPorte()
{
    int x,y;
    do {
        x = rand()%largeur;
        y = rand()%hauteur;
    } while (terrain[y*largeur+x]!=VIDE);

    posPorteX = x,
    posPorteY = y;
}

void Jeu::positionnnerPowerUps()
{

}

int Jeu::getPorteX() const
{
    return posPorteX;
}

int Jeu::getPorteY() const
{
    return posPorteY;
}

//pour la classe pacman

Pacman::Pacman() // Constructeur par défaut
{
    posPacmanX = 0; posPacmanY = 0;
    DirecPacman=DROITE;     // on initialise la direction des pacmans a Gauche
}


Pacman::Pacman(int x, int y)
{
    posPacmanX = x; posPacmanY = y;
}

int Pacman::getPospX() const
{
    return posPacmanX;
}

int Pacman::getPospY() const
{
    return posPacmanY;
}


void Pacman::setpospX(int x)
{
    posPacmanX=x;
}


void Pacman::setpospY(int y)
{
    posPacmanY=y;
}

Direction Pacman::getDirecPacman()
{
    return DirecPacman;
}

void Pacman::setDirecPacman(Direction der)
{
    DirecPacman=der;
}
