#include <iostream>
#include "mainwindow.h"

using namespace std;

PacmanWindow::PacmanWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;


    if (pixmapPacman1.load(":/icons/data/pacmandroit.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman.bmp"<<endl;
        exit(-1);
    }
    if (pixmapPacman2.load(":/icons/data/pacman2.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacman2.bmp"<<endl;
        exit(-1);
    }

    if (pixmapFantome.load(":/icons/data/fantome.bmp")==false)
    {
        cout<<"Impossible d'ouvrir fantome.bmp"<<endl;
        exit(-1);
    }

    if (pixmapMur.load(":/icons/data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    if (pixmapPacgomme.load(":/icons/data/pacgomme.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacgomme.bmp"<<endl;
        exit(-1);
    }

    if (pixmapPacgommePowerUp.load(":/icons/data/pacgommepowerup.bmp")==false)
    {
        cout<<"Impossible d'ouvrir pacgommepowerup.bmp"<<endl;
        exit(-1);
    }

    if (pixmapIntro.load(":/icons/data/intro.bmp")==false)
    {
        cout<<"Impossible d'ouvrir intro.bmp"<<endl;
        exit(-1);
    }

    setStyleSheet("color: white; background-color: black");   //changer le style de la fenetre

    PacmanButton *btn_ajout_fantome = new PacmanButton(this);  //bouton pour ajouter des fantomes

    connect(btn_ajout_fantome, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton_ajoutfantome);
    btn_ajout_fantome->setText("Ajouter fantome");
    btn_ajout_fantome-> move(10,10);
    btn_ajout_fantome->setStyleSheet("color: white; background-color: blue");   //changer le style de la fenetre

    PacmanButton *btn_suppr_fantome = new PacmanButton(this);//bouton pour supprimer un fantome
    connect(btn_suppr_fantome, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton_supprfantome);
    btn_suppr_fantome->setText("supprimer fantome");
    btn_suppr_fantome-> move(10,45);
    btn_suppr_fantome->setStyleSheet("color: white; background-color: blue");

    PacmanButton *btn_ajout_pacman = new PacmanButton(this);//bouton pour ajouter un autre joueur
    connect(btn_ajout_pacman, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton_ajoutpacman);
    btn_ajout_pacman->setText("Ajouter Pacman");
    btn_ajout_pacman-> move(260,10);
    btn_ajout_pacman->setStyleSheet("color: white; background-color: blue");

    PacmanButton *btn_suppr_pacman = new PacmanButton(this);//bouton pour supprimer l'autre joueur
    connect(btn_suppr_pacman, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton_supprpacman);
    btn_suppr_pacman->setText("Supprimer pacman");
    btn_suppr_pacman-> move(260,45);
    btn_suppr_pacman->setStyleSheet("color: white; background-color: blue");

    PacmanButton *btn_pause = new PacmanButton(this);//bouton pour pauser la partie
    connect(btn_pause, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton_pause);
    btn_pause->setText("Pause");
    btn_pause-> move(550,45);
    btn_pause->setStyleSheet("color: white; background-color: blue");

    PacmanButton *btn_parametr = new PacmanButton(this);//bouton pour aller aux paramètres
    connect(btn_parametr, &QPushButton::clicked, this, &PacmanWindow::clickMonBouton_parametre);
    btn_parametr->setText("Parametres");
    btn_parametr-> move(550,10);
    btn_parametr->setStyleSheet("color: white; background-color: blue");

    labelInfo = new QLabel(this);
    labelInfo->setGeometry(QRect(QPoint(10, 70), QSize(450, 30)));
    labelInfo->setFont(QFont("Calibri", 15));// Police et taille
    labelInfo->setStyleSheet("QLabel { color : blue; }");//couleur

    jeu.init(1);            // on commence le Jeu avec un seul PACMAN

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PacmanWindow::handleTimer);
    timer->start(120-10*jeu.getNiveau());              //la vitesse des fantomes change avec les niveaux

    QTimer *timerPacman = new QTimer(this);            //le timer pour le mouvement du pacman
    connect(timerPacman, &QTimer::timeout, this, &PacmanWindow::handleTimerPacman);
    timerPacman->start(100);                           //la vitesse des Pacmans

    connect(timerPacgommPowerUp, &QTimer::timeout, this, &PacmanWindow::handleTimerPacgommePU); //connexion de timesPowerUP et de la fonction handlerTimrePacgommePU

    QTimer *timerRegeneration = new QTimer(this);      //le timer pour le mouvement du pacman
    connect(timerRegeneration, &QTimer::timeout, this, &PacmanWindow::handleTimerRegeneration);
    timerRegeneration->start(3000);                    //la durée du mode PacGomme PowerUp

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase+100);
}

