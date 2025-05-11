#include "ObjectModelToMenu.h"

MenuGetterReturn BoolObjectModelIface::get(bool &ref) const {
    if (objectModel == nullptr)
        return MenuGetterReturn::GETTER_UNDEF;
    if (label.empty())
        return MenuGetterReturn::VAL_UNDEF;

    ObjectModelItemValue value;
    ObjectModelGetterReturn ret = objectModel->getProperty(label, value);
    if (ret == ObjectModelGetterReturn::OK) {
        if (std::holds_alternative<bool>(value) == false)
            return MenuGetterReturn::VAL_INVALID;
        ref = std::get<bool>(value);
    } else if (ret == ObjectModelGetterReturn::INVLABEL) {
        return MenuGetterReturn::VAL_UNDEF;
    } else {
        return MenuGetterReturn::VAL_INVALID;
    }
    return MenuGetterReturn::OK;
}
MenuSetterReturn BoolObjectModelIface::set(bool &ref) const {
    if (objectModel == nullptr)
        return MenuSetterReturn::SETTER_UNDEF;
    if (label.empty())
        return MenuSetterReturn::VAL_UNDEF;

    ObjectModelSetterReturn ret = objectModel->setProperty(label, ref);
    if (ret == ObjectModelSetterReturn::OK) {
        return MenuSetterReturn::OK;
    } else if (ret == ObjectModelSetterReturn::INVLABEL) {
        return MenuSetterReturn::VAL_UNDEF;
    } else if (ret == ObjectModelSetterReturn::READONLY) {
        return MenuSetterReturn::VAL_READONLY;
    }
    return MenuSetterReturn::VAL_INVALID;
}

MenuGetterReturn IntObjectModelIface::get(int &ref) const {
    if (objectModel == nullptr)
        return MenuGetterReturn::GETTER_UNDEF;
    if (label.empty())
        return MenuGetterReturn::VAL_UNDEF;

    ObjectModelItemValue value;
    ObjectModelGetterReturn ret = objectModel->getProperty(label, value);
    if (ret == ObjectModelGetterReturn::OK) {
        if (std::holds_alternative<int>(value) == false)
            return MenuGetterReturn::VAL_INVALID;
        ref = std::get<int>(value);
    } else if (ret == ObjectModelGetterReturn::INVLABEL) {
        return MenuGetterReturn::VAL_UNDEF;
    } else {
        return MenuGetterReturn::VAL_INVALID;
    }
    return MenuGetterReturn::OK;
}
MenuSetterReturn IntObjectModelIface::set(int &ref) const {
    if (objectModel == nullptr)
        return MenuSetterReturn::SETTER_UNDEF;
    if (label.empty())
        return MenuSetterReturn::VAL_UNDEF;
    if (floor > ceil)
        return MenuSetterReturn::SETTER_INVALID;

    if (ref < floor)
        return MenuSetterReturn::VAL_TOO_SMALL;
    if (ref >= ceil)
        return MenuSetterReturn::VAL_TOO_BIG;

    ObjectModelSetterReturn ret = objectModel->setProperty(label, ref);
    if (ret == ObjectModelSetterReturn::OK) {
        return MenuSetterReturn::OK;
    } else if (ret == ObjectModelSetterReturn::INVLABEL) {
        return MenuSetterReturn::VAL_UNDEF;
    } else if (ret == ObjectModelSetterReturn::READONLY) {
        return MenuSetterReturn::VAL_READONLY;
    }
    return MenuSetterReturn::VAL_INVALID;
}