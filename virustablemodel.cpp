#include "virustablemodel.h"
#include <QBrush>
#include <QDebug>

VirusTableModel::VirusTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    rows(0),
    columns(3),
    thread(new QThread(this)),
    cleaner(new FileCleaner(nullptr))
{
    cleaner->moveToThread(thread);
    thread->start();
    connect(this, &VirusTableModel::remove, cleaner, &FileCleaner::remove);
    connect(cleaner, &FileCleaner::removed, this, &VirusTableModel::removed);
}

int VirusTableModel::rowCount(const QModelIndex &parent) const
{
    (void)(parent);
    qDebug() << rows;
    return rows;
}

int VirusTableModel::columnCount(const QModelIndex &parent) const
{
    (void)(parent);
    return columns;
}

QVariant VirusTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()/* || index.row() >= m_data.size()*/)
            return QVariant();

    switch(role)
    {
        case Qt::CheckStateRole:
            switch(index.column())
            {
                case 0: return selected[index.row()];
                case 1: return QVariant();
                case 2: return QVariant();
                default: return QVariant();
            }

        case Qt::DisplayRole:
            switch(index.column())
            {
                case 0: return QVariant();
                case 1: return viruses[index.row()];
                case 2: return files[index.row()];
                default: return QVariant();
            }

        case Qt::ForegroundRole:
            switch(index.column())
            {
                case 0: return QVariant();
                case 1: return QBrush(Qt::red);
                case 2: return QBrush(Qt::white);
                default: return QVariant();
            }

        case Qt::UserRole:
            return index.row();

        case Qt::UserRole + 1:
            return index.column();

        default:
            return QVariant();
    }
}

QHash<int, QByteArray> VirusTableModel::roleNames() const
{
    return {
        {Qt::CheckStateRole, "selected"},
        {Qt::DisplayRole, "display"},
        {Qt::ForegroundRole, "color"},
        {Qt::UserRole, "row"},
        {Qt::UserRole + 1, "column"},
    };
}

int VirusTableModel::count()
{
    return rows;
}

void VirusTableModel::select(bool selected, int row)
{
    if(row >= rows)
    {
        emit cleanFinished();
        return;
    }
    this->selected[row] = selected;
}

void VirusTableModel::selectAll(bool selected)
{
    for(auto& i : this->selected)
    {
        i = selected;
    }
}

void VirusTableModel::append(const QString& file, const QString virus)
{
    this->selected.append(true);
    this->files.append(file);
    this->viruses.append(virus);
    rows += 1;
}

void VirusTableModel::clean()
{
    for(index = 0; !selected[index]; index++);
    if(index >= rows)
        return;

    emit remove(files[index]);
}

void VirusTableModel::clear()
{
    rows = 0;
    selected.clear();
    viruses.clear();
    files.clear();
}

void VirusTableModel::removed(const QString& file)
{
    (void)(file);
    selected.removeAt(index);
    files.removeAt(index);
    viruses.removeAt(index);
    rows -= 1;
    for(; !selected[index]; index++);
    if(index >= rows)
    {
        emit cleanFinished();
        return;
    }

    if(thread->isRunning() == false)
    {
        thread->start();
    }
    emit remove(files[index]);
}


void VirusTableModel::stop()
{
    thread->terminate();
    thread->wait();
}
