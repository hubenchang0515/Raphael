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

//void Downloader::moveToThread(QThread* th)
//{
//    QObject::moveToThread(th);
//    network->moveToThread(th);
//}

void Downloader::download(QUrl url, QString local)
{
    QNetworkRequest request;
    request.setUrl(url);
    this->url = url;
    this->local = local;
    reply = network->head(request);
    connect(reply, &QNetworkReply::finished, this, &Downloader::getContent);
}

void Downloader::getContent()
{
    length = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    qDebug() << length << "will be download.";
    emit contentLength(length);
    reply->deleteLater();
    reply = nullptr;

    QNetworkRequest request;
    request.setUrl(url);
    reply = network->get(request);
    connect(reply, &QNetworkReply::readyRead, this, &Downloader::read);
    connect(reply, &QNetworkReply::finished, this, &Downloader::save);

    file.setFileName(local);
    qDebug() << file.open(QIODevice::WriteOnly);
    readLength = 0;
}

void Downloader::read()
{
    auto data = reply->readAll();
    readLength += data.length();
    emit progress(readLength);
    file.write(data);
}

void Downloader::save()
{
    file.close();
    reply->deleteLater();
    reply = nullptr;
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
