TEMPLATE = app


UI_DIR  = obj/Gui
MOC_DIR = obj/Moc
OBJECTS_DIR = obj/Obj

TARGET = ../lib/QtClientDemo
QT +=	core \
	gui \
	opengl


TRANSLATIONS = ../../translation/QtDemo_zh_CN.ts	

RESOURCES = ../../src/Player.qrc


win32 {
}

unix {
LIBS += -L../lib/ -Wl,-rpath=./:./HCNetSDKCom:../lib -lhcnetsdk -lPlayCtrl -lAudioRender -lSuperRender
}

HEADERS += ../../src/MainWindow/DeviceTree/DeviceAttr/deviceattr.h \
    ../../src/MainWindow/DeviceTree/ChannelAttr/channelattr.h \
    ../../src/MainWindow/DeviceTree/AddNode/addnode.h \
    ../../src/MainWindow/DeviceTree/treemodel.h \
    ../../src/MainWindow/DeviceTree/treeitem.h \
    ../../src/MainWindow/DeviceTree/devicedata.h \
    ../../src/MainWindow/DeviceTree/channeldata.h \
    ../../src/MainWindow/LogAlarm/AlarmTable/alarmtable.h \
    ../../src/MainWindow/LogAlarm/LogTable/logtable.h \
    ../../src/RealPlay/realplay.h \
    ../../src/RealPlay/PTZ/PtzCruise/ptzcruise.h \
    ../../src/RealPlay/PTZ/PtzPreset/ptzpreset.h \
    ../../src/PlayBack/playback.h \
    ../../src/ManageDevice/Timing/timing.h \
    ../../src/ManageDevice/Update/remoteupdate.h \
    ../../src/ManageDevice/Format/formatdisk.h \
    ../../src/ManageDevice/Reboot/rebootdevice.h \
    ../../src/ManageDevice/Shutdown/shutdowndevice.h \
    ../../src/ManageDevice/LogSearch/logsearch.h \
    ../../src/ManageDevice/DeviceState/devicestate.h \
    ../../src/ManageDevice/Reset/resetdevice.h \
    ../../src/OtherFunc/Deployment/deploy.h \
    ../../src/OtherFunc/DevicePan/devicepan.h \
    ../../src/OtherFunc/ImportConfig/importconfig.h \
    ../../src/OtherFunc/SerialTransfer/serialtransfer.h \
    ../../src/OtherFunc/AudioBroadcast/audiobroadcast.h \
    ../../src/OtherFunc/AudioIntercom/audiointercom.h \
    ../../src/OtherFunc/AudioTransfer/audiotransfer.h \
    ../../src/ExitModule/exitdemo.h \
    ../../src/qtclientdemo.h \
    ../../src/ParaConfig/configure_params/PublicFuc/publicfuc.h \
    ../../src/ParaConfig/configure_params/cconfigureparams.h \
    ../../src/RealPlay/FramePlayWnd.h \
    ../../src/Public/DemoPublic.h
     
SOURCES += ../../src/MainWindow/DeviceTree/DeviceAttr/deviceattr.cpp \
    ../../src/MainWindow/DeviceTree/ChannelAttr/channelattr.cpp \
    ../../src/MainWindow/DeviceTree/AddNode/addnode.cpp \
    ../../src/MainWindow/DeviceTree/treemodel.cpp \
    ../../src/MainWindow/DeviceTree/treeitem.cpp \
    ../../src/MainWindow/DeviceTree/devicedata.cpp \
    ../../src/MainWindow/DeviceTree/channeldata.cpp \
    ../../src/MainWindow/LogAlarm/AlarmTable/alarmtable.cpp \
    ../../src/MainWindow/LogAlarm/LogTable/logtable.cpp \
    ../../src/RealPlay/realplay.cpp \
    ../../src/RealPlay/PTZ/PtzCruise/ptzcruise.cpp \
    ../../src/RealPlay/PTZ/PtzPreset/ptzpreset.cpp \
    ../../src/PlayBack/playback.cpp \
    ../../src/ManageDevice/Timing/timing.cpp \
    ../../src/ManageDevice/Update/remoteupdate.cpp \
    ../../src/ManageDevice/Format/formatdisk.cpp \
    ../../src/ManageDevice/Reboot/rebootdevice.cpp \
    ../../src/ManageDevice/Shutdown/shutdowndevice.cpp \
    ../../src/ManageDevice/LogSearch/logsearch.cpp \
    ../../src/ManageDevice/DeviceState/devicestate.cpp \
    ../../src/ManageDevice/Reset/resetdevice.cpp \
    ../../src/OtherFunc/Deployment/deploy.cpp \
    ../../src/OtherFunc/DevicePan/devicepan.cpp \
    ../../src/OtherFunc/ImportConfig/importconfig.cpp \
    ../../src/OtherFunc/SerialTransfer/serialtransfer.cpp \
    ../../src/OtherFunc/AudioBroadcast/audiobroadcast.cpp \
    ../../src/OtherFunc/AudioIntercom/audiointercom.cpp \
    ../../src/OtherFunc/AudioTransfer/audiotransfer.cpp \
    ../../src/ExitModule/exitdemo.cpp \
    ../../src/main.cpp \
    ../../src/qtclientdemo.cpp \
	  ../../src/ParaConfig/configure_params/PublicFuc/publicfuc.cpp \
	  ../../src/ParaConfig/configure_params/cconfigureparams.cpp \
          ../../src/RealPlay/FramePlayWnd.cpp
	  
