#include "SerialEnumerator.h"
#include "qextserialport.h"
#include <QDir>
#include <QApplication>

#ifdef _TTY_POSIX_
QStringList GetSerialList()
{
	QString save = QDir::currentPath();
	QextSerialPort port;
	QDir directory;
	QStringList filter;
	QStringList holder;
	QStringList retval;
	filter << "ttyS[0-9]*" << "ttyUSB*";
	directory.setCurrent("/dev");
	holder = directory.entryList(filter,QDir::System|QDir::Files|QDir::Readable|QDir::Writable);
	for(int j=0;j<holder.size();j++)
	{
		holder[j]="/dev/"+holder[j];
		port.setPortName(holder[j]);
		if(port.open(QIODevice::ReadWrite))
		{
			retval << holder[j];
			port.close();
		}
	}
	directory.setCurrent(save);
	return retval;
}
#else
QStringList GetSerialList()
{
	QStringList retval;
	QString holder;
	QextSerialPort port;
	for(int j=1;j<256;j++)
	{
		holder=QString.setNum(j);
		holder="COM"+holder;
		port.setPortName(holder);
		if(port.open(QIODevice::ReadWrite))
		{
			retval<<holder;
			port.close();
		}
	}
	return retval; 
}
#endif



