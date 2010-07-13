/****************************************************************************
** Meta object code from reading C++ file 'ResamplerOptions.h'
**
** Created: Sun Jul 11 21:14:37 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Resampler/ResamplerOptions.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ResamplerOptions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ResamplerOptions[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      27,   18,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ResamplerOptions[] = {
    "ResamplerOptions\0\0newIndex\0"
    "currentIndexChanged(int)\0"
};

const QMetaObject ResamplerOptions::staticMetaObject = {
    { &LabeledSectionGroup::staticMetaObject, qt_meta_stringdata_ResamplerOptions,
      qt_meta_data_ResamplerOptions, 0 }
};

const QMetaObject *ResamplerOptions::metaObject() const
{
    return &staticMetaObject;
}

void *ResamplerOptions::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ResamplerOptions))
        return static_cast<void*>(const_cast< ResamplerOptions*>(this));
    return LabeledSectionGroup::qt_metacast(_clname);
}

int ResamplerOptions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LabeledSectionGroup::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
