#ifndef SILUBIUMVERSIONCHECKER_H
#define SILUBIUMVERSIONCHECKER_H

#include <QObject>

//#define SILUBIUM_RELEASES "https://github.com/silubiumproject/silubium/releases"
//#define SILUBIUM_RELEASES "http://172.16.0.201/index.html"
//#define SILUBIUM_URL "http://172.16.0.201/silubium-"
#define SILUBIUM_RELEASES "http://update.silubium.org/index.html"
#define SILUBIUM_URL "http://update.silubium.org/silubium-"

class Version {

public:
    int _major;
    int _minor;
    int _revision;
    int _compiles;

    Version(){
        SetNull();
    }

    Version(int maj, int min, int rev,int compiles){
        SetNull();

        _major = maj;
        _minor = min;
        _revision = rev;
        _compiles = compiles;
    }

    Version(QString str){
        SetNull();

        QStringList parts = str.split(".");

        if(!parts.isEmpty())
            _major = parts[0].toInt();
        if(parts.length() > 1)
            _minor = parts[1].toInt();
        if(parts.length() > 2)
            _revision = parts[2].toInt();
        if(parts.length() > 3)
            _compiles = parts[3].toInt();
    }

    Version(const Version &v){
        _major = v._major;
        _minor = v._minor;
        _revision = v._revision;
        _compiles = v._compiles;
    }

    bool operator >(const Version& other) const
    {
        return compareAll(other) > 0;
    }

    bool operator <(const Version& other) const
    {
        return compareAll(other) < 0;
    }

    bool operator ==(const Version& other) const
    {
        return compareAll(other) == 0;
    }

    void SetNull()
    {
        _major = 0;
        _minor = 0;
        _revision = 0;
        _compiles = 0;
    }

private:
    int compare(int first, int second) const
    {
        int diff = first - second;
        return diff > 0 ? 1 : diff < 0 ? -1 : 0;
    }
    int compareAll(const Version& other) const
    {
        return 8 * compare(_major, other._major) +
               4 * compare(_minor, other._minor) +
               2 * compare(_revision, other._revision) +
               compare(_compiles,other._compiles);
    }
};

class SilubiumVersionChecker : public QObject
{
    Q_OBJECT
public:
    explicit SilubiumVersionChecker(QObject *parent = 0);
    ~SilubiumVersionChecker();

    bool newVersionAvailable();
    bool isWin();

private:
    QList<Version> getVersions();
    QString getPlatform();
    Version getMaxReleaseVersion();

    Version currentVersion;
};

#endif // SILUBIUMVERSIONCHECKER_H
