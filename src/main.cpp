#include <QApplication>
#include <QTabWidget>
#include <QPushButton>
#include <QObject>
#include <QtSql/QtSql>
#include <QTextEdit>
#include <QFontDatabase>
#include <QtGui>
#include <QTextCodec>
#include <QHash>
#include <QComboBox>
#include <QMouseEvent>
#include <QFileInfo>
#include <QLineEdit>
#include <QKeyEvent>
#include <QDir>
#include <QGroupBox>
#include <QDateTime>
#include "./main.h"

//asset folder path
QString assetpath;

QString dictDb;
QString pdictDb;

bool pDbExists; //bool check to make sure personal db exists.

//global zhuyin arrays
QList<QString> zy1;
QList<QString> zy2;
QList<QString> zy3;
QList<QString> zya;
QHash<QString,int> zy2num;
QHash<QString,int> zya2num;
QHash<int,QString> num2zy;
QHash<int,QString> num2zya;

//global tab/button labels (in chinese)
QHash<QString,QString> lbls;

//global rad array
QList<QString> rad;
QList<int> radstrk;
QHash<QString, int> rad2num;

//global href because qt5.6's QWebEnginePage::acceptNavigationRequest needs a http: or anything def: in the front to work
QString protocol="def:";

//holding QList for personal list and random word holder  
QHash<int,QString> rnd;
QHash<QString,int> pListOrd; //word -> order number
QList<int> pListId; // order number -> word id 
QList<QString> pListChar; //order number -> word
QString addWord;     

//Max number of char
int charMax;      

//global defpage
defpage *dp;


void loadRad2Num(){
QString sqlq="select char,id from radical order by id";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
    rad2num[db->result.value(0).toString()]=db->result.value(1).toInt();
        db->next();
    }
db->close();
delete(db);
}



void loadNum2Zy(){
QString sqlq="select id,char from zhuyin order by id";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
    num2zy[db->result.value(0).toInt()]=db->result.value(1).toString();
        db->next();
    }
db->close();
delete(db);
}

void loadNum2Zya(){
QString sqlq="select id, char from zhuyinTone order by id";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
    num2zya[db->result.value(0).toInt()]=db->result.value(1).toString();
        db->next();
    }
db->close();
delete(db);
}

void loadZy2Num(){
QString sqlq="select char, id from zhuyin order by id";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
    zy2num[db->result.value(0).toString()]=db->result.value(1).toInt();
        db->next();
    }
db->close();
delete(db);
}

void loadZya2Num(){
QString sqlq="select char, id from zhuyinTone order by id";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
    zya2num[db->result.value(0).toString()]=db->result.value(1).toInt();
        db->next();
    }
db->close();
delete(db);
}

void list2Combo(QList<QString> &ls, QComboBox *cb){
int cbi=cb->count();
int i=1;
int size=ls.count();
    while(i<size){
    cb->insertItem(cbi,ls[i],i);
    cbi++;
    i++;
    }
}

void list2ComboRad(QList<QString> &ls, QList<int> &lsi, QComboBox *cb){
int cbi=cb->count();
int i=1;
int size=ls.count();
    while(i<size){
    cb->insertItem(cbi, "[" + QString::number(lsi[i]) + "] " + ls[i],i);
    cbi++;
    i++;
    }
}

void loadZhuyin(){
//int's to determine which zhuyin are the top, mid and bottom group.
int zyn1[2];
int zyn2[2];
int zyn3[2];

zyn1[0]=1;
zyn1[1]=21;
zyn3[0]=22;
zyn3[1]=34;
zyn2[0]=35;
zyn2[1]=37;

int i=1;
zy1.append(""); //using these space out the zero index
zy2.append("");
zy3.append("");
zya.append("");
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query("select char from zhuyin order by id");
        while(db->result.isValid()){
                if(i>=zyn1[0]&&i<=zyn1[1]){
                zy1.insert(i, db->result.value(0).toString());
                }
                else if(i>=zyn2[0]&&i<=zyn2[1]){
                zy2.insert(i, db->result.value(0).toString());
                }
                else if(i>=zyn3[0]&&i<=zyn3[1]){
                zy3.insert(i, db->result.value(0).toString());
                }
        i++;
        db->next();
        }
i=1;
db->query("select char from zhuyinTone order by id");
        while(db->result.isValid()){
        zya.append(db->result.value(0).toString());
        db->next();
        }
db->close();
delete(db);
}


void loadRad(){
int i=1;
rad.append("");//qlists have to have an index zero or it'll crash.
radstrk.append(0);//qlists have to have an index zero or it'll crash.
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query("select char,stroke from radical order by id");
        while(db->result.isValid()){
        rad.insert(i,db->result.value(0).toString());
        radstrk.insert(i,db->result.value(1).toInt());
    //qDebug() << "i) rad: "<<rad[i]<<", strk: "<<radstrk[i];
        i++;
        db->next();
        }

db->close();
delete(db);
}

bool ifexists(QString path){
QFileInfo check_file(path);
        if(check_file.exists() && check_file.isFile()){
    return true;
    }
return false;
}

bool pathexists(QString path){
QDir check_file(path);
        if(check_file.exists() && check_file.isReadable()){
        return true;
        }
return false;

}

QString oggFileCheck(QString num){
//check if need audio
QString ahtml="";
QString path=assetpath;
QString redir=path+"voices/gohere.txt";
    if(ifexists(redir)){
    path=readFile(redir);
    path=path.trimmed();
    qDebug() << "voices folder redirect found: " + path;
    }


QString testhtml=path+"voices/testhtml.txt";
QString testahtml="";
        if(ifexists(testhtml)){
        testahtml=readFile(testhtml);
        qDebug() << "inserting test html: " + testahtml;
        }



QString oggnum=num.rightJustified(4,'0');
QString oggpath=path + "voices/" + oggnum + ".ogg";
QFileInfo check_file(oggpath);
    if(check_file.exists() && check_file.isFile()){
    ahtml="<audio controls><source src=\"file:" + oggpath + "\" type=\"audio/ogg\"></audio><br>cmdline: mplayer " + oggpath + "<br><br>"+testahtml;
    qDebug() << "audio file found: " + oggpath + "\n" +ahtml;
    }
    else{
    ahtml="<br>";
    qDebug() << "audio file NOT found: " + oggpath;
    }
return ahtml;
}



/*------------------------
pre: db lib and database existing
post: none
gets the id of the word. if word doesn't exist, returns 0
------------------------*/
int getWordId(QString word){
	if(word.isEmpty()){
	return 0;
	}
int rtrn=0;

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query("select id from char where char=\""+word+"\"");
        rtrn=db->result.value(0).toInt();
db->close();
delete(db);
return rtrn;
}

/*------------------------
pre: db lib and database existing
post: none
gets the id of the word. if word doesn't exist, returns 0
------------------------*/
QString getWord(int id){
QString rtrn="";
	if(id<=0 || id>charMax){
	return rtrn;
	}
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query("select char from char where id="+QString::number(id));
        rtrn=db->result.value(0).toString();
db->close();
delete(db);
return rtrn;
}


