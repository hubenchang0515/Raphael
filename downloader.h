#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
//    void moveToThread(QThread* th);

signals:
    void progress(qlonglong readLength);
    void contentLength(qlonglong length);
    void finished();
    void canceled();

public slots:
    void download(QUrl url, QString local);
    void getContent();
    void read();
    void save();
    void cancel();

private:
    QNetworkAccessManager* network;
    QNetworkReply* reply;
    QUrl url;
    QString local;
    qlonglong readLength;
    qlonglong length;
    QFile file;
};

#endif // DOWNLOADER_H
