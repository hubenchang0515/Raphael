#include "updater.h"
#include <QtGlobal>
#include <QCoreApplication>
#include <QDir>

Updater::Updater(QObject *parent) :
    QObject(parent),
    main(new Downloader(nullptr)),
    daily(new Downloader(nullptr)),
    bytecode(new Downloader(nullptr)),
    thread(new QThread(nullptr)),
    isMainFinished(false),
    isDailyFinished(false),
    isBytecodeFinished(false)
{
    main->moveToThread(thread);
    daily->moveToThread(thread);
    bytecode->moveToThread(thread);

    connect(this, &Updater::startMain, main, &Downloader::download);
    connect(this, &Updater::startDaily, daily, &Downloader::download);
    connect(this, &Updater::startBytecode, bytecode, &Downloader::download);

    connect(main, &Downloader::contentLength, this, &Updater::mainLength);
    connect(daily, &Downloader::contentLength, this, &Updater::dailyLength);
    connect(bytecode, &Downloader::contentLength, this, &Updater::bytecodeLength);

    connect(main, &Downloader::progress, this, &Updater::mainProgress);
    connect(daily, &Downloader::progress, this, &Updater::dailyProgress);
    connect(bytecode, &Downloader::progress, this, &Updater::bytecodeProgress);

    connect(main, &Downloader::finished, this, &Updater::mainFinished);
    connect(daily, &Downloader::finished, this, &Updater::dailyFinished);
    connect(bytecode, &Downloader::finished, this, &Updater::bytecodeFinished);

    connect(main, &Downloader::defeated, this, &Updater::mainDefeated);
    connect(daily, &Downloader::defeated, this, &Updater::dailyDefeated);
    connect(bytecode, &Downloader::defeated, this, &Updater::bytecodeDefeated);

    connect(this, &Updater::cancel, main, &Downloader::cancel);
    connect(this, &Updater::cancel, daily, &Downloader::cancel);
    connect(this, &Updater::cancel, bytecode, &Downloader::cancel);
}


void Updater::update()
{
    if(thread->isRunning() == false)
    {
        thread->start();
    }

    isMainFinished = false;
    isDailyFinished = false;
    isBytecodeFinished = false;

    QString dirPath = qApp->applicationDirPath() + "/CVD";
    QDir dir(dirPath);
    if(dir.exists() == false && dir.mkpath(dirPath))
    {
        emit mainDefeated(QNetworkReply::NoError);
        emit dailyDefeated(QNetworkReply::NoError);
        emit bytecodeDefeated(QNetworkReply::NoError);
        qDebug() << dirPath << "not exists.";
        return;
    }
    emit startMain(QUrl("http://database.clamav.net/main.cvd"), dirPath + "/main.cvd");
    emit startDaily(QUrl("http://database.clamav.net/daily.cvd"), dirPath + "/daily.cvd");
    emit startBytecode(QUrl("http://database.clamav.net/bytecode.cvd"), dirPath + "/bytecode.cvd");
}

void Updater::stop()
{
    emit cancel();
    QThread::usleep(100000);
    thread->terminate();
    thread->wait();
}
