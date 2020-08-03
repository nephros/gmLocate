

#include "filesmodel.h"
//QVector<QString>  FilesModel::backing;// = "";//<< "sea cow" << "platypus" << "axolotl" << "quokka" << "pitahui" << "jerboa";
//int FilesModel::counter = 1;


FilesModel::FilesModel(QObject *parent) :
    QAbstractListModel(parent)
{
    systemDB = "/var/cache/harbour-mlocate.db";
    userDB = "locateDB.db";
    //   backing << "sea cow" << "platypus" << "axolotl" << "quokka" << "pitahui" << "jerboa";
    //    counter++;
}



QHash<int, QByteArray> FilesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int FilesModel::getsMyCounter() {
    return lcount;
}

QVariant FilesModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == NameRole) {
        return QVariant(backing[index.row()]);
    }
    return QVariant();
}
void FilesModel::appends(QString s) {
    beginInsertRows(QModelIndex(), 0, 0);
    backing.insert(0 , s);
    endInsertRows();
}
void FilesModel::activate(const int i) {
    if(i < 0 || i >= backing.size()) {
        return;
    }
    QString value = backing[i];

    // Remove the value from the old location.
    beginRemoveRows(QModelIndex(), i, i);
    backing.erase(backing.begin() + i);
    endRemoveRows();

    // Add it to the top.
    beginInsertRows(QModelIndex(), 0, 0);
    backing.insert(0, value);
    endInsertRows();
}
void FilesModel::remove(const int i) {
    if(i < 0 || i >= backing.size()) {
        return;
    }
    backing.erase(backing.begin() + i);
}
void FilesModel::swap2top(const int i) {
    if(i < 1 || i >= backing.size()) {
        return;
    }
    //  beginMoveRows(QModelIndex(), i, i,QModelIndex(),0);
    backing.move(i,0);
    // endMoveRows();

}
QString FilesModel::diskFree() {
    QStringList args;
    QString line = "";
    QString lines = "";
    QProcess process;
    args << "-h";
    process.start("/bin/df", args);
    process.waitForFinished(3000);
    //QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();
    while (process.canReadLine()) {
        line = process.readLine();
        // if (!line.contains("tmpfs",Qt::CaseInsensitive)) {
        lines += process.readLine();
        //}
    }
    //line.truncate(16);

    return lines;

}

QString FilesModel::updateDb(bool useUserDB, bool doUpdate) {
    //args << "-c" <<  "%y" << "/var/cache/harbour-mlocate.db";
    //updatedb -o locateDB.db
    QProcess process;
    process.setWorkingDirectory("/home/nemo");
    QString stderr;
    QStringList args;
    bool gotResult = false;
    QString retline;
    QString line;
    if (doUpdate) {
        // process.setWorkingDirectory("/home/nemo");
        if (useUserDB) {
            args << "-o" << userDB;
        }
        process.start("updatedb",args);// . -name \"" + s + "*\"");
        process.waitForFinished(30000); // will wait forever(-1) or msec until finished
    }
    args.clear();
    args  << "-c" <<  "%y";
    if (useUserDB) {
        args  << userDB;
        retline = "Last updateDB USER";
    } else {
        args  << systemDB;
        retline = "Last updateDB SYSTEM";
    }
    //args  << "-c" <<  "%y" << "/var/cache/pk-zypp-cache";
    process.start("/usr/bin/stat",args);
    process.waitForFinished(1000);
    //QString stdout = process.readAllStandardOutput();
    while (process.canReadLine()) {
        line += ": " + process.readLine();
        gotResult = true;
    }
    line.truncate(18);
    if (!gotResult) {
        retline = process.readAllStandardError();
        retline.remove(0, 26);
    } else {
        retline += line;
    }
    //retline = process.workingDirectory();
    return retline;
}
int FilesModel::locate(QString s, bool useUserDB, bool ignoreCase, bool useRegex, bool exists, bool useAllPatterns  ) {
    //locate -d locateDB.db
    QProcess process;
    QStringList params;
    int count = 0;
    if (ignoreCase ) {
        params << "-i";
    }
    if (useRegex) {
        params << "--regex";
    }
    if (exists) {
        params << "-e";
    }
    if (useUserDB){
        params << "-d" << userDB;
    }
    if (useAllPatterns){
        params << "-A";
    }

    params << "-l" << "1000";
    s.simplified();
    params.append(s.split(' '));
    // process.setArguments(params);
    process.setWorkingDirectory("/home/nemo");
    process.start("/usr/bin/locate",params);// . -name \"" + s + "*\"");
    process.waitForFinished(-1); // will wait forever(-1) or msec until finished

    //QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();
    backing.clear();
    // backing.append(params);
    while (process.canReadLine()) {
        QString line = process.readLine();
        //this->appends(line);
        backing << line;
        count++;
    }
    if (count == 0) {
        backing << "***** Nothing found :-( ";
        backing << "mlocate installed? No Database?";
        backing << "";
        backing << "your search: ";
        backing.append(params);
        backing << stderr;
    }
    this->lcount = count;
    return count;
}
// für qml model dataModel
QStringList FilesModel::getFileList() {
    return backing;
}

bool FilesModel::execXdgOpen(QString filename) {
    QProcess process;
    QStringList params;
    params << filename;
    //process.setWorkingDirectory("/home/nemo");
    return process.startDetached("/usr/bin/xdg-open",params);
}

bool FilesModel::startFileBrowser(QString dir) {
    QProcess process;
    // to start browser with correct dir the HOME env Variable has to be set
    /* thanks to matt https://stackoverflow.com/questions/28758421/qprocess-set-environment-variables-for-startdetached
     As child process inherits the environment from the parent,
        I think that the easiest workaround is to save/modify/restore own environment using qgetenv() and qputenv()
        before and after QProcess::startDetached() call.*/
    QFileInfo qfi(dir);
    QString oldhome = "/home";
    if (!qfi.isDir()) {
        dir.truncate(dir.lastIndexOf("/"));
    }
    qfi.setFile(dir);
    if (!qfi.isDir()) {
        return false;
    }
    dir.trimmed();
    oldhome = qgetenv("HOME");
    qputenv("HOME", QByteArray(dir.toUtf8()));
   // qDebug() << "oldhome: " << oldhome << " newHome: " << qgetenv("HOME");
    bool started = process.startDetached("/usr/bin/harbour-file-browser");
    qputenv("HOME", QByteArray(oldhome.toUtf8()));
    return started;
}
  /*  The following doesn't works because
   startDetached is static, so doesn't use the env set by setProcessEnvironment :-(
    QString env_variable;
    QStringList paths_list;// = env.toStringList();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    paths_list = env.toStringList();
    foreach( env_variable, paths_list )
        qDebug() << env_variable;
    //oldhome = env.value("HOME");
    env.insert("HOME",dir);
process.setProcessEnvironment(env);
    qDebug() << "-------------";
    paths_list = env.toStringList();
    foreach( env_variable, paths_list )
        qDebug() << env_variable;

    paths_list = env.toStringList();
    foreach( env_variable, paths_list )
        qDebug() << env_variable;
    process.setProgram("/usr/bin/harbour-file-browser");
    process.startDetached("/usr/bin/harbour-file-browser");
*/

