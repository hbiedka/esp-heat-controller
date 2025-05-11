#ifndef MENU_VALUE_IFACE_H
#define MENU_VALUE_IFACE_H

enum class MenuGetterReturn {
    OK = 0,
    GETTER_UNDEF,
    VAL_UNDEF,
    VAL_INVALID,
};

enum class MenuSetterReturn {
    OK = 0,
    SETTER_UNDEF,
    SETTER_INVALID,
    VAL_UNDEF,
    VAL_READONLY,
    VAL_TOO_BIG,
    VAL_TOO_SMALL,
    VAL_INVALID,
};



class MenuValueIface {
    public:
        virtual MenuGetterReturn get(bool &ref) const { 
            return MenuGetterReturn::GETTER_UNDEF;
        }
        virtual MenuGetterReturn get(int &ref) const { 
            return MenuGetterReturn::GETTER_UNDEF;
        }

        virtual MenuSetterReturn set(bool &ref) const { 
            return MenuSetterReturn::SETTER_UNDEF;
        }
        virtual MenuSetterReturn set(int &ref) const { 
            return MenuSetterReturn::SETTER_UNDEF;
        }
        virtual ~MenuValueIface() = default;
};

class BoolMenuValueIface : public MenuValueIface {
    private:
        bool &value;
    
    public:
        BoolMenuValueIface(bool &v);
        MenuGetterReturn get(bool &ref) const override;
        MenuSetterReturn set(bool &ref) const override;
    };
    
    class IntMenuValueIface : public MenuValueIface {
    private:
        int &value;
        int floor;
        int ceil;
    
    public:
        IntMenuValueIface(int &v, int limit);
        IntMenuValueIface(int &v, int bottom_limit, int top_limit);
        MenuGetterReturn get(int &ref) const override;
        MenuSetterReturn set(int &ref) const override;
    };

#endif