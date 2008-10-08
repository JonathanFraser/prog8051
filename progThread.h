#include <QThread>
#include <QString>
#include <QByteArray>
#include "qextserialport.h"

#define P89V51RD2_MEMSIZE 0x10000

class progThread : public QThread
{
	Q_OBJECT
	public:
		void Init(QString Filename,QString Portname,int Baudrate);
		void Cancel();
	private:
		QString port;
		QString file;
		BaudRateType baud;	
		QByteArray FlashBuff;
		QextSerialPort* PortHandle;
		bool stop;
		unsigned char delay;
		
		virtual void run();
		int LoadFile();
		int PortWrRd(QByteArray,int);
		bool PortWrStr(QByteArray);
		bool PrepareIntelLine(QByteArray* ,int,int);
		
	signals:
		void Status(int);
		void Error(QString);
		void Info(QString);
};
