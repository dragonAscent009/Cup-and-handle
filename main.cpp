#include <QApplication>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QtCharts/QScatterSeries>
#include "stock.h"

QT_CHARTS_USE_NAMESPACE

QList<Stock> initStockData(QFile &file);
void findMaxMin(QList<Stock> &stockData);
void writePattern(QList<Stock> stockData, QList<Stock> A, QList<Stock> B, QList<Stock> C, QList<Stock> D, QList<Stock> E, int p_no);
void userInpLoop(void);
void initPatterns();
void initACE();
void initBDK();
void drawChart(QList<Stock> stockData);

QList<Stock> maximas;
QList<Stock> minimas;

QList<Stock> K;QList<Stock> A;QList<Stock> C;QList<Stock> D;QList<Stock> E;
QList<Stock> B;

bool b_closing = false;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString filepath;
    filepath = QFileDialog::getOpenFileName(
            qApp->activeWindow(),
            QObject::tr( "Select Stock Data file to open:" ),
            "C:/Users/Pratik/CupWithHandle/Data"
            );
    qDebug() << filepath;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "File Not Found";
    QFileInfo fileInfo (file.fileName());

    QList<Stock> stockData;
    stockData = initStockData(file);
//    for(Stock stock : stockData)
//        stock.print();

    QLineSeries *series = new QLineSeries();
    for(Stock stock : stockData)
    {
        series->append(stock.index,stock.close);
    }
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, stockData.length());
    axisX->setTickCount(40);
    axisX->setLabelFormat("%d");

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAxisX(axisX);
    chart->setTitle(fileInfo.fileName());

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QMainWindow chartwindow;
    chartwindow.setCentralWidget(chartView);
    chartwindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    chartwindow.resize(1920, 580);
    chartwindow.show();

    findMaxMin(stockData);
//    qDebug() << "MINIMAS : ";
//    for(Stock stock : minimas) stock.print();
    qDebug() << "MINIMAS SIZE : " << minimas.length();
//    qDebug() << "MAXIMAS : " << ;
//    for(Stock stock : maximas) stock.print();
    qDebug() << "MAXIMAS SIZE : " << maximas.length();

    //        initPatterns();

    initACE();
    initBDK();

    for(Stock s : A) s.close = 0.0;
    for(Stock s : C) s.close = 0.0;
    for(Stock s : E) s.close = 0.0;

    for(Stock s : K) s.close = 9999.0;
    for(Stock s : B) s.close = 9999.0;
    for(Stock s : D) s.close = 9999.0;

//    for(Stock stock : B)
//        stock.print();

    QFile shapefiletodel("C:/Users/Pratik/Pattern.txt");
    shapefiletodel.remove();

//            DETECTION
    int x = 0;  //FOR MAXIMA
    int y = 0;  //FOR MINIMA
    int p_no = 0; //FOR PATTERN NO

    while(x<maximas.length() && (p_no < 6) /*&& (y<minimas.length())*/)
    {     
//      FINDING A

PEAK: while(maximas.at(x).close < maximas.at(x+1).close)
        {
            x++;
        }

        A.operator [](p_no).close = maximas.at(x).close;
        A.operator [](p_no).index = maximas.at(x).index;

        while((maximas.at(x).index - A.at(p_no).index)<23 /*&& (x<maximas.length())*/)
        {
            if(maximas.at(x).close > A.at(p_no).close)
            {
                A.operator [](p_no).close = maximas.at(x).close;
                A.operator [](p_no).index = maximas.at(x).index;
                goto PEAK;
            }x++;
        }

//      FINDING C

        while((maximas.at(x).index - A.at(p_no).index)<145 /*&& (x<maximas.length())*/)
        {
            if((maximas.at(x).close > (A.at(p_no).close*0.99))
             &&(maximas.at(x).close < (1.01*A.at(p_no).close)))
            {
                C.operator [](p_no).close = maximas.at(x).close;
                C.operator [](p_no).index = maximas.at(x).index;
                break;
            }
            x++;
        }

        if(C.at(p_no).close == 0.0)
        {
            A.operator [](p_no).close = maximas.at(x+1).close;
            A.operator [](p_no).index = maximas.at(x+1).index;
            goto PEAK;
        }

//      FINDING B

        while(minimas.at(y).index < A.at(p_no).index /*&& (y<minimas.length())*/)
        {
            y++;
        }
        y++;

        int bottom = 0;
        while(minimas.at(y).index < C.at(p_no).index)
        {
            if(B.at(p_no).close > minimas.at(y).close)
            {
                B.operator [](p_no).close = minimas.at(y).close;
                B.operator [](p_no).index = minimas.at(y).index;
                if(minimas.at(y).close < (0.8*A.at(p_no).close))
                    bottom++;
            }
            y++;
        }

        if((B.at(p_no).close == 999999.0) && (bottom < 3))
        {
            A.operator [](p_no).close = C.at(p_no).close;
            A.operator [](p_no).index = C.at(p_no).index;
            goto PEAK;
        }


//        FINDING E

        while((maximas.at(x).index-C.at(p_no).index) < 60 /*&& (x<maximas.length())*/)
        {
            if(maximas.at(x).close>C.at(p_no).close)
            {
                E.operator [](p_no).close = maximas.at(x).close;
                E.operator [](p_no).index = maximas.at(x).index;
                break;
            }
            x++;
        }

//        FINDING D

        while(minimas.at(y).index < C.at(p_no).index /*&& (y<minimas.length())*/)
            y++;
        y++;

        D.operator [](p_no).close = minimas.at(y).close;

        while(minimas.at(y).index < E.at(p_no).index /*&& (y<minimas.length())*/)
        {
            if((minimas.at(y).close < D.at(p_no).close)
                && D.at(p_no).close < C.at(p_no).close)
            {
                D.operator [](p_no).close = minimas.at(y).close;
                D.operator [](p_no).index = minimas.at(y).index;
            }
            y++;
        }

//        if(
//             (A.at(p_no).index !=0)
//           &&(B.at(p_no).index !=0)
//           &&(C.at(p_no).index !=0)
//           &&(D.at(p_no).index !=0)
//           &&(E.at(p_no).index !=0)
//          )
//        {
            qDebug () << "SHAPE #" << p_no <<" : ";
            qDebug ()<< "A : " <<A.at(p_no).close <<"A INDEX : "<< A.at(p_no).index;
            qDebug ()<< "B : " <<B.at(p_no).close <<"B INDEX : "<< B.at(p_no).index;
            qDebug ()<< "C : " <<C.at(p_no).close <<"C INDEX : "<< C.at(p_no).index;
            qDebug ()<< "D : " <<D.at(p_no).close <<"D INDEX : "<< D.at(p_no).index;
            qDebug ()<< "E : " <<E.at(p_no).close <<"E INDEX : "<< E.at(p_no).index;
            writePattern(stockData,A,B,C,D,E,p_no);
//        }
//        writePattern(stockData,A,B,C,D,E,p_no);
        p_no++;
    }
