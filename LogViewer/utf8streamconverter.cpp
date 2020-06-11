#include "utf8streamconverter.h"

void UTF8StreamConverter::addNewByte(const char c)
{
    if (cur_octet == 0)
    {
        if ((c & 0b10000000) == 0b00000000)
        {
            total_octet_count = 1;
            c1 = c & 0b01111111;
        }
        else if ((c & 0b11100000) == 0b11000000)
        {
            total_octet_count = 2;
            c1 = c & 0b00011111;
        }
        else
        {
            Q_ASSERT(0);
        }
    }
    else if (cur_octet == 1)
    {
        Q_ASSERT(total_octet_count > 1);

        if ((c & 0b11000000) == 0b10000000)
        {
            c2 = c & 0b00111111;
        }
        else
        {
            Q_ASSERT(0);
        }
    }
    else
    {
        Q_ASSERT(0);
    }

    cur_octet++;

    Q_ASSERT(total_octet_count>0);

    if (total_octet_count == cur_octet)
    {
        if (total_octet_count == 1)
        {
            cur_octet = 0;
            total_octet_count = 0;
            QChar ch = c1;
            emit newChar(ch);
        }
        else if (total_octet_count == 2)
        {
            cur_octet = 0;
            total_octet_count = 0;

            wchar_t wCh = c1;
            wCh <<=6;
            wCh |= c2;

            QChar ch = wCh;

            emit newChar(ch);
        }
        else
        {
            Q_ASSERT(0);
        }
    }
}
