/****************************************************************************
** Meta object code from reading C++ file 'WavelengthUnitsComboBox.h'
**
** Created: Tue Jun 22 09:28:36 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SpectralUtilities/WavelengthUnitsComboBox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WavelengthUnitsComboBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WavelengthUnitsComboBox[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      31,   25,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   80,   24,   24, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WavelengthUnitsComboBox[] = {
    "WavelengthUnitsComboBox\0\0units\0"
    "unitsActivated(Wavelengths::WavelengthUnitsType)\0"
    "index\0translateActivated(int)\0"
};

const QMetaObject WavelengthUnitsComboBox::staticMetaObject = {
    { &QComboBox::staticMetaObject, qt_meta_stringdata_WavelengthUnitsComboBox,
      qt_meta_data_WavelengthUnitsComboBox, 0 }
};

const QMetaObject *WavelengthUnitsComboBox::metaObject() const
{
    return &staticMetaObject;
}

void *WavelengthUnitsComboBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WavelengthUnitsComboBox))
        return static_cast<void*>(const_cast< WavelengthUnitsComboBox*>(this));
    return QComboBox::qt_metacast(_clname);
}

int WavelengthUnitsComboBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QComboBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: unitsActivated((*reinterpret_cast< Wavelengths::WavelengthUnitsType(*)>(_a[1]))); break;
        case 1: translateActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void WavelengthUnitsComboBox::unitsActivated(Wavelengths::WavelengthUnitsType _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