//    userInpLoop();
//    scattwindow.show();
    return a.exec();
}

void writePattern(QList<Stock> stockData , QList<Stock> A,QList<Stock> B,QList<Stock> C, QList<Stock> D, QList<Stock> E,int p_no)
{
    QString filename="C:/Users/Pratik/Pattern.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
    {
        QTextStream stream( &file );

        stream << A.at(p_no).index
               << " "
               << A.at(p_no).close
               << endl
               << B.at(p_no).index
               << " "
               << B.at(p_no).close
               << endl
               << C.at(p_no).index
               << " "
               << C.at(p_no).close
               << endl
               << D.at(p_no).index
               << " "
               << D.at(p_no).close
               << endl
               << E.at(p_no).index
               << " "
               << E.at(p_no).close
               << endl;


//        for( i = A.at(p_no).index ; i <= C.at(p_no).index ; i++ )
//            stream << stockData.at(i).timestamp
//                   << ","
//                   << stockData.at(i).open
//                   << ","
//                   << stockData.at(i).high
//                   << ","
//                   << stockData.at(i).low
//                   << ","
//                   << stockData.at(i).close
//                   << ","
//                   //<< stockData.at(i).index
//                   << endl;
    }
}

void printPatterns()
{

}

void initACE()
{
    int i = 0;
    Stock s;
    while(i<1000)
    {
        s.ticker = "PATTERN";
        s.open = 0.0;
        s.high = 0.0;
        s.low = 0.0;
        s.close = 0.0;
        s.volume = 0;
        s.index = 0;

        A.append(s);
        C.append(s);
        E.append(s);

        i++;
    }
}

void initBDK()
{
    int i = 0;
    Stock s;
    while(i<1000)
    {
        s.ticker = "PATTERN";
        s.open = 0.0;
        s.high = 0.0;
        s.low = 0.0;
        s.close = 999999.0;
        s.volume = 0;
        s.index = 0;

        B.append(s);
        D.append(s);
        K.append(s);

        i++;
    }
}

void findMaxMin(QList<Stock> &stockData)
{
    int i = 0;
    int x = 0;//MINIMAS
    int y = 0;//MAXIMAS
    while(i < stockData.length()-5)
    {
        while(stockData.at(i).close > stockData.at(i+1).close) i++;
        minimas.append(stockData.at(i));

        while(stockData.at(i).close<=stockData.at(i+1).close) i++;
        maximas.append(stockData.at(i));
    }
}


QList<Stock> initStockData(QFile &file)
{
    Stock s;
    QList<Stock> stockData;
    QTextStream in(&file);
    QString line;
    QStringList splitline;
    int i = 1;
    while (!in.atEnd())
    {
        line = in.readLine();
        splitline = line.split(",");
        s.ticker = splitline.at(0);
        s.datetime = QDateTime::fromString(splitline.at(1),"mm/dd/yyyy");
        s.timestamp = s.datetime.toTime_t();
        s.open = splitline.at(2).toFloat();
        s.high = splitline.at(3).toFloat();
        s.low = splitline.at(4).toFloat();
        s.close = splitline.at(5).toFloat();
        s.volume = splitline.at(6).toInt();
        s.index = i++;
        stockData.append(s);
        s.init();
    }

    return stockData;
}


void drawChart(QList<Stock> stockData)
{

}


void userInpLoop ( void )
{
    QFile    cons_inp;
    QFile    cons_outp;
    QString  s_ln;

    cons_inp.open( stdin, QIODevice::ReadOnly );
    cons_outp.open( stdout, QIODevice::WriteOnly );

    for ( ;; )
    {
        if ( b_closing )
            break;

        cons_outp.write( "\n>" );
        cons_outp.flush();
        s_ln = cons_inp.readLine().trimmed();

        if ( s_ln == "q" )
        {
            b_closing = true;
            cons_outp.write( "Closing...\n" );
        }
        else
        {
            cons_outp.write( "ERROR!!! \nInvalid input!\n" );
        }
        cons_outp.flush();
        //break; /* just to test that a.exec() hides the dialog */
    }

}