PacmanButton::PacmanButton(QWidget *pParent):QPushButton(pParent)
{
}

void PacmanButton::keyPressEvent(QKeyEvent *e)
{
    if (parent()!=NULL)
        QCoreApplication::sendEvent(parent(), e);
}

void PacmanWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    list<Fantome>::const_iterator itFantome;
    list<Pacgomme>::const_iterator itPacgomme;
    list<Pacman>::const_iterator itPacman;
    int x, y;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    int decalageY = 100;
    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    if (intro==false){
            // Dessine les cases
            for (y=0;y<jeu.getNbCasesY();y++)
                for (x=0;x<jeu.getNbCasesX();x++)
                    if (jeu.getCase(x,y)==MUR)
                        painter.drawPixmap(x*largeurCase, y*hauteurCase+decalageY, pixmapMur);

            // Dessine les pacgommes
            for (itPacgomme=jeu.pacgommes.begin(); itPacgomme!=jeu.pacgommes.end(); itPacgomme++)
                if(itPacgomme->getTypePg()==0){
                    painter.drawPixmap(itPacgomme->getPosgX()*largeurCase, itPacgomme->getPosgY()*hauteurCase+decalageY, pixmapPacgomme);
                }else if(itPacgomme->getTypePg()==1){
                    painter.drawPixmap(itPacgomme->getPosgX()*largeurCase, itPacgomme->getPosgY()*hauteurCase+decalageY, pixmapPacgommePowerUp);
                }

            // Dessine les fantomes
            if(jeu.getPacgommePowerMode()==false){
                    pixmapFantome.load(":/icons/data/fantome.bmp");
                for (itFantome=jeu.fantomes.begin(); itFantome!=jeu.fantomes.end(); itFantome++)
                    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase+decalageY, pixmapFantome);
            }else if(jeu.getPacgommePowerMode()==true){
                    pixmapFantome.load(":/icons/data/fantomemort.bmp");
                for (itFantome=jeu.fantomes.begin(); itFantome!=jeu.fantomes.end(); itFantome++)
                    painter.drawPixmap(itFantome->getPosX()*largeurCase, itFantome->getPosY()*hauteurCase+decalageY, pixmapFantome);
            }

            // Dessine Pacman
            for (itPacman=jeu.pacmans.begin(); itPacman!=jeu.pacmans.end(); itPacman++)  // affichage des pacmans
            {
                if (itPacman==jeu.pacmans.begin())
                {
                 painter.drawPixmap(itPacman->getPospX()*largeurCase, itPacman->getPospY()*hauteurCase+decalageY, pixmapPacman1);
                }else{
                 painter.drawPixmap(itPacman->getPospX()*largeurCase, itPacman->getPospY()*hauteurCase+decalageY, pixmapPacman2);
                }
            }

            // Dessine Porte

            if (jeu.mangerPacgomme()==true)
                timerPacgommPowerUp->start(5000);         //si le pacman mange un pacgommePower up on initialise le timerPacgommPowerUp


            if ( jeu.pacgommes.size()==0  )                // si tous les pacgommes ont été mangés on affiche la porte de passage au niveau suivant
            {
                pixmapPorte.load(":/icons/data/porte.bmp");
                painter.drawPixmap(jeu.getPorteX()*largeurCase, jeu.getPorteY()*hauteurCase+decalageY, pixmapPorte);
                Portepositionne=true;
            }

           if(jeu.getPacgommePowerMode()==true)
                {
                   if(jeu.ModePacgommePowerUp()==true)     //gestione du mode de PacgommePower up
                      timerPacgommPowerUp->start(8000);     //si le pacman mange un fantom mort on reinitialise laa conpeire
                }
           else if(jeu.collision()==true)                  //gestione des collisione a chaque instante qu'on rafrichier l'affichage
                {
                    Collision();
                }

            //Affichare du Meilleure Score
            labelInfo->setText(QString(("Meilleure Score : " + to_string(jeu.getMeilleureScore())+" | Score : "+to_string(jeu.getScore())+" | Niveau : "+to_string(jeu.getNiveau())).c_str()));
            labelInfo->show();

    }else{
            painter.drawPixmap(0, 100, pixmapIntro);  // affichage de l'intro

            labelInfo->setText(QString(("Cliquer sur le clavier pour Commencer ")));
            labelInfo->show();
    }
}

