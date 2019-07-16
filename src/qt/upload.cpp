#include <qt/upload.h>
#include <qt/forms/ui_upload.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QString>
#include <QSysInfo>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <util.h>
#define paternVersion "silubium-([0-9]+\\.)?([0-9]+\\.)?([0-9]+\\.)?([0-9]+)-"


QUpload::QUpload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QUpload)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    currentVersion = Version(2,15,2,9);//Version(CLIENT_VERSION_MAJOR, CLIENT_VERSION_MINOR, CLIENT_VERSION_REVISION,CLIENT_VERSION_BUILD);
    ui->lblUploadFile->setText(getLocalName());
    on_btnOk_clicked();
    httpRequestAbort=false;
}

QUpload::~QUpload()
{
    delete ui;
}

bool QUpload::newVersionAvailable()
{
    Version maxReleaseVersion = getMaxReleaseVersion();
    return maxReleaseVersion > currentVersion;
}

bool QUpload::isWin()
{
    if(getPlatform()=="win32e" | getPlatform()=="win64e")
        return true;
    else
        return false;
}

QList<Version> QUpload::getVersions()
{
    QNetworkAccessManager manager;

    QNetworkReply *response;
    if(!gArgs.IsArgSet("-testnet"))
        response= manager.get(QNetworkRequest(QUrl(SILUBIUM_RELEASES)));
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


QString QUpload::getLocalName()
{
    QString strPlatform=getPlatform();
    QString strVersion=QString::number(getMaxReleaseVersion()._major)+"."+ \
            QString::number(getMaxReleaseVersion()._minor)+"."+ \
            QString::number(getMaxReleaseVersion()._revision)+"."+ \
            QString::number(getMaxReleaseVersion()._compiles);
    //    QString strLocalDir=QApplication::applicationDirPath()+"/Updater/silubium-";
    QString strLocalDir="silubium-";
    if(strPlatform=="linux64t")
        return strLocalDir+strVersion+"-x86_64.tar.gz";
    else if(strPlatform=="win64e")
        return strLocalDir+strVersion+"-Win64.exe";
    else if(strPlatform=="win32e")
        return strLocalDir+strVersion+"-Win32.exe";
    else
        return "unknown";
}

QString QUpload::getNetName()
{
    QString strPlatform=getPlatform();
    QString strVersion=QString::number(getMaxReleaseVersion()._major)+"."+ \
            QString::number(getMaxReleaseVersion()._minor)+"."+ \
            QString::number(getMaxReleaseVersion()._revision)+"."+ \
            QString::number(getMaxReleaseVersion()._compiles);
    if(strPlatform=="linux64t")
        return SILUBIUM_URL+strVersion+"-x86_64.tar.gz";
    else if(strPlatform=="win64e")
        return SILUBIUM_URL+strVersion+"-Win64.exe";
    else if(strPlatform=="win32e")
        return SILUBIUM_URL+strVersion+"-Win32.exe";
    else
        return "unknown";
}

int QUpload::getSize()
{
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(SILUBIUM_RELEASES)));
    QEventLoop event;
    int filelen=0;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString html = response->readAll();
    QString strPaternSize=QString("<td id=\"%1\" align=right size=\"(.*?)\">").arg(getPlatform());
    QRegularExpression regEx(strPaternSize);
    QRegularExpressionMatchIterator regExIt = regEx.globalMatch(html);
    while (regExIt.hasNext()) {
        QRegularExpressionMatch match = regExIt.next();
        filelen=match.captured(1).toInt();
    }
    return filelen;
}

QString QUpload::getPlatform()
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

Version QUpload::getMaxReleaseVersion()
{
    QList<Version> versions = getVersions();
    Version maxVersion;

    if(!versions.isEmpty())
    {
        maxVersion = *std::max_element(versions.begin(), versions.end());
    }
    return maxVersion;
}

void QUpload::onProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setValue(bytesReceived);
}

void QUpload::on_btnOk_clicked()
{
    if(ui->btnOk->text()==tr("stop"))
    {
        reply->abort();//响应终止
//        httpRequestAbort=true;
        ui->btnOk->setText(tr("Start"));
        ui->btnQuit->setEnabled(true);
    }
    else
    {
        int filesize=getSize();
        QString strRemoteFile=getNetName();
        QDir dir(QApplication::applicationDirPath());
        if(!dir.exists("Updater"))
            dir.mkdir("Updater");
        strLocalFile=QApplication::applicationDirPath()+"/Updater/"+getLocalName();
        ui->progressBar->setMaximum(filesize);
        ui->progressBar->setValue(0);

        QUrl url(strRemoteFile);
        file=new QFile(strLocalFile);
        file->open(QIODevice::WriteOnly);//只读方式打开文件
        accessManager=new QNetworkAccessManager(this);
        request.setUrl(url);

        reply=accessManager->get(request);//通过发送数据，返回值保存在reply指针里.
        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
        connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
        ui->btnOk->setText(tr("Stop"));
//        ui->btnQuit->setEnabled(false);
    }


}

/***************响应结束**************************/
void QUpload::replyFinished(QNetworkReply *reply){
    //获取响应的信息，状态码为200表示正常
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QByteArray bytes = reply->readAll();  //获取字节
    QString result(bytes);  //转化为字符串
    qDebug()<<result;

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        ui->lblDisplay->setText(tr("Success"));
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        ui->btnOk->hide();
        ui->btnQuit->setEnabled(true);
        ui->btnQuit->setText(tr("StartUpdate"));
        httpRequestAbort=true;
        qDebug()<<result;
    }
    else
    {
        //处理错误
        ui->lblDisplay->setText(tr("Failed"));
        ui->btnOk->setText(tr("Start"));
    }
    file->close();
    reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;

}

/***********更新进度条*************/
void QUpload::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal){
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesSent);

}

void QUpload::onReadyRead(){

    file->write(reply->readAll());
    ui->lblDisplay->setText(tr("Downloading..."));
}

void QUpload::on_btnQuit_clicked()
{
    if(ui->btnOk->isHidden())
    {
        if(QFile::exists(strLocalFile))
        {
            QProcess::startDetached(strLocalFile);
            QDialog::accept();
        }
        else
            QDialog::reject();
    }
    else
        QDialog::reject();
//    this->close();
}
