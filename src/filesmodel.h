#ifndef FILESMODEL_H
#define FILESMODEL_H
#include <QAbstractListModel>
#include <QProcess>

class FilesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DemoRoles {
        NameRole = Qt::UserRole + 1,
    };

    explicit FilesModel(QObject *parent = NULL);

    virtual int rowCount(const QModelIndex&) const { return backing.size(); }
    virtual QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE void appends(QString s) ;
    Q_INVOKABLE void activate(const int i);
    Q_INVOKABLE void swap2top(const int i);
    Q_INVOKABLE int getsMyCounter();
    Q_INVOKABLE int locate(QString s);
    Q_INVOKABLE int updateDb();
    int lcount = 0;
private:
   QVector<QString> backing;


};
#endif // FILESMODEL_H
