/****************************************************************************
**   nodemail is a nodecast email agent
**   Copyright (C) 2010-2013  Frédéric Logier <frederic@logier.org>
**
**   https://github.com/nodecast/nodemail
**
**   This program is free software: you can redistribute it and/or modify
**   it under the terms of the GNU Affero General Public License as
**   published by the Free Software Foundation, either version 3 of the
**   License, or (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**   but WITHOUT ANY WARRANTY; without even the implied warranty of
**   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**   GNU Affero General Public License for more details.
**
**   You should have received a copy of the GNU Affero General Public License
**   along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#ifndef EMAIL_H
#define EMAIL_H

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QtNetwork/QHostAddress>
#include <QxtJSON>
//#include <QxtNetwork/qxtsmtp.h>
#include <QxtNetwork>
#include <QtTest/QTest>
#include <QFile>
#include <QTextStream>

struct params {
    QString smtp_hostname;
    QString smtp_username;
    QString smtp_password;
    QString smtp_sender;
    QString smtp_recipient;
};


class Nodemail : public QObject
{
    Q_OBJECT
public:
    Nodemail(params a_params);
    ~Nodemail();


signals:
    void emit_response(QString res);


private:
    void writeStdout(QString s);
    params m_params;
    QMutex *m_mutex;
    QxtSmtp *m_smtp;
    QxtMailMessage *m_message;

    QTextStream* output;
    QFile *log;

public slots:
    void init();
    void receive_payload(QString s);

private slots:
    void reconnect();
    void failed();
    void success();

};

#endif // EMAIL_H