void PacmanWindow::keyPressEvent(QKeyEvent *event)
{
    if(PAUSE==false)
        {
            list<Pacman>::iterator itPacman;

       for(itPacman=jeu.pacmans.begin();itPacman!=jeu.pacmans.end();itPacman++)
       {
        if (itPacman==jeu.pacmans.begin() )
        {
            if (event->key()==Qt::Key_Left  )
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(GAUCHE,&jeu);

                itPacman->setDirecPacman(GAUCHE);
                pixmapPacman1.load(":/icons/data/pacmangauche.bmp");

            }
            else if (event->key()==Qt::Key_Right)
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(DROITE,&jeu);

                itPacman->setDirecPacman(DROITE);
                pixmapPacman1.load(":/icons/data/pacmandroit.bmp");

            }
            else if (event->key()==Qt::Key_Up)
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(HAUT,&jeu);

                itPacman->setDirecPacman(HAUT);
                pixmapPacman1.load(":/icons/data/pacmanhaut.bmp");

            }
            else if (event->key()==Qt::Key_Down )
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(BAS,&jeu);

                itPacman->setDirecPacman(BAS);
                pixmapPacman1.load(":/icons/data/pacmanbas.bmp");

            }
        }else{
            if (event->key()==Qt::Key_Q )
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(GAUCHE,&jeu);

                itPacman->setDirecPacman(GAUCHE);
                pixmapPacman2.load(":/icons/data/pacmangauche2.bmp");
            }
            else if (event->key()==Qt::Key_D )
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(DROITE,&jeu);

                itPacman->setDirecPacman(DROITE);
                pixmapPacman2.load(":/icons/data/pacmandroit2.bmp");

            }
            else if (event->key()==Qt::Key_Z )
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(HAUT,&jeu);

                itPacman->setDirecPacman(HAUT);
                pixmapPacman2.load(":/icons/data/pacmanhaut2.bmp");
            }
            else if (event->key()==Qt::Key_S )
            {
                if(jeu.getMouvementEtDirection()==false)
                    itPacman->deplacePacman(BAS,&jeu);

                itPacman->setDirecPacman(BAS);
                pixmapPacman2.load(":/icons/data/pacmanbas2.bmp");
            }
        }

         update();
         if ( (jeu.getPorteX()==itPacman->getPospX()) && (jeu.getPorteY()==itPacman->getPospY()) && (findeniveauAffiche==false) && (Portepositionne==true))
            FinDuNiveau();

        }
    }
    if(intro ==true){  //pour régler la parametre de mode de controle
    QMessageBox msgBox;
    msgBox.setText(tr("Mode de controle des Pacmans"));
    msgBox.setInformativeText("Vous voulez controler : ");

    QAbstractButton* pButtonDirecMove = msgBox.addButton(tr("direction et mouvement"), QMessageBox::YesRole);
    QAbstractButton* pButtonDire = msgBox.addButton(tr("direction"), QMessageBox::YesRole);
    msgBox.exec();

    if (msgBox.clickedButton()==pButtonDirecMove) {
        jeu.setMouvementEtDirection(false);
    }else if (msgBox.clickedButton()==pButtonDire){
        jeu.setMouvementEtDirection(true);
    }
    }

        intro=false;
}


