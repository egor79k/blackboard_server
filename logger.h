#ifndef LOGGER_H
#define LOGGER_H

#include <QTextBrowser>
#include <QString>
#include <QDebug>

class Logger
{
public:
    virtual ~Logger();
    virtual Logger &operator<<(const QString &str) = 0;
};


class TextBrowser_Logger : public Logger
{
public:
    TextBrowser_Logger (QTextBrowser *_text);

    Logger &operator<<(const QString &str) override;

private:
    QTextBrowser *text;
};

#endif // LOGGER_H
