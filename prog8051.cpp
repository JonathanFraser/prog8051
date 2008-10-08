#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "prog8051.h"

prog8051::prog8051(QWidget* parent):QDialog(parent)
{
	setupUi(this);
	connect(QuitButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(BrowseButton,SIGNAL(clicked()),this,SLOT(BrowseClicked()));
	connect(ProgramButton,SIGNAL(clicked()),this,SLOT(ProgramClicked()));
	connect(StopButton,SIGNAL(clicked()),this,SLOT(StopClicked()));
	StopButton->setEnabled(false);
	ProgressBar->setRange(0,100);
}

void prog8051::BrowseClicked(void)
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,tr("Open Hex File"),"",tr("Hex Files (*.ihx *.hex)"));
	HexFile->setText(filename);
}

void prog8051::Alerts(QString message)
{
	QMessageBox::information(this,"ERROR",message);
}

void prog8051::SetStatus(int i)
{
	i = (i>100)?100:i;
	i = (i<0)?0:i;
	ProgressBar->setValue(i);
}

void prog8051::ThreadFinished()
{
	ButtonsToggle(true);
}

void prog8051::ButtonsToggle(bool on)
{
	SerialPort->setEnabled(on);
	Baudrate->setEnabled(on);
	HexFile->setEnabled(on);
	BrowseButton->setEnabled(on);
	ProgramButton->setEnabled(on);
	QuitButton->setEnabled(on);
	StopButton->setEnabled(!on);
	
}

void prog8051::SetInst(QString str)
{
	Instructions->setText(str);
}

void prog8051::StopClicked()
{
	if(progthread != NULL)
		progthread->Cancel();
}

void prog8051::ProgramClicked(void)
{
	if(HexFile->text() == "")
	{
		BrowseClicked();
		if(HexFile->text() == "")
		{
			QMessageBox::information(this,"ERROR","ERROR: I need a filename to proceed.");
			return;
		}
	}
	
	progthread = new progThread;
	connect(progthread,SIGNAL(Status(int)),this,SLOT(SetStatus(int)));
	connect(progthread,SIGNAL(Error(QString)),this,SLOT(Alerts(QString)));
	connect(progthread,SIGNAL(finished()),this,SLOT(ThreadFinished()));
	connect(progthread,SIGNAL(Info(QString)),this,SLOT(SetInst(QString)));

	progthread->Init(HexFile->text(),SerialPort->text(),115200);
	progthread->start();
	ButtonsToggle(false);}


