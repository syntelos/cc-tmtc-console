/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QAction>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QKeySequence>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QObject>
#include <QScriptValue>
#include <QStatusBar>
#include <QRect>
#include <QSvgWidget>

#include "Window.h"
#include "Storage/StorageTreeEditorDialog.h"
#include "System/SystemScriptSymbol.h"
#include "Configuration/Configuration.h"
#include "Configuration/HCDB.h"
#include "Configuration/Devices.h"
#include "Configuration/Device.h"
#include "Graphics/GraphicsBody.h"
#include "Terminal/Terminal.h"
#include "XPORT/XportConnection.h"
#include "TMTC/TMTCMessage.h"
#include "Multiplex/Multiplex.h"

Q_DECLARE_METATYPE(Window*)

QScriptValue windowToScriptValue(QScriptEngine *engine, Window* const &in){
    return engine->newQObject(in);
}

void windowFromScriptValue(const QScriptValue &object, Window* &out){
    out = qobject_cast<Window*>(object.toQObject());
}


Window* Window::instance;

/*
 */
Window::Window(QScriptEngine* script)
    : QMainWindow(vnul)
{
    Window::instance = this;

    Configuration* configuration = Configuration::Init(script);

    qScriptRegisterMetaType(script, windowToScriptValue, windowFromScriptValue);

    initSystemScriptable(this);

    /*
     */
    QMenuBar* menuBar = this->menuBar();
    {
        QKeySequence ctrlC(Qt::CTRL + Qt::Key_C);
        QKeySequence ctrlE(Qt::CTRL + Qt::Key_E);
        QKeySequence ctrlF(Qt::CTRL + Qt::Key_F);
        QKeySequence ctrlO(Qt::CTRL + Qt::Key_O);
        QKeySequence ctrlQ(Qt::CTRL + Qt::Key_Q);
        QKeySequence ctrlS(Qt::CTRL + Qt::Key_S);
        QKeySequence ctrlW(Qt::CTRL + Qt::Key_W);

        QMenu* file = menuBar->addMenu("File");

        file->addAction("Open",this,SLOT(open()),ctrlO);
        file->addAction("Edit",this,SLOT(edit()),ctrlE);
        file->addAction("Save",this,SLOT(save()),ctrlS);
        file->addAction("Close",this,SLOT(close()),ctrlC);
        file->addAction("Configure",this,SLOT(configure()),ctrlF);
        file->addSeparator();
        {
            QList<QKeySequence> shortcuts;
            shortcuts << ctrlQ << ctrlW;

            QAction* action = file->addAction("Quit");
            action->setShortcuts(shortcuts);
            action->connect(action,SIGNAL(triggered()),this,SLOT(quit()));
        }
    }
    /*
     */
    GraphicsBody* body = new GraphicsBody(this);

    this->setCentralWidget(body);

    /*****************************************************************
     * Temporary
     *
     * Some script would add a terminal into the "body" widget,
     * initialize devices and connect the database and terminal.
     * 
     * The database is the solitary sink - slot for each connection,
     * and the solitary sink - slot for the terminal input.
     */
    HCDB* hcdb = configuration->getHCDB();
    if (hcdb->isUp()){

        Devices* devices = hcdb->getDevices();
        if (devices){
            QList<QObject*> device_list = devices->children();
            if (0 < device_list.count()){

                Multiplex* multiplex = new Multiplex(configuration);

                Device* device = static_cast<Device*>(device_list.at(0));

                XportConnection* connection = new XportConnection(device);

                Terminal* terminal = new Terminal();

                body->add(terminal);

                if (QObject::connect(connection,SIGNAL(received(const SystemDeviceIdentifier*,const TMTCMessage*)),multiplex,SLOT(receivedFromDevice(const SystemDeviceIdentifier*,const TMTCMessage*))) &&
                    QObject::connect(multiplex,SIGNAL(sendToDevice(const SystemDeviceIdentifier*,const TMTCMessage*)),connection,SLOT(send(const SystemDeviceIdentifier*,const TMTCMessage*)))
                    )
                {
                    qDebug() << "Window/Terminal: device & multiplex configured (duplex)";

                    if (QObject::connect(terminal,SIGNAL(send(const SystemDeviceIdentifier*,const TMTCMessage*)),multiplex,SLOT(receivedFromUser(const SystemDeviceIdentifier*,const TMTCMessage*))) &&
                        QObject::connect(multiplex,SIGNAL(sendToUser(const SystemDeviceIdentifier*,const TMTCMessage*)),terminal,SLOT(received(const SystemDeviceIdentifier*,const TMTCMessage*)))
                        )
                    {

                        connection->start();

                        qDebug() << "Window/Terminal: terminal & multiplex configured (duplex)";
                    }
                    else {

                        delete connection;

                        delete multiplex;

                        qDebug() << "Window/Terminal: terminal & multiplex configuration (duplex) failed";
                    }
                }
                else {

                    delete connection;

                    delete multiplex;

                    qDebug() << "Window/Terminal: device & multiplex configuration (duplex) failed";
                }
            }
            else {
                qDebug() << "Window/Terminal: HCDB device not configured";
            }
        }
        else {
            qDebug() << "Window/Terminal: HCDB devices not found";
        }
    }
    else {
        qDebug() << "Window/Terminal: HCDB is down";
    }
    /*
     * Normal default process 
     */
    this->statusBar();
    /*
     * TODO: use QSettings for user's window geometry.
     */
    const QRect& available = qApp->desktop()->availableGeometry();

    QSize window(available.width()>>1,available.height()>>1);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight,
                                          Qt::AlignCenter,
                                          window,
                                          available));
    /*
     */
    this->show();
}
/*
 */
