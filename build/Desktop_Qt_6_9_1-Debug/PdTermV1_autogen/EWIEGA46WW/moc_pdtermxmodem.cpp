/****************************************************************************
** Meta object code from reading C++ file 'pdtermxmodem.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../pdtermxmodem.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pdtermxmodem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN12PdTermXmodemE_t {};
} // unnamed namespace

template <> constexpr inline auto PdTermXmodem::qt_create_metaobjectdata<qt_meta_tag_ZN12PdTermXmodemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PdTermXmodem",
        "transmissaoCancelada",
        "",
        "transmissaoConcluida",
        "erroOcorreu",
        "mensagem",
        "progressoAtualizado",
        "porcentagem"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'transmissaoCancelada'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'transmissaoConcluida'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'erroOcorreu'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'progressoAtualizado'
        QtMocHelpers::SignalData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PdTermXmodem, qt_meta_tag_ZN12PdTermXmodemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PdTermXmodem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PdTermXmodemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PdTermXmodemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12PdTermXmodemE_t>.metaTypes,
    nullptr
} };

void PdTermXmodem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PdTermXmodem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->transmissaoCancelada(); break;
        case 1: _t->transmissaoConcluida(); break;
        case 2: _t->erroOcorreu((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->progressoAtualizado((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PdTermXmodem::*)()>(_a, &PdTermXmodem::transmissaoCancelada, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PdTermXmodem::*)()>(_a, &PdTermXmodem::transmissaoConcluida, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PdTermXmodem::*)(const QString & )>(_a, &PdTermXmodem::erroOcorreu, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PdTermXmodem::*)(int )>(_a, &PdTermXmodem::progressoAtualizado, 3))
            return;
    }
}

const QMetaObject *PdTermXmodem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PdTermXmodem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12PdTermXmodemE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PdTermXmodem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PdTermXmodem::transmissaoCancelada()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PdTermXmodem::transmissaoConcluida()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PdTermXmodem::erroOcorreu(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void PdTermXmodem::progressoAtualizado(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
