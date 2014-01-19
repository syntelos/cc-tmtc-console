/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QMessageBox>

#include "ConfigurationScriptable.h"
#include "Scripts.h"

/*!
 * 
 */
ConfigurationScriptable::ConfigurationScriptable(){
}
/*!
 * 
 */
ConfigurationScriptable::~ConfigurationScriptable(){
}
Configuration* ConfigurationScriptable::configuration(QObject* node){

    if (node){
        Configuration* test = dynamic_cast<Configuration*>(node);
        if (test)
            return test;
        else {
            while (true){
                QObject* p = node->parent();
                if (p){
                    test = dynamic_cast<Configuration*>(node);
                    if (test)
                        return test;
                    else {
                        node = p;
                    }
                }
                else {
                    const QMetaObject* nodeMeta = node->metaObject();
                    const char* nodeClass = nodeMeta->className();
                    const char* objectName = node->objectName().toUtf8().constData();

                    if (nodeClass && objectName){

                        qDebug().nospace() << "ConfigurationScriptable.configuration failed for argument " << nodeClass << "#" << objectName;
                    }
                    else {
                        qDebug().nospace() << "ConfigurationScriptable.configuration failed for argument " << nodeClass;
                    }
                    return 0;
                }
            }
        }
    }
    else {
        qDebug() << "ConfigurationScriptable.configuration failed for null argument";
        return 0;
    }
}
/*!
 * 
 */
void ConfigurationScriptable::initConfigurationScriptable(QObject* subclass){

    Configuration* configuration = this->configuration(subclass);

    if (configuration){

        QScriptEngine* engine = configuration->getScriptEngine();

        QScriptValue global = engine->globalObject();

        const QObjectList& scripts = configuration->getScripts()->children();
        const int size = scripts.size();
        int cc;

        for (cc = 0; cc < size; cc++){

            const Script* script = dynamic_cast<const Script*>(scripts.at(cc));
            if (script){
                const SystemScriptSymbol* source = script->getLinkSource();
                const SystemScriptSymbol* target = script->getLinkTarget();
                const QString* content = script->getContent();

                if (source && target && content && 
                    source->isNotInert() && target->isNotInert() && 0 < content->length())
                    {
                        if (*source == subclass){

                            const QString* file = script->getFile();

                            QString fileName;
                            if (file){
                                fileName.append(*file);
                            }

                            QScriptValue script = engine->evaluate(*content,fileName);

                            if (script.isError()){

                                QMessageBox::critical(0, "Script Error",
                                                      QString::fromLatin1("%0:%1: %2")
                                                      .arg(fileName)
                                                      .arg(script.property("lineNumber").toInt32())
                                                      .arg(script.toString()));
                            }
                            else {
                                const char* signal = source->signal(subclass->metaObject());

                                QScriptValue function1 = script.property(target->getSuffix(),QScriptValue::ResolveLocal);

                                if (function1.isFunction()){

                                    if (qScriptConnect(subclass,signal,script,function1)){

                                        qDebug() << "ConfigurationScriptable.initConfigurationScriptable successfully connected signal: " << source->toString() << " to function: " << target->getSuffix();
                                    }
                                    else {

                                        qDebug() << "ConfigurationScriptable.initConfigurationScriptable failed to connect signal: " << source->toString() << " to function: " << target->getSuffix();
                                    }
                                }
                                else {

                                    QScriptValue function2 = global.property(target->getSuffix(),QScriptValue::ResolveLocal);

                                    if (function2.isFunction()){

                                        if (qScriptConnect(subclass,signal,global,function2)){

                                            qDebug() << "ConfigurationScriptable.initConfigurationScriptable successfully connected signal: " << source->toString() << " to function: " << target->getSuffix();
                                        }
                                        else {

                                            qDebug() << "ConfigurationScriptable.initConfigurationScriptable failed to connect signal: " << source->toString() << " to function: " << target->getSuffix();
                                        }
                                    }
                                    else {

                                        QMessageBox::critical(0, "Script Function Error",
                                                              QString::fromLatin1("Unable to identify function %0 in script file %1 (HCDB).")
                                                              .arg(target->getSuffix())
                                                              .arg(fileName));
                                    }
                                }
                            }
                        }
                    }
                else {

                    qDebug() << "ConfigurationScriptable.initConfigurationScriptable: Skipping script record missing source or target or content.";
                }
            }
        }
    }
    else {
        qDebug() << "ConfigurationScriptable.initConfigurationScriptable: missing configuration node.";
    }
}
