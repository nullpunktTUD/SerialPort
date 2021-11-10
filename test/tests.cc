#include <gtest/gtest.h>

#include <cstring>
#include <iostream>

#include "serial_port/serial_port.h"

#if defined (__linux__)
#define output_port_name "/dev/ttyUSB0"
#define input_port_name "/dev/ttyUSB1"
#elif defined(_WIN32)
constexpr auto output_port_name = "COM5";
constexpr auto input_port_name = "COM6";
#endif


// Test the various ways to instantiate a serial port
TEST(SerialPortTests, Instantiation)
{
	serial_port::SerialPort default_port;
	EXPECT_EQ(default_port.GetSettings(), serial_port::Settings());

	auto custom_settings = serial_port::Settings("COM2", 115200, serial_port::Parity::kNone,
	                                             serial_port::NumStopBits::kTwo, false, 0, 10);
	serial_port::SerialPort settings_port(custom_settings);
	EXPECT_EQ(settings_port.GetSettings(), custom_settings);
}

// Test opening a port
TEST(SerialPortTests, Open)
{
	serial_port::SerialPort port(output_port_name, 9600);
	port.Open();
	port.Close();
}

// Test writing a C string
TEST(SerialPortTests, WriteData)
{
	serial_port::SerialPort port(output_port_name, 115200);
	constexpr auto test_c_string = "I am a C string!\r\n";
	const auto num_bytes = static_cast<unsigned long>(strlen(test_c_string));
	port.Open();
	const auto num_bytes_written = port.WriteData(test_c_string, num_bytes);
	port.Close();
	EXPECT_EQ(num_bytes_written, num_bytes);
}

// Test writing an STL string
TEST(SerialPortTests, WriteString)
{
	serial_port::SerialPort port(output_port_name, 115200);
	const auto test_string = std::string("I am an STL string!\r\n");
	port.Open();
	const auto num_bytes_written = port.WriteString(test_string);
	EXPECT_EQ(num_bytes_written, test_string.size());
}

// Test reading a C string
TEST(SerialPortTests, ReadData)
{
	serial_port::SerialPort out_port(output_port_name, 115200);
	serial_port::SerialPort in_port(input_port_name, 115200);

	constexpr auto test_c_string = "I am a C string!\r\n";
	const auto num_bytes = static_cast<unsigned long>(strlen(test_c_string));
	out_port.Open();
	in_port.Open();

	out_port.WriteData(test_c_string, num_bytes);

	// Make Rx buffer one element larger to hold the terminating \0
	char* buf = new char[num_bytes+1];
	memset(buf, 0, num_bytes+1);

	const auto bytes_read = in_port.ReadData(buf, num_bytes);
	EXPECT_STREQ(test_c_string, buf);

	delete[] buf;	
}

// Test reading an STL string
TEST(SerialPortTests, ReadString)
{
	serial_port::SerialPort out_port(output_port_name, 115200);
	serial_port::SerialPort in_port(input_port_name, 115200);

	const auto out_string = std::string("I am an STL string!\r\n");
	out_port.Open();
	in_port.Open();

	out_port.WriteString(out_string);

	const auto in_string = in_port.ReadString();
	EXPECT_EQ(in_string, out_string);
}