#include <QCoreApplication>

#include "parser_impl.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(new JsonParser);
    server.startServer();

    return a.exec();
}
