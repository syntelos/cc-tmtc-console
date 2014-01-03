TARGET   = Storage
TEMPLATE = lib

QT      += sql
QT      += script

CONFIG  += staticlib

INCLUDEPATH += ../

SOURCES += StorageEditor.cpp \
           StorageObjectForm.cpp \
           StorageEditorDialog.cpp \
           StorageTreeEditor.cpp \
           StorageTreeEditorDialog.cpp


HEADERS += StorageEditor.h \
           StorageObjectForm.h \
           StorageEditorDialog.h \
           StorageList.h \
           StorageListItem.h \
           StorageTreeEditor.h \
           StorageTreeEditorDialog.h \
           StorageInterface.h \
           StorageModel.h \
           StorageModelRoot.h \
           StorageError.h \
           PropertyEditorInterface.h


