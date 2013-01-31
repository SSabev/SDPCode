/****************************************************************************
** Meta object code from reading C++ file 'BTComm.h'
**
** Created: Thu Jan 31 13:12:44 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Comm/BTComm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BTComm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CBtComm[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      25,    8,    8,    8, 0x08,
      36,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CBtComm[] = {
    "CBtComm\0\0ConnectedSlot()\0ConnLost()\0"
    "SockErr()\0"
};

const QMetaObject CBtComm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CBtComm,
      qt_meta_data_CBtComm, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CBtComm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CBtComm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CBtComm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CBtComm))
        return static_cast<void*>(const_cast< CBtComm*>(this));
    return QWidget::qt_metacast(_clname);
}

int CBtComm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ConnectedSlot(); break;
        case 1: ConnLost(); break;
        case 2: SockErr(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
