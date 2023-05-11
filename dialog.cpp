#pragma once
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "dialog.h"
#include <QDate>
#include <QVariant>
#include <io.h>
#include <QFontMetrics>
#include <cmath>
#include <cstring>

FILE* f;

mainwin::mainwin(QWidget* parent) {

	crdelbutton = new QPushButton("Создание и удаление записей");
	trbutton = new QPushButton("Сведения о платежах за период");
	sumdatebutton = new QPushButton("Сумма платежей за дату");
	sumcombutton = new QPushButton("Суммарная комиссия от переводов");
	graphbutton = new QPushButton("График зависимости комиссии от суммы");
	//connect(Button, SIGNAL(clicked()), this, SLOT(click()));
	connect(sumdatebutton, SIGNAL(clicked()), this, SLOT(sumdatebuttoncl()));
	connect(crdelbutton, SIGNAL(clicked()), this, SLOT(crdelbuttoncl()));
	connect(trbutton, SIGNAL(clicked()), this, SLOT(trbuttoncl()));
	connect(sumcombutton, SIGNAL(clicked()), this, SLOT(sumcombuttoncl()));
	connect(graphbutton, SIGNAL(clicked()), this, SLOT(graphbuttoncl()));

	QVBoxLayout* dlgLayout = new QVBoxLayout();

	dlgLayout->addWidget(crdelbutton);
	dlgLayout->addWidget(trbutton);
	dlgLayout->addWidget(sumdatebutton);
	dlgLayout->addWidget(sumcombutton);
	dlgLayout->addWidget(graphbutton);
	setLayout(dlgLayout);	

	cranddelwindow = new cranddelwin();
	sumcomwindow = new sumcomwin();
	trwindow = new trwin();
	sumdatewindow = new sumdatewin();
	graphwindow = new graphwin();

	trwindow->setWindowTitle("Сведения о платежах");
	sumcomwindow->setWindowTitle("Комиссия от переводов");
	cranddelwindow->setWindowTitle("Создание и удаление записей");
	sumdatewindow->setWindowTitle("Сведения о платежах");
	graphwindow->setWindowTitle("Сведения о платежах");


	f = fopen("plat.dat", "r+b");
	if (f == nullptr)
	{
		f = fopen("plat.dat", "w+b");
	}
	
}

void mainwin::crdelbuttoncl()
{
	cranddelwindow->show();
}

void mainwin::trbuttoncl()
{
	trwindow->show();
}

void mainwin::sumdatebuttoncl()
{
	sumdatewindow->show();
}

void mainwin::sumcombuttoncl()
{	
	sumcomwindow->show();
}

void mainwin::graphbuttoncl()
{
	graphwindow->show();
}

cranddelwin::cranddelwin(QWidget* parent)
{
	dateedit = new QLineEdit();
	dateedit->setPlaceholderText("01.01.2019");
	numedit = new QLineEdit();
	numedit->setPlaceholderText("123456789");
	sumedit = new QLineEdit();
	sumedit->setPlaceholderText("1234.56");
	commisedit = new QLineEdit();
	commisedit->setPlaceholderText("12.5");
	crbutton = new QPushButton("Добавить запись");
	delbutton = new QPushButton("Удалить запись");
	datelabel = new QLabel("Дата:");
	numlabel = new QLabel("Номер счёта:");
	sumlabel = new QLabel("Сумма:");
	commislabel = new QLabel("Комиссия:");
	connect(crbutton, SIGNAL(clicked()), this, SLOT(crbuttoncl()));
	connect(delbutton, SIGNAL(clicked()), this, SLOT(delbuttoncl()));
	QVBoxLayout* dlgLayout = new QVBoxLayout();
	QHBoxLayout* date = new QHBoxLayout();
	QHBoxLayout* num = new QHBoxLayout();
	QHBoxLayout* sum = new QHBoxLayout();
	QHBoxLayout* commis = new QHBoxLayout();
	QHBoxLayout* buttons = new QHBoxLayout();
	date->addWidget(datelabel);
	date->addWidget(dateedit);
	num->addWidget(numlabel);
	num->addWidget(numedit);
	sum->addWidget(sumlabel);
	sum->addWidget(sumedit);
	commis->addWidget(commislabel);
	commis->addWidget(commisedit);
	buttons->addWidget(delbutton);
	buttons->addWidget(crbutton);
	dlgLayout->addLayout(date);
	dlgLayout->addLayout(num);
	dlgLayout->addLayout(sum);
	dlgLayout->addLayout(commis);
	dlgLayout->addLayout(buttons);
	setLayout(dlgLayout);
}

