#include "textstreamlinebuilder.h"

void TextStreamLineBuilder::addSymbol(const QChar & c)
{
    if (c == '\r' || c == '\n')
    {
        if (!current_line.isEmpty())
        {
            emit newLine(current_line);
            current_line = "";
        }
    }
    else
    {
        current_line.append(c);
    }
}