void PacmanWindow::handleTimer()
{
    jeu.evolue();
    update();
}

void PacmanWindow::handleTimerPacman()
{
    if (PAUSE==false&&jeu.getMouvementEtDirection()==true) // si tous les PACGOMMEs ont ete mangés on arrete le mouvement des FANTOMES
    {
        list<Pacman>::iterator itPacman;
        for(itPacman=jeu.pacmans.begin();itPacman!=jeu.pacmans.end();itPacman++)
        {
            itPacman->deplacePacman(itPacman->getDirecPacman(),&jeu);     // gérer la fin du niveau dans le cas ou l'on controle seulement la direction
            if ( (jeu.getPorteX()==itPacman->getPospX()) && (jeu.getPorteY()==itPacman->getPospY()) && (findeniveauAffiche==false) && (Portepositionne==true))
                FinDuNiveau();
        }
        update();
    }
}

void PacmanWindow::handleTimerPacgommePU()
{
    jeu.setPacgommePowerMode(false);         //on arrete le modePacgomme power up si le compteur s'arrete
}

void PacmanWindow::handleTimerRegeneration() //on ajoute des fantomes si leur nombre est inferieur à celui fixé pour le niveau
{
    if(jeu.fantomes.size()<(2+jeu.getNiveau())){
         jeu.ajout("fantome");
    }
}

void PacmanWindow::clickMonBouton_ajoutpacman()
{


    if (jeu.pacmans.size()<2)   // selon le nombre de pacmans on affiche des messages d'information
        {
         jeu.ajout("pacman");     // les limites de nombre sont déja pris en compte par la methode "ajout"
         PAUSE=true;
         QMessageBox msgBox;
         msgBox.setText("Un noveau PACMAN  :)");
         msgBox.setInformativeText("HAUT^ =Z | < GAUCHE=Q | DROIT > = Q | BAS  v = S");
         msgBox.exec();
         PAUSE=false;
    }else{
         PAUSE=true;
         QMessageBox msgBox;
         msgBox.setText("OUPS le nombre de PACMANS est limite a 2 :P ");
         msgBox.exec();
         PAUSE=false;
        }
}

void PacmanWindow::clickMonBouton_supprpacman()
{
         jeu.suppr("pacman");        // les limites de nombre sont déja pris en compte par la methode "ajout"
    if (jeu.pacmans.size()==1)  // selon le nombre de pacmans on affiche des message d'information
        {

         PAUSE=true;
         QMessageBox msgBox;
         msgBox.setText("Mais comment voulez-vous Jouer sans PACMAN!!! :P ");
         msgBox.exec();
         PAUSE=false;
        }
}

void PacmanWindow::clickMonBouton_ajoutfantome()
{
    cout<<"ajout"<<endl;
    jeu.ajout("fantome");      // les limites de nombre de fantomes est déja prise en compet par la methode "ajout"

    if(jeu.fantomes.size()>19){ // selon le nombre de fantomes on affiche des messages d'information
         PAUSE=true;
         QMessageBox msgBox;
         msgBox.setText("OOOLALA !!! Vous voulez plus ? NO :P ");
         msgBox.exec();
         PAUSE=false;
    }
}

void PacmanWindow::clickMonBouton_supprfantome()
{
    cout<<"suppression"<<endl;
    jeu.suppr("fantome");      // les limites de nombre sont déja pris en compte par la methode "ajout"

     if(jeu.fantomes.size()==1){  // selon le nombre de fantomes on affiche des messages d'information
         PAUSE=true;
         QMessageBox msgBox;
         msgBox.setText("Quand meme un seul fantome !!! Vous pouvez faire mieux, aller !");
         msgBox.exec();
         PAUSE=false;
         }
}

