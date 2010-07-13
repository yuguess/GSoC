/****************************************************************************
** Meta object code from reading C++ file 'DifferenceImageDlg.h'
**
** Created: Tue Jun 22 09:28:45 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Mnf/DifferenceImageDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DifferenceImageDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DifferenceImageDlg[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      28,   20,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_DifferenceImageDlg[] = {
    "DifferenceImageDlg\0\0useAuto\0"
    "methodChanged(bool)\0"
};

const QMetaObject DifferenceImageDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DifferenceImageDlg,
      qt_meta_data_DifferenceImageDlg, 0 }
};

const QMetaObject *DifferenceImageDlg::metaObject() const
{
    return &staticMetaObject;
}

void *DifferenceImageDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DifferenceImageDlg))
        return static_cast<void*>(const_cast< DifferenceImageDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int DifferenceImageDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: methodChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
