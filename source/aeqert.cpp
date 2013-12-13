#include <iostream>
#include <cstring>

using namespace std;

#define		DEFAULT_LEN		32

class CLongInt
{
	char * m_Digit;		// ������������������ ���� � ������� little-endian (1 ���� - 1 �����)
	int m_Len;			// ����� ����� � ������
	int m_Sign;			// ���� �����

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

	// ������ ������ ������� ����� ����������� �����
	int dest_index = m_Len - 1;

	// ������ ������ ������� ����� ��������� � ���� ������ �����
	int src_index;

	// ���� � ������ ������ ������ �����
	if (_digit[0] == '-')
	{
		m_Sign = -1;	// ������� �������������� �����
		src_index = 1;	// ���� ����� ���������� �� ������ �������
	}
	else
	{
		m_Sign = 0;				// ����� ����� ���, ������ ����� �������������

		// ���� ���� ���� ���� ������
		if (_digit[0] == '+')
		{
			src_index = 1;		// ����� ���������� �� ������ �������
		}
		else
		{
			src_index = 0;		// ���� �� ������, ����� ���������� � ������ �������
		}
	}

	// ����������� ��������� ����� � ������ ��� ��������
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
