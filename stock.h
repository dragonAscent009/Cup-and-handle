#ifndef STOCK_H
#define STOCK_H

#include <QString>
#include <QDate>
#include <QFile>

#define URPV 1
#define DRPV 0
class Stock
{
    public:

        QString ticker;
        QDate date;
        QDateTime datetime;
        uint timestamp;
        float open;
        float high;
        float low;
        float close;
        int volume;
        int RPV;
        int UDRPV;
        int index;
        Stock();
        void init();
        void print();

};

#endif // STOCK_H
