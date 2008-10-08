
#include "progThread.h"
#include <QTime>


void progThread::Init(QString Filename,QString Portname,int Baudrate)
{
	port = Portname;
	file = Filename;
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

bool progThread::PortWrRd(char c,int delay)
{
	char s;
	PortHandle->write(&c,1);
	msleep(delay+1);
	PortHandle->read(&s,1);
	return (c==s)?true:false;
}

bool progThread::PortWrStr(QByteArray Str)
{
	QTime timer;
	char c='$';
	PortHandle->write(Str);
	timer.start();
	while(c!='.' && !(timer.elapsed() > 10000))
	{
		PortHandle->read(&c,1);
	}
	return (c=='.')?true:false;
}

bool progThread::PrepareIntelLine(QByteArray* x,int address,int length)
{
	unsigned char chksum;
	bool retval=false;
	QString Buff;
	emit Info("Loading Hex File");	
	Buff = ":%1%200";
	Buff.arg(length,2,16).arg(address,4,16);
	
	chksum = (unsigned char)(length + (address%0x100) + (address/0x100));
	
	for(int j=0;j<length;j++)
	{
		Buff+=QString("%1").arg((unsigned char)FlashBuff[address+j],2,16);
		chksum += (unsigned char) FlashBuff[address+j];
		if(((unsigned char) FlashBuff[address+j])!=0xff) retval=true;
	}
	Buff+=QString("%1\r\n").arg((unsigned char)(0x100-chksum),2,16);
	*x += Buff.toAscii();
	return retval;
}

void progThread::run(void)
{
	PortHandle = new QextSerialPort();
	PortHandle->setPortName(port);
	PortHandle->setBaudRate(baud);
	PortHandle->setDataBits(DATA_8);
    	PortHandle->setParity(PAR_NONE);
    	PortHandle->setStopBits(STOP_2);
    	PortHandle->setFlowControl(FLOW_OFF);
	PortHandle->open(QIODevice::ReadWrite);
	LoadFile();
	PortHandle->close();
	delete PortHandle;
}