/*------------------------
pre: dictionaries existing, custom sql class
post: none
generates a word id from it's respective dicitionary
returns 0 if error
-------------------------*/
QHash<int,QString> rndWord(QString dict){
QHash<int,QString> rtrn;
rtrn[0]="";
	if(dict.isEmpty()){
	return rtrn;
	}

	if(dict=="personal"){
		if(pListId.count()>0){
		//get random word from personal dictionary
		//find min and max from keys
		int pListMax=pListId.count(); //min is 0
		// generate random number between min and max
		qsrand(QDateTime::currentDateTime().toTime_t());
		int rndid=(qrand()%(pListMax));
		rtrn.remove(0);
		rtrn[pListId[rndid]]=pListChar[rndid];// word id -> word
		}
	//return that word
	return rtrn;
	}
	else if(dict=="all"){
	qsrand(QDateTime::currentDateTime().toTime_t());
	//assume min is 1, max is global charMax
	// random between those two numbers
	int rndid=(qrand()%(charMax))+1;
	rtrn.remove(0);
	rtrn[rndid]=getWord(rndid);
	return rtrn;
	}
return rtrn;
}

/*-------------------------
pre:
post:
checks to make sure that the personal dictionary exists
-------------------------*/
bool chckPersDict(){
return ifexists(assetpath + pdictDb);
}

/*---------------------------
pre: custom SQL Lib
post: pList is populated;
load all list
----------------------------*/
void loadPDict(){
QString sqlq="select char, id from personal order by datetime desc";

sqlitedb *pdb=new sqlitedb(assetpath + pdictDb);
pdb->query(sqlq);
pListId.clear();
pListOrd.clear();
pListChar.clear();
int i=0;
int id=0;
QString word="";
        while(pdb->result.isValid()){
	id=pdb->result.value(1).toInt();
	word=pdb->result.value(0).toString();
        pListOrd[word]=i;
        pListId.append(id);
        pListChar.append(word);
	i++;
        pdb->next();
        }
pdb->close();
delete(pdb);
return;
}

/*--------------------------
pre: this class, custom sql class/lib
post: pList is modified (potentially) and database modified (potentiall)
takes input word and (after some sanity checks) inserts it into the database and pList.
---------------------------*/
bool add2PList(QString word){
        //if word is empty or database doesn't exist, do nothing
        if(word.isEmpty()||!pDbExists){
        return false;
        }

	addWord=word;

        //word already in personal dictionary. do ntohing.
        if(pListOrd.contains(word)){
        return false;
        }

        //make sure word being entered does exist in the dictionary
        int id=getWordId(word);
        if(id>0){
        sqlitedb *db=new sqlitedb(assetpath + pdictDb);
        db->exec("insert into personal (id, datetime, char) values("+QString::number(id)+",CURRENT_TIMESTAMP, \""+word+"\")");
        db->close();
        delete(db);
        pListOrd[word]=id;
	pListId.prepend(id);
	pListChar.prepend(word);
        return true;
        }
return false;
}


/*--------------------------
pre: this class, custom sql class/lib
post: pList is modified (potentially) and database modified (potentiall)
takes input word and (after some sanity checks) inserts it into the database and pList.
---------------------------*/
bool delFrmPList(QString word){
        //if word is empty or database doesn't exist, do nothing
        if(word.isEmpty()||!pDbExists){
        return false;
        }

        //if word not in dictionary, do nothing
        if(!pListOrd.contains(word)){
        return false;
        }

	addWord=word;

        //make sure word being entered does exist in the dictionary*
        int id=getWordId(word);
        if(id>0){
        sqlitedb *db=new sqlitedb(assetpath + pdictDb);
        db->exec("delete from personal where id="+QString::number(id));
        db->close();
        delete(db);
	int ord=pListOrd[word];
        pListOrd.remove(word);
	pListChar.removeAt(ord);
	pListId.removeAt(ord);
        return true;
        }
return false;
}



/*-----------------------------------------------------
 * pre:
 * post:
 * defoage initializer
------------------------------------------------------*/
defpage::defpage(){
/*
CREATE TABLE char(id int primary key, char text, unum text, rad_id int, strokes int, strk_rad int);
CREATE TABLE def(id int primkary key, hold_id int, type text, f text, e text, q text, s text, l text, a text);
CREATE TABLE hold(id int primary key, num int, pinyin text, char_id int, zy_1 int, zy_2 int, zy_3 int, accent int, zhuyin text);
CREATE TABLE translation(id int primary key, char_id int, lang text, def text);
*/
    tblchar["id"]=0;
    tblchar["char"]=1;
    tblchar["unum"]=2;
    tblchar["rad_id"]=3;
    tblchar["strokes"]=4;
    tblchar["strk_rad"]=5;

    tblhold["id"]=0;
    tblhold["num"]=1;
    tblhold["pinyin"]=2;
    tblhold["char_id"]=3;
    tblhold["zy_1"]=4;
    tblhold["zy_2"]=5;
    tblhold["zy_3"]=6;
    tblhold["accent"]=7;
    tblhold["zhuyin"]=8;

    tbldef["id"]=0;
    tbldef["hold_id"]=1;
    tbldef["type"]=2;
    tbldef["def"]=3;
    tbldef["example"]=4;
    tbldef["quote"]=5;
    tbldef["short"]=6;
    tbldef["like"]=7;
    tbldef["anto"]=8;

    tbltrans["id"]=0;
    tbltrans["char_id"]=1;
    tbltrans["lang"]=2;
    tbltrans["def"]=3;
        glayout = new QGridLayout(this);
        //deftxt = new QTextBrowser(this);
        deftxt = new QWebEngineView(this);

        //deftxt->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        //deftxt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //deftxt->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOn);
        deftxt->setMinimumSize(740, 380);
        //deftxt->setReadOnly(true);
}




/*-----------------------------------------------------
 * pre:
 * post:
 * takes the translation info from sqlitedband generates
 * a single string
------------------------------------------------------*/
QString defpage::trans2Str(sqlitedb *db){
QString rtrn;
    if(!db->result.isValid()){
    qDebug() << db->result.lastError();
    return rtrn;
    }

QHash<QString, QString> trans;
    while(db->result.isValid()){
    trans[db->result.value(tbltrans["lang"]).toString()] += db->result.value(tbltrans["def"]).toString()+", ";
    db->next();
    }
QList<QString> keys=trans.keys();
int size=keys.size();
int i=0;
    while(i<size){
    rtrn+=keys[i]+": "+trans[keys[i]]+"\n<br>";
    i++;
    }

return rtrn;
}

/*-------------------------------
 *sets up the widget to display the informaiton
 * ---------------------------*/
