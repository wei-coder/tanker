
#include <iostream>
#include <fstream>
#include <wiringPiI2C.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include "iicdriver.h"

using std::cout;
using std::endl;
/* ===========================================================================
 I2C_dev Class
===========================================================================*/

int I2C_dev::getPiRevision()
{
	return 2;
}

int I2C_dev::getPiI2CBusNumber()
{
    return getPiRevision()>1?1:0;
}

I2C_dev::I2C_dev(int address, bool debug=false)
{
    addr = address;
    dev = wiringPiI2CSetup(addr);
    if(dev < 0)
    {
	cout << "Open dev/i2c-  failed!" << endl;
	exit(1);
    }
    this->debug = debug;
}

int I2C_dev::errMsg()
{
	cout << "Error accessing 0x" << addr << ": Check your I2C address" << endl;
	return -1;
}

int I2C_dev::write8(int reg, int value)
{
	int ret = wiringPiI2CWriteReg8(dev, reg, value);
	if(ret < 0)
	{
        	cout << "I2C: Wrote " << value << " to register " << reg << endl;
		return errMsg();
	}
	return ret;
}

int I2C_dev::write16(int reg, int value)
{
      int ret = wiringPiI2CWriteReg16(dev, reg, value);
	  if(ret < 0)
	  {
		  cout << "I2C: Wrote " << value << " to register " << reg << endl;
		  return errMsg();
	  }
	  return ret;
}

int I2C_dev::writeRaw8(int value)
{
	int ret = wiringPiI2CWrite(dev,value);
	if(ret < 0)
	{
		cout << "I2C: Wrote " << value << " to slave device " << endl;
		return errMsg();
	}
	return ret;
}

#if 0
int I2C_dev::writeList(unsigned char reg, unsigned char length, unsigned char * plist)
{
	int ret = i2c_smbus_write_i2c_block_data(dev,reg,length,plist);
	if(ret < 0)
	{
		cout << "I2C: Wrote " << value << " to slave device " << reg << endl;
		return errMsg();
	}
	return ret;
}

int I2C_dev::readList(unsigned char reg, unsigned char length)
{
	int ret = i2c_smbus_read_i2c_block_data(dev,reg,plist);
	if(ret < 0)
	{
		cout << "I2C: Wrote " << value << " to slave device " << reg << endl;
		return errMsg();
	}
	return ret;
}
#endif
int I2C_dev::readU8(int reg)
{
	int ret = wiringPiI2CReadReg8(dev, reg);
	if(ret < 0)
	{
		cout << "I2C: Read u8bit from register:" << reg << " failed!" << endl;
		return errMsg();
	}
	return ret;
}

int I2C_dev::readS8(int reg)
{
	int ret = wiringPiI2CReadReg8(dev, reg);
	if(ret > 127)
	{
		ret -= 256;
	}
	if(ret < 0)
	{
		cout << "I2C: Read 8bit from register:" << reg << " failed!" << endl;
		return errMsg();
	}
	return ret;
}

int I2C_dev::readU16(int reg)
{
	int ret = wiringPiI2CReadReg16(dev, reg);
	ret = ((ret << 8) & 0xFF00) + (ret >> 8);
	//需要注意字节序
	if(ret < 0)
	{
		cout << "I2C: Read 16bit from register:" << reg << " failed!" << endl;
		return errMsg();
	}
	return ret;
}

int I2C_dev::readS16(int reg)
{
	int ret = readU16(reg);
	if(ret > 32767)
	{
		ret -= 65536;
	}
	//需要注意字节序
	if(ret < 0)
	{
		cout << "I2C: Read 16bit from register:" << reg << " failed!" << endl;
		return errMsg();
	}
	return ret;
}
