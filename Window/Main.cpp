/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <iostream>
#include <QFile>
#include <QScriptEngine>
#include <QUrl>

#include "Window.h"

/*!
 * Window main
 */
int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("syntelos");
    QCoreApplication::setOrganizationDomain("syntelos.com");
    QCoreApplication::setApplicationName("tmtc-console");

    QApplication a(argc, argv);

    QScriptEngine* script = new QScriptEngine();

    Window* window = new Window(script);
    {
        QScriptValue global = script->newQObject(window);

        script->setGlobalObject(global);

        global.setProperty("window",global);

        global.setProperty("alert",script->newFunction(Window::alert),QScriptValue::Undeletable);
        global.setProperty("status",script->newFunction(Window::status),QScriptValue::Undeletable);
    }

    if (1 < argc){

        int ac;
        for (ac = 1; ac < argc; ac++){

            QString arg(argv[ac]);

            QUrl url(arg);
            if (url.isValid()){

                window->read(url);

                break;
            }
            else if (arg.startsWith("http:/")){

                std::cerr << argv[0] << ": Error: Invalid url '" << argv[ac] << "'" << std::endl;

                return 1;
            }
            else {
                QFile file(arg);
                if (file.exists()){

                    window->read(file);

                    break;
                }
                else if (!arg.startsWith("-")){

                    std::cerr << argv[0] << ": Error: Unrecognized argument '" << argv[ac] << "'" << std::endl;

                    return 1;
                }
            }
        }
    }

    window->run();

    return a.exec();
}
