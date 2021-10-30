#include <QObject> 
#include <QLabel> 
#include <QUrl>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QComboBox>
#include <sqlitedb.h>
#include <QGridLayout>
#include <QBoxLayout>
#include <QDialog>
#include <QMouseEvent>
#include <QLineEdit>
#include <QScrollArea>

QString readCSS();
QString readFile(QString path);
bool ifexists(QString path);
QString audFileCheck(QString num);


class loadLbl : public QLabel{
Q_OBJECT

public slots:
        void setLoad();
        void setLoad(QUrl url);
        void setClear();
	void setClear(QUrl url);
	void sgnAdded();
};

/*------------------------------------
 * pre: QWidget
 * post: defpage object
 * A way to define a Qwidget for me since there's like 12 different wants to do it and if you don't do it just right, it punks on you
 * will eventually show the definition of the word
 * ---------------------------------*/
class defpage : public QDialog{
Q_OBJECT

public:
        defpage();
        void setup(int charid);
        QString trans2Str(sqlitedb *db);
        QString def2Str(sqlitedb *db);
        QString preBrTb(QString str);


/*
CREATE TABLE char(id int primary key, char text, unum text, rad_id int, strokes int, strk_rad int);
CREATE TABLE def(id int primkary key, hold_id int, type text, f text, e text, q text, s text, l text, a text);
CREATE TABLE hold(id int primary key, num int, pinyin text, char_id int, zy_1 int, zy_2 int, zy_3 int, accent int, zhuyin text);
CREATE TABLE translation(id int primary key, char_id int, lang text, def text);
*/
        QHash<QString, int> tblchar;
        QHash<QString, int> tblhold;
        QHash<QString, int> tbldef;
        QHash<QString, int> tbltrans;
        QGridLayout *glayout;
        //QTextBrowser *deftxt;
        QWebEngineView *deftxt;

signals:
	void loadDefSgn(int i);

public slots:

	void sgnRun(QUrl url);
	//void sgnLoadDef(int i); not being used anymore
	void keyPressEvent(QKeyEvent *e); //used for crtl-c

};


class clickTBP : public QWebEnginePage{
Q_OBJECT
public:
    clickTBP(QObject* parent = 0) : QWebEnginePage(parent){}

    bool acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool){

        if (type == QWebEnginePage::NavigationTypeLinkClicked){
            emit linkClicked(url);
            return false;
        }
        return true;
    }

signals:
    void linkClicked(const QUrl&);
};


//class clickTB : public QTextBrowser{
class clickTB : public QWebEngineView{
Q_OBJECT


public:
	clickTB(QComboBox *cb1, QComboBox *cb2, QComboBox *cb3, QComboBox *cb4, loadLbl *ldlbl); //to get value of comboboxes
	clickTB(QLineEdit *lineedit, loadLbl *ldlbl); //to get value of comboboxes
	clickTB(); //to get value of comboboxes
	clickTB(int minw, int minh, int maxw, int maxh); //to get value of comboboxes
	QString sqlStr();
	QString db2Str(QString sqlstr);

	QString radSqlStr();
	QString radDb2Str(QString sqlstr);

	QString freeSqlStr();
	QString freeDb2Str(QString sqlstr);

	QString engSqlStr();
	QString engDb2Str(QString sqlstr);

	QString qlist2Html(QList<QString> qlist);

	QString pList2Html();
	
	QComboBox *cbzy1;
        QComboBox *cbzy2;
        QComboBox *cbzy3;
        QComboBox *cbzya;
	QComboBox *dict;
	loadLbl *llbl;
	
	QLineEdit *qle;

  QWebEnginePage *page;
signals:
	void clicked();

protected:
	void mousePressEvent(QMouseEvent* event);

public slots:

	void sgnRun();
        void sgnRadRun();
        void sgnFreeRun();
	void sgnEngRun();
	void sgnRnd();
	void sgnAddPDict();
	void sgnDelPDict(QUrl url);
};


class clickComboBox : public QComboBox{
Q_OBJECT

public slots:
	void sgnHideShow(QString str);

};

/*-------------------------
pre:
post:
learning tabl
--------------------------*/
class lrnTb : public QWidget{
Q_OBJECT

public:
	loadLbl *ldlbl;
        void setup();
/*
public slots:
        void sgnLdLbl(); //generate random word from button press
        void sgnWrdAdd(); //add word from input
        void sgnDelWrd(QUrl url);
*/
};

