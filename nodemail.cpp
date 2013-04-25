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

#include "nodemail.h"
#include <iostream>

Nodemail::Nodemail(params a_params) : m_params(a_params)
{
    //qDebug() << "Email:Email CONSTRUCT";
    m_mutex = new QMutex();

//    m_smtp = NULL;
    m_message = NULL;

    m_smtp = new QxtSmtp(this);

    connect(m_smtp, SIGNAL(mailFailed(int,int)), this, SLOT(failed()));
    connect(m_smtp, SIGNAL(mailSent(int)), this, SLOT(success()));
    connect(m_smtp, SIGNAL(disconnected()), this, SLOT(reconnect()));

}



void Nodemail::init()
{
    output = new QTextStream( stdout, QIODevice::WriteOnly );

    log = new QFile("/tmp/nodemail");
     if (!log->open(QIODevice::Append | QIODevice::Text))
             return;


     m_smtp->connectToSecureHost(m_params.smtp_hostname.toAscii());
     m_smtp->setStartTlsDisabled(true);
     m_smtp->setUsername(m_params.smtp_username.toAscii());
     m_smtp->setPassword(m_params.smtp_password.toAscii());
}



void Nodemail::reconnect()
{
    //QTest::qSleep(2000);

    m_smtp->connectToSecureHost(m_params.smtp_hostname.toAscii());
    m_smtp->setStartTlsDisabled(true);
    m_smtp->setUsername(m_params.smtp_username.toAscii());
    m_smtp->setPassword(m_params.smtp_password.toAscii());
}



void Nodemail::writeStdout(QString s)
{
    (*output) << s;
    //while (!(*output).atEnd()) {};
    (*output).flush();
}



void Nodemail::receive_payload(QString s)
{
   // m_mutex->lock();
    //qDebug() << "!!!!!! Email::sendEmail  : " << json;

     //writeStdout("PARSE : " + s);

    QVariant json = QxtJSON::parse(s);


    if (json.toMap().contains("exp")
            && json.toMap().contains("dest")
            && json.toMap().contains("object")
            && json.toMap().contains("body")
            )
    {
         QString exp = json.toMap()["exp"].toString();
         QString dest = json.toMap()["dest"].toString();
         QString object = json.toMap()["object"].toString();
         QString body = json.toMap()["body"].toString();

         //QString log = exp + " " + dest + " " + field3;

       // if (m_smtp) delete(m_smtp);
        if (m_message) delete(m_message);
       // m_smtp = NULL;
        m_message = NULL;


/*
        m_smtp = new QxtSmtp();

        connect(m_smtp, SIGNAL(mailFailed(int,int)), this, SLOT(failed()));
        connect(m_smtp, SIGNAL(mailSent(int)), this, SLOT(success()));
*/

        m_message = new QxtMailMessage();

        m_message->setSender(exp.toLatin1());
        m_message->setSubject(object.toLatin1());
        m_message->setBody(body.toLatin1());
        m_message->addRecipient(dest.toLatin1());

        QHash<QString,QString> headers;
        headers.insert("MIME-Version","1.0");
        headers.insert("Content-type","text/html; charset=utf-8");
        headers.insert("from", exp.toLatin1());
        m_message->setExtraHeaders(headers);

        /*
        m_smtp->connectToSecureHost(m_host.toAscii());
        m_smtp->setStartTlsDisabled(true);
        m_smtp->setUsername(m_username.toAscii());
        m_smtp->setPassword(m_password.toAscii());
*/

        m_smtp->send(*m_message);
    }
    else
        writeStdout("{\"error\": \"parsing json\", \"status\": \"false\"}");


}

void Nodemail::failed()
{
    writeStdout("{\"action\": \"terminate\", \"status\": \"ko\"}");

   // qDebug() << "Email::failed";

  //  m_smtp->disconnectFromHost();
    //m_mutex->unlock();
}


void Nodemail::success()
{
    writeStdout("{\"action\": \"terminate\", \"status\": \"ok\"}");


   // qDebug() << "Email::sucess";

    //m_smtp->disconnectFromHost();
  //  m_mutex->unlock();
}


Nodemail::~Nodemail()
{
    delete(m_smtp);
}
