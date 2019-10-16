#include "clamavthread.h"

ClamAVThread::ClamAVThread(QObject *parent) :
    QObject(parent),
    engine(new ClamAVEngine(nullptr)),
    thread(new QThread(this))
{
    engine->moveToThread(thread);
    thread->start();

    connect(this, &ClamAVThread::detect, engine,
            static_cast<bool(ClamAVEngine::*)(const QString&)>(&ClamAVEngine::detect));
}
