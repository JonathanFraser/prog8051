/****************************************************************************
** Meta object code from reading C++ file 'QespTest.h'
**
** Created: Sat Jun 7 14:53:52 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QespTest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QespTest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QespTest[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      24,    9,    9,    9, 0x08,
      37,    9,    9,    9, 0x08,
      48,    9,    9,    9, 0x08,
      59,    9,    9,    9, 0x08,
      71,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QespTest[] = {
    "QespTest\0\0transmitMsg()\0receiveMsg()\0"
    "appendCR()\0appendLF()\0closePort()\0"
    "openPort()\0"
};

const QMetaObject QespTest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QespTest,
      qt_meta_data_QespTest, 0 }
};

const QMetaObject *QespTest::metaObject() const
{
    return &staticMetaObject;
}

void *QespTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QespTest))
	return static_cast<void*>(const_cast< QespTest*>(this));
    return QWidget::qt_metacast(_clname);
}

int QespTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: transmitMsg(); break;
        case 1: receiveMsg(); break;
        case 2: appendCR(); break;
        case 3: appendLF(); break;
        case 4: closePort(); break;
        case 5: openPort(); break;
        }
        _id -= 6;
    }
    return _id;
}
