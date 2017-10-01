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
#include <QWebFrame>
#include <QKeyEvent>
#include <QDir>
#include "./main.h"

//asset folder path
QString assetpath;

//global zhuyin arrays
QList<QString> zy1;
QList<QString> zy2;
QList<QString> zy3;
QList<QString> zya;
QHash<QString,int> zy2num;
QHash<QString,int> zya2num;
QHash<int,QString> num2zy;
QHash<int,QString> num2zya;
QHash<QString,QString> lbls;

//global rad array
QList<QString> rad;
QList<int> radstrk;
QHash<QString, int> rad2num;

//global defpage
defpage *dp;


void loadRad2Num(){
QString sqlq="select char,id from radical order by id";

sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlq);
        while(db->result.isValid()){
	rad2num[db->result.value(0).toString()]=db->result.value(1).toInt();
        db->next();
	}
db->close();
}



void loadNum2Zy(){
QString sqlq="select id,char from zhuyin order by id";

sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlq);
        while(db->result.isValid()){
	num2zy[db->result.value(0).toInt()]=db->result.value(1).toString();
        db->next();
	}
db->close();
}

void loadNum2Zya(){
QString sqlq="select id, char from zhuyinTone order by id";

sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlq);
        while(db->result.isValid()){
	num2zya[db->result.value(0).toInt()]=db->result.value(1).toString();
        db->next();
	}
db->close();
}

void loadZy2Num(){
QString sqlq="select char, id from zhuyin order by id";

sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlq);
        while(db->result.isValid()){
	zy2num[db->result.value(0).toString()]=db->result.value(1).toInt();
        db->next();
	}
db->close();
}

void loadZya2Num(){
QString sqlq="select char, id from zhuyinTone order by id";

sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlq);
        while(db->result.isValid()){
	zya2num[db->result.value(0).toString()]=db->result.value(1).toInt();
        db->next();
	}
db->close();
}

void list2Combo(QList<QString> &ls, QComboBox *cb){
int cbi=cb->count();
int i=1;
int size=ls.count();
//qDebug() << "---------------------------";
	while(i<size){
//	qDebug() << i << ": " << ls[i];
	cb->insertItem(cbi,ls[i],i);
	cbi++;
	i++;
	}
}

void list2ComboRad(QList<QString> &ls, QList<int> &lsi, QComboBox *cb){
int cbi=cb->count();
int i=1;
int size=ls.count();
//qDebug() << "---------------------------";
	while(i<size){
//	qDebug() << "ls["<< i << "]: " << ls[i] << ", "<< lsi[i];
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
sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
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
}


void loadRad(){
int i=1;
rad.append("");//qlists have to have an index zero or it'll crash. 
radstrk.append(0);//qlists have to have an index zero or it'll crash. 
sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query("select char,stroke from radical order by id");
        while(db->result.isValid()){
        rad.insert(i,db->result.value(0).toString());
        radstrk.insert(i,db->result.value(1).toInt());
	//qDebug() << "i) rad: "<<rad[i]<<", strk: "<<radstrk[i];
        i++;
        db->next();
        }

db->close();
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
        deftxt = new QWebView(this);

        //deftxt->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        //deftxt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	deftxt->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOn);
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
	sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
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
	inTxt += "<h1 id=\"char\">" + chr + "</h1>\n\n[" + rad[rad_id] + "] " + lbls["part"] + " + " + QString::number(strk_rad) + " = [" + QString::number(strokes) + "] " + lbls["hua"] + "\n\n";
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
int charid=url.toString().toInt();
//emit loadDefSgn(charid); a fix to try to generate a searching dialog when a link is clicked that failed. as even chained signals don't get drawn/ran until all the actions are done.
this->setup(charid);
this->show();
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

clickTB *tbRes = new clickTB(cbzy1, cbzy2,cbzy3,cbzya, ldlbl);
tbRes->setParent(this);


zyg->addWidget(cbzy1,0,0,Qt::AlignTop);
zyg->addWidget(cbzy2,0,1,Qt::AlignTop);
zyg->addWidget(cbzy3,0,2,Qt::AlignTop);
zyg->addWidget(cbzya,0,3,Qt::AlignTop);

zyg->addWidget(ldlbl,1,0,Qt::AlignTop);
zyg->addWidget(pbSubmit,1,3,Qt::AlignTop);



zyg->addWidget(tbRes,2,0,1,4,Qt::AlignTop| Qt::AlignHCenter);
//connect button to clickTB slot to call function to get zhuyin from comboboxes then run sql query
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbRes, SLOT(sgnRun()));
//QObject::connect(tbRes, SIGNAL(clicked()), ldlbl, SLOT(setLoad()));
QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setClear(QUrl)));