void defpage::setup(int charid){

    this->setWindowTitle(QString::number(charid));
    sqlitedb *db=new sqlitedb(assetpath + dictDb);
    QString q="select id,char,unum,rad_id,strokes,strk_rad from char where id=" + QString::number(charid);
    //qDebug() << q;
    db->query(q);

    //char table
    QString chr=db->result.value(tblchar["char"]).toString();
    QString unum=db->result.value(tblchar["unum"]).toString();
    int rad_id=db->result.value(tblchar["rad_id"]).toInt();
    int strokes=db->result.value(tblchar["strokes"]).toInt();
    int strk_rad=db->result.value(tblchar["strk_rad"]).toInt();

    //translation table
    //CREATE TABLE translation(id int primary key, char_id int, lang text, def text);
    q="select id,char_id, lang, def from translation where char_id=" + QString::number(charid);
    db->query(q);
    QString trans=trans2Str(db);


    //hold table
    //CREATE TABLE hold(id int primary key, num int, pinyin text, char_id int, zy_1 int, zy_2 int, zy_3 int, accent int, zhuyin text);
    q="select h.id, z1.char, z2.char, z3.char, za.char, h.pinyin,h.num,h.zhuyin from hold as h left join zhuyin as z1 on h.zy_1=z1.id left join zhuyin as z2 on h.zy_2=z2.id left join zhuyin as z3 on h.zy_3=z3.id left join zhuyinTone as za on h.accent=za.id where h.char_id=" + QString::number(charid);
    db->query(q);
    QList<int> holdid;
    QHash<int,QString> zhuyin;
    QHash<int,QString> pinyin;
    QHash<int,QString> oggnum;
    QHash<int,QString> zhuyinstr;
    int int_holdid;
    while(db->result.isValid()){
    int_holdid=db->result.value(0).toInt();
    holdid.append(int_holdid);//hold_id
    zhuyin[int_holdid]=db->result.value(1).toString();
    zhuyin[int_holdid]+=db->result.value(2).toString();
    zhuyin[int_holdid]+=db->result.value(3).toString();
    zhuyin[int_holdid]+=db->result.value(4).toString();
    pinyin[int_holdid]=db->result.value(5).toString();
    oggnum[int_holdid]=db->result.value(6).toString();
    zhuyinstr[int_holdid]=db->result.value(7).toString();
    db->next();
    }

    //CREATE TABLE def(id int primkary key, hold_id int, type text, f text, e text, q text, s text, l text, a text);
    //def table
    QHash<int,QString> def;//indexed by holdid
    int i=0;
    int size=holdid.size();
    while(i<size){
        q="select id,hold_id, type, f, e, q, s, l, a from def where hold_id=" + QString::number(holdid[i]);
    db->query(q);
    def[holdid[i]]+=def2Str(db);
    i++;
    }
    db->close();

    //gen/get css for webview
    QString cssStyle=readCSS();

    //generate the text to show
    // word, radical and stroke number
	

	QString inTxt="<html><head><style>@font-face{font-family: \"DroidSansFallbackFull\"; src url(\"file:" + assetpath + "DroidSansFallbackFull.ttf\");}" + cssStyle + "</style></head><body>";
	inTxt += "<div id=\"char\">" + chr + "</div>\n\n[" + rad[rad_id] + "] " + lbls["part"] + " + " + QString::number(strk_rad) + " = [" + QString::number(strokes) + "] " + lbls["hua"] + "\n\n";
	QString sep="<hr>\n";

	//zhuyin, pinyin and definition.
	QString ahtml="";
	i=0;
	size=holdid.size();
	while(i<size){
	ahtml=oggFileCheck(oggnum[holdid[i]]);
	inTxt += sep + zhuyinstr[holdid[i]]+"\n<br>"+pinyin[holdid[i]]+"\n<br>" + ahtml;
	inTxt += def[holdid[i]] + "\n";
	i++;
	}

	QString sep2="\n\n<hr>\n";
	inTxt += sep2 + trans;

	inTxt+="</body></html>";
	deftxt->setHtml(inTxt);
	glayout->addWidget(deftxt);
	this->setLayout(glayout);

}



/*-----------------------------------------------------
 * pre:
 * post:
 * takes the definition results (from "select * from def
 * where hold_id=<hold_id>") in the sqlitedb object
 * and returns a single formatted string
------------------------------------------------------*/
QString defpage::def2Str(sqlitedb *db){
/*
tbldef["id"]=0;
tbldef["hold_id"]=1;
tbldef["type"]=2;
tbldef["def"]=3;
tbldef["example"]=4;
tbldef["quote"]=5;
tbldef["short"]=6;
tbldef["like"]=7;
tbldef["anto"]=8;
*/
QString rtrn="";
    if(!db->result.isValid()){
    return rtrn;
    }
QHash<QString,QString> defs;
QHash<QString,int> nums;
QString type;
QString def;
QString example;
QString quote;
QString shrt;
QString like;
QString anto;
QString defnum;
    while(db->result.isValid()){
    type=db->result.value(tbldef["type"]).toString();
    def=db->result.value(tbldef["def"]).toString();
    example=preBrTb(db->result.value(tbldef["example"]).toString());
    quote=preBrTb(db->result.value(tbldef["quote"]).toString());
    shrt=preBrTb(db->result.value(tbldef["short"]).toString());
    like=preBrTb(db->result.value(tbldef["like"]).toString());
    anto=preBrTb(db->result.value(tbldef["anto"]).toString());
        if(def.size()>=1){
        nums[type]++;
        //defnum=QString::number(nums[type]) + ") " +def;
        defnum="<li>" +def;
        }
        else{
        defnum="";
        }
    defs[type]+=defnum + example + quote  + shrt  + like  + anto + "\n<br>\n<br>";
    db->next();
    }
QList<QString> keys=defs.keys();

int i=0;
int size=keys.size();
QString typestr;
QString typestrend;
    while(i<size){
        if(keys[i].size()>0){
        typestr="["+keys[i]+"]\n<br><ol>";
        typestrend="</ol>";
        }
        else{
        typestr="<ol>";
        typestrend="</ol>";
        }
    rtrn+=typestr+defs[keys[i]]+typestrend;
    i++;
    }
return rtrn;
}

QString defpage::preBrTb(QString str){
    if(str.size()<=0){
    return str;
    }
str.replace(QString("\n"), QString("\n\t"));
return "\n\t" + str;
}


void defpage::sgnRun(QUrl url){
int charid=url.toString().remove(0,protocol.count()).toInt();
//emit loadDefSgn(charid); a fix to try to generate a searching dialog when a link is clicked that failed. as even chained signals don't get drawn/ran until all the actions are done.
	if(charid>0){
	//emit loadDefSgn(charid); a fix to try to generate a searching dialog when a link is clicked that failed. as even chained signals don't get drawn/ran until all the actions are done.
	this->setup(charid);
	this->show();
	this->activateWindow();
	}
	else{ 
	};
}

//not being used anymore
void defpage::sgnLoadDef(int i){
this->setup(i);
this->show();
}



void defpage::keyPressEvent(QKeyEvent *e){
    if((e->key() == Qt::Key_C) && (e->modifiers().testFlag(Qt::ControlModifier))) {
    qApp->clipboard()->setText(deftxt->selectedText());
    }
}


