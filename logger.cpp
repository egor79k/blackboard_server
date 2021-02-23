#include "logger.h"


Logger::~Logger() {}

//=============================================================================
// TextBrowser_Logger
//=============================================================================

TextBrowser_Logger::TextBrowser_Logger (QTextBrowser *_text) :
    text(_text)
{}
//_____________________________________________________________________________

Logger &TextBrowser_Logger::operator<<(const QString &str)
{
    text->append(str);
    return *this;
}
//=============================================================================
