/****************************************************************************
** Meta object code from reading C++ file 'StatisticsDlg.h'
**
** Created: Tue Jun 22 09:28:45 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Mnf/StatisticsDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StatisticsDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StatisticsDlg[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x09,
      39,   28,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_StatisticsDlg[] = {
    "StatisticsDlg\0\0loadRaster()\0rasterName\0"
    "rasterChanged(QString)\0"
};

const QMetaObject StatisticsDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_StatisticsDlg,
      qt_meta_data_StatisticsDlg, 0 }
};

const QMetaObject *StatisticsDlg::metaObject() const
{
    return &staticMetaObject;
}

void *StatisticsDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatisticsDlg))
        return static_cast<void*>(const_cast< StatisticsDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int StatisticsDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loadRaster(); break;
        case 1: rasterChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
