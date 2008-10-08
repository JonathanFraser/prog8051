#include <QDialog>
#include <QTimer>
#include "ui_prog8051.h"
#include "progThread.h"

class prog8051: public QDialog, public Ui::prog8051
{
	Q_OBJECT
	public:
		prog8051(QWidget* parent =0);
	
	private:
		progThread *progthread;
		void ButtonsToggle(bool);
		
	private slots:
		void BrowseClicked(void);
		void ProgramClicked(void);
		void SetStatus(int);
		void Alerts(QString);
		void SetInst(QString);
		void ThreadFinished();
};
