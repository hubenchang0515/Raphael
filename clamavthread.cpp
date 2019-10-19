#include "clamavthread.h"
#include <QtDebug>
#include <QtGlobal>
#include <QTimer>

ClamAVThread::ClamAVThread(QObject *parent) :
    QObject(parent),
    engine(new ClamAVEngine(nullptr)),
    thread(new QThread(this)),
    timer(new QTimer(this))
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

    timer->setInterval(1000/60);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, &ClamAVThread::updateMessage);
    timer->start();
}

void ClamAVThread::start(const QUrl& path)
{
    QString file = path.toLocalFile();
    qDebug() << "start to scan " + file;
    if(thread->isRunning() == false)
    {
        thread->start();
    }

    if(path.isEmpty())
    {
        emit globalScan();
    }
    else
    {
        emit scan(file);
    }
}

void ClamAVThread::cancel()
{
    thread->terminate();
    if(thread->wait())
    {
        qDebug() << thread->isRunning();
        emit abandon();
    }
}


void ClamAVThread::proxyDetecting(const QString& file)
{
    current = file;
}

void ClamAVThread::proxyDetected(const QString& file, bool isSafe, const QString& virname)
{
    DetectedMessage data;
    data.file = file;
    data.isSafe = isSafe;
    data.virusName = virname;
    detectedBuffer.append(data);
}

void ClamAVThread::proxyMessage(const QString& text)
{
    messageBuffer.append(text);
}


void ClamAVThread::proxyFinished()
{
    emit finished();
    thread->terminate();
    thread->wait();
}


void ClamAVThread::updateMessage()
{
    if(current.isEmpty() == false)
    {
        emit detecting(current);
        current.clear();
    }


    if(messageBuffer.isEmpty() && detectedBuffer.isEmpty())
    {
        return;
    }
    for(auto& data : messageBuffer)
    {
        emit message(data);
    }
    messageBuffer.clear();

    for(auto& data : detectedBuffer)
    {
        emit detected(data.file, data.isSafe, data.virusName);
    }
    detectedBuffer.clear();
    emit sent();
}
