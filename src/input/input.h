#ifndef INPUT_H
#define INPUT_H

class Input
{
    protected:
        bool state = false;

    public:
        virtual ~Input() = default;
        virtual void Spin(unsigned long ts) = 0;   // czysto wirtualna (bo zależna od hardware)
        virtual bool getState() {
            return state;
        }
};

class AnalogInput
{
    protected:
        unsigned int state = false;

    public:
        virtual ~AnalogInput() = default;
        virtual void Spin(unsigned long ts) = 0;
        virtual unsigned int getState() {
            return state;
        }
};
#endif