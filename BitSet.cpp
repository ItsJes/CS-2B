#include <iostream>
#include <sstream>
using namespace std;

class Bitset 
{
private:
	unsigned char * bits;
	int size;
	static const int BITS_PER_BYTE = 8;
	static const int MIN_BIT = 0;
	static const int DEFAULT_BIT = 1000;
public:
	Bitset(int bit);
	~Bitset();

	int cardinality() const;
	int getSize() const;
	bool getBit(const int index) const;
	bool setBit(const int index, const bool bit);
	string toString() const;
	friend Bitset& operator &(const Bitset& b1, const Bitset& b2);
	friend Bitset& operator |(const Bitset& b1, const Bitset& b2);
	friend Bitset& operator!(const Bitset& b);
	bool operator[](int bitIndex);

};

int main() 
{
	int size = 2000;
	Bitset b1 = Bitset(size);
	Bitset b2 = Bitset(size - 1);
	b1.setBit(7, true);
	b1.setBit(15, true);
	b1.setBit(20, true);
	b1.setBit(25, true);
	
	cout << b1.toString() << endl;
	cout << "cardinality: " << b1.cardinality() << endl << endl;

	b2.setBit(7, true);
	
	Bitset result = b2 | b1;
   cout << "Testing OR:\n";
	cout << result.toString();
	cout << endl << endl;
	
	
	result = b2 & b1;
   cout << "Testing AND:\n";
	cout << result.toString();
	cout << endl << endl;

	result = !result;
   cout << "Testing NEGATION:\n";
	cout << result.toString();
	cout << endl << endl;

	return 0;
}

Bitset::Bitset(int bit) 
{
	if (bit < MIN_BIT)
		bit = DEFAULT_BIT;

	size = bit;
	
	int numBytes = bit / BITS_PER_BYTE + 1;
	bits = new unsigned char[numBytes];
	for (int i = 0; i < numBytes; i++) 
	{
		bits[i] = 0;
	}

}

Bitset::~Bitset()
{
	delete[] bits;
}

int Bitset::cardinality() const
{
	int count = 0;
	for (int i = 0; i < size; i++) 
	{
		if (getBit(i)) 
		{
			count++;
		}
	}

	return count;
}

int Bitset::getSize() const
{
	return size; // total bit
}

// index is starting from zero
bool Bitset::getBit(int index) const
{
	if(index < MIN_BIT || index > this->size)
      return false; // Always false for invalid index

	int byteIndex = index / BITS_PER_BYTE;
	char byte = bits[byteIndex];

	int bitMask = 1;
	for (int i = 0; i < index % BITS_PER_BYTE; i++)
	{
		bitMask*= 2;
	}

	return (byte & bitMask) != 0;
}

// index is starting from zero
bool Bitset::setBit(const int index, const bool bit)
{
	if (index < MIN_BIT || index > this->size)
		return false;
	
	int byteIndex = index / BITS_PER_BYTE;
	char byte = bits[byteIndex];

	int bitMask = 1;
	for (int i = 0; i < index % BITS_PER_BYTE; i++)
	{
		bitMask *= 2;
	}

	if (bit)
	{
		byte += bitMask;
		bits[byteIndex] = byte;
	}
	else
	{
		byte -= bitMask;
		bits[byteIndex] = byte;
	}

   return true;
}

string Bitset::toString() const
{
	stringstream ss;
	for (int i = 1; i < size; i++)
	{
		ss << getBit(i - 1);
		if (i % 10 == 0)
			ss << " ";
		if (i % 100 == 0)
			ss << endl;
		if (i % 1000 == 0)
			ss << endl;
	}
	
	ss << getBit(size - 1) << endl;
	
	return ss.str();
}

bool Bitset::operator[](int bitIndex)
{
	int byteIndex;
	byteIndex = bitIndex / 8;
	int actualIndex = (bitIndex - (byteIndex * 8)); // actual index of the bit inside the char
	bool bit = ((bits[byteIndex] & (1 << actualIndex)) != 0) ? 1 : 0;
	return bit;
}

Bitset & operator&(const Bitset & b1, const Bitset & b2)
{
	int size = (b1.size > b2.size ? b1.size : b2.size);
	Bitset* resultBitSet = new Bitset(size);

	if (b1.size > b2.size) 
	{
		for (int i = 0; i < b2.size / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i] && b2.bits[i];
		for (int i = 0; i < (size - b2.size) / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i];
	}
	else if (b1.size < b2.size)
	{
		for (int i = 0; i < b1.size / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i] && b2.bits[i];
		for (int i = 0; i < (size - b1.size) / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b2.bits[i];
	}
	else
	{
		for (int i = 0; i < size / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i] && b2.bits[i];
	}

	return *resultBitSet;
}

Bitset & operator|(const Bitset & b1, const Bitset & b2)
{
	int size = (b1.size > b2.size ? b1.size : b2.size);
	Bitset* resultBitSet = new Bitset(size);

	if (b1.size > b2.size)
	{
		for (int i = 0; i < b2.size / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i] || b2.bits[i];
		for (int i = 0; i < (size - b2.size) / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i];
	}
	else if (b1.size < b2.size)
	{
		for (int i = 0; i < b1.size / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i] || b2.bits[i];
		for (int i = 0; i < (size - b1.size) / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b2.bits[i];
	}
	else
	{
		for (int i = 0; i < size / Bitset::BITS_PER_BYTE + 1; i++)
			(*resultBitSet).bits[i] = b1.bits[i] || b2.bits[i];
	}

	return *resultBitSet;
}

Bitset & operator!(const Bitset & b)
{
	int size = (b.size);
	Bitset* resultBitSet = new Bitset(size);
	for (int i = 0; i < size / Bitset::BITS_PER_BYTE + 1; i++)
		(*resultBitSet).bits[i] = ~b.bits[i];
	return *resultBitSet;
}

