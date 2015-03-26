#include "dialog_ware_name.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    rele_auto::dialog_ware_name w;
    w.show();

    return a.exec();
}
