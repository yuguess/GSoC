/****************************************************************************
** Meta object code from reading C++ file 'AceDlg.h'
**
** Created: Fri Jun 11 15:33:38 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AceDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AceDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AceDlg[] = {

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

static const char qt_meta_stringdata_AceDlg[] = {
    "AceDlg\0\0customButtonClicked()\0"
};

const QMetaObject AceDlg::staticMetaObject = {
    { &SpectralSignatureSelector::staticMetaObject, qt_meta_stringdata_AceDlg,
      qt_meta_data_AceDlg, 0 }
};

const QMetaObject *AceDlg::metaObject() const
{
    return &staticMetaObject;
}

void *AceDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AceDlg))
        return static_cast<void*>(const_cast< AceDlg*>(this));
    return SpectralSignatureSelector::qt_metacast(_clname);
}

int AceDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SpectralSignatureSelector::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: customButtonClicked(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
