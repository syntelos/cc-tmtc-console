/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QAction>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QFileDialog>
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
      engine(script),
      net(0),
      devices(new Devices(this)),
      libraries(new Libraries(this)),
      scripts(new Scripts(this)),
      canvas(new GraphicsCanvas(this))
{
    setObjectName("window");

    Window::instance = this;

    qScriptRegisterMetaType(script, windowToScriptValue, windowFromScriptValue);

    Devices::InitScriptMetaType(script);
    Libraries::InitScriptMetaType(script);
    Scripts::InitScriptMetaType(script);
    GraphicsCanvas::InitScriptMetaType(script);

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
GraphicsCanvas* Window::getCanvas() const {

    return canvas;
}
QScriptEngine* Window::getScriptEngine() const {

    return engine;
}
void Window::open(){

    QString app_dir = QCoreApplication::applicationDirPath();

    QFileDialog dialog(this,"Open Window",app_dir,"XML files (*.xml)");

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.selectFile("window.xml");

    if (dialog.exec()){

        QStringList list = dialog.selectedFiles();
        if (1 == list.size()){

            QFile file(list.at(0));

            read(file);
        }
    }
}
void Window::edit(){

}
void Window::save(){

    QString app_dir = QCoreApplication::applicationDirPath();

    QFileDialog dialog(this,"Save Window",app_dir,"XML files (*.xml)");

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.selectFile("window.xml");

    if (dialog.exec()){

        QStringList list = dialog.selectedFiles();
        if (1 == list.size()){

            QFile file(list.at(0));

            write(file);
        }
    }
}
void Window::close(){

}
void Window::quit(){

    QApplication::exit();
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
    SystemCatalogNode::start(this);
}
void Window::stop(){
    SystemCatalogNode::stop(this);
}
void Window::read(QFile& src){
    if (src.open(QIODevice::ReadOnly)){

        QDomDocument doc;

        if (doc.setContent(&src,true)){

            src.close();

            SystemCatalogInput properties(SystemCatalog::PropertyConfigureConcrete,true);

            read(properties,doc.documentElement());

            properties.postprocessing();
        }
    }
}
void Window::write(QFile& tgt){
    if (tgt.open(QIODevice::WriteOnly)){
        QDomDocument doc;
        QDomElement window = doc.createElementNS(SystemCatalog::XMLNS::URI,"tmtc:window");
        doc.appendChild(window);

        SystemCatalogOutput properties(SystemCatalog::PropertyPublishConcrete,true);

        write(properties,window);

        tgt.write(doc.toByteArray());
        tgt.close();
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
void Window::read(const SystemCatalogInput& properties, const QDomElement& node){

    if (node.localName() == "window"){
        QDomNodeList children = node.childNodes();
        const uint count = children.length();
        int cc;
        for (cc = 0; cc < count; cc++){
            QDomNode child = children.item(cc);
            if (child.isElement()){

                QString name = child.localName();

                QDomElement cel = child.toElement();

                if (name == "devices"){

                    devices->read(properties,cel);
                }
                else if (name == "libraries"){

                    libraries->read(properties,cel);
                }
                else if (name == "scripts"){

                    scripts->read(properties,cel);
                }
                else if (name == "canvas"){

                    canvas->read(properties,cel);
                }
                else {
                    qDebug() << "Window.read: skipping unrecognized element" << name ;
                }
            }
        }
    }
    else {
        qDebug() << "Window.read: Unrecognized document element" << node.localName();
    }
}
void Window::write(SystemCatalogOutput& properties, QDomElement& node){

}
