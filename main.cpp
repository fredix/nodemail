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

#include <QCoreApplication>
#include "main.h"


int Service::sighupFd[2]={};
int Service::sigtermFd[2]={};

QSettings settings("nodecast", "nodemail");


// http://doc.qt.nokia.com/4.7/unix-signals.html
static void setup_unix_signal_handlers()
{
    struct sigaction hup, term;

    hup.sa_handler = Service::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    /*if (sigaction(SIGHUP, &hup, 0) > 0)
       return 1;*/

    term.sa_handler = Service::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    /*if (sigaction(SIGTERM, &term, 0) > 0)
       return 2;

    return 0;*/

    sigaction (SIGINT, &hup, NULL);
    sigaction (SIGTERM, &term, NULL);
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    params nodemail_params;
    bool debug;
    bool verbose;

    QxtCommandOptions options;

    options.add("debug", "show debug informations");
    options.alias("debug", "d");

    options.add("smtp-hostname", "set the smtp hostname", QxtCommandOptions::Required);
    options.alias("smtp-hostname", "sph");

    options.add("smtp-username", "set the smtp username", QxtCommandOptions::Required);
    options.alias("smtp-username", "spu");

    options.add("smtp-password", "set the smtp password", QxtCommandOptions::Required);
    options.alias("smtp-password", "spp");


    options.add("smtp-sender", "set the smtp sender", QxtCommandOptions::Required);
    options.alias("smtp-sender", "sps");


    options.add("smtp-recipient", "set the smtp recipient", QxtCommandOptions::Required);
    options.alias("smtp-recipient", "spr");


    options.add("help", "show this help text");
    options.alias("help", "h");
    options.parse(QCoreApplication::arguments());



    if(options.count("help") || options.showUnrecognizedWarning()) {
        options.showUsage();
        return -1;
    }




    if(options.count("smtp-hostname")) {
        nodemail_params.smtp_hostname = options.value("smtp-hostname").toString();
        settings.setValue("smtp-hostname", nodemail_params.smtp_hostname);
    }
    else if(settings.contains("smtp-hostname"))
    {
        nodemail_params.smtp_hostname = settings.value("smtp-hostname").toString();
    }
    else {
        std::cout << "ncs: --smtp-hostname requires a parameter" << std::endl;
        options.showUsage();
        return -1;
    }


    if(options.count("smtp-username")) {
        nodemail_params.smtp_username = options.value("smtp-username").toString();
        settings.setValue("smtp-username", nodemail_params.smtp_username);
    }
    else if(settings.contains("smtp-username"))
    {
        nodemail_params.smtp_username = settings.value("smtp-username").toString();
    }
    else {
        std::cout << "ncs: --smtp-username requires a parameter" << std::endl;
        options.showUsage();
        return -1;
    }


    if(options.count("smtp-password")) {
        nodemail_params.smtp_password = options.value("smtp-password").toString();
        settings.setValue("smtp-password", nodemail_params.smtp_password);
    }
    else if(settings.contains("smtp-password"))
    {
        nodemail_params.smtp_password = settings.value("smtp-password").toString();
    }
    else {
        std::cout << "ncs: --smtp-password requires a parameter" << std::endl;
        options.showUsage();
        return -1;
    }

    settings.sync();
    setup_unix_signal_handlers();

    Service *service;
    service = new Service(nodemail_params);


    return a.exec();
}
