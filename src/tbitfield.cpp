// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
     if (len < 0)
        throw "Err";
    BitLen = len;
     MemLen = BitLen / 32 + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}


TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    if (bf.BitLen <= 0)
    {
        throw "Err";
    }

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM res = 1;
    res = res << n % 32;
    return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw "Err";
    }
        int i = GetMemIndex(n);
    TELEM m = GetMemMask(n);
    pMem[i] = pMem[i] | m;

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
    {
        throw "Err";
    }
    int i = GetMemIndex(n);
    TELEM m = ~GetMemMask(n);
    pMem[i] = pMem[i] & m;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen))
    {
        throw "Err";
    }
    int i = GetMemIndex(n);
    TELEM m = GetMemMask(n);
    return (pMem[i] & m);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (bf.MemLen != MemLen) 
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    int res = 1;
    for (int i = 0; i < MemLen - 1; i++) 
    {
        if (pMem[i] != bf.pMem[i])
        {
            res = 0;
        }
    }
    for (int i = (MemLen - 1) * 32; i < BitLen; i++)
    {
        if (GetBit(i) != bf.GetBit(i)) 
        {
            res = 0;
        }
    }
    return res;
}


int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    int res = 1;
    if (&bf == this) 
    {
        res = 0;
    }
    return res;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int len = BitLen;


    if (bf.BitLen > len) 
    {
        len = bf.BitLen;
    }
    TBitField temp(len);
    for (int i = 0; i < MemLen; i++)
    {
        temp.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++)
    {
        temp.pMem[i] |= bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int len = BitLen;
    if (bf.BitLen > len) 
    {
         len = bf.BitLen;
    }
    TBitField temp(len);
    for (int i = 0; i < MemLen; i++)
    {
        temp.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++)
    {
        temp.pMem[i] &= bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    int len = BitLen;
        TBitField temp(len);
    for (int i = 0; i < MemLen - 1; i++)
    {
        temp.pMem[i] = ~pMem[i];

    }
    for (int i = (MemLen - 1) * 32; i < BitLen; i++) 
    {
        if (!GetBit(i))
        {
            temp.SetBit(i);
        }
    }
    return temp;
}

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    TELEM bit;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> bit;

        if (i / 32 == 0)
        {
            bf.pMem[bf.GetMemIndex(i / 32)] |= bit;
        }
        else {
            bf.pMem[bf.GetMemIndex(i / 32)] <<= 1;
            bf.pMem[bf.GetMemIndex(i / 32)] |= bit;
        }
    }

    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i) != 0) 
        {
            ostr << 1 << " ";
        }
        else 
        {
            ostr << 0 << " ";
        }
    }
    return ostr;
}