#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include "qextserialport.h"
#include "prog8051.h"

prog8051::prog8051(QWidget* parent):QDialog(parent)
{
	setupUi(this);
	connect(QuitButton,SIGNAL(clicked()),this,SLOT(accept()));
	connect(BrowseButton,SIGNAL(clicked()),this,SLOT(BrowseClicked()));
	connect(ProgramButton,SIGNAL(clicked()),this,SLOT(ProgramClicked()));
}

void prog8051::BrowseClicked(void)
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this,tr("Open Hex File"),"",tr("Hex Files (*.ihx *.hex)"));
	HexFile->setText(filename);
}

void prog8051::ProgramClicked(void)
{
	int j;
	QextSerialPort port;
	QByteArray ReadString;
	QFile DataFile;
	port.setPortName(SerialPort->text());
	port.setBaudRate(BAUD9600);
	port.setDataBits(DATA_8);
    	port.setParity(PAR_NONE);
    	port.setStopBits(STOP_1);
    	port.setFlowControl(FLOW_OFF);
	port.open(QIODevice::ReadWrite);
	
	DataFile.setFileName(HexFile->text());
	DataFile.open(QIODevice::ReadOnly);
	while(!DataFile.atEnd())
	{
		ReadString=DataFile.readLine();
		port.write(ReadString);
	}
	DataFile.close();
	port.close();}