void PacmanWindow::FinDuNiveau()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Il n'y plus de PACGOMME ! Vous avez GAGNE!!!"));
    msgBox.setInformativeText("Vous voulez: ");

    QAbstractButton* pButtonNiveauSuivent = msgBox.addButton(tr("Niveau suivant"), QMessageBox::YesRole);
    msgBox.exec();
    findeniveauAffiche=true;
    if (msgBox.clickedButton()==pButtonNiveauSuivent)
     {
        jeu.setNiveau(jeu.getNiveau()+1); // on incremente le niveau du jeu
        findeniveauAffiche = false;
        Portepositionne = false;
        jeu.setPacgommePowerMode(false);
        jeu.init(jeu.pacmans.size());
     }
}

void PacmanWindow::Collision()
{
        this->hide();

    jeu.pacgommes.clear(); //supprimer tous les pacgommes
    jeu.fantomes.clear();  //supprimer tous les fantomes

    QMessageBox msgBox;

    msgBox.setText(tr("Il y a une collision ! Vous avez perdu!!!"));

    msgBox.setInformativeText("Vous voulez: ");

    QAbstractButton* pButtonRStart = msgBox.addButton(tr("Relancer"), QMessageBox::YesRole);
    QAbstractButton* pButtonClose = msgBox.addButton(tr("Quitter"), QMessageBox::NoRole);

    msgBox.exec();

    jeu.enregestrerMeilleureScore(); // s'il y a collision on enregistre la valeur du meilleur score

    if (msgBox.clickedButton()==pButtonRStart) {

       msgBox.close();
       this->show();

       if(jeu.getNiveau()>1)      // si l'on veut rendre le jeu plus complexe on perd un niveau s'il y a collision
            jeu.setNiveau(jeu.getNiveau()-1); // si on a collistion on descend d'un niveau
       jeu.setPacgommePowerMode(false);
       jeu.init(jeu.pacmans.size());

    }else if (msgBox.clickedButton()==pButtonClose)
        {
         msgBox.close();
         close();
        }
}

void PacmanWindow::clickMonBouton_pause()
{
    PAUSE=true;   // si on clique sur le bouton pause on met le jeu en pause et on affiche un MSGBOX

    QMessageBox msgBox;
    msgBox.setText(tr("PAUSE..."));
    msgBox.setInformativeText("Vous voulez: ");

    QAbstractButton* pButtonContibue = msgBox.addButton(tr("Continuer"), QMessageBox::YesRole);
    QAbstractButton* pButtonRestart = msgBox.addButton(tr("Recommencer"), QMessageBox::YesRole);
    QAbstractButton* pButtonQuitter = msgBox.addButton(tr("Quitter"), QMessageBox::YesRole);
    msgBox.exec();
    if (msgBox.clickedButton()==pButtonContibue)
     {
         PAUSE=false;
     }
     else if(msgBox.clickedButton()==pButtonRestart)
     {
         jeu.pacgommes.clear(); //supprimer tous les pacgomme
         jeu.fantomes.clear();  //suprimmer tous les fantomes
         jeu.setScore(0);       //on réinitialise le score
         jeu.setPacgommePowerMode(false);
         jeu.init(jeu.pacmans.size());//je relance avec un nombre de pacman egal au nombre déja existant
         PAUSE=false;
     }
     else if(msgBox.clickedButton()==pButtonQuitter)
     {
         msgBox.close();
         close();
     }

}

void PacmanWindow::clickMonBouton_parametre()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Mode de controle des Pacmans"));
    msgBox.setInformativeText("Vous voulez controler : ");

    QAbstractButton* pButtonDirecMove = msgBox.addButton(tr("direction et mouvement"), QMessageBox::YesRole);
    QAbstractButton* pButtonDire = msgBox.addButton(tr("direction"), QMessageBox::YesRole);
    msgBox.exec();

    if (msgBox.clickedButton()==pButtonDirecMove) {
        jeu.setMouvementEtDirection(false);
    }else if (msgBox.clickedButton()==pButtonDire){
        jeu.setMouvementEtDirection(true);
    }
}
