/****************************************************************************
** Meta object code from reading C++ file 'SpectralSignatureSelector.h'
**
** Created: Tue Jun 22 09:28:36 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../SpectralUtilities/SpectralSignatureSelector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SpectralSignatureSelector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SpectralSignatureSelector[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x09,
      55,   26,   26,   26, 0x09,
      64,   26,   26,   26, 0x09,
      72,   26,   26,   26, 0x09,
      86,   26,   26,   26, 0x09,
     103,   26,   26,   26, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SpectralSignatureSelector[] = {
    "SpectralSignatureSelector\0\0"
    "enablePseudocolorCheckBox()\0accept()\0"
    "apply()\0setModified()\0refreshAoiList()\0"
    "updateSignatureList()\0"
};

const QMetaObject SpectralSignatureSelector::staticMetaObject = {
    { &SignatureSelector::staticMetaObject, qt_meta_stringdata_SpectralSignatureSelector,
      qt_meta_data_SpectralSignatureSelector, 0 }
};

const QMetaObject *SpectralSignatureSelector::metaObject() const
{
    return &staticMetaObject;
}

void *SpectralSignatureSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SpectralSignatureSelector))
        return static_cast<void*>(const_cast< SpectralSignatureSelector*>(this));
    return SignatureSelector::qt_metacast(_clname);
}

int SpectralSignatureSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SignatureSelector::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: enablePseudocolorCheckBox(); break;
        case 1: accept(); break;
        case 2: apply(); break;
        case 3: setModified(); break;
        case 4: refreshAoiList(); break;
        case 5: updateSignatureList(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
