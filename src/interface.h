#ifndef SERIAL_PORT_INTERFACE_H_
#define SERIAL_PORT_INTERFACE_H_

#include <string>

#include "serial_port/types.h"

namespace serial_port
{

    /// @brief An abstract base class defining the interface of all serial port implementations
    class Interface
    {
        // **************************************************************************
        // **************************************************************************

    public:
        // Constructors
        Interface() = default;
        Interface(const Settings& settings);
        Interface(const std::string& port_name, int baud_rate,
            Parity parity = Parity::kNone,
            NumStopBits stop_bits = serial_port::NumStopBits::kOne,
            bool hardware_flow_control = false,
            unsigned long int timeout_s = 0, unsigned long int timeout_ms = 0);
        // Allow moving a serial port
        Interface(Interface&&) = default;
        // Do not allow copying a serial port
        Interface(const Interface&) = delete;

        // Destructor. Cannot call pure virtual methods in destructor so derived classes
    	// must call their respective close() method in destructor themselves!
        virtual ~Interface() = 0;
        

        virtual void Open() = 0;
        virtual void Close() = 0;
        virtual bool IsOpen() = 0;
        const Settings& GetSettings() const;

        virtual unsigned long NumBytesAvailable() = 0;
        virtual void FlushBuffer() const = 0;
        virtual unsigned long ReadData(char* data, unsigned long num_bytes) = 0;
        virtual unsigned long WriteData(const char* data, unsigned long num_bytes) = 0;

        // **************************************************************************
        // **************************************************************************

    protected:
        Settings settings_;
    };

}

#endif // !SERIAL_PORT_INTERFACE_H_