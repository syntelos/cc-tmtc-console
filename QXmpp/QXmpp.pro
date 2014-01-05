TARGET   = QXmpp
TEMPLATE = lib

QT      += network
QT      += xml

CONFIG  += shared
CONFIG  += thread

HEADERS += QXmppArchiveIq.h \
           QXmppArchiveManager.h \
           QXmppBindIq.h \
           QXmppBookmarkManager.h \
           QXmppBookmarkSet.h \
           QXmppByteStreamIq.h \
           QXmppCallManager.h \
           QXmppClientExtension.h \
           QXmppClient.h \
           QXmppCodec_p.h \
           QXmppConfiguration.h \
           QXmppConstants.h \
           QXmppDataForm.h \
           QXmppDiscoveryIq.h \
           QXmppDiscoveryManager.h \
           QXmppElement.h \
           QXmppEntityTimeIq.h \
           QXmppEntityTimeManager.h \
           QXmppGlobal.h \
           QXmppIbbIq.h \
           QXmppInvokable.h \
           QXmppIq.h \
           QXmppJingleIq.h \
           QXmppLogger.h \
           QXmppMessage.h \
           QXmppMessageReceiptManager.h \
           QXmppMucIq.h \
           QXmppMucManager.h \
           QXmppNonSASLAuth.h \
           QXmppOutgoingClient.h \
           QXmppPingIq.h \
           QXmppPresence.h \
           QXmppPubSubIq.h \
           QXmppRegisterIq.h \
           QXmppRemoteMethod.h \
           QXmppResultSet.h \
           QXmppRosterIq.h \
           QXmppRosterManager.h \
           QXmppRpcIq.h \
           QXmppRpcManager.h \
           QXmppRtpChannel.h \
           QXmppSasl_p.h \
           QXmppSessionIq.h \
           QXmppSocks.h \
           QXmppStanza.h \
           QXmppStreamFeatures.h \
           QXmppStream.h \
           QXmppStreamInitiationIq_p.h \
           QXmppStun.h \
           QXmppTransferManager.h \
           QXmppTransferManager_p.h \
           QXmppUtils.h \
           QXmppVCardIq.h \
           QXmppVCardManager.h \
           QXmppVersionIq.h \
           QXmppVersionManager.h

SOURCES += QXmppArchiveIq.cpp \
           QXmppArchiveManager.cpp \
           QXmppBindIq.cpp \
           QXmppBookmarkManager.cpp \
           QXmppBookmarkSet.cpp \
           QXmppByteStreamIq.cpp \
           QXmppCallManager.cpp \
           QXmppClient.cpp \
           QXmppClientExtension.cpp \
           QXmppCodec.cpp \
           QXmppConfiguration.cpp \
           QXmppConstants.cpp \
           QXmppDataForm.cpp \
           QXmppDiscoveryIq.cpp \
           QXmppDiscoveryManager.cpp \
           QXmppElement.cpp \
           QXmppEntityTimeIq.cpp \
           QXmppEntityTimeManager.cpp \
           QXmppGlobal.cpp \
           QXmppIbbIq.cpp \
           QXmppInvokable.cpp \
           QXmppIq.cpp \
           QXmppJingleIq.cpp \
           QXmppLogger.cpp \
           QXmppMessage.cpp \
           QXmppMessageReceiptManager.cpp \
           QXmppMucIq.cpp \
           QXmppMucManager.cpp \
           QXmppNonSASLAuth.cpp \
           QXmppOutgoingClient.cpp \
           QXmppPingIq.cpp \
           QXmppPresence.cpp \
           QXmppPubSubIq.cpp \
           QXmppRegisterIq.cpp \
           QXmppRemoteMethod.cpp \
           QXmppResultSet.cpp \
           QXmppRosterIq.cpp \
           QXmppRosterManager.cpp \
           QXmppRpcIq.cpp \
           QXmppRpcManager.cpp \
           QXmppRtpChannel.cpp \
           QXmppSasl.cpp \
           QXmppSessionIq.cpp \
           QXmppSocks.cpp \
           QXmppStanza.cpp \
           QXmppStream.cpp \
           QXmppStreamFeatures.cpp \
           QXmppStreamInitiationIq.cpp \
           QXmppStun.cpp \
           QXmppTransferManager.cpp \
           QXmppUtils.cpp \
           QXmppVCardIq.cpp \
           QXmppVCardManager.cpp \
           QXmppVersionIq.cpp \
           QXmppVersionManager.cpp
