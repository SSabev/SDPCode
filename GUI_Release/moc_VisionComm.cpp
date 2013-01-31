/****************************************************************************
** Meta object code from reading C++ file 'VisionComm.h'
**
** Created: Thu Jan 31 12:24:06 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Comm/VisionComm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VisionComm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CVisionComm[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      28,   12,   12,   12, 0x08,
      39,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CVisionComm[] = {
    "CVisionComm\0\0ConnedToServ()\0ConnLost()\0"
    "SockErr()\0"
};

const QMetaObject CVisionComm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CVisionComm,
      qt_meta_data_CVisionComm, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CVisionComm::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CVisionComm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CVisionComm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CVisionComm))
        return static_cast<void*>(const_cast< CVisionComm*>(this));
    return QWidget::qt_metacast(_clname);
}

int CVisionComm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ConnedToServ(); break;
        case 1: ConnLost(); break;
        case 2: SockErr(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