void cranddelwin::crbuttoncl()
{
	rewind(f);
	dat data;
	dat data2;
	data.date = QDate::fromString(dateedit->text(), "dd.MM.yyyy");
	data.num = numedit->text().toInt();
	data.sum = sumedit->text().toFloat();
	data.com = commisedit->text().toFloat();
	fgetc(f);
	long test = ftell(f);
	if (feof(f))
	{
		rewind(f);
		fwrite(&data, sizeof(data), 1, f);
	}
	else
	{
		rewind(f);
		while((fread(&data2, sizeof(data2), 1, f), !feof(f)) && (data.sum > data2.sum))
		{}
		if (data.sum > data2.sum)
		{
			fseek(f, 0, 1);
			fwrite(&data, sizeof(data), 1, f);
		}
		else
		{
			long num = ftell(f) - sizeof(data);
			fseek(f, -sizeof(data), 2);
			test = ftell(f);
			while (num != ftell(f))
			{
				fread(&data2, sizeof(data), 1, f);
				fseek(f, 0, 1);
				fwrite(&data2, sizeof(data), 1, f);
				fseek(f, -3*sizeof(data), 1);
				test = ftell(f);
			}
			fread(&data2, sizeof(data), 1, f);
			fseek(f, 0, 1);
			fwrite(&data2, sizeof(data), 1, f);
			fseek(f, -2 * sizeof(data), 1);
			fwrite(&data, sizeof(data), 1, f);
		}
	}
	dateedit->setText("запись создана");
	numedit->setText("");
	sumedit->setText("");
	commisedit->setText("");
}

void cranddelwin::delbuttoncl()
{
	rewind(f);
	dat data;
	dat data2;
	data.date = QDate::fromString(dateedit->text(), "dd.MM.yyyy");
	data.num = numedit->text().toInt();
	data.sum = sumedit->text().toFloat();
	data.com = commisedit->text().toFloat();
	bool found = false;
	long num;
	while ((fread(&data2, sizeof(data2), 1, f), !feof(f)) && !found)
	{
		if ((data2.date == data.date) && (data2.num == data.num) && (data2.sum == data.sum) && (data2.com == data.com))
		{
			num = ftell(f) - sizeof(data);
			found = true;
		}
	}
	if (found)
	{
		bool ed = false;
		fseek(f, num, 0);
		while ((fread(&data2, sizeof(data2), 1, f), !feof(f)) && !ed)
		{

			if ((fread(&data2, sizeof(data2), 1, f), !feof(f)))
			{
				fseek(f, num, 0);
				fwrite(&data2, sizeof(data2), 1, f);
				num += sizeof(data);
			}
			else
			{
				ed = true;
				fseek(f, num, 0);
				_chsize(fileno(f), num);
			}
			fseek(f, num, 0);
		}
		dateedit->setText("запись удалена");
	}
	else { dateedit->setText("запись не найдена"); }
	numedit->setText("");
	sumedit->setText("");
	commisedit->setText("");
}

Area::Area(QWidget* parent) : QWidget(parent)
{
	setFixedSize(QSize(667, 283));
}

