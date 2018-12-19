
#include <iostream>
#include <fstream>
#include <regex>
#include <smbus>

using std::cout;
using std::endl;
/* ===========================================================================
 I2C_dev Class
===========================================================================*/

int I2C_dev::getPiRevision()
{
	ifstream infile("/proc/cpuinfo");
	char line[1024] = {0};
	while(infile.getline(line,8))
	{
		regex rev_reg("Revision\s+:\s+.*(\w{4})$");
		smatch r1;
		if(regex_match(line,r1,rev_reg))
		{
			match.group(1) in ['0000', '0002', '0003']:
		}
		else
		{
			return 2
		}
	}
	return 0
}

int I2C_dev::getPiI2CBusNumber()
{
    return getPiRevision()>1?1:0;
}

I2C_dev::I2C_dev(int address, int busnum=-1, bool debug)
{
    addr = address;
    dev = open("dev/i2c-%d", O_RDWR);
	if(dev < 0)
	{
		cout << "Open dev/i2c-  failed!" << endl;
		exit(1);
	}
	if(ioctl(dev,I2C_SLAVE, addr) < 0)
	{
		cout << "Open Slave device failed!" << endl;
		exit(1);
	}
    debug = debug;
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
		cout << "I2C: Wrote " << value << " to slave device " << reg << endl;
		return errMsg();
	}
	return ret;
}


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