/*-----------------------------------------------------
 * pre:
 * post:
 * class for zhuyinw widget. Generates the widget to
 * put into the zhuyin tab
------------------------------------------------------*/
class zhuyinw : public QWidget{
public:
    void setup();
};


/*-----------------------------------------------------
 * pre:
 * post:
 * populates the widget of thiis class with the things
 * it needs.
------------------------------------------------------*/
void zhuyinw::setup(){
QGridLayout *zyg=new QGridLayout(this);
QComboBox *cbzy1 = new QComboBox(this);
QComboBox *cbzy2 = new QComboBox(this);
QComboBox *cbzy3 = new QComboBox(this);
QComboBox *cbzya = new QComboBox(this);
loadLbl *ldlbl=new loadLbl;
ldlbl->setParent(this);

zyg->setAlignment(Qt::AlignHCenter);

// need button to call slot on clickTB to get value of comboboxes
//need clickTB to display search results.
// QObject::connect(QPushButton, SIGNAL(released()), clickTB, SLOT(findChars())); onclick, runs clickTB->findChars(); finds and displays results
//clickTB->findChars() to display in clickTB
//        QObject::connect(clickTB, SIGNAL(anchorClicked(QUrl)), defpage, SLOT(openWidget(QUrl))); Qurl->qstring->int

cbzy1->insertItem(-1, "*", -1);
cbzy2->insertItem(-1, "*", -1);
cbzy3->insertItem(-1, "*", -1);
cbzya->insertItem(-1, "*", -1);

cbzy1->insertItem(0, lbls["not"],0);
cbzy2->insertItem(0, lbls["not"],0);
cbzy3->insertItem(0, lbls["not"],0);

list2Combo(zy1, cbzy1);
list2Combo(zy2, cbzy2);
list2Combo(zy3, cbzy3);
list2Combo(zya, cbzya);

QPushButton *pbSubmit = new QPushButton(this);
pbSubmit->setText(lbls["search"]);

QWebEngineView *tbRes = new QWebEngineView();
clickTBP *tbResV = new clickTBP(cbzy1, cbzy2,cbzy3,cbzya, ldlbl);
tbRes->setPage(tbResV);
tbResV->setParent(tbRes);


zyg->addWidget(cbzy1,0,0,Qt::AlignTop);
zyg->addWidget(cbzy2,0,1,Qt::AlignTop);
zyg->addWidget(cbzy3,0,2,Qt::AlignTop);
zyg->addWidget(cbzya,0,3,Qt::AlignTop);

zyg->addWidget(ldlbl,1,0,Qt::AlignTop);
zyg->addWidget(pbSubmit,1,3,Qt::AlignTop);



zyg->addWidget(tbRes,2,0,1,4,Qt::AlignTop| Qt::AlignHCenter);
//connect button to clickTB slot to call function to get zhuyin from comboboxes then run sql query
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbResV, SLOT(sgnRun()));
//QObject::connect(tbRes, SIGNAL(clicked()), ldlbl, SLOT(setLoad()));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
QObject::connect(tbRes->page(), SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setClear(QUrl)));

this->setLayout(zyg);
}

clickTB::clickTB(){
this->setMinimumSize(740,320);
}

bool clickTBP::acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool){
    if (type == QWebEnginePage::NavigationTypeLinkClicked)
    {
        qWarning() << "url: " + url.toString();
        emit linkClicked(url);
        return false;
    }
    return true;
}



clickTBP::clickTBP(QComboBox *cb1, QComboBox *cb2, QComboBox *cb3, QComboBox *cb4,loadLbl *ldlbl){
//this->setOpenLinks(false);

//this->setMinimumSize(740,320);
//this->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOn);
//this->page()->setLinkDelegationPolicy(QWebEnginePage::DelegateAllLinks);

cbzy1=cb1;
cbzy2=cb2;
cbzy3=cb3;
cbzya=cb4;
llbl=ldlbl;
}

clickTBP::clickTBP(QLineEdit *lineedit, loadLbl *ldlbl){
//this->setOpenLinks(false);

//this->setMinimumSize(740,300);
//this->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOn);
//this->page()->setLinkDelegationPolicy(QWebEnginePage::DelegateAllLinks);

llbl=ldlbl;
qle=lineedit;
}


//allows to set a width and height
//for learn tab
//clickTBP::clickTBP(int minw,int minh, int maxw, int maxh){
clickTBP::clickTBP(){
//this->setOpenLinks(false);
/*
this->setMinimumSize(minw,minh);
this->setMaximumSize(maxw,maxh);
this->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAsNeeded);
this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
*/
}


void clickTBP::sgnRun(){
this->setHtml(db2Str(sqlStr()));
llbl->setClear();
}

//signal to get and fill random word
void clickTBP::sgnRnd(){
QHash<int,QString> word=rndWord(dict->itemData(dict->currentIndex()).toString());
QString rtrn="<a href=\"" + protocol + QString::number(word.begin().key()) + "\"><span>" + word.begin().value() + "</span></a> ";
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
qle->setText(word.begin().value());
this->setHtml(rtrn);
}

//add to pList AND database (run lrnTB::add2PList(QString word) and then reload this clickTB
void clickTBP::sgnAddPDict(){
	if(add2PList(qle->text())){
	this->llbl->setText("[" +addWord + "] " + lbls["added"]);
	}
	else{
	this->llbl->setText("[" +addWord + "] " + lbls["notAdded"]);
	}
//add to pList and db
	loadPDict();
//render pList
this->setHtml(pList2Html());
}

//add to pList AND database (run lrnTB::add2PList(QString word) and then reload this clickTB
void clickTBP::sgnDelPDict(QUrl url){
QString urlstr=url.toString().remove(0,protocol.count());
int charid=urlstr.toInt();
	if(charid<=0){
	//this is to check if the url has "d:" in front of it as that's a hack to determine an action from just the QUrl
		if(urlstr[0]=='d' && urlstr[1]==':'){
		urlstr.remove(0,2);
		delFrmPList(urlstr);
		this->llbl->setText("[" + addWord + "] " + lbls["del"]);
		loadPDict();
		this->setHtml(pList2Html());
		}
	}
}



/*-------------------------
pre: pList existing. pListNum existing, lrnTb existing
post: lrnTb->myList 
takes pList (a QHash<QString,int>) and makes html out of it so it can be used for setHtml
-------------------------*/
QString clickTBP::pList2Html(){
QString rtrn="";
QList<QString> keys=pListOrd.keys();
int pMax=pListId.count();
int i=0;

	while(i<pMax){
	rtrn+="<a href=\"" + protocol + QString::number(pListId[i]) + "\"><span>" + pListChar[i] + "</span></a> <a href=\"" + protocol + "d:" + pListChar[i] + "\"><span>[Delete]</span></a><br>";
	i++;
	}

rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;
}


