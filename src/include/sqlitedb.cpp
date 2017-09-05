#include <QtSql/QtSql>
#include <QFileInfo>
#include "./sqlitedb.h"

bool fileexists(QString path){
QFileInfo check_file(path);
return check_file.exists() && check_file.isFile();
}


/*-----------------------------------
 * pre:
 * post:
 *
 * --------------------------------*/
sqlitedb::sqlitedb(QString dbfname){
hnd=QSqlDatabase::addDatabase("QSQLITE");
hnd.setDatabaseName(dbfname);
hnd.open();
}



/*-----------------------------------
 * pre:
 * post:
 *
 * --------------------------------*/
sqlitedb::sqlitedb(){
hnd=QSqlDatabase::addDatabase("QSQLITE");
dbname="/home/user/MyDocs/.mae-moedict/assets/db/mae-moedict.db";
hnd.setDatabaseName(dbname);
hnd.open();
}

/*--------------------------------
 * pre:
 * post:
 *
 * ------------------------------*/
sqlitedb::~sqlitedb(){
close();
}


void sqlitedb::close(){
QString cnnct=hnd.connectionName();
hnd.close();
hnd=QSqlDatabase();
hnd.removeDatabase(cnnct);
};



/*--------------------------------
 * pre:
 * post:
 * -----------------------------*/
QString sqlitedb::query(QString qstr){
QString rtrn="";
result=QSqlQuery(qstr);

result.next();
rtrn+=result.value(0).toString();

return rtrn;
}

/*-----------------------------
 * pre:
 * post:
 * --------------------------*/
QString sqlitedb::next(){
QString rtrn="";
result.next();
rtrn+=result.value(0).toString();
return rtrn;
}


void sqlitedb::clear(){
result.finish();
}

