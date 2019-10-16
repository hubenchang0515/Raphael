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
    connect(this, &ClamAVThread::detect, engine, &ClamAVEngine::start);
    connect(engine, &ClamAVEngine::detecting, this, &ClamAVThread::proxyDetecting);
}

void ClamAVThread::start(const QString& path)
{
    if(thread->isRunning() == false)
    {
        thread->start();
        time.start();
        emit detect(path);
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
    if(time.elapsed() > 1000/60)
    {
        emit detecting(file);
        time = QTime::currentTime();
        time.restart();
    }
}
