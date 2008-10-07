#include <QDialog>
#include "ui_prog8051.h"


class prog8051: public QDialog, public Ui::prog8051
{
	Q_OBJECT
	public:
		prog8051(QWidget* parent =0);
	
	private slots:
		void BrowseClicked(void);
		void ProgramClicked(void);
};
