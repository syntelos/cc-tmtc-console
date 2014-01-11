/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <iostream>
#include <QScriptEngine>
#include <QScriptValue>

#include "Window.h"
#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationError.h"
#include "Storage/StorageTreeEditor.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Syntelos Software");
    QCoreApplication::setOrganizationDomain("syntelos.com");
    QCoreApplication::setApplicationName("TMTC Console");

    QApplication a(argc, argv);
    try {
        QScriptEngine* script = new QScriptEngine();

        Window* window = new Window(script);

        QScriptValue global = script->newQObject(window);

        script->setGlobalObject(global);

        global.setProperty("window",global);

        global.setProperty("alert",script->newFunction(Window::alert),QScriptValue::Undeletable);
        global.setProperty("status",script->newFunction(Window::status),QScriptValue::Undeletable);
        /*
         * Bind, evaluate, and signal script "Window:init".
         */
        window->run();

        return a.exec();
    }
    catch (ConfigurationError::DatabaseDriver x){
        std::cerr << x.component << " Error Using Database Driver" << std::endl;
        return 1;
    }
    catch (ConfigurationError::DatabaseOpen x){
        std::cerr << x.component << " Error Opening Database" << std::endl;
        return 1;
    }
    catch (ConfigurationError::DatabaseInitTable x){
        std::cerr << x.component << " Error Initializing Database Table" << std::endl;
        return 1;
    }
    catch (ConfigurationError::DatabaseInitRecord x){
        std::cerr << x.component << " Error Initializing Database Record" << std::endl;
        return 1;
    }
    catch (ConfigurationError::DatabaseRead x){
        std::cerr << x.component << " Error Reading Database" << std::endl;
        return 1;
    }

}
