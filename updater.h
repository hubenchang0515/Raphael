#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QThread>
#include <downloader.h>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr);

signals:
    void mainProgress(qlonglong);
    void dailyProgress(qlonglong);
    void bytecodeProgress(qlonglong);

    void mainLength(qlonglong);
    void dailyLength(qlonglong);
    void bytecodeLength(qlonglong);

    void mainFinished();
    void dailyFinished();
    void bytecodeFinished();

    void startMain(QUrl, QString);
    void startDaily(QUrl, QString);
    void startBytecode(QUrl, QString);
    void cancel();

public slots:
    void update();
    void stop();

private:
    Downloader* main;
    Downloader* daily;
    Downloader* bytecode;
    QThread* thread;

    bool isMainFinished;
    bool isDailyFinished;
    bool isBytecodeFinished;
};

#endif // UPDATER_H