FORMS += ../../src/MainWindow/DeviceTree/DeviceAttr/deviceattr.ui \
    ../../src/MainWindow/DeviceTree/ChannelAttr/channelattr.ui \
    ../../src/MainWindow/DeviceTree/AddNode/addnode.ui \
    ../../src/MainWindow/LogAlarm/AlarmTable/alarmtable.ui \
    ../../src/MainWindow/LogAlarm/LogTable/logtable.ui \
    ../../src/RealPlay/realplay.ui \
    ../../src/RealPlay/PTZ/PtzCruise/ptzcruise.ui \
    ../../src/RealPlay/PTZ/PtzPreset/ptzpreset.ui \
    ../../src/PlayBack/playback.ui \
    ../../src/ManageDevice/Timing/timing.ui \
    ../../src/ManageDevice/Update/remoteupdate.ui \
    ../../src/ManageDevice/Format/formatdisk.ui \
    ../../src/ManageDevice/Reboot/rebootdevice.ui \
    ../../src/ManageDevice/Shutdown/shutdowndevice.ui \
    ../../src/ManageDevice/LogSearch/logsearch.ui \
    ../../src/ManageDevice/DeviceState/devicestate.ui \
    ../../src/ManageDevice/Reset/resetdevice.ui \
    ../../src/OtherFunc/Deployment/deploy.ui \
    ../../src/OtherFunc/DevicePan/devicepan.ui \
    ../../src/OtherFunc/ImportConfig/importconfig.ui \
    ../../src/OtherFunc/SerialTransfer/serialtransfer.ui \
    ../../src/OtherFunc/AudioBroadcast/audiobroadcast.ui \
    ../../src/OtherFunc/AudioIntercom/audiointercom.ui \
    ../../src/OtherFunc/AudioTransfer/audiotransfer.ui \
    ../../src/ExitModule/exitdemo.ui \
    ../../src/qtclientdemo.ui \
    ../../src/ParaConfig/configure_params/cconfigureparams.ui \
    ../../src/RealPlay/FramePlayWnd.ui

INCLUDEPATH += ../../src/MainWindow/DeviceTree/ \
    ../../src/MainWindow/DeviceTree/AddNode/ \
    ../../src/MainWindow/DeviceTree/DeviceAttr/ \
    ../../src/MainWindow/LogAlarm/AlarmTable/ \
    ../../src/MainWindow/LogAlarm/LogTable/ \
    ../../src/MainWindow/DeviceTree/ChannelAttr/ \
    ../../src/RealPlay/ \
    ../../src/RealPlay/PTZ/PtzCruise/ \
    ../../src/RealPlay/PTZ/PtzPreset/  \
    ../../src/PlayBack/ \
    ../../src/ManageDevice/Timing/ \
    ../../src/ManageDevice/Update/ \
    ../../src/ManageDevice/Format/ \
    ../../src/ManageDevice/Reboot/ \
    ../../src/ManageDevice/Shutdown/ \
    ../../src/ManageDevice/LogSearch/ \
    ../../src/ManageDevice/DeviceState/ \
    ../../src/ManageDevice/Reset/ \
    ../../src/OtherFunc/Deployment/ \
    ../../src/OtherFunc/DevicePan/ \
    ../../src/OtherFunc/ImportConfig/ \
    ../../src/OtherFunc/SerialTransfer/ \
    ../../src/OtherFunc/AudioBroadcast/ \
    ../../src/OtherFunc/AudioIntercom/ \
    ../../src/OtherFunc/AudioTransfer/ \
    ../../src/ExitModule/ \
    ../../src/ParaConfig/configure_params/ \
    ../../src/ParaConfig/configure_params/RemoteParams \
    ../../src/ParaConfig/configure_params/RemoteParams/NetParams \
    ../../src/ParaConfig/configure_params/RemoteParams/ChannelParams \
    ../../src/ParaConfig/configure_params/RemoteParams/UserParams \
    ../../src/ParaConfig/configure_params/RemoteParams/ExceptionParams \
    ../../src/ParaConfig/configure_params/RemoteParams/SerialParams \
    ../../src/ParaConfig/configure_params/RemoteParams/AlarmParams \
    ../../src/ParaConfig/configure_params/RemoteParams/ATMParams\
    ../../src/ParaConfig/configure_params/PublicFuc\
    ../../src/ParaConfig/configure_params/IPParams\
    ../../src/ParaConfig/configure_params/HardDiskParams \
    ../../src/  \
    ../../includeCn \
    ../../src/Public

include(../../src/ParaConfig/configure_params/RemoteParams/RemoteParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/NetParams/NetParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/ChannelParams/ChannelParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/UserParams/UserParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/ExceptionParams/ExceptionParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/SerialParams/SerialParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/AlarmParams/AlarmParams.pri)
include(../../src/ParaConfig/configure_params/RemoteParams/ATMParams/ATMParams.pri)
include(../../src/ParaConfig/configure_params/IPParams/IPParams.pri)
include(../../src/ParaConfig/configure_params/HardDiskParams/HardDiskParams.pri)

