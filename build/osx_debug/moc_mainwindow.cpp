/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata[238];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 23),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 16),
QT_MOC_LITERAL(4, 53, 8),
QT_MOC_LITERAL(5, 62, 5),
QT_MOC_LITERAL(6, 68, 12),
QT_MOC_LITERAL(7, 81, 16),
QT_MOC_LITERAL(8, 98, 8),
QT_MOC_LITERAL(9, 107, 8),
QT_MOC_LITERAL(10, 116, 9),
QT_MOC_LITERAL(11, 126, 14),
QT_MOC_LITERAL(12, 141, 23),
QT_MOC_LITERAL(13, 165, 15),
QT_MOC_LITERAL(14, 181, 12),
QT_MOC_LITERAL(15, 194, 21),
QT_MOC_LITERAL(16, 216, 5),
QT_MOC_LITERAL(17, 222, 5),
QT_MOC_LITERAL(18, 228, 8)
    },
    "MainWindow\0on_actionOpen_triggered\0\0"
    "action_makeVideo\0setColor\0index\0"
    "stateChanged\0QListWidgetItem*\0cur_item\0"
    "layerSet\0activeSet\0chooseSavePath\0"
    "chooseAlternativeScript\0grabFrameBuffer\0"
    "save_graphic\0changeAllLayersColors\0"
    "color\0about\0openHelp\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08,
       3,    0,   75,    2, 0x08,
       4,    1,   76,    2, 0x08,
       6,    1,   79,    2, 0x08,
       9,    1,   82,    2, 0x08,
      11,    0,   85,    2, 0x08,
      12,    0,   86,    2, 0x08,
      13,    0,   87,    2, 0x08,
      14,    0,   88,    2, 0x08,
      15,    1,   89,    2, 0x08,
      17,    0,   92,    2, 0x08,
      18,    0,   93,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_actionOpen_triggered(); break;
        case 1: _t->action_makeVideo(); break;
        case 2: _t->setColor((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 3: _t->stateChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->layerSet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->chooseSavePath(); break;
        case 6: _t->chooseAlternativeScript(); break;
        case 7: _t->grabFrameBuffer(); break;
        case 8: _t->save_graphic(); break;
        case 9: _t->changeAllLayersColors((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->about(); break;
        case 11: _t->openHelp(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
