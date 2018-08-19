/****************************************************************************
** Meta object code from reading C++ file 'main.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "main.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_loadLbl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      23,   19,    8,    8, 0x0a,
      37,    8,    8,    8, 0x0a,
      48,   19,    8,    8, 0x0a,
      63,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_loadLbl[] = {
    "loadLbl\0\0setLoad()\0url\0setLoad(QUrl)\0"
    "setClear()\0setClear(QUrl)\0sgnAdded()\0"
};

void loadLbl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        loadLbl *_t = static_cast<loadLbl *>(_o);
        switch (_id) {
        case 0: _t->setLoad(); break;
        case 1: _t->setLoad((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 2: _t->setClear(); break;
        case 3: _t->setClear((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 4: _t->sgnAdded(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData loadLbl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject loadLbl::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_loadLbl,
      qt_meta_data_loadLbl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &loadLbl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *loadLbl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *loadLbl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_loadLbl))
        return static_cast<void*>(const_cast< loadLbl*>(this));
    return QLabel::qt_metacast(_clname);
}

int loadLbl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
static const uint qt_meta_data_defpage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   27,    8,    8, 0x0a,
      46,   44,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_defpage[] = {
    "defpage\0\0i\0loadDefSgn(int)\0url\0"
    "sgnRun(QUrl)\0e\0keyPressEvent(QKeyEvent*)\0"
};

void defpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        defpage *_t = static_cast<defpage *>(_o);
        switch (_id) {
        case 0: _t->loadDefSgn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sgnRun((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        case 2: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData defpage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject defpage::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_defpage,
      qt_meta_data_defpage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &defpage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *defpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *defpage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_defpage))
        return static_cast<void*>(const_cast< defpage*>(this));
    return QDialog::qt_metacast(_clname);
}

int defpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void defpage::loadDefSgn(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_clickTB[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      19,    8,    8,    8, 0x0a,
      28,    8,    8,    8, 0x0a,
      40,    8,    8,    8, 0x0a,
      53,    8,    8,    8, 0x0a,
      65,    8,    8,    8, 0x0a,
      74,    8,    8,    8, 0x0a,
      92,   88,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_clickTB[] = {
    "clickTB\0\0clicked()\0sgnRun()\0sgnRadRun()\0"
    "sgnFreeRun()\0sgnEngRun()\0sgnRnd()\0"
    "sgnAddPDict()\0url\0sgnDelPDict(QUrl)\0"
};

void clickTB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        clickTB *_t = static_cast<clickTB *>(_o);
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->sgnRun(); break;
        case 2: _t->sgnRadRun(); break;
        case 3: _t->sgnFreeRun(); break;
        case 4: _t->sgnEngRun(); break;
        case 5: _t->sgnRnd(); break;
        case 6: _t->sgnAddPDict(); break;
        case 7: _t->sgnDelPDict((*reinterpret_cast< QUrl(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData clickTB::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject clickTB::staticMetaObject = {
    { &QWebView::staticMetaObject, qt_meta_stringdata_clickTB,
      qt_meta_data_clickTB, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &clickTB::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *clickTB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *clickTB::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_clickTB))
        return static_cast<void*>(const_cast< clickTB*>(this));
    return QWebView::qt_metacast(_clname);
}

int clickTB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWebView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void clickTB::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_clickComboBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   15,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_clickComboBox[] = {
    "clickComboBox\0\0str\0sgnHideShow(QString)\0"
};

void clickComboBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        clickComboBox *_t = static_cast<clickComboBox *>(_o);
        switch (_id) {
        case 0: _t->sgnHideShow((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData clickComboBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject clickComboBox::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_clickComboBox,
      qt_meta_data_clickComboBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &clickComboBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *clickComboBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *clickComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_clickComboBox))
        return static_cast<void*>(const_cast< clickComboBox*>(this));
    return QComboBox::qt_metacast(_clname);
}

int clickComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_lrnTb[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_lrnTb[] = {
    "lrnTb\0"
};

void lrnTb::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData lrnTb::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject lrnTb::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_lrnTb,
      qt_meta_data_lrnTb, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &lrnTb::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *lrnTb::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *lrnTb::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_lrnTb))
        return static_cast<void*>(const_cast< lrnTb*>(this));
    return QWidget::qt_metacast(_clname);
}

int lrnTb::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
