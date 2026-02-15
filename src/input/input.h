#ifndef INPUT_H
#define INPUT_H

class Input
{
    protected:
        bool state = false;

    public:
        virtual ~Input() = default;
        virtual void Spin() = 0;   // czysto wirtualna (bo zależna od hardware)
        virtual bool* getStatePtr()
        {
            return &state;
        }
};

#endif