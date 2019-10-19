#ifndef CLAMAVTHREAD_H
#define CLAMAVTHREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QUrl>
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
    void start(const QUrl& path=QUrl(""));
    void cancel();

    /* *********************************************
     * Proxy forward signals of ClamAVEngine
     * Update UI real-time will let performance bad
     * So these functions buffer the data
     * *********************************************/
    void proxyDetecting(const QString& file);
    void proxyDetected(const QString& file, bool isSafe, const QString& virname);
    void proxyMessage(const QString& text);
    void proxyFinished();
    void updateMessage();

private:
    ClamAVEngine* engine;
    QThread* thread;
    QTimer* timer;
    QStringList messageBuffer;
    QList<DetectedMessage> detectedBuffer;
    QString current;
};

#endif // CLAMAVTHREAD_H