this->setLayout(zyg);
}


clickTB::clickTB(QComboBox *cb1, QComboBox *cb2, QComboBox *cb3, QComboBox *cb4,loadLbl *ldlbl){
//this->setOpenLinks(false);

this->setMinimumSize(740,320);
this->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOn);
this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	
cbzy1=cb1;
cbzy2=cb2;
cbzy3=cb3;
cbzya=cb4;
llbl=ldlbl;
}

clickTB::clickTB(QLineEdit *lineedit, loadLbl *ldlbl){
//this->setOpenLinks(false);

this->setMinimumSize(740,300);
this->page()->currentFrame()->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOn);
this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

llbl=ldlbl;
qle=lineedit;	
}

void clickTB::sgnRun(){
this->setHtml(db2Str(sqlStr()));
llbl->setClear();
}

QString clickTB::db2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlstr);
QString rtrn="";
QString curzy="";
QString temp="";
QString head="";
        while(db->result.isValid()){
		// select c.id, c.char, h.zy_1, h.zy_2, h.zy_3, h.accent, h.zhuyin from hold as h left join char as c on h.char_id=c.id
		temp=num2zy[db->result.value(2).toInt()] + num2zy[db->result.value(3).toInt()] + num2zy[db->result.value(4).toInt()] + num2zya[db->result.value(5).toInt()];
		//temp=db->result.value(2).toString() + " " + db->result.value(3).toString() + " " +db->result.value(4).toString() + " " +db->result.value(5).toString();
		if(curzy!=temp){
		curzy=temp;
		head="[" + temp + "] ";
		}
		else{
		head="";
		}
	  rtrn+= head + "<a href=\"" + db->result.value(0).toString() + "\"><span>" + db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;
}


QString clickTB::sqlStr(){
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

clickTB *tbRes = new clickTB(cbrad,cbstrk,cblgeq,cbnum, ldlbl);
tbRes->setParent(this);

radg->addWidget(cbrad,0,0,Qt::AlignTop);
radg->addWidget(cbnum,0,1,Qt::AlignTop);
radg->addWidget(cblgeq,0,2,Qt::AlignTop);
radg->addWidget(cbstrk,0,3,Qt::AlignTop);

radg->addWidget(ldlbl,1,0,Qt::AlignTop);
radg->addWidget(pbSubmit,1,3,Qt::AlignTop);

radg->addWidget(tbRes,2,0,1,4,Qt::AlignTop | Qt::AlignHCenter);



QObject::connect(cblgeq, SIGNAL(currentIndexChanged(QString)), cbnum, SLOT(sgnHideShow(QString)));
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbRes, SLOT(sgnRadRun()));
//QObject::connect(tbRes, SIGNAL(clicked()), ldlbl, SLOT(setLoad()));
QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setClear(QUrl)));

this->setLayout(radg);
}


void clickTB::sgnRadRun(){
QString sqlq=radSqlStr();
this->setHtml(radDb2Str(sqlq));
llbl->setClear();
}

QString clickTB::radDb2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
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
	rtrn+= head + subhead + "<a href=\"" + db->result.value(0).toString() + "\"><span>" + db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;
}



QString clickTB::radSqlStr(){
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

void clickTB::mousePressEvent(QMouseEvent *event){
emit clicked();
}

QString clickTB::freeSqlStr(){
QString word=qle->text();
QString sqlq="select id, char from char where char LIKE \"%" + word + "%\" order by rad_id desc, id asc";
return sqlq;
}

QString clickTB::freeDb2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlstr);
QString rtrn="";
QString head="";
        while(db->result.isValid()){
	rtrn+= head + "<a href=\"" + db->result.value(0).toString() + "\" class=\"free\"><span>" + db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;

}


QString clickTB::engSqlStr(){
QString word=qle->text();
QString sqlq="select c.id, c.char, t.def from translation as t left join char as c on t.char_id=c.id where t.lang=\"English\" and lower(def) LIKE \"%" + word + "%\" order by length(def),c.id asc";
return sqlq;
}