void Area::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setPen(Qt::black);

	//painter.drawLine(10, 10, 20, 20);
	const int sizex = 619;
	const int sizey = 283;
	const int repnumx = 4;
	int repnumy = 10;
	const int margin = 5;
	const int margin2 = 50;
	float maxcom = 0;
	float maxsum = 0;
	rewind(f);
	dat data;
	int numdata = 0;
	while ((fread(&data, sizeof(data), 1, f), !feof(f)))
	{
		numdata++;
		if (data.com > maxcom)
		{
			maxcom = data.com;
		}
		
	}
	rewind(f);
	if ((fread(&data, sizeof(data), 1, f), !feof(f)))
	{
		fseek(f, -sizeof(data), 2);
		fread(&data, sizeof(data), 1, f);
		maxsum = data.sum;
	}

	QFontMetrics fm(painter.font());

	painter.drawLine(margin2, margin, margin2, sizey - margin2);
	painter.drawLine(margin2, sizey - margin2, sizex - margin, sizey - margin2);
	painter.drawText(margin2-5-fm.horizontalAdvance("0"), sizey - margin2 + 13, "0");
	
	for (int i = 1; i <= repnumx; i++)
	{
		char number[20];
		float fl_i = i;
		sprintf(number, "%6.2f%%", (fl_i / repnumx) * maxcom);
		int liney = round(sizey - margin2 - (fl_i / repnumx * (sizey - margin - margin2 - 5)));
		painter.drawLine(margin2 - 2, liney, margin2 + 3, liney);
		painter.drawText(margin2-6 - fm.horizontalAdvance(number), liney+4,number);
	}
	for (int i = 1; i <= repnumy; i++)
	{
		char number[20];
		float fl_i = i;
		sprintf(number, "%6.2f", (fl_i / repnumy) * maxsum);
		int linex = round(margin2 + (fl_i / repnumy * (sizex - margin - margin2 - 5)));
		painter.drawLine(linex, sizey - margin2 - 2, linex, sizey - margin2 + 3);
		painter.drawText(linex-fm.horizontalAdvance(number)/2, sizey - margin2 + 18,number);
	}
	rewind(f);
	int prevposx;
	int prevposy;
	if ((fread(&data, sizeof(data), 1, f), !feof(f)))
	{
		prevposx = round(margin2 + data.sum * (sizex - margin - margin2-5) / maxsum);
		prevposy = round(sizey - margin2 - data.com * (sizey - margin2 - margin-5) / maxcom);
	}
	while ((fread(&data, sizeof(data), 1, f), !feof(f)))
	{
		int posx = round(margin2 + data.sum * (sizex - margin - margin2-5) / maxsum);
		int posy = round(sizey - margin2 - data.com * (sizey - margin2 - margin-5) / maxcom);
		painter.drawLine(prevposx, prevposy, posx, posy);
		prevposx = posx;
		prevposy = posy;
	}

	painter.drawText(55, 9, "Комиссия");
	painter.drawText(614 , sizey - margin2 -6, "Сумма");
}

graphwin::graphwin(QWidget* parent)
{
	area = new Area();
	QVBoxLayout* dlgLayout = new QVBoxLayout();
	dlgLayout->addWidget(area);
	setLayout(dlgLayout);
}

sumcomwin::sumcomwin(QWidget* parent)
{
	numedit = new QLineEdit();
	numedit->setPlaceholderText("123456789");
	sumedit = new QLineEdit();	
	sumedit->setReadOnly(true);
	numlabel = new QLabel("Номер счёта:");
	sumlabel = new QLabel("Сумма:");
	calcbutton = new QPushButton("Подсчитать");
	connect(calcbutton, SIGNAL(clicked()), this, SLOT(calcbuttoncl()));
	QVBoxLayout* dlgLayout = new QVBoxLayout();
	dlgLayout->addWidget(numlabel);
	dlgLayout->addWidget(numedit);
	dlgLayout->addWidget(calcbutton);
	dlgLayout->addWidget(sumlabel);
	dlgLayout->addWidget(sumedit);
	setLayout(dlgLayout);
}

