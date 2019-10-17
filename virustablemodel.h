#ifndef VIRUSTABLEMODEL_H
#define VIRUSTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>

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

public slots:
    int count();
    void select(bool selected, int row, int column);
    void selectAll(bool selected);
    void append(const QString& file, const QString virus);
    void remove();
    void clear();

private:
    int rows;
    int columns;
    QList<bool> selected;
    QList<QString> virus;
    QList<QString> file;
};

#endif // VIRUSTABLEMODEL_H
