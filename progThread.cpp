#include <QApplication>
#include "progThread.h"
#include <QTime>
#define LINESIZE 32

void progThread::Init(QString Filename,QString Portname,int Baudrate)
{
	port = Portname;
	file = Filename;
	stop=false;
	delay = (unsigned char)14000/Baudrate;
	switch(Baudrate)
	{
		case 300:
			baud=BAUD300;
			break;
		case 600:
			baud=BAUD600;
			break;
		case 1200:
			baud=BAUD1200;
			break;
		case 4800:
			baud=BAUD4800;
			break;
		case 9600:
			baud=BAUD9600;
			break;
		case 19200:
			baud=BAUD19200;
			break;
		case 38400:
			baud=BAUD38400;
			break;
		case 57600:
			baud=BAUD57600;
			break;
		case 115200:
			baud=BAUD115200;
			break;
		default:
			baud=BAUD300;
	}
}

int progThread::LoadFile()
{
	QFile DataFile;
	int numbytes;
	bool ok;
	unsigned char linesize,recordtype,rchksum;
	unsigned short address;
	int linecounter=0;
	int MaxAddress=0;
	int chksum;
	int numread;
	QByteArray ReadLine;
	QByteArray Data;
	
	emit Status(0);
	DataFile.setFileName(file);
	if(!DataFile.open(QIODevice::ReadOnly))
	{
		emit Error("Failed to open HexFile");
		return -1;
	}
	
	numbytes=DataFile.size();
	for(int j=0;j<P89V51RD2_MEMSIZE;j++) FlashBuff[j]=0xff;
	while(!DataFile.atEnd())
	{
		ReadLine = DataFile.readLine();
		numread+=ReadLine.length();
		emit Status(numread*100/numbytes);
		linecounter++;
		if(ReadLine.startsWith(':'))
		{
			ReadLine = ReadLine.right(ReadLine.length()-1); 	//Truncate :
			
			linesize=ReadLine.left(2).toInt(&ok,16);	//Read Length
			ReadLine = ReadLine.right(ReadLine.length()-2); // Truncate Record Length	
			
			address=ReadLine.left(4).toInt(&ok,16);		//Read Address
			ReadLine = ReadLine.right(ReadLine.length()-4);  	//Truncate Address Portion
			
			recordtype=ReadLine.left(2).toInt(&ok,16);	//Read Type
			ReadLine = ReadLine.right(ReadLine.length()-2);	//Truncate RecordType
			
			Data = ReadLine.left(linesize*2);
			ReadLine = ReadLine.right(ReadLine.length()-linesize*2); //Truncate data
			
			rchksum = ReadLine.left(2).toInt(&ok,16);
			chksum=linesize+(address/0x100)+(address%0x100)+recordtype+rchksum;
			
			Data=QByteArray::fromHex(Data); //convert Data
			
			if(recordtype==1) break;
			
			for(int j=0;j<linesize;j++)
			{
				chksum+=Data[j];
				if((address+j)<P89V51RD2_MEMSIZE) FlashBuff[address+j]=Data[j];	
			}
			
			if((chksum%0x100)!=0)
			{
				emit Error("File Contains Bad Checksum");
				return -1;
			}
		}
	}	
	
	DataFile.close();
	return MaxAddress;
}

int progThread::PortWrRd(QByteArray c,int delay)
{
	QByteArray s;
	PortHandle->write(c);
	msleep(delay+1);
	s=PortHandle->read(c.length());
	if(c==s)
		return c.length();
	else
		return 0;
}

bool progThread::PortWrStr(QByteArray Str)
{
	QTime timer;
	bool retval =false;
	char c='$';
	PortHandle->write(Str);
	timer.start();
	while(c!='.' && !(timer.elapsed() > 10000))
	{
		PortHandle->read(&c,1);
	}
	retval = (c=='.')?true:false;
	while(PortHandle->read(&c,1));
	return retval;
}

bool progThread::PrepareIntelLine(QByteArray* x,int address,int length)
{
	unsigned char chksum;
	bool retval=false;
	QString Buff;
	unsigned char c;	
	QChar spacer('0');
	Buff = QString(":%1%2").arg(length,2,16,spacer).arg(address,4,16,spacer);
	Buff += "00";
	chksum = (unsigned char)(length + (address%0x100) + (address/0x100));
	for(int j=0;j<length;j++)
	{
		c = FlashBuff[address+j];
		Buff+=QString("%1").arg( c,2,16,spacer);
		chksum += c;
		if(c!=0xff) retval=true;
	}
	Buff+=QString("%1\r\n").arg((unsigned char)(0x100-chksum),2,16,spacer);
	*x = Buff.toAscii();
	return retval;
}

void progThread::Cancel()
{
	stop=true;
}

void progThread::run(void)
{
	int GoodU,Utries;
	QByteArray Buffer;
	PortHandle = new QextSerialPort();
	PortHandle->setPortName(port);
	PortHandle->setBaudRate(baud);
	PortHandle->setDataBits(DATA_8);
    	PortHandle->setParity(PAR_NONE);
    	PortHandle->setStopBits(STOP_2);
    	PortHandle->setFlowControl(FLOW_OFF);
	

	LoadFile();
	emit Status(0);
	
	if(!PortHandle->open(QIODevice::ReadWrite))
	{
		emit Error("Could not open Serial Port");
		return;
	}

	PortHandle->setDtr(0);
	msleep(5);
	PortHandle->setDtr(1);
	msleep(5);
	
	emit Info("Please Reset Microcontroller Now....");
		
	PortHandle->setTimeout(0,0);
	
	while(!stop)
	{
		if((GoodU==0) && ((Utries%200)==0))
		{
			PortHandle->setDtr(0);
			msleep(5);
			PortHandle->setDtr(1);
			msleep(5);
		}
		Utries++;
		
		if(PortWrRd("U",delay))
			GoodU++;
		else
			GoodU=0;
		
		emit Status(GoodU*2);
		if(GoodU == 50) break;
	}
	
	if(stop)
	{
		emit Error("Programming Aborted By user");
		emit Info("ERROR");
		PortHandle->close();
		delete PortHandle;
		return;
	}	
	
	if(!PortWrStr(":0100000301FB"))
	{
		emit Error("Flash Erase Failed");
		emit Info("ERROR");
		PortHandle->close();
		delete PortHandle;
		return;
	}
	
	emit Info("Flash Erased");
	emit Status(0);
		
	for(int j=0;j<0x10000;j+=LINESIZE)
	{
		if(stop)
		{
			emit Error("Programming Aborted By user");
			emit Info("ERROR");
			PortHandle->close();
			delete PortHandle;
			return;
		}
		
		Info(QString("0x%1").arg(j,4,16,QChar('0')));
		Status(j*100/0x10000);
		if(PrepareIntelLine(&Buffer,j,LINESIZE))
		{
			if(!PortWrStr(Buffer))
			{
				emit Error("Flash Program Command Failed");
				emit Info("ERROR");
				PortHandle->close();
				delete PortHandle;
				return;
			}
		}
	}
	
	if(!PortWrStr(":00000001FF\r\n"))
	{
		emit Error("Flash end Failed");
		emit Info("ERROR");
		PortHandle->close();
		delete PortHandle;
		return;
	}
	emit Status(100);
	emit Info("Flash Complete!!!");
	
	PortHandle->setDtr(0);
	msleep(5);
	PortHandle->setDtr(1);
	msleep(5);
			
	PortHandle->close();
	delete PortHandle;
}
