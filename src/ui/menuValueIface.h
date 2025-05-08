enum class MenuGetterReturn {
    OK = 0,
    GETTER_UNDEF,
};

enum class MenuSetterReturn {
    OK = 0,
    SETTER_UNDEF,
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
        BoolMenuValueIface(bool &v) : value(v) {}
        MenuGetterReturn get(bool &ref) const override { 
            ref = value;    //assign from value to ref
            return MenuGetterReturn::OK;
        }
        MenuSetterReturn set(bool &ref) const override { 
            value = ref;
            return MenuSetterReturn::OK;
        }
        
};

class IntMenuValueIface : public MenuValueIface {
    private:
        int &value;
        int floor;
        int ceil;
    public:
        IntMenuValueIface(int &v, int limit) : 
            value(v),
            floor(0),
            ceil(limit)
        {}
        IntMenuValueIface(int &v, int bottom_limit, int top_limit) : 
            value(v),
            floor(bottom_limit),
            ceil(top_limit)
        {}
        MenuGetterReturn get(int &ref) const override { 
            ref = value;    //assign from value to ref
            return MenuGetterReturn::OK;
        }

        MenuSetterReturn set(int &ref) const override {
            //validate
            if (ref < floor)
                return MenuSetterReturn::VAL_TOO_SMALL;
            if (ref >= ceil)
                return MenuSetterReturn::VAL_TOO_BIG;

            value = ref;
            return MenuSetterReturn::OK;
        
        }
};
