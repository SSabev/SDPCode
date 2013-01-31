/****************************************************************************
** Meta object code from reading C++ file 'PitchSideDlg.h'
**
** Created: Thu Jan 31 12:05:35 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/Tools/PitchSideDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PitchSideDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPitchSideDlg[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      24,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CPitchSideDlg[] = {
    "CPitchSideDlg\0\0OkSlot()\0CancelSlot()\0"
};

const QMetaObject CPitchSideDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CPitchSideDlg,
      qt_meta_data_CPitchSideDlg, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPitchSideDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPitchSideDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPitchSideDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPitchSideDlg))
        return static_cast<void*>(const_cast< CPitchSideDlg*>(this));
    if (!strcmp(_clname, "Ui::PitchSideDlg"))
        return static_cast< Ui::PitchSideDlg*>(const_cast< CPitchSideDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CPitchSideDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OkSlot(); break;
        case 1: CancelSlot(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