Window::~Window(){
}
void Window::run(){

    if (Configuration::Instance()->getHCDB()->isUp()){

        initConfigurationScriptable(this);

        emit init();
    }
    else {

        Window::configure();
    }
}
/*
 * TODO Review Qt Contextual Menu
 */
QMenu* Window::addMenu(const QString& title){

    return this->menuBar()->addMenu(title);
}
void Window::removeMenu(const QString& title){
    QMenuBar* menuBar = this->menuBar();
    const QObjectList& children = menuBar->children();
    const int sz = children.size();
    int cc;
    for (cc = 0; cc < sz; cc++){
        QObject* child_object = children.at(cc);
        QMenu* child = qobject_cast<QMenu*>(child_object);
        if (child && title == child->title()){

            child->setParent(vnul);
            child->deleteLater();
            break;
        }
    }
}
Configuration* Window::getConfiguration(){

    return Configuration::Instance();
}
void Window::open(){

    Configuration::Instance()->configureWindowInit();
}
void Window::edit(){

}
void Window::save(){

}
void Window::close(){

    Configuration::Instance()->deconfigureWindowInit();
}
void Window::configure(){
    if (!configureOpen){
        configureOpen = true;

        StorageTreeEditorDialog* dialog = new StorageTreeEditorDialog(Configuration::Instance(),this);

        dialog->connectFinishedTo(this,SLOT(configureDone(int)));
    }
}
void Window::quit(){

    QApplication::exit();
}
void Window::configureDone(int error){

    configureOpen = false;
}
QScriptValue Window::alert(QScriptContext* cx, QScriptEngine* se){
    switch(cx->argumentCount()){
    case 1:

        QMessageBox::critical(Window::instance, "Script Alert", cx->argument(0).toString());
        return QScriptValue(true);

    case 2:

        QMessageBox::critical(Window::instance, cx->argument(0).toString(), cx->argument(1).toString());
        return QScriptValue(true);

    default:
        return QScriptValue(false);
    }
}
QScriptValue Window::status(QScriptContext* cx, QScriptEngine* se){
    QString string;
    const int count = cx->argumentCount();
    int cc;
    for (cc = 0; cc < count; cc++){
        if (0 < cc){
            string.append(' ');
        }
        string.append(cx->argument(cc).toString());
    }
    QStatusBar* statusBar = Window::instance->statusBar();
    statusBar->clearMessage();
    statusBar->showMessage(string);

    return QScriptValue(true);
}
