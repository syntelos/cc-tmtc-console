/*
 * Copyright 2013 John Pritchard, Syntelos.  All rights reserved.
 */
#include <QDebug>
#include <QObject>
#include <QMetaClassInfo>
#include <QMetaProperty>
#include <QMetaType>

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QTimeEdit>
#include <QVBoxLayout>

#include "StorageObjectForm.h"
#include "ObjectTree/ObjectTreeNode.h"
#include "ObjectTree/ObjectTreeList.h"
#include "ObjectTree/ObjectTreeImportExport.h"
#include "Null.h"


StorageObjectForm::StorageObjectForm(QObject* subject, QWidget* parent)
    : QWidget(parent), subject(subject), storageInterface(dynamic_cast<StorageInterface*>(subject)), propertyEditor(dynamic_cast<PropertyEditorInterface*>(subject)), countUserProperties(0)
{
    init();
}
StorageObjectForm::StorageObjectForm(QObject* subject, StorageInterface* storage, QWidget* parent)
    : QWidget(parent), subject(subject), storageInterface(dynamic_cast<StorageInterface*>(storage)), propertyEditor(dynamic_cast<PropertyEditorInterface*>(subject)), countUserProperties(0)
{
    init();
}
StorageObjectForm::~StorageObjectForm(){
    /*
     * Do not delete the subject (model) when destroying the viewer
     */
    subject = vnul;
    propertyEditor = vnul;
    /*
     * Clear any reference to <this>.
     */
    ObjectTreeNode* node = dynamic_cast<ObjectTreeNode*>(this->subject);
    if (node){

        node->setObjectNodeView(vnul);
    }
}
void StorageObjectForm::init(){

    ObjectTreeNode* node = dynamic_cast<ObjectTreeNode*>(this->subject);
    if (node){

        node->setObjectNodeView(this);
    }
    
    QFormLayout* formLayout = new QFormLayout();
    {
        const QMetaObject *metaobject = subject->metaObject();
        const char* identityProperty = this->identityProperty(metaobject);
        const int count = metaobject->propertyCount();
        /*
         */
        for (int cc = 0; cc < count; cc++) {

            QMetaProperty metaproperty = metaobject->property(cc);

            if (metaproperty.isUser(subject)){

                const char *propName = metaproperty.name();

                bool wri = metaproperty.isWritable();

                QVariant value = subject->property(propName);
                /*
                 * Derivation of editor and label
                 */
                QWidget* editor = vnul;
                QWidget* label = vnul;

                if (propertyEditor){
                    editor = propertyEditor->createPropertyFormEditor(cc,metaproperty);
                    label = propertyEditor->createPropertyFormLabel(cc,metaproperty);
                }
                //
                if (vnul == editor){

                    if (wri){

                        editor = createEditor(value);
                    }
                    else {

                        editor = new QLineEdit(value.toString());
                        editor->setProperty("readOnly",true);
                    }
                }
                //
                if (vnul == label){

                    label = new QLabel(propName);
                }
                /*
                 * Install into form, or throw editor exception
                 */
                if (vnul != editor){
                    /*
                     */
                    if (vnul == label){

                        label = new QLabel(propName);
                    }
                    /*
                     */
                    {
                        QString editorName(propName);
                        editorName += "Editor";

                        editor->setObjectName(editorName);
                    }
                    {
                        QString labelName(propName);
                        labelName += "Label";

                        label->setObjectName(labelName);
                    }
                    /*
                     */
                    formLayout->addRow(label, editor);

                    countUserProperties += 1;
                }
                else {

                    if (vnul != label){

                        delete label;
                    }

                    throw StorageError::EditorNotFoundFor(metaobject->className(),propName);
                }
            }
        }
        this->setLayout(formLayout);
    }
}
bool StorageObjectForm::hasUserProperties(){
    return (0 < countUserProperties);
}
int StorageObjectForm::getCountUserProperties(){
    return countUserProperties;
}
QWidget* StorageObjectForm::createEditor(QVariant& value){

    switch(value.type()){
    case QMetaType::Void:
        return vnul;
    case QMetaType::Bool:
        {
            QCheckBox* editor = new QCheckBox(this);
            editor->setProperty("checked",value);
            return editor;
        }
    case QMetaType::Int:
        {
            QSpinBox* editor = new QSpinBox(this);
            editor->setProperty("value",value);
            editor->setProperty("maximum",INT_MAX);
            editor->setProperty("minimum",INT_MIN);
            return editor;
        }
    case QMetaType::UInt:
        {
            QSpinBox* editor = new QSpinBox(this);
            editor->setProperty("value",value);
            editor->setProperty("maximum",INT_MAX);
            editor->setProperty("minimum",0);
            return editor;
        }
    case QMetaType::Double:
        {
            QDoubleSpinBox* editor = new QDoubleSpinBox(this);
            editor->setProperty("value",value);
            return editor;
        }
    case QMetaType::QChar:
        {
            QLineEdit* editor = new QLineEdit(this);
            editor->setProperty("text",value);
            editor->setProperty("maxLength",1);
            return editor;
        }
    case QMetaType::QString:
        {
            QLineEdit* editor = new QLineEdit(this);
            editor->setProperty("text",value);
            editor->setProperty("maxLength",40);
            return editor;
        }
    case QMetaType::QByteArray:
        return vnul;
    case QMetaType::LongLong:
        {
            QSpinBox* editor = new QSpinBox(this);
            editor->setProperty("value",value);
            editor->setProperty("maximum",INT_MAX);
            editor->setProperty("minimum",INT_MIN);
            return editor;
        }
    case QMetaType::ULongLong:
        {
            QSpinBox* editor = new QSpinBox(this);
            editor->setProperty("value",value);
            editor->setProperty("maximum",INT_MAX);
            editor->setProperty("minimum",0);
            return editor;
        }
    case QMetaType::QCursor:
        return vnul;
    case QMetaType::QDate:
        {
            QDateEdit* editor = new QDateEdit(this);
            editor->setProperty("date",value);
            return editor;
        }
    case QMetaType::QSize:
        return vnul;
    case QMetaType::QTime:
        {
            QTimeEdit* editor = new QTimeEdit(this);
            editor->setProperty("time",value);
            return editor;
        }
    case QMetaType::QVariantList:
    case QMetaType::QPolygon:
    case QMetaType::QColor:
    case QMetaType::QSizeF:
    case QMetaType::QRectF:
    case QMetaType::QLine:
        return vnul;
    case QMetaType::QTextLength:
        return vnul;
    case QMetaType::QStringList:
        return vnul;
    case QMetaType::QVariantMap:
    case QMetaType::QVariantHash:
    case QMetaType::QIcon:
    case QMetaType::QPen:
    case QMetaType::QLineF:
        return vnul;
    case QMetaType::QTextFormat:
        return vnul;
    case QMetaType::QRect:
    case QMetaType::QPoint:
        return vnul;
    case QMetaType::QUrl:
        return vnul;
    case QMetaType::QRegExp:
        return vnul;
    case QMetaType::QDateTime:
        {
            QDateTimeEdit* editor = new QDateTimeEdit(this);
            editor->setProperty("dateTime",value);
            return editor;
        }
    case QMetaType::QPointF:
    case QMetaType::QPalette:
    case QMetaType::QFont:
    case QMetaType::QBrush:
    case QMetaType::QRegion:
    case QMetaType::QBitArray:
    case QMetaType::QImage:
    case QMetaType::QKeySequence:
    case QMetaType::QSizePolicy:
    case QMetaType::QPixmap:
    case QMetaType::QLocale:
    case QMetaType::QBitmap:
    case QMetaType::QMatrix:
    case QMetaType::QTransform:
    case QMetaType::QMatrix4x4:
    case QMetaType::QVector2D:
    case QMetaType::QVector3D:
    case QMetaType::QVector4D:
    case QMetaType::QQuaternion:
    case QMetaType::QEasingCurve:
        return vnul;
    default:
        return vnul;
    }
}
void StorageObjectForm::accept(){
    /*
     * Copy form data to storage model
     */
    const QMetaObject* subjectObject = subject->metaObject();
    const int count = subjectObject->propertyCount();

    for (int cc = 0; cc < count; cc++) {

        QMetaProperty subjectProperty = subjectObject->property(cc);

        if (subjectProperty.isWritable() && subjectProperty.isUser(subject)){

            const char* propName = subjectProperty.name();

            QString editorName(propName);
            editorName += "Editor";

            const QWidget* editor = findChild<QWidget*>(editorName);

            if (editor){

                const QMetaObject* editorMeta = editor->metaObject();

                if ((!propertyEditor) || (!propertyEditor->setPropertyForEditor(cc,subjectProperty,*editor))){
                    /*
                     * Default property processing for
                     * PropertyEditorInterface setPropertyForEditor
                     */
                    if (-1 < editorMeta->indexOfProperty("text")){
                        /*
                         * Line Edit
                         */
                        QVariant value = editor->property("text");

                        qDebug() << "StorageObjectForm accept text " << value;
                        /*
                         * Trusting GUI integrity for "read-only" and
                         * "label" cases. [TODO?]
                         */
                        subject->setProperty(propName,value);
                    }
                    else if (-1 < editorMeta->indexOfProperty("value")){
                        /*
                         * Spin Box
                         */
                        QVariant value = editor->property("value");
                        subject->setProperty(propName,value);
                    }
                    else if (-1 < editorMeta->indexOfProperty("currentIndex")){
                        /*
                         * Combo Box
                         */
                        const QComboBox* comboxBox = qobject_cast<const QComboBox*>(editor);
                        if (vnul != comboxBox){

                            QVariant value = comboxBox->itemData(comboxBox->currentIndex());

                            subject->setProperty(propName,value);
                        }
                    }
                    else if (-1 < editorMeta->indexOfProperty("checked")){
                        /*
                         * Check Box
                         */
                        QVariant value = editor->property("checked");
                        subject->setProperty(propName,value);
                    }
                    else if (-1 < editorMeta->indexOfProperty("dateTime")){

                        const QDateEdit* dateEditor = qobject_cast<const QDateEdit*>(editor);
                        if (vnul != dateEditor){
                            /*
                             * Date Edit
                             */
                            QVariant value = editor->property("date");
                            subject->setProperty(propName,value);
                        }
                        else {
                            const QTimeEdit* timeEditor = qobject_cast<const QTimeEdit*>(editor);
                            if (vnul != timeEditor){
                                /*
                                 * Time Edit
                                 */
                                QVariant value = editor->property("time");
                                subject->setProperty(propName,value);
                            }
                            else {
                                const QDateTimeEdit* dateTimeEditor = qobject_cast<const QDateTimeEdit*>(editor);
                                if (vnul != dateTimeEditor){
                                    /*
                                     * Date Time Edit
                                     */
                                    QVariant value = editor->property("dateTime");
                                    subject->setProperty(propName,value);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /*
     * Write update to storage
     */
    if (storageInterface){

        qDebug() << "StorageObjectForm accept storageInterface->write()";

        storageInterface->write();
    }
    else {

        qDebug() << "StorageObjectForm accept [missing storage interface]";
    }

    emit completed(0);
}
void StorageObjectForm::cancel(){

    emit completed(1);
}
bool StorageObjectForm::isSubject(QObject* node){

    return (node == subject);
}
bool StorageObjectForm::isObjectTreeList(){

    return (dynamic_cast<ObjectTreeList*>(subject));
}
bool StorageObjectForm::isObjectTreeImportExport(){

    return (dynamic_cast<ObjectTreeImportExport*>(subject));
}
const char* StorageObjectForm::identityProperty(const QMetaObject* meta){
    const int ciofs = meta->classInfoOffset();
    const int cicnt = meta->classInfoCount();
    int cc;
    for (cc = ciofs; cc < cicnt; cc++){
        QMetaClassInfo ci = meta->classInfo(cc);
        if (0 == strcmp("Object Name",ci.name())){

            return ci.value();
        }
    }
    return "value not found";
}
void StorageObjectForm::importNode(){
    ObjectTreeImportExport* node = dynamic_cast<ObjectTreeImportExport*>(this->subject);
    if (node){
        node->importToObjectTreeNode();
    }
}
void StorageObjectForm::exportNode(){
    ObjectTreeImportExport* node = dynamic_cast<ObjectTreeImportExport*>(this->subject);
    if (node){
        node->exportFromObjectTreeNode();
    }
}
void StorageObjectForm::nodeDataChanged(const char* propname){
    QString editorName(propname);
    editorName += "Editor";

    QObject* editor = QObject::findChild<QObject*>(editorName);
    if (editor){

        QVariant value = subject->property(propname);

        if (!editor->setProperty("text",value)){

            if (!editor->setProperty("plainText",value)){

                if (!editor->setProperty("value",value)){

                    if (!editor->setProperty("datetime",value)){

                        if (!editor->setProperty("date",value)){

                            if (!editor->setProperty("time",value)){

                                qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") unable to identify editor property [" << editor->metaObject()->className() << "]";

                            }
                            else {
                                qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") set editor(time)";
                            }
                        }
                        else {
                            qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") set editor(date)";
                        }
                    }
                    else {
                        qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") set editor(datetime)";
                    }
                }
                else {
                    qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") set editor(value)";
                }
            }
            else {
                qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") set editor(plainText)";
            }
        }
        else {
            qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") set editor(text)";
        }
    }
    else {
        qDebug() << "StorageObjectForm nodeDataChanged(" << propname << ") missing editor";
    }
}