QString clickTBP::db2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlstr);
QString rtrn="";
QString curzy="";
QString temp="";
QString head="";
QString top="";
        while(db->result.isValid()){
        // select c.id, c.char, h.zy_1, h.zy_2, h.zy_3, h.accent, h.zhuyin from hold as h left join char as c on h.char_id=c.id
        temp=num2zy[db->result.value(2).toInt()] + num2zy[db->result.value(3).toInt()] + num2zy[db->result.value(4).toInt()] + num2zya[db->result.value(5).toInt()];
        //temp=db->result.value(2).toString() + " " + db->result.value(3).toString() + " " +db->result.value(4).toString() + " " +db->result.value(5).toString();
        if(curzy!=temp){
        curzy=temp;
		head=top + "[" + temp + "] ";
                        if(top==""){
                        top="<br>";
                        }
        }
        else{
        head="";
        }
      rtrn+= head + "<a href=\"" + protocol + db->result.value(0).toString() + "\"><span>" + db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;
}


QString clickTBP::sqlStr(){
QString rtrn;
//CREATE TABLE hold(id int primary key, num int, pinyin text, char_id int, zy_1 int, zy_2 int, zy_3 int, accent int, zhuyin text);
//CREATE TABLE char(id int primary key, char text, unum text, rad_id int, strokes int, strk_rad int);
QString sqlstr="select c.id, c.char, h.zy_1, h.zy_2, h.zy_3, h.accent, h.zhuyin from hold as h left join char as c on h.char_id=c.id";
QString sqlsort=" order by h.zy_1, h.zy_2, h.zy_3, h.accent";
QString where[4];

    if(cbzy1->itemData(cbzy1->currentIndex())==0){
    where[0]="zy_1 IS NULL ";
    }
    else if(cbzy1->itemData(cbzy1->currentIndex())==-1){
    where[0]="";
    }
    else{
    where[0]= "zy_1=" + cbzy1->itemData(cbzy1->currentIndex()).toString() + " ";
    }

    if(cbzy2->itemData(cbzy2->currentIndex())==0){
    where[1]="zy_2 IS NULL ";
    }
    else if(cbzy2->itemData(cbzy2->currentIndex())==-1){
    where[1]="";
    }
    else{
    where[1]= "zy_2=" + QString::number(zy2num[cbzy2->currentText()]) + " ";
    }

    if(cbzy3->itemData(cbzy3->currentIndex())==0){
    where[2]="zy_3 IS NULL ";
    }
    else if(cbzy3->itemData(cbzy3->currentIndex())==-1){
    where[2]="";
    }
    else{
    where[2]= "zy_3=" + QString::number(zy2num[cbzy3->currentText()]) + " ";
    }


    if(cbzya->currentIndex()==0){
    where[3]="";
    }
    else{
    where[3]= "accent=" + QString::number(zya2num[cbzya->currentText()]) + " ";
    }

QString wherestr;
int i=0;
int size=4;
    while(i<size){
        if(wherestr==""&&where[i]!=""){
        wherestr=" where " + where[i];
        }
        else if(where[i]!=""){
        wherestr+="and " + where[i];
        }
    i++;
    }

rtrn=sqlstr + wherestr + sqlsort;
return rtrn;
}



/*-----------------------------------------------------
pre:
post;
class for generating a widget for the radicals tab
-----------------------------------------------------*/
class radw : public QWidget{
public:
    void setup();
};






/*----------------------------------------------------
pre:
post:
Sets up the widget for the radicals tab
----------------------------------------------------*/
void radw::setup(){
QGridLayout *radg=new QGridLayout(this);
QComboBox *cbrad = new QComboBox(this);
QComboBox *cblgeq = new QComboBox(this);
QComboBox *cbstrk = new QComboBox(this);
clickComboBox *cbnum = new clickComboBox;
cbnum->setParent(this);
loadLbl *ldlbl = new loadLbl;
ldlbl->setParent(this);

cbrad->insertItem(0,lbls["stroke"], 0);
list2ComboRad(rad, radstrk, cbrad);

radg->setAlignment(Qt::AlignHCenter);


cblgeq->addItem("=", 0);
cblgeq->addItem("~", 1);
cblgeq->addItem("<", 2);
cblgeq->addItem("<=", 3);


int i=0;
int size=36;
    while(i<=size){
    cbstrk->addItem(QString::number(i),i);
    i++;
    }

i=0;
    while(i<=size){
    cbnum->addItem(QString::number(i),i);
    i++;
    }

cbnum->hide();


QPushButton *pbSubmit = new QPushButton(this);
pbSubmit->setText(lbls["search"]);

clickTB *tbRes = new clickTB();
clickTBP *tbResV = new clickTBP(cbrad,cbstrk,cblgeq,cbnum, ldlbl);
tbRes->setPage(tbResV);
tbResV->setParent(this);

radg->addWidget(cbrad,0,0,Qt::AlignTop);
radg->addWidget(cbnum,0,1,Qt::AlignTop);
radg->addWidget(cblgeq,0,2,Qt::AlignTop);
radg->addWidget(cbstrk,0,3,Qt::AlignTop);

radg->addWidget(ldlbl,1,0,Qt::AlignTop);
radg->addWidget(pbSubmit,1,3,Qt::AlignTop);

radg->addWidget(tbRes,2,0,1,4,Qt::AlignTop | Qt::AlignHCenter);



QObject::connect(cblgeq, SIGNAL(currentIndexChanged(QString)), cbnum, SLOT(sgnHideShow(QString)));
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbResV, SLOT(sgnRadRun()));
//QObject::connect(tbRes, SIGNAL(clicked()), ldlbl, SLOT(setLoad()));
QObject::connect(tbResV, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setClear(QUrl)));

this->setLayout(radg);
}


void clickTBP::sgnRadRun(){
QString sqlq=radSqlStr();
this->setHtml(radDb2Str(sqlq));
llbl->setClear();
}

QString clickTBP::radDb2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlstr);
//select id, char, rad_id, strokes, strk_rad from char where strokes=1 order by strokes
//select id, char, rad_id, strokes, strk_rad from char where strokes>1 and strokes<1 order by strokes
//select id, char, rad_id, strokes, strk_rad from char where rad_id=18 and strk_rad=1 order by strokes
//select id, char, rad_id, strokes, strk_rad from char where rad_id=112 and strk_rad>1 and strk_rad<10 order by strokes

int israd=1;
        if(cbzy1->currentText()==lbls["stroke"]){
        israd=0;
        }
QString rtrn="";
QString cursec="";
QString temp="";
QString head="";
QString subhead="";
int subheadind=0;
QString top="";

    while(db->result.isValid()){
        if(israd==1){
        //select id, char, rad_id, strokes, strk_rad from char where strokes=1 order by strokes
        //select id, char, rad_id, strokes, strk_rad from char where strokes>1 and strokes<1 order by strokes
        temp=rad.at(db->result.value(2).toInt())+"+"+db->result.value(4).toString();
        }
        else{
                temp=db->result.value(3).toString();
        }
        if(cursec!=temp){
        cursec=temp;
        head=top+"[" + temp + "] ";
            if(top==""){
            top="<br>";
            }
        }
        else{
        head="";
        }
        if(israd==0&&subheadind!=db->result.value(2).toInt()){
        subheadind=db->result.value(2).toInt();
        subhead="    ("+rad.at(db->result.value(2).toInt())+") ";
        }
        else{
        subhead="";
        }
    rtrn+= head + subhead + "<a href=\"" + protocol + db->result.value(0).toString() + "\"><span>" + db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;
}



