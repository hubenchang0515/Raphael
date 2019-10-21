#include "downloader.h"
#include <QDebug>
#include <QFile>

Downloader::Downloader(QObject *parent) :
    QObject(parent),
    network(new QNetworkAccessManager(this)),
    reply(nullptr),
    url(""),
    length(0)
{

}

void Downloader::download(QUrl url, QString local)
{
    isDefeated = false;
    QNetworkRequest request;
    request.setUrl(url);
    this->url = url;
    this->local = local;
    reply = network->head(request);
    connect(reply, &QNetworkReply::finished, this, &Downloader::getContent);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &Downloader::defeated);
}

void Downloader::getContent()
{
    if(isDefeated)
    {
        return;
    }
    length = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    qDebug() << length << "will be download.";
    emit contentLength(length);
    reply->deleteLater();
    reply = nullptr;

    QNetworkRequest request;
    request.setUrl(url);
    reply = network->get(request);
    connect(reply, &QNetworkReply::readyRead, this, &Downloader::read);
    connect(reply, &QNetworkReply::finished, this, &Downloader::finish);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &Downloader::defeat);

    file.setFileName(local+".download");
    qDebug() << file.open(QIODevice::WriteOnly);
    readLength = 0;
}

void Downloader::read()
{
    auto data = reply->readAll();
    readLength += data.length();
    emit progress(readLength);
    file.write(data);
    file.flush();
}

void Downloader::finish()
{
    if(isDefeated)
    {
        return;
    }
    file.close();
    reply->deleteLater();
    reply = nullptr;
    QFile::remove(local);
    QFile::rename(local + ".download", local);
    emit finished();
    qDebug() << local << " finished";
}

void Downloader::cancel()
{
    if(reply != nullptr)
    {
        reply->deleteLater();
        reply = nullptr;
    }
    file.close();
    emit canceled();
    qDebug() << local << " canceled";
}

void Downloader::defeat(QNetworkReply::NetworkError code)
{
    isDefeated = true;
    file.close();
    reply->deleteLater();
    reply = nullptr;
    emit defeated(code);
    qDebug() << local << " defeated";
}
