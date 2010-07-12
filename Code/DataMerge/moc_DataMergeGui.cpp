/****************************************************************************
** Meta object code from reading C++ file 'DataMergeGui.h'
**
** Created: Sun Jul 11 21:29:43 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DataMergeGui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataMergeGui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataMergeGui[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      27,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataMergeGui[] = {
    "DataMergeGui\0\0applyScale()\0generateNewView()\0"
};

const QMetaObject DataMergeGui::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DataMergeGui,
      qt_meta_data_DataMergeGui, 0 }
};

const QMetaObject *DataMergeGui::metaObject() const
{
    return &staticMetaObject;
}

void *DataMergeGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataMergeGui))
        return static_cast<void*>(const_cast< DataMergeGui*>(this));
    return QDialog::qt_metacast(_clname);
}

int DataMergeGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: applyScale(); break;
        case 1: generateNewView(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
