/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _TOPWINDOW_H
#define _TOPWINDOW_H

#include <QApplication>
#include <QFile>
#include <QMainWindow>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QScriptContext>
#include <QScriptEngine>
#include <QString>
#include <QUrl>
#include <QWidget>

#include "System/SystemCatalog.h"
#include "System/SystemScriptable.h"
#include "Graphics/GraphicsCanvas.h"
#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationScriptable.h"
#include "Configuration/Devices.h"
#include "Configuration/Libraries.h"
#include "Configuration/Scripts.h"

/*!
 * Main window and root of the system tree.
 */
class Window : public QMainWindow,
    public Configuration,
    public SystemCatalogNode,
    public ConfigurationScriptable
{
    Q_OBJECT;
    Q_PROPERTY(Devices*        devices   READ getDevices    USER false FINAL);
    Q_PROPERTY(Libraries*      libraries READ getLibraries  USER false FINAL);
    Q_PROPERTY(Scripts*        scripts   READ getScripts    USER false FINAL);
    Q_PROPERTY(GraphicsCanvas* canvas    READ getCanvas USER false FINAL);

    QScriptEngine* engine;
    QNetworkAccessManager* net;
    Devices* devices;
    Libraries* libraries;
    Scripts* scripts;
    GraphicsCanvas* canvas;

    static Window* instance; //< For alert and status script global functions

 public:
    /*!
     */
    Window(QScriptEngine* engine);
    /*!
     */
    ~Window();
    /*!
     * Special accesss from main to bind and evaluate script
     * "Window::init".
     */
    void run();
    /*!
     */
    QNetworkAccessManager* getNetworkManager();
    /*!
     */
    QScriptEngine* getScriptEngine() const;
    /*!
     */
    Libraries* getLibraries() const;
    /*!
     */
    Devices* getDevices() const;
    /*!
     */
    Scripts* getScripts() const;
    /*!
     */
    GraphicsCanvas* getCanvas() const;
    /*!
     * One argument is the message body, two arguments is title and
     * body.
     */
    static QScriptValue alert(QScriptContext* cx, QScriptEngine* se);
    /*!
     * Put a space delimited string of arguments into the status bar.
     */
    static QScriptValue status(QScriptContext* cx, QScriptEngine* se);

 signals:
    /*!
     * Emitted for window initialization following the definition of
     * default components (menu and body), and following "show" and
     * "bind".
     */
    void init();

 public slots:
     /*!
      * Open Window
      */
    void open();
     /*!
      * Edit Window
      */
    void edit();
     /*!
      * Save Window
      */
    void save();
     /*!
      * Close Window
      */
    void close();
     /*!
      * Quit Window
      */
    void quit();
    /*!
      * Begin window system processing
     */
    virtual void start();
    /*!
      * Halt window system processing
     */
    virtual void stop();
     /*!
      * Configure window system from file
      */
    void read(QFile&);
    /*!
     * Publish window system to file
     */
    void write(QFile&);
     /*!
      * Configure system from XML reference
      */
    void read(const QUrl&);
    /*!
      * Configure window system 
     */
    virtual void read(const SystemCatalogInput&, const QDomElement&);
    /*!
     * Publish window system 
     */
    virtual void write(SystemCatalogOutput&, QDomElement&);

 private:
    Q_DISABLE_COPY(Window)
};

Q_DECLARE_METATYPE(Window*)
#endif
