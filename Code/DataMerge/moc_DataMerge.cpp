/****************************************************************************
** Meta object code from reading C++ file 'DataMerge.h'
**
** Created: Sun Jul 11 21:29:33 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DataMerge.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataMerge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataMerge[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataMerge[] = {
    "DataMerge\0\0dialogClosed()\0"
};

const QMetaObject DataMerge::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataMerge,
      qt_meta_data_DataMerge, 0 }
};

const QMetaObject *DataMerge::metaObject() const
{
    return &staticMetaObject;
}

void *DataMerge::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataMerge))
        return static_cast<void*>(const_cast< DataMerge*>(this));
    if (!strcmp(_clname, "ViewerShell"))
        return static_cast< ViewerShell*>(const_cast< DataMerge*>(this));
    return QObject::qt_metacast(_clname);
}

int DataMerge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dialogClosed(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
