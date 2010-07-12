/****************************************************************************
** Meta object code from reading C++ file 'MnfDlg.h'
**
** Created: Tue Jun 22 09:28:45 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Mnf/MnfDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MnfDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MnfDlg[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MnfDlg[] = {
    "MnfDlg\0\0browse()\0"
};

const QMetaObject MnfDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MnfDlg,
      qt_meta_data_MnfDlg, 0 }
};

const QMetaObject *MnfDlg::metaObject() const
{
    return &staticMetaObject;
}

void *MnfDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MnfDlg))
        return static_cast<void*>(const_cast< MnfDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int MnfDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: browse(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