QString clickTBP::radSqlStr(){
//clickTB *tbRes = new clickTB(cbrad,cbstrk,cblgeq,cbnum);

///QString qsrad = cbzy1->currentText(); // cbrad
QString qsrad = cbzy1->itemData(cbzy1->currentIndex()).toString(); // cbrad
QString qsstrk = cbzy2->currentText(); // cbstrk
QString qslgeq = cbzy3->currentText(); // cblgeq
QString qsnum = cbzya->currentText(); // cbnum

QString rtrn;

QString whrcnd;
QString where;
//CREATE TABLE char(id int primary key, char text, unum text, rad_id int, strokes int, strk_rad int);
// select id, char, rad_id, strokes, strk_rad from char where (rad_id="<some rad id>" and strk_rad and strk_rad)|(strokes and stroke)"
//2017-05-27 changing over from currentIndex or currentText to itemData for something more useul //jabbwock
    if(qsrad=="0"){
    whrcnd="strokes";
    }
    else{
    whrcnd="strk_rad";
    where+="rad_id=" + qsrad;
    }

    if(where!=""){
    where+=" and ";
    }

    if(qslgeq=="="){
    where+=whrcnd + qslgeq + qsstrk;
    }
    else if(qslgeq=="~"){
    int num=qsstrk.toInt();
    QString genum=QString::number(num+1);
    QString lenum=QString::number(num-1);
    where+=whrcnd + ">=" + lenum + " and " + whrcnd + "<=" + genum;
    }
    else if(qslgeq=="<"){
        where+=whrcnd + ">" + qsnum + " and " + whrcnd + "<" + qsstrk;
    }
    else if(qslgeq=="<="){
        where+=whrcnd + ">=" + qsnum + " and " + whrcnd + "<=" + qsstrk;
    }
rtrn="select id, char, rad_id, strokes, strk_rad from char where "+ where + " order by strokes, rad_id";
return rtrn;
}

/*
void clickTBP::mousePressEvent(QMouseEvent *event){
emit clicked();
}
*/

QString clickTBP::freeSqlStr(){
QString word=qle->text();
QString sqlq="select id, char from char where char LIKE \"%" + word + "%\" order by rad_id desc, id asc";
return sqlq;
}

QString clickTBP::freeDb2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlstr);
QString rtrn="";
QString head="";
        while(db->result.isValid()){
    rtrn+= head + "<a href=\"" + protocol + db->result.value(0).toString() + "\" class=\"free\"><span>" + db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;

}


QString clickTBP::engSqlStr(){
QString word=qle->text();
QString sqlq="select c.id, c.char, t.def from translation as t left join char as c on t.char_id=c.id where t.lang=\"English\" and lower(def) LIKE \"%" + word + "%\" order by length(def),c.id asc";
return sqlq;
}

QString clickTBP::engDb2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlstr);
QString rtrn="";
QString head="";
        while(db->result.isValid()){
    rtrn+= head + "<a href=\"" + protocol + db->result.value(0).toString() + "\" class=\"eng\"><span>[" + db->result.value(2).toString() + "] "+ db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;

}

void clickTBP::sgnFreeRun(){
QString sqlq=freeSqlStr();
this->setHtml(freeDb2Str(sqlq));
llbl->setClear();
}

void clickTBP::sgnEngRun(){
QString sqlq=engSqlStr();
this->setHtml(engDb2Str(sqlq));
llbl->setClear();
}


void clickComboBox::sgnHideShow(QString str){
    if(str=="="||str=="~"){
    this->hide();
    return;
    }
this->show();
}

void loadLbl::setLoad(){
this->setText(lbls["searching"]);
}
void loadLbl::setLoad(QUrl url){
this->setText(lbls["searching"]);
}
void loadLbl::setClear(){
this->setText("");
}
void loadLbl::setClear(QUrl url){
this->setText("");
}
void loadLbl::sgnAdded(){
this->setText(lbls["added"]);
}


/*-----------------------------------------------------
 *pre:
 *post:
 * reads the css, erhm... qss file into a qstring and
 * returns it
------------------------------------------------------*/
QString readStyle(){
 QFile File(assetpath + "mae-moedict/stylesheet.qss");
  File.open(QFile::ReadOnly);
return QLatin1String(File.readAll());
}


QString readCSS(){
 QFile File(assetpath + "mae-moedict/stylesheet.css");
  File.open(QFile::ReadOnly);
return QLatin1String(File.readAll());
}

QString readFile(QString fname){
 QFile File(fname);
  File.open(QFile::ReadOnly);
return QLatin1String(File.readAll());
}

/*----------------------------------------------------
pre:
post:
----------------------------------------------------*/
class freeSearch : public QWidget{
public:
    void setup();
};

/*-----------------------------------------------------
 * pre:
 * post:
 * populates the widget of thiis class with the things
 * it needs.
------------------------------------------------------*/
void freeSearch::setup(){
QGridLayout *zyg=new QGridLayout(this);
zyg->setAlignment(Qt::AlignHCenter);

QLineEdit *leIn = new QLineEdit(this);
loadLbl *ldlbl=new loadLbl;
ldlbl->setParent(this);

leIn->setMinimumSize(200,50);


QPushButton *pbSubmit = new QPushButton(this);
pbSubmit->setFixedWidth(200);
pbSubmit->setText(lbls["search"]);

clickTB *tbRes = new clickTB();
clickTBP *tbResV = new clickTBP(leIn, ldlbl);
tbRes->setPage(tbResV);
tbResV->setParent(this);


zyg->addWidget(leIn,0,0,Qt::AlignTop | Qt::AlignLeft);
zyg->addWidget(ldlbl,1,0,Qt::AlignTop);
zyg->addWidget(pbSubmit,1,3,Qt::AlignTop | Qt::AlignRight);
zyg->addWidget(tbRes,2,0,1,4,Qt::AlignBottom);

//connect button to clickTB slot to call function to get zhuyin from comboboxes then run sql query
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbResV, SLOT(sgnFreeRun()));
//QObject::connect(tbRes, SIGNAL(clicked()), ldlbl, SLOT(setLoad()));
QObject::connect(tbResV, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));

this->setLayout(zyg);
}


/*----------------------------------------------------
pre:
post:
----------------------------------------------------*/
class engw : public QWidget{
public:
    void setup();
};

