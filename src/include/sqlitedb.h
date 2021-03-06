#include <QtSql/QtSql>
#include <QObject>

class sqlitedb{
private:
	QSqlDatabase hnd; //holds QSqlDatabase handle: QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
	QString dbname;	// string to hold database filename for: db.setDatabaseName("./mae-moedict.db");
	
public:
	QSqlQuery result; //holds QSql Query:         QSqlQuery query("select char from zhuyin");
	sqlitedb(QString dbfname);
	sqlitedb();
	QString query(QString qstr);
	bool exec(QString qstr);
	QString next();
	void clear();
	void close();	
	~sqlitedb();		
};
