// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"


TBitField::TBitField(int len)
{
    if (len > 0)
    {
        BitLen = len;
        MemLen = ((len + BitsInElem - 1) >> ShiftSize);
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    }

    else if (len == 0)
    {
        BitLen = 0;
        MemLen = 0;
        pMem = nullptr;
    }
    else {
        throw "Error: ";
    }
        
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    if (MemLen > 0)
        delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> ShiftSize;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & (BitsInElem - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0) || (n >= BitLen))
        throw "error";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen))
        throw "error";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen))
        throw "error";
    if ((pMem[GetMemIndex(n)] | GetMemMask(n)) == pMem[GetMemIndex(n)])
        return 1;
    else
        return 0;
}

// битовые операции

const TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (MemLen != bf.MemLen)
        if (MemLen > 0)
        {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }
            

    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return(*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return false;
    bool flag = true;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
        {
            flag = false;
            break;
        }
    }
    return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !((*this) == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len;
    if (bf.BitLen >= BitLen)
        len = bf.BitLen;
    else
        len = BitLen;

    TBitField res(len);

    for (int i = 0; i < res.MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int j = 0; j < bf.MemLen; j++)
        res.pMem[j] |= bf.pMem[j];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len;
    if (bf.BitLen >= BitLen)
        len = bf.BitLen;
    else
        len = BitLen;

    TBitField res(len);

    for (int i = 0; i < res.MemLen; i++)
        res.pMem[i] = pMem[i];
    for (int j = 0; j < bf.MemLen; j++)
        res.pMem[j] &= bf.pMem[j];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
        if (!GetBit(i)) res.SetBit(i);
    return res;

}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++)
        bf.pMem[i] = 0;

    string str;
    istr >> str;
    int len = (str.size() < bf.BitLen) ? str.size() : bf.BitLen;
    for (int i = 0; i < len; i++)
    {
        if (str[i] == '1')
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
