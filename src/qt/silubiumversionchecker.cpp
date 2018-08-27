#include "silubiumversionchecker.h"
#include "../clientversion.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QSysInfo>

#define paternVersion "silubium-([0-9]+\\.)?([0-9]+\\.)?([0-9]+\\.)?([0-9]+)-"

SilubiumVersionChecker::SilubiumVersionChecker(QObject *parent) : QObject(parent)
{
    currentVersion = Version(CLIENT_VERSION_MAJOR, CLIENT_VERSION_MINOR, CLIENT_VERSION_REVISION,CLIENT_VERSION_BUILD);
}

SilubiumVersionChecker::~SilubiumVersionChecker()
{

}

bool SilubiumVersionChecker::newVersionAvailable()
{
    Version maxReleaseVersion = getMaxReleaseVersion();
    return maxReleaseVersion > currentVersion;
}

bool SilubiumVersionChecker::isWin()
{
    if(getPlatform()=="win32e" | getPlatform()=="win64e")
        return true;
    else
        return false;
}

QList<Version> SilubiumVersionChecker::getVersions()
{
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(SILUBIUM_RELEASES)));
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString html = response->readAll();

    QRegularExpression regEx(paternVersion);
    QRegularExpressionMatchIterator regExIt = regEx.globalMatch(html);

    QList<Version> versions;

    while (regExIt.hasNext()) {
        QRegularExpressionMatch match = regExIt.next();
        QString versionString = match.captured().mid(9, match.captured().length() - 10); //.mid(5, match.captured().length() - 6);// get version string in format XX.XX.XX
        Version version(versionString);
        if(!versions.contains(version))
        {
            versions.append(version);
        }
    }
    return versions;
}

QString SilubiumVersionChecker::getPlatform()
{
    QSysInfo info;
    QString strArch;
    if(info.buildCpuArchitecture()=="i686" |  info.buildCpuArchitecture()=="i386")
    {
        if(info.kernelType()=="linux")
            return "linux32t";
        else if(info.kernelType()=="winnt")
            return "win32e";
    }
    if(info.buildCpuArchitecture()=="x86_64")
    {
        if(info.kernelType()=="linux")
            return "linux64t";
        else if(info.kernelType()=="winnt")
            return "win64e";
    }
    return "unknown";
}

Version SilubiumVersionChecker::getMaxReleaseVersion()
{
    QList<Version> versions = getVersions();
    Version maxVersion;

    if(!versions.isEmpty())
    {
        maxVersion = *std::max_element(versions.begin(), versions.end());
    }
    return maxVersion;
}
