#include "dialog.h"
#include <QApplication>

#define UH_TOOL_VERSION        "v1.1"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}
