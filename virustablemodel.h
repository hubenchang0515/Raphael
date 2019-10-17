#ifndef VIRUSTABLEMODEL_H
#define VIRUSTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QThread>
#include "filecleaner.h"

class VirusTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VirusTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void remove(const QString& files);
    void cleanFinished();

public slots:
    int count();
    void select(bool selected, int row);
    void selectAll(bool selected);
    void append(const QString& files, const QString viruses);
    void clean();
    void clear();
    void removed(const QString& files);
    void stop();

private:
    int rows;
    int columns;
    QList<bool> selected;
    QList<QString> viruses;
    QList<QString> files;

    QThread* thread;
    FileCleaner* cleaner;
    int index;
};

#endif // VIRUSTABLEMODEL_H
