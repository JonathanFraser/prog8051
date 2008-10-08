#include <QThread>
#include <QString>
#include "qextserialport.h"

#define P89V51RD2_MEMSIZE 0x10000

class progThread : public QThread
{
	Q_OBJECT
	public:
		void Init(QString Filename,QString Portname,int Baudrate);
	
	private:
		QString port;
		QString file;
		BaudRateType baud;	
		QByteArray FlashBuff;
		QextSerialPort* PortHandle;
		
		virtual void run();
		int LoadFile();
		bool PortWrRd(char,int);
		bool PortWrStr(QByteArray);
		bool PrepareIntelLine(QByteArray* ,int,int);
		
	signals:
		void Status(int);
		void Error(QString);
		void Info(QString);
};
