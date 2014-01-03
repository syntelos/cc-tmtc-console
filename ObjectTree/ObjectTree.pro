TARGET   = ObjectTree
TEMPLATE = lib

CONFIG  += staticlib

INCLUDEPATH += ../


SOURCES += ObjectTreeModel.cpp \
           ObjectTreeView.cpp \
           ObjectTreeNode.cpp

HEADERS += ObjectTreeModel.h \
           ObjectTreeView.h \
           ObjectTreeList.h \
           ObjectTreeImportExport.h \
           ObjectTreeNode.h

