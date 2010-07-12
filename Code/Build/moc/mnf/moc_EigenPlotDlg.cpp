/****************************************************************************
** Meta object code from reading C++ file 'EigenPlotDlg.h'
**
** Created: Tue Jun 22 09:28:45 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Mnf/EigenPlotDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EigenPlotDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EigenPlotDlg[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_EigenPlotDlg[] = {
    "EigenPlotDlg\0"
};

const QMetaObject EigenPlotDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EigenPlotDlg,
      qt_meta_data_EigenPlotDlg, 0 }
};

const QMetaObject *EigenPlotDlg::metaObject() const
{
    return &staticMetaObject;
}

void *EigenPlotDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EigenPlotDlg))
        return static_cast<void*>(const_cast< EigenPlotDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int EigenPlotDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
