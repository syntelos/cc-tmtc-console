/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QScriptValue>
#include <QObjectList>

#include "Configuration.h"
#include "Scripts.h"
#include "Script.h"

Q_DECLARE_METATYPE(Configuration*)

QScriptValue configurationToScriptValue(QScriptEngine *engine, Configuration* const &in){
    return engine->newQObject(in);
}

void configurationFromScriptValue(const QScriptValue &object, Configuration* &out){
    out = qobject_cast<Configuration*>(object.toQObject());
}

static Configuration* instance;

Configuration* Configuration::Instance(){

    return instance;
}
Configuration* Configuration::Init(QScriptEngine* engine){

    if (instance){

        instance->deleteLater();
    }

    return new Configuration(engine);
}
Configuration::Configuration(QScriptEngine* engine)
    : ObjectTreeModel(vnul), engine(engine), ccdb(vnul), hcdb(vnul)
{
    instance = this;

    initSystemScriptable(this);

    ccdb = new CCDB(this);
    hcdb = new HCDB(ccdb, this);

    connect(ccdb,SIGNAL(writeSuccess()),hcdb,SLOT(read()));

    qScriptRegisterMetaType(engine, configurationToScriptValue, configurationFromScriptValue);
}

Configuration::~Configuration(){

    hcdb->deleteLater();

    ccdb->deleteLater();

    engine->deleteLater();
}
CCDB* Configuration::getCCDB() const {

    return ccdb;
}
HCDB* Configuration::getHCDB() const {

    return hcdb;
}
QScriptEngine* Configuration::getScriptEngine() const {

    return engine;
}
void Configuration::configureWindowInit(){

    if (hcdb->isUp()){
        Scripts* scripts = hcdb->getScripts();

        Script* init = scripts->find("Window::init");
        if (!init){
            /*
             */
            init = new Script(scripts);
            init->setLinkSource(new SystemScriptSymbol("Window::init"));
            init->setLinkTarget(new SystemScriptSymbol("init"));
        }
        /*
         */
        init->importToObjectTreeNode();
        /*
         */
        scripts->write();
    }
}
void Configuration::deconfigureWindowInit(){

    if (hcdb->isUp()){
        Scripts* scripts = hcdb->getScripts();

        Script* init = scripts->find("Window::init");
        if (init){
            /*
             */
            scripts->deconfigure(init);
        }
    }
}