void sumcomwin::calcbuttoncl()
{
	dat data;
	int num = numedit->text().toInt();
	rewind(f);
	float sumcom = 0;
	while ((fread(&data, sizeof(data), 1, f), !feof(f)))
	{
		if (num == data.num)
		{
			sumcom += data.sum*data.com/100;
		}
	}
	char sumcomstr[20];
	sprintf(sumcomstr, "%f", sumcom);
	sumedit->setText(sumcomstr);
}

sumdatewin::sumdatewin(QWidget* parent)
{
	dateedit = new QLineEdit();
	dateedit->setPlaceholderText("01.01.2019");
	sumedit = new QLineEdit();
	sumedit->setReadOnly(true);
	datelabel = new QLabel("Дата:");
	sumlabel = new QLabel("Сумма:");
	findbutton = new QPushButton("Найти");
	connect(findbutton, SIGNAL(clicked()), this, SLOT(findbuttoncl()));
	QVBoxLayout* dlgLayout = new QVBoxLayout();
	dlgLayout->addWidget(datelabel);
	dlgLayout->addWidget(dateedit);
	dlgLayout->addWidget(findbutton);
	dlgLayout->addWidget(sumlabel);
	dlgLayout->addWidget(sumedit);
	setLayout(dlgLayout);

}

void sumdatewin::findbuttoncl()
{
	dat data;
	QDate date = QDate::fromString(dateedit->text(), "dd.MM.yyyy");
	rewind(f);
	float sum = 0;
	while ((fread(&data, sizeof(data), 1, f), !feof(f)))
	{
		if (date == data.date)
		{
			sum += data.sum;
		}
	}
	char sumstr[20];
	sprintf(sumstr, "%f", sum);
	sumedit->setText(sumstr);
}

trwin::trwin(QWidget* parent)
{
	fromedit = new QLineEdit();
	fromedit->setPlaceholderText("01.01.2019");
	toedit = new QLineEdit();
	toedit->setPlaceholderText("11.11.2019");
	enterperiodlabel = new QLabel("Введите период:");
	fromlabel = new QLabel("От:");
	infolabel = new QLabel("Информация за период:");
	tolabel = new QLabel("До:");
	findbutton = new QPushButton("Найти");
	outtext = new QTextEdit();
	outtext->setReadOnly(true);
	connect(findbutton, SIGNAL(clicked()), this, SLOT(findbuttoncl()));
	QVBoxLayout* dlgLayout = new QVBoxLayout();
	QHBoxLayout* fromto = new QHBoxLayout();
	fromto->addWidget(fromlabel);
	fromto->addWidget(fromedit);	
	fromto->addWidget(tolabel);
	fromto->addWidget(toedit);


	dlgLayout->addWidget(enterperiodlabel);
	dlgLayout->addLayout(fromto);
	dlgLayout->addWidget(findbutton);
	dlgLayout->addWidget(infolabel);
	dlgLayout->addWidget(outtext);
	setLayout(dlgLayout);
}

void trwin::findbuttoncl()
{
	dat data;
	QDate frdate = QDate::fromString(fromedit->text(), "dd.MM.yyyy");
	QDate todate = QDate::fromString(toedit->text(), "dd.MM.yyyy");

	rewind(f);
	outtext->clear();
	while ((fread(&data, sizeof(data), 1, f),!feof(f)))
	{

		if ((data.date <= todate) && (data.date >= frdate))
		{
			QString result = "Дата:";

			result += data.date.toString("dd.MM.yyyy");
			result += " ID:";
			QVariant buf(data.num);
			result += buf.toString();
			result += " Сумма:";
			buf = data.sum;
			result += buf.toString();
			result += " Коммисия:";
			buf = data.com;
			result += buf.toString();
			outtext->append(result);
		}
	}
}