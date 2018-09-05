#ifndef UPLOAD_H
#define UPLOAD_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include "silubiumversionchecker.h"

namespace Ui {
class QUpload;
}

//#define SILUBIUM_RELEASES "http://update.silubium.org/index.html"
//#define SILUBIUM_URL "http://update.silubium.org/silubium-"

//class Version{
//public:
//    int _major;
//    int _minor;
//    int _revision;
//    int _compiles;

//    Version(){
//        SetNull();
//    }

//    Version(int maj, int min, int rev,int compiles){
//        SetNull();

//        _major = maj;
//        _minor = min;
//        _revision = rev;
//        _compiles = compiles;
//    }

//    Version(QString str){
//        SetNull();

//        QStringList parts = str.split(".");

//        if(!parts.isEmpty())
//            _major = parts[0].toInt();
//        if(parts.length() > 1)
//            _minor = parts[1].toInt();
//        if(parts.length() > 2)
//            _revision = parts[2].toInt();
//        if(parts.length() > 3)
//            _compiles = parts[3].toInt();
//    }

//    Version(const Version &v){
//        _major = v._major;
//        _minor = v._minor;
//        _revision = v._revision;
//        _compiles = v._compiles;
//    }

//    bool operator >(const Version& other) const
//    {
//        return compareAll(other) > 0;
//    }

//    bool operator <(const Version& other) const
//    {
//        return compareAll(other) < 0;
//    }

//    bool operator ==(const Version& other) const
//    {
//        return compareAll(other) == 0;
//    }

//    void SetNull()
//    {
//        _major = 0;
//        _minor = 0;
//        _revision = 0;
//        _compiles = 0;
//    }

//private:
//    int compare(int first, int second) const
//    {
//        int diff = first - second;
//        return diff > 0 ? 1 : diff < 0 ? -1 : 0;
//    }
//    int compareAll(const Version& other) const
//    {
//        return 8 * compare(_major, other._major) +
//                4 * compare(_minor, other._minor) +
//                2 * compare(_revision, other._revision) +
//                compare(_compiles,other._compiles);
//    }
//};


class QUpload : public QDialog
{
    Q_OBJECT

public:
    explicit QUpload(QWidget *parent = 0);
    ~QUpload();

    bool newVersionAvailable();
    bool isWin();

private Q_SLOTS:
    void on_btnOk_clicked();
    void onProgress(qint64 bytesReceived, qint64 bytesTotal);
    void replyFinished(QNetworkReply*reply);
    void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);
    void onReadyRead();//
    void on_btnQuit_clicked();

private:
    Ui::QUpload *ui;
    QList<Version> getVersions();
    QString getPlatform();
    Version getMaxReleaseVersion();
    int getSize();

    QString getNetName();
    QString getLocalName();

    Version currentVersion;
    QFile *file;
    QFileInfo fileInfo;
    QNetworkAccessManager *accessManager;
    QNetworkRequest request;
    QNetworkReply *reply;
    bool httpRequestAbort; //请求停止的标志位,false表示允许请求
    QString strLocalFile;
};

#endif // UPLOAD_H
