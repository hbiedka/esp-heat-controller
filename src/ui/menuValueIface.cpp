#include "menuValueIface.h"

BoolMenuValueIface::BoolMenuValueIface(bool &v) 
    : value(v) {}

MenuGetterReturn BoolMenuValueIface::get(bool &ref) const {
    ref = value;
    return MenuGetterReturn::OK;
}

MenuSetterReturn BoolMenuValueIface::set(bool &ref) const {
    value = ref;
    return MenuSetterReturn::OK;
}

IntMenuValueIface::IntMenuValueIface(int &v, int limit)
    : value(v), floor(0), ceil(limit) {}

IntMenuValueIface::IntMenuValueIface(int &v, int bottom_limit, int top_limit)
    : value(v), floor(bottom_limit), ceil(top_limit) {}

MenuGetterReturn IntMenuValueIface::get(int &ref) const {
    ref = value;
    return MenuGetterReturn::OK;
}

MenuSetterReturn IntMenuValueIface::set(int &ref) const {
    if (ref < floor)
        return MenuSetterReturn::VAL_TOO_SMALL;
    if (ref >= ceil)
        return MenuSetterReturn::VAL_TOO_BIG;

    value = ref;
    return MenuSetterReturn::OK;
}