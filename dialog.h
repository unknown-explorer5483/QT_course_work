#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QSignalMapper>
#include <QLabel>
#include <QImage>
#include <QDate>
#include <QPainter>


struct dat { QDate date; int num; float sum; float com; };

class cranddelwin : public QDialog
{
    Q_OBJECT
public:
    cranddelwin(QWidget* parent = 0);
protected:
    QPushButton* crbutton, * delbutton;
    QLabel* datelabel, * numlabel, * sumlabel, * commislabel;
    QLineEdit* dateedit, * numedit, * sumedit, * commisedit;
private slots:
    void crbuttoncl();
    void delbuttoncl();
};

class Area : public QWidget
{

public:
    Area(QWidget* parent = 0);
    void paintEvent(QPaintEvent* event);


};


class graphwin : public QDialog
{
    Q_OBJECT
public:
    graphwin(QWidget* parent = 0);
    Area* area;
};

class sumcomwin : public QDialog
{
    Q_OBJECT
public:
    sumcomwin(QWidget* parent = 0);
protected:
    QPushButton* calcbutton;
    QLabel* numlabel, * sumlabel;
    QLineEdit* numedit, * sumedit;
private slots:
    void calcbuttoncl();
};

class sumdatewin : public QDialog
{
    Q_OBJECT
public:
    sumdatewin(QWidget* parent = 0);
protected:
    QPushButton* findbutton;
    QLabel* datelabel,* sumlabel;
    QLineEdit* dateedit, * sumedit;
private slots:
    void findbuttoncl();
};

class trwin : public QDialog
{
    Q_OBJECT
public:
    trwin(QWidget* parent = 0);
protected:
    QPushButton* findbutton;
    QLabel* enterperiodlabel, * fromlabel, * tolabel,* infolabel;
    QLineEdit * fromedit, * toedit;
    QTextEdit* outtext;
private slots:
    void findbuttoncl();
};



class mainwin : public QDialog
{
    Q_OBJECT
public:
    mainwin(QWidget* parent = 0);
protected:
    QPushButton* crdelbutton, * trbutton, * sumdatebutton, * sumcombutton, * graphbutton;
    cranddelwin* cranddelwindow;
    trwin* trwindow;
    sumdatewin* sumdatewindow;
    sumcomwin* sumcomwindow;
    graphwin* graphwindow;
private slots:
    void crdelbuttoncl();
    void trbuttoncl();
    void sumdatebuttoncl();
    void sumcombuttoncl();
    void graphbuttoncl();
};
