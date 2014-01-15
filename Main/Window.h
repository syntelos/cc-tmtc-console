/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _TOPWINDOW_H
#define _TOPWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QScriptContext>
#include <QScriptEngine>
#include <QString>
#include <QWidget>

#include "System/SystemScriptable.h"
#include "Configuration/Configuration.h"
#include "Configuration/ConfigurationScriptable.h"
#include "Init.h"

/*!
 * Scriptable "window".
 *
 * Window class called from Main.cpp
 */
class Window : public QMainWindow,
    public ConfigurationScriptable
{
    Q_OBJECT;
    Q_PROPERTY(Configuration* configuration READ getConfiguration USER false FINAL);
    Q_PROPERTY(QWidget* body READ centralWidget WRITE setCentralWidget USER false FINAL);

    bool configureOpen;

    Init* init_program;

    static Window* instance;

 public:
    Window(QScriptEngine* engine);
    ~Window();

    Configuration* getConfiguration() const ;

    Init* getInit() const ;
    /*!
     * Special accesss from main to bind and evaluate script
     * "Window::init".
     */
    void run();
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
      * Select and configure file for "Window::init".
      */
    void open();
     /*!
      * Select script file for debugging.  The selected file is not
      * configured to "Window::init".
      */
    void edit();
     /*!
      * Configure file within edit process to "Window::init".
      */
    void save();
     /*!
      * Deconfigure "Window::init".
      */
    void close();
     /*!
      * Run the configuration tool.
      */
    void configure();
     /*!
      * Quit the window.
      */
    void quit();

 private slots:

   void configureDone(int);

 private:
    Q_DISABLE_COPY(Window)
};

Q_DECLARE_METATYPE(Window*)
#endif
