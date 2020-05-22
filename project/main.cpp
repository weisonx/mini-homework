#include "calculator.h"
#include "mainwidget.h"
#include <QApplication>


using std::vector;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    mainWidget w;
    w.setWindowTitle(QObject::tr("计算三元一次方程组"));
    w.show();

    return a.exec();
}
