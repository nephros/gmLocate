/*
  Copyright (C) 2015 Jolla Ltd.
  Contact: Jussi Pakkanen <jussi.pakkanen@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
int FilesModel::locate(QString s, bool useUserDB, bool ignoreCase  ) {
    //locate -d locateDB.db
    QProcess process;
    QStringList params;
    int count = 0;
    if (ignoreCase ) {
        params << "-i";
    }
    if (useUserDB){
        params << "-d" << userDB;
    }
    params << "-l" << "1000" << s ;
    // process.setArguments(params);
    process.setWorkingDirectory("/home/nemo");
    process.start("/usr/bin/locate",params);// . -name \"" + s + "*\"");
    process.waitForFinished(3000); // will wait forever(-1) or msec until finished

    //QString stdout = process.readAllStandardOutput();
    QString stderr;// = process.readAllStandardError();
    backing.clear();
   // backing.append(params);
    while (process.canReadLine()) {
        QString line = process.readLine();
        //this->appends(line);
        backing << line;
        count++;
    }
    if (count == 0) {
        backing << "***** Nothing found :-( ****----------------------------------------------------------------------------------/----------------------------------------------------------------------------------------------------------------------------";
        backing << " " << " ";
        backing << "mlocate installed?";
        backing << "No Database?";
        backing << " or try a less complex search...";
    }
//backing.clear();
//backing << "test.jpg" << "test2.jpg";
    this->lcount = count;
    //this->appends(line);
    return count;//stdout.length();
}
// für qml model dataModel
QStringList FilesModel::getFileList() {
    return backing;
}

bool FilesModel::execXdgOpen(QString filename) {
    QProcess process;
    QStringList params;
    params << filename;
    process.setWorkingDirectory("/home/nemo");
    process.startDetached("/usr/bin/xdg-open",params);
    return true;
}

