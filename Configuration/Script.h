/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#ifndef _CONSOLE_CONFIGURATION_Script_H
#define _CONSOLE_CONFIGURATION_Script_H

#include <QByteArray>
#include <QFileInfo>
#include <QMetaProperty>
#include <QObject>
#include <QScriptEngine>
#include <QString>
#include <QUuid>
#include <QWidget>

#include "System/SystemScriptSymbol.h"
#include "ObjectTree/ObjectTreeNode.h"
#include "ObjectTree/ObjectTreeImportExport.h"

class Script : public ObjectTreeNode, public ObjectTreeImportExport {
    Q_OBJECT;
    Q_PROPERTY(SystemScriptSymbol linkSource READ getLinkSource WRITE setLinkSource USER true FINAL);
    Q_PROPERTY(SystemScriptSymbol linkTarget READ getLinkTarget WRITE setLinkTarget USER true FINAL);
    Q_PROPERTY(QString file READ getFile WRITE setFile USER true FINAL);
    Q_PROPERTY(QString content READ getContent WRITE setContent USER true FINAL);
    Q_CLASSINFO("Object Name", "linkSource");

    /*!
     * The binding link source symbol is a class name and a method
     * name (using the C++ scope resolution syntax "::").
     * 
     * A network sharing reference is the combination of a Host UUID
     * and Script Symbol.
     */
    SystemScriptSymbol* linkSource;
    /*!
     * The binding link target symbol is a function name.
     */
    SystemScriptSymbol* linkTarget;
    /*!
     * File (UI chooser memory)
     */
    QString* file;
    /*!
     * Script source text
     */
    QString* content;


 public:
    static void InitScriptMetaType(QScriptEngine* engine);
    /*!
     * User constructor for emitting new values via database write.
     */
    Script(QObject* parent);
    /*!
     */
    ~Script();
    /*!
     */
    bool isInert();
    /*!
     * Read-only host UUID
     */
    const QString* getHostUuid() const;
    /*!
     */
    const SystemScriptSymbol* getLinkSource() const;
    /*!
     */
    void setLinkSource(SystemScriptSymbol* symbol);
    /*!
     */
    void setLinkSource(SystemScriptSymbol& symbol);
    /*!
     */
    void setLinkSource(QString& symbol);
    /*!
     */
    const SystemScriptSymbol* getLinkTarget() const;
    /*!
     */
    void setLinkTarget(SystemScriptSymbol* symbol);
    /*!
     */
    void setLinkTarget(SystemScriptSymbol& symbol);
    /*!
     */
    void setLinkTarget(QString& symbol);
    /*!
     */
    const QString* getFile() const;
    /*!
     */
    void setFile(QString* file);
    /*!
     */
    void setFile(QString& file);
    /*!
     */
    void setFile(QFileInfo& file);
    /*!
     */
    const QString* getContent() const;
    /*!
     */
    void setContent(QString* content);
    /*!
     */
    void setContent(QString& content);
    /*!
     */
    void setContent(QByteArray& content);
    /*!
     */
    virtual QWidget* createPropertyFormEditor(int index, const QMetaProperty& property);
    /*!
     */
    virtual QWidget* createPropertyFormLabel(int index, const QMetaProperty& property);
    /*!
     */
    virtual bool setPropertyForEditor(int index, const QMetaProperty& property, const QWidget& editor);
    /*!
     */
    virtual void importToObjectTreeNode();
    /*!
     */
    virtual void exportFromObjectTreeNode();

 private:
    Q_DISABLE_COPY(Script)
};

Q_DECLARE_METATYPE(Script*)
#endif
