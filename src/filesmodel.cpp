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

    backing << "sea cow" << "platypus" << "axolotl" << "quokka" << "pitahui" << "jerboa";
//    counter++;
}



QHash<int, QByteArray> FilesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int FilesModel::getsMyCounter() {
    return -1;
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
void FilesModel::swap2top(const int i) {
    if(i < 1 || i >= backing.size()) {
        return;
    }


    // Remove the value from the old location.
    beginMoveRows(QModelIndex(), i, i,QModelIndex(),0);
    backing.move(i,0);
    endMoveRows();

}
int FilesModel::updateDb() {
    QProcess process;
    QStringList params;
    int count = 0;
    process.setWorkingDirectory("/home/nemo");
    process.start("updatedb",params);// . -name \"" + s + "*\"");
    process.waitForFinished(3000); // will wait forever(-1) or msec until finished

    //QString stdout = process.readAllStandardOutput();
    QString stderr;// = process.readAllStandardError();
    while (process.canReadLine()) {
        QString line = process.readLine();
        this->appends(line);
        count++;
    }
    this->lcount = count;
    QString pwd = process.workingDirectory();
    //this->appends(line);
    return count;//stdout.length();
}
int FilesModel::locate(QString s) {
    QProcess process;
    QStringList params;
    int count = 0;
    //s = "'*" + s + "*'";
    params << "." << "-name" << s;
    // process.setArguments(params);
    process.setWorkingDirectory("/home/nemo");
    process.start("find",params);// . -name \"" + s + "*\"");
    process.waitForFinished(3000); // will wait forever(-1) or msec until finished

    //QString stdout = process.readAllStandardOutput();
    QString stderr;// = process.readAllStandardError();
    while (process.canReadLine()) {
        QString line = process.readLine();
        this->appends(line);
        count++;
    }
    this->lcount = count;
    QString pwd = process.workingDirectory();
    //this->appends(line);
    return count;//stdout.length();
}
