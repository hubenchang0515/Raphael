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

signals:
    void progress(qlonglong readLength);
    void contentLength(qlonglong length);
    void finished();
    void canceled();
    void defeated(QNetworkReply::NetworkError code);

public slots:
    void download(QUrl url, QString local);
    void getContent();
    void read();
    void finish();
    void cancel();
    void defeat(QNetworkReply::NetworkError code);

private:
    QNetworkAccessManager* network;
    QNetworkReply* reply;
    QUrl url;
    QString local;
    qlonglong readLength;
    qlonglong length;
    QFile file;
    bool isDefeated;
};

#endif // DOWNLOADER_H
