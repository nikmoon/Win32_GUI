#include <iostream>
#include <cstring>

using namespace std;

#define		DEFAULT_LEN		32

class CLongInt
{
	char * m_Digit;		// последовательность цифр в формате little-endian (1 байт - 1 цифра)
	int m_Len;			// длина числа в байтах
	int m_Sign;			// знак числа

public:
	CLongInt();
	CLongInt(const char *_digit);
	~CLongInt();

	CLongInt operator+(CLongInt &_b);
	friend ostream &operator<<(ostream &_stream, CLongInt &_a);
};


CLongInt::CLongInt()
{
	m_Len = DEFAULT_LEN;
	m_Digit = new char[DEFAULT_LEN];
	m_Sign = 0;
}


CLongInt::CLongInt(const char *_digit)
{
	m_Len = strlen(_digit);
	m_Digit = new char[m_Len];

	// индекс первой старшей цифры получаемого числа
	int dest_index = m_Len - 1;

	// индекс первой старшей цифры заданного в виде строки числа
	int src_index;

	// если в начале строки указан минус
	if (_digit[0] == '-')
	{
		m_Sign = -1;	// признак отрицательного числа
		src_index = 1;	// само число начинается со второй позиции
	}
	else
	{
		m_Sign = 0;				// знака минус нет, значит число положительное

		// если знак плюс явно указан
		if (_digit[0] == '+')
		{
			src_index = 1;		// число начинается со второй позиции
		}
		else
		{
			src_index = 0;		// знак не указан, число начинается с первой позиции
		}
	}

	// преобразуем строковое число в формат для хранения
	while (src_index < m_Len)
	{
		m_Digit[dest_index] = _digit[src_index];

		src_index++;
		dest_index--;
	}

}


ostream &
operator<<(ostream &_stream, CLongInt &_a)
{

	return _stream;
}



CLongInt
CLongInt::operator+(CLongInt &_b)
{
	CLongInt a;
	return a;
}


int main() {
	return 0;
}
