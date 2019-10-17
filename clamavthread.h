#ifndef CLAMAVTHREAD_H
#define CLAMAVTHREAD_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <clamavengine.h>

struct DetectedMessage
{
    QString file;
    bool isSafe;
    QString virusName;
};

class ClamAVThread : public QObject
{
    Q_OBJECT
public:
    explicit ClamAVThread(QObject *parent = nullptr);

signals:
    void scan(const QString& path);
    void globalScan();
    void abandon();
    void opened();
    void closed();
    void finished();
    void detecting(const QString& file);
    void detected(const QString& file, bool isSafe, const QString& virname);
    void message(const QString& text);
    void sent(); // to move ListVoew to the bottom

public slots:
    void start(const QString& path="");
    void cancel();

    /* *********************************************
     * Proxy forward signals of ClamAVEngine
     * Update UI real-time will let performance bad
     * So these functions buffer the data
     * *********************************************/
    void proxyDetecting(const QString& file);
    void proxyDetected(const QString& file, bool isSafe, const QString& virname);
    void proxyMessage(const QString& text);

private:
    ClamAVEngine* engine;
    QThread* thread;
    QStringList messageBuffer;
    QList<DetectedMessage> detectedBuffer;
};

#endif // CLAMAVTHREAD_H