/*-----------------------------------------------------
 * pre:
 * post:
 * populates the widget of thiis class with the things
 * it needs.
------------------------------------------------------*/
void engw::setup(){
QGridLayout *zyg=new QGridLayout(this);
zyg->setAlignment(Qt::AlignHCenter);

QLineEdit *leIn = new QLineEdit(this);
loadLbl *ldlbl=new loadLbl;
ldlbl->setParent(this);

leIn->setMinimumSize(200,50);


QPushButton *pbSubmit = new QPushButton(this);
pbSubmit->setFixedWidth(200);
pbSubmit->setText(lbls["search"]);

clickTB *tbRes = new clickTB();
clickTBP *tbResV = new clickTBP(leIn, ldlbl);
tbRes->setPage(tbResV);
tbResV->setParent(this);


zyg->addWidget(leIn,0,0,Qt::AlignTop | Qt::AlignLeft);
zyg->addWidget(ldlbl,1,0,Qt::AlignTop);
zyg->addWidget(pbSubmit,1,3,Qt::AlignTop | Qt::AlignRight);
zyg->addWidget(tbRes,2,0,1,4,Qt::AlignBottom);

//connect button to clickTB slot to call function to get zhuyin from comboboxes then run sql query
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbResV, SLOT(sgnEngRun()));

//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setLoad(QUrl)));
QObject::connect(tbResV, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
//QObject::connect(dp, SIGNAL(loadDefSgn(int)), dp, SLOT(sgnLoadDef(int)));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setClear(QUrl)));

this->setLayout(zyg);
}


