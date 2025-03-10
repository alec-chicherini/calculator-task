#include <QApplication>
#include <QIcon>
#include <WidgetApplication.h>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    app.setWindowIcon(QIcon::fromTheme("calculator-task"));
    WidgetCalculator application;
    application.show();

    return app.exec();
}
