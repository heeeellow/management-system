#include "my_system.h"
#include <QtWidgets/QApplication>
#include "Login.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    my_system w;
    LoginD d;
   if (d.exec() == QDialog::Accepted) {
        w.show();
        w.dis_information();
        return a.exec();
    }
    return 0;
}
