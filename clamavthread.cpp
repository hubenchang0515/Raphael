#include "clamavthread.h"
#include <QtDebug>
#include <QtGlobal>
#include <QTime>

ClamAVThread::ClamAVThread(QObject *parent) :
    QObject(parent),
    engine(new ClamAVEngine(nullptr)),
    thread(new QThread(this))
{
    engine->moveToThread(thread);
    connect(this, &ClamAVThread::scan, engine, &ClamAVEngine::scan);
    connect(this, &ClamAVThread::globalScan, engine, &ClamAVEngine::globalScan);

    connect(engine, &ClamAVEngine::opened, this, &ClamAVThread::opened);
    connect(engine, &ClamAVEngine::closed, this, &ClamAVThread::closed);
    connect(engine, &ClamAVEngine::finished, this, &ClamAVThread::proxyFinished);

    connect(engine, &ClamAVEngine::detecting, this, &ClamAVThread::proxyDetecting);
    connect(engine, &ClamAVEngine::detected, this, &ClamAVThread::proxyDetected);
    connect(engine, &ClamAVEngine::message, this, &ClamAVThread::proxyMessage);
}

void ClamAVThread::start(const QUrl& path)
{
    QString file = path.toLocalFile();
    if(thread->isRunning() == false)
    {
        thread->start();
        if(path.isEmpty())
        {
            emit globalScan();
        }
        else
        {
            emit scan(file);
        }
    }
}

void ClamAVThread::cancel()
{
    thread->terminate();
    if(thread->wait())
    {
        emit abandon();
    }
}


void ClamAVThread::proxyDetecting(const QString& file)
{
    static QTime time;
    if(time.isValid() == false)
    {
        emit detecting(file);
        time.start();
    }
    else if(time.elapsed() > 1000/60)
    {
        emit detecting(file);
        time.restart();
    }
}

void ClamAVThread::proxyDetected(const QString& file, bool isSafe, const QString& virname)
{
    static QTime time;
    if(time.isValid() == false)
    {
        emit detected(file, isSafe, virname);
        time.start();
    }
    else if(time.elapsed() > 1000/60)
    {
        for(auto& data : detectedBuffer)
        {
            emit detected(data.file, data.isSafe, data.virusName);
        }
        detectedBuffer.clear();
        emit detected(file, isSafe, virname);
        emit sent();
        time.restart();
    }
    else
    {
        DetectedMessage data;
        data.file = file;
        data.isSafe = isSafe;
        data.virusName = virname;
        detectedBuffer.append(data);
    }
}

void ClamAVThread::proxyMessage(const QString& text)
{
    qDebug() << text;
    static QTime time;
    if(time.isValid() == false)
    {
        emit message(text);
        time.start();
    }
    else if(time.elapsed() > 1000/60)
    {
        for(auto& data : messageBuffer)
        {
            emit message(data);
        }
        messageBuffer.clear();
        emit message(text);
        emit sent();
        time.restart();
    }
    else
    {
        messageBuffer.append(text);
    }
}


void ClamAVThread::proxyFinished()
{
    if(messageBuffer.isEmpty() == false)
    {
        for(auto& data : messageBuffer)
        {
            emit message(data);
        }
        messageBuffer.clear();
        emit sent();
    }

    if(detectedBuffer.isEmpty() == false)
    {
        for(auto& data : detectedBuffer)
        {
            emit detected(data.file, data.isSafe, data.virusName);
        }
        detectedBuffer.clear();
        emit sent();
    }
    emit finished();
}
