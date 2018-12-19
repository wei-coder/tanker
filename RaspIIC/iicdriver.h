
class I2C_dev
{
private:
	int addr;
	bool debug;
	int dev;
	int getPiRevision();
	int getPiI2CBusNumber();

public:
	void I2C_dev(int address, int busnum=-1, bool debug);
	int errMsg();
	int write8(int reg, int value);
	int write16(int reg, int value);
	int writeRaw8(int value);
	int writeList(unsigned char reg, unsigned char length, unsigned char * plist);
	int readList(unsigned char reg, unsigned char length);
	int readU8(int reg);
	int readS8(int reg);
	int readU16(int reg);
	int readS16(int reg);
};

