#include "stock.h"
#include <QDebug>
#include <QFile>
Stock::Stock()
{

}


void Stock::init()
{
    this->ticker = "NULL";
    this->open = 0;
    this->high = 0;
    this->low = 0;
    this->close = 0;
    this->volume = 0;
}

void Stock::print()
{
//    qDebug() << "TICKER"<<"\t"
//             << "OPEN"<<"\t"
//             << "HIGH"<<"\t"
//             << "LOW\t"
//             << "CLOSE\t"
//             << "VOLUME\t"
//             << "INDEX";

    qDebug() << this->ticker<<"\t"
             << QString::number( this->open, 'f', 2 )<<"\t"
             << QString::number( this->high, 'f', 2 )<<"\t"
             << QString::number( this->low, 'f', 2 )<<"\t"
             << QString::number( this->close, 'f', 2 )<<"\t"
             << QString::number( this->volume, 'f', 2 )<<"\t"
             << this->index;
}