/*-----------------------------------------------------
 * pre:
 * post:
 * populates the widget of thiis class with the things
 * it needs. 
------------------------------------------------------*/
void lrnTb::setup(){
QGridLayout *mnGrd=new QGridLayout(this);
mnGrd->setAlignment(Qt::AlignHCenter);

pDbExists=chckPersDict();

QComboBox *wDB=new QComboBox(this);
wDB->insertItem(0,lbls["all"]+lbls["dict"],"all");
	if(pDbExists){
	wDB->insertItem(1,lbls["pers"]+lbls["dict"],"personal");
	}

ldlbl=new loadLbl;//label to confirm adding of word
ldlbl->setText("");

//ComboBox for which dictionary
//rnd.append("<a class=\"rnd\" href=\"#\">"+lbls["rand"]+"</a>");
//pList.append("list1");
QWebEngineView *rndChar= new QWebEngineView();
clickTBP *rndCharV = new clickTBP();
rndChar->setPage(rndCharV);
rndCharV->setParent(rndChar);

QWebEngineView *myList=new QWebEngineView();
clickTBP *myListV=new clickTBP();
myList->setPage(myListV);
myListV->setParent(myList);

myListV->llbl=ldlbl;
	if(pDbExists){
	loadPDict();
	myListV->setHtml(myListV->pList2Html());
	}

QPushButton *rndSubmit = new QPushButton(this);
rndSubmit->setText(lbls["search"]);


//GroupBox Random
QGroupBox *rndBox=new QGroupBox(lbls["rand"]);
rndBox->setMinimumWidth(390);
QGridLayout *rndBoxL=new QGridLayout(rndBox);
rndBox->setAlignment(Qt::AlignLeft);
QWidget *spacer=new QWidget(this);
spacer->setMinimumHeight(12);


rndBoxL->addWidget(spacer,0,0,1,3,Qt::AlignVCenter | Qt::AlignHCenter);
rndBoxL->addWidget(rndChar,1,0,2,1,Qt::AlignVCenter | Qt::AlignHCenter);
rndBoxL->addWidget(wDB,1,2,1,1,Qt::AlignVCenter | Qt::AlignRight);
rndBoxL->addWidget(rndSubmit,2,2,1,1,Qt::AlignVCenter | Qt::AlignRight);
rndBox->setLayout(rndBoxL);

QWidget *vspacer=new QWidget(this);
vspacer->setMinimumHeight(12);

//GroupBox Personal Dictionary
QGroupBox *persDictBox=new QGroupBox(lbls["pers"]+lbls["dict"]);
QGridLayout *persDictL=new QGridLayout(persDictBox);
persDictL->addWidget(vspacer,0,0,1,1,Qt::AlignVCenter | Qt::AlignHCenter);
persDictL->addWidget(myList,1,0,1,1,Qt::AlignVCenter | Qt::AlignHCenter);
persDictBox->setLayout(persDictL);

//controls for the personal
QGroupBox *persDictCtrlBox=new QGroupBox(lbls["pers"]+lbls["dict"]);
persDictCtrlBox->setMinimumWidth(390);
persDictCtrlBox->setMinimumHeight(330);

QGridLayout *persDictCtrlBoxL=new QGridLayout(persDictCtrlBox);
QWidget *persDictCtrlSpace=new QWidget(persDictCtrlBox);
persDictCtrlSpace->setMinimumHeight(12);
QWidget *persDictCtrlVSpace=new QWidget(this);
persDictCtrlVSpace->setMinimumHeight(260);
QLineEdit *add2PersDictLE = new QLineEdit(persDictCtrlBox);
add2PersDictLE->setMinimumSize(200,50);
QPushButton *add2PersDict = new QPushButton(persDictCtrlBox);
add2PersDict->setText(lbls["add"]);
ldlbl->setParent(persDictCtrlBox);

persDictCtrlBoxL->addWidget(persDictCtrlSpace,0,0,1,2,Qt::AlignTop | Qt::AlignLeft);
persDictCtrlBoxL->addWidget(add2PersDictLE,1,0,1,1,Qt::AlignTop | Qt::AlignHCenter);
persDictCtrlBoxL->addWidget(add2PersDict,1,1,1,1,Qt::AlignTop | Qt::AlignRight);
persDictCtrlBoxL->addWidget(ldlbl,2,0,1,1,Qt::AlignTop | Qt::AlignLeft);
persDictCtrlBoxL->addWidget(persDictCtrlVSpace,3,0,3,2,Qt::AlignTop | Qt::AlignHCenter);

persDictCtrlBox->setLayout(persDictCtrlBoxL);


mnGrd->addWidget(rndBox,0,0,1,1,Qt::AlignTop | Qt::AlignLeft);

	if(pDbExists){
	mnGrd->addWidget(persDictBox,0,1,2,1,Qt::AlignTop | Qt::AlignRight);
	mnGrd->addWidget(persDictCtrlBox,1,0,1,1,Qt::AlignTop | Qt::AlignJustify);
	}
	else{
	QWidget *wideSp=new QWidget(this);
	wideSp->setMinimumWidth(390);
	mnGrd->addWidget(wideSp,0,1,1,1,Qt::AlignTop | Qt::AlignRight);
	}

//QHash<int,QString> rtrn=this->rndWord("personal");
//qWarning() << "key: " << QString::number(rtrn.begin().key()) << ", word: " << rtrn.begin().value() <<endl;
rndCharV->dict=wDB; //set rndChar's instance of QComboBox with this object's 
//QComboBox of which dictionary selected. Because signal -> slot needs the instance to call it
rndCharV->qle=add2PersDictLE;
myListV->qle=add2PersDictLE;
QObject::connect(rndSubmit, SIGNAL(pressed()), rndCharV, SLOT(sgnRnd()));
QObject::connect(rndCharV, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
QObject::connect(add2PersDict, SIGNAL(pressed()), myListV, SLOT(sgnAddPDict()));
QObject::connect(myListV, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
QObject::connect(myListV, SIGNAL(linkClicked(QUrl)), myListV, SLOT(sgnDelPDict(QUrl)));

this->setLayout(mnGrd);
}



/*-----------------------------------------
pre: Qt functions
post: none 
tries to determine the path of the assets folder across all the different OS's
-------------------------------------------*/
QString confpath(){
QString confDirNm="/.mae-moedict";
QString assets="/assets/";
QString n900MyDocs="/MyDocs";
QString n900ext="/media/mmc1" + confDirNm;
QString cwd=QDir::currentPath();
QString homepath=QDir::homePath() + confDirNm + assets;
QDir dirobj(homepath);
	if(pathexists(homepath)){
	return homepath;
	}
	qWarning() << "homepath: "+homepath+" doesn't exist. Continuing to search for asset folder.";

homepath=QDir::homePath() + n900MyDocs + confDirNm + assets;
	if(pathexists(homepath)){
	return homepath;
	}
	qWarning() << "homepath: "+homepath+" doesn't exist. Continuing to search for asset folder.";

homepath=n900ext + assets;
	if(pathexists(homepath)){
	return homepath;
	}
	qWarning() << "external: "+homepath+" doesn't exist. Continuing to search for asset folder.";

homepath=cwd + assets;
	if(pathexists(homepath)){
	return homepath;
	}
	qWarning() << "current working directory: "+homepath+" doesn't exist. Exhausted all alternatives. Exitting...";

return "";
}


QString checkfont(QString fontp, int fonti){
QString exists="";
    if(ifexists(fontp)){
    exists="File does exists. Please check permissions.";
    }

    if(fonti<0){
    QString msg="Unable to read font at [" + fontp + "]. " + exists;
    return msg;
    }

return "";
}

/*---------------------------
pre: custom sqldb class previous written
post: program global lbls populated
populates the lbls with lbls in the DB as the windows version can't handle UTF text very well if it's written in the source code files
----------------------------*/
void loadlbls(){
QString sqlq="select eng,chn from lbls";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
        lbls[db->result.value(0).toString()]=db->result.value(1).toString();
        db->next();
        }
db->close();
}

/*-----------------------------------------------------
pre: the sqldb class and the database existing
post: the max filled
gets the last character 
-----------------------------------------------------*/
void getMax(){
QString sqlq="select id from char order by id desc limit 1";

sqlitedb *db=new sqlitedb(assetpath + dictDb);
db->query(sqlq);
        while(db->result.isValid()){
        charMax=db->result.value(0).toInt();
        db->next();
        }
db->close();

}

/*-----------------------------------------------------
 * pre:
 * post:
 * main function
-----------------------------------------------------*/
int main(int argc, char **argv)
{

QApplication app (argc, argv);
app.setApplicationName("mae-moedict");
//asset path
assetpath=confpath();
dictDb="db/mae-moedict.db";
pdictDb="db/personal.db";
//assetpath="/scratchbox/users/sleepingkirby/home/sleepingkirby/dev/mae-moedict/assets/";
    if(assetpath==""){
    QTextEdit *te=new QTextEdit();
    te->setText("All alternative paths for the asset folder exhausted. Can't find assets folder. Exitting...");
    te->resize(400, 100);
    te->show();
    app.exec();
    delete(te);
    return 0;
    }
qWarning() << "using assetpath "+assetpath;


// set font for chinese
QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
//QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
QString fontpath=assetpath + "fonts/DroidSansFallbackFull.ttf";

int fontId = QFontDatabase::addApplicationFont(fontpath);//only font that can do zhuyin and characters

//if font doesn't exists but assets folder does or permissons for it are messed up.
QString checkf=checkfont(fontpath, fontId);
    if(checkf!=""){
    QTextEdit *te=new QTextEdit();
    te->setText(checkf);
    te->resize(400, 100);
    te->show();
    app.exec();
    delete(te);
    return 0;
    }

QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
QFont font(msyh,10);
QApplication::setFont(font);

loadlbls();
//get max char id number
getMax();

QString about= "<html><head><style>" + readCSS() + "</style></head><body id=\"about\"><div>Compiled: 2017-10-05<br><a href=\"https://github.com/sleepingkirby/mae-moedict\">https://github.com/sleepingkirby/mae-moedict</a><br><br>If you find this program useful, please consider donation: <br>" + lbls["ifuseful"]+ " <br><br>Patreon: <a href=\"https://www.patreon.com/wklaume\">https://www.patreon.com/wklaume</a></div><div>PayPal: <a href=\"https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3EE2P5RCJ6V9S\">https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3EE2P5RCJ6V9S</a></div><br><br>All suggestions/bug reports are welcome. <br>" + lbls["bugsugg"] + "</body></html>";

//tab headers
QString tabzhuyin=lbls["zhuyin"];
QString tabstroke=lbls["stroke"];
QString tabrad=lbls["rad"];
QString tabdb=lbls["free"];
QString tabeng=lbls["eng"];
QString tababt="About";
QString tabLearn=lbls["learn"];

//load global zhuyin
loadZhuyin();
loadZy2Num();
loadZya2Num();
loadNum2Zy();
loadNum2Zya();
//load global radical
loadRad();
loadRad2Num();
//init global defpage;
dp=new defpage();
//init load label


QWidget *pWindow = new QWidget;
pWindow->setWindowTitle("mae-moedict");
QGridLayout *mainLayout = new QGridLayout;
QTabWidget *maintw = new QTabWidget();

//widget for zhuyin tab
zhuyinw *zy_w=new zhuyinw();
zy_w->setup();

//widget for radical tab
radw *rdw=new radw();
rdw->setup();

//widget for free search
freeSearch *frees=new freeSearch();
frees->setup();

//widet for English ssearch
engw *eng= new engw();
eng->setup();

//about tab
QWidget *aboutw=new QWidget();
QGridLayout *aboutGL = new QGridLayout();
clickTB *aboutTB = new clickTB();
aboutTB->setHtml(about);
aboutTB->setMinimumSize(780,400);
aboutGL->addWidget(aboutTB,0,0,Qt::AlignCenter);
aboutw->setLayout(aboutGL);

//learn tab (random and personal dicitonary
lrnTb *lrn=new lrnTb();
lrn->setup();

maintw->addTab(zy_w, tabzhuyin);
maintw->addTab(rdw, tabstroke + "/" + tabrad);
maintw->addTab(frees, tabdb);
maintw->addTab(eng, tabeng);
maintw->addTab(lrn, tabLearn);
maintw->addTab(aboutw, tababt);

mainLayout->addWidget(maintw,0,0, Qt::AlignCenter);

pWindow->setLayout(mainLayout);
qApp->setStyleSheet(readStyle());
pWindow->setVisible(true);
int i=app.exec();

delete(zy_w);
delete(rdw);
delete(frees);
delete(eng);
delete(aboutTB);
delete(lrn);
delete(dp);
return i;
}
