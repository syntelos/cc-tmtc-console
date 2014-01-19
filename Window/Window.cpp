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
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QScriptValue>
#include <QStatusBar>
#include <QRect>


#include "System/SystemScriptSymbol.h"
#include "Graphics/GraphicsCanvas.h"
#include "Terminal/Terminal.h"
#include "XPORT/XportConnection.h"
#include "TMTC/TMTCMessage.h"
#include "Window.h"



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
    : QMainWindow(), 
      configureOpen(false),
      engine(script),
      net(0),
      devices(new Devices(this)),
      libraries(new Libraries(this)),
      scripts(new Scripts(this))
{
    setObjectName("window");

    Window::instance = this;

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
    GraphicsCanvas* canvas = new GraphicsCanvas(this);

    this->setCentralWidget(canvas);


        /*****************************************************************
         * Temporary

        Terminal* terminal = new Terminal();

        canvas->add(terminal);

        if (QObject::connect(terminal,SIGNAL(send(const TMTCMessage*)),devices,SLOT(receivedFromUser(const TMTCMessage*))) &&
            QObject::connect(devices,SIGNAL(sendToUser(const TMTCMessage*)),terminal,SLOT(received(const TMTCMessage*)))
            )
        {
            qDebug() << "Window/Terminal: terminal & devices configured (duplex)";
        }
        else {

            qDebug() << "Window/Terminal: terminal & devices configuration (duplex) failed";
        }
         */

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

    Window::instance = 0;

    if (this->engine){

        QScriptEngine* engine = this->engine;

        this->engine = 0;

        engine->deleteLater();
    }

    /*
     * QObject tree managed...
     */
    this->net = 0;
    this->devices = 0;
    this->libraries = 0;
    this->scripts = 0;
}
void Window::run(){

    initConfigurationScriptable(this);

    emit init();
}
QNetworkAccessManager* Window::getNetworkManager(){

    if (0 == this->net){

        this->net = new QNetworkAccessManager(this);
    }
    return this->net;
}
Devices* Window::getDevices() const {

    return devices;
}
Libraries* Window::getLibraries() const {

    return libraries;
}
Scripts* Window::getScripts() const {

    return scripts;
}
QScriptEngine* Window::getScriptEngine() const {

    return engine;
}
void Window::open(){

    Scripts* scripts = getScripts();

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
}
void Window::edit(){

}
void Window::save(){

}
void Window::close(){

    Scripts* scripts = getScripts();

    Script* init = scripts->find("Window::init");
    if (init){
        /*
         */
        scripts->deconfigure(init);
    }
}
void Window::configure(){
    if (!configureOpen){
        configureOpen = true;
        //
        // TODO -- ObjectTreeModel model(this);
        //
        // TreeEditorDialog* dialog = new TreeEditorDialog(this,this);
        //
        // dialog->connectFinishedTo(this,SLOT(configureDone(int)));
    }
}
void Window::quit(){

    QApplication::exit();
}
void Window::configureDone(int error){

    configureOpen = false;
}
QScriptValue Window::alert(QScriptContext* cx, QScriptEngine* se){
    if (Window::instance){
        switch(cx->argumentCount()){
        case 1:

            QMessageBox::critical(Window::instance, "Script Alert", cx->argument(0).toString());
            return QScriptValue(true);

        case 2:

            QMessageBox::critical(Window::instance, cx->argument(0).toString(), cx->argument(1).toString());
            return QScriptValue(true);

        default:
            break;
        }
    }
    return QScriptValue(false);
}
QScriptValue Window::status(QScriptContext* cx, QScriptEngine* se){
    if (Window::instance){
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
    else
        return QScriptValue(false);
}
void Window::start(){
}
void Window::stop(){
}
void Window::read(QFile& src){
    QDomDocument doc;
    if (doc.setContent(&src,true)){

        SystemCatalogInput properties(SystemCatalog::PropertyConfigureConcrete,true);

        read(properties,doc.documentElement());

        properties.postprocessing();
    }
}
void Window::read(const QUrl& src){

    QNetworkAccessManager* net = getNetworkManager();
    QNetworkRequest q(src);
    {
        q.setRawHeader("User-Agent", "Syntelos-TMTC/1.0");
    }
    QNetworkReply* p = net->get(q);
    if (QNetworkReply::NoError == p->error()){

        QDomDocument doc;
        if (doc.setContent(p,true)){

            SystemCatalogInput properties(SystemCatalog::PropertyConfigureConcrete,true);

            read(properties,doc.documentElement());

            properties.postprocessing();
        }
    }
    p->close();
    p->deleteLater();
}
void Window::read(const SystemCatalogInput& properties, const QDomElement& parent){


}
void Window::write(SystemCatalogOutput& properties, QDomElement& parent){
}