QString clickTB::engDb2Str(QString sqlstr){
sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlstr);
QString rtrn="";
QString head="";
        while(db->result.isValid()){
	rtrn+= head + "<a href=\"" + db->result.value(0).toString() + "\" class=\"eng\"><span>[" + db->result.value(2).toString() + "] "+ db->result.value(1).toString() + "</span></a> ";
        db->next();
        }
db->close();
rtrn="<html><head><style>" + readCSS() + "</style></head><body>" + rtrn + "</body></html>";
return rtrn;

}

void clickTB::sgnFreeRun(){
QString sqlq=freeSqlStr();
this->setHtml(freeDb2Str(sqlq));
llbl->setClear();
}

void clickTB::sgnEngRun(){
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

clickTB *tbRes = new clickTB(leIn, ldlbl);
tbRes->setParent(this);


zyg->addWidget(leIn,0,0,Qt::AlignTop | Qt::AlignLeft);
zyg->addWidget(ldlbl,1,0,Qt::AlignTop);
zyg->addWidget(pbSubmit,1,3,Qt::AlignTop | Qt::AlignRight);
zyg->addWidget(tbRes,2,0,1,4,Qt::AlignBottom);

//connect button to clickTB slot to call function to get zhuyin from comboboxes then run sql query
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbRes, SLOT(sgnFreeRun()));
//QObject::connect(tbRes, SIGNAL(clicked()), ldlbl, SLOT(setLoad()));
QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));

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

clickTB *tbRes = new clickTB(leIn, ldlbl);
tbRes->setParent(this);


zyg->addWidget(leIn,0,0,Qt::AlignTop | Qt::AlignLeft);
zyg->addWidget(ldlbl,1,0,Qt::AlignTop);
zyg->addWidget(pbSubmit,1,3,Qt::AlignTop | Qt::AlignRight);
zyg->addWidget(tbRes,2,0,1,4,Qt::AlignBottom);

//connect button to clickTB slot to call function to get zhuyin from comboboxes then run sql query
QObject::connect(pbSubmit, SIGNAL(pressed()), ldlbl, SLOT(setLoad()));
QObject::connect(pbSubmit, SIGNAL(released()), tbRes, SLOT(sgnEngRun()));

//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setLoad(QUrl)));
QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), dp, SLOT(sgnRun(QUrl)));
//QObject::connect(dp, SIGNAL(loadDefSgn(int)), dp, SLOT(sgnLoadDef(int)));
//QObject::connect(tbRes, SIGNAL(linkClicked(QUrl)), ldlbl, SLOT(setClear(QUrl)));

this->setLayout(zyg);
}


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
	if(!pathexists(homepath)){
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


void loadlbls(){
QString sqlq="select eng,chn from lbls";

sqlitedb *db=new sqlitedb(assetpath + "db/mae-moedict.db");
db->query(sqlq);
        while(db->result.isValid()){
        lbls[db->result.value(0).toString()]=db->result.value(1).toString();
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
//assetpath="/scratchbox/users/sleepingkirby/home/sleepingkirby/dev/mae-moedict/assets/";


// set font for chinese
QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
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

QString about= "<html><head><style>" + readCSS() + "</style></head><body id=\"about\"><div>Compiled: 2017-09-30<br><a href=\"https://github.com/sleepingkirby/mae-moedict\">https://github.com/sleepingkirby/mae-moedict</a><br><br>If you find this program useful, please consider donation: <br>" + lbls["ifuseful"]+ " <br><br>Patreon: <a href=\"https://www.patreon.com/wklaume\">https://www.patreon.com/wklaume</a></div><div>PayPal: <a href=\"https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3EE2P5RCJ6V9S\">https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3EE2P5RCJ6V9S</a></div><br><br>All suggestions/bug reports are welcome. <br>" + lbls["bugsugg"] + "</body></html>";


//tab headers
QString tabzhuyin=lbls["zhuyin"];
QString tabstroke=lbls["stroke"];
QString tabrad=lbls["rad"];
QString tabdb=lbls["free"];
QString tabeng=lbls["eng"];
QString tababt="About";

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
QWebView *aboutTB = new QWebView();
aboutTB->setHtml(about);
aboutTB->setMinimumSize(780,400);
aboutGL->addWidget(aboutTB,0,0,Qt::AlignCenter);
aboutw->setLayout(aboutGL);

maintw->addTab(zy_w, tabzhuyin);
maintw->addTab(rdw, tabstroke + "/" + tabrad);
maintw->addTab(frees, tabdb);
maintw->addTab(eng, tabeng);
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
return i;
}
