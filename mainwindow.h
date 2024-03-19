#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.h"

class PacmanWindow : public QFrame
{
  protected:
    Jeu jeu;
    QPixmap pixmapPacman1, pixmapPacman2, pixmapFantome, pixmapMur, pixmapPacgomme, pixmapPacgommePowerUp , pixmapPorte, pixmapIntro;
    QLabel *labelInfo; //label pour afficher des informations
    QTimer *timerPacgommPowerUp = new QTimer(this);

  public:
    PacmanWindow(QWidget *pParent=0, Qt::WindowFlags flags=0);
    void handleTimer();
    void handleTimerPacman();
    void handleTimerPacgommePU();
    void handleTimerRegeneration();
    bool intro=true;


    void clickMonBouton_ajoutfantome();
    void clickMonBouton_supprfantome();
    void clickMonBouton_ajoutpacman();
    void clickMonBouton_supprpacman();
    void clickMonBouton_superPower();
    void clickMonBouton_parametre();
    void clickMonBouton_pause();
    void clickMonBouton_start();

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void FinDuNiveau();
    void Collision();


    bool findeniveauAffiche = false;
    bool Portepositionne = false;
    bool PAUSE=false;
};

class PacmanButton : public QPushButton
{
public:
    PacmanButton(QWidget *pParent=0);
    void keyPressEvent(QKeyEvent *);
};
#endif // MAINWINDOW_H
