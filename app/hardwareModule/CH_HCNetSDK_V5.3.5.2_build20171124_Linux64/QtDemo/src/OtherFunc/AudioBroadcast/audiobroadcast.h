/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   audiobroadcast.h
 *        Description                   :   audio broadcast
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef AUDIOBROADCAST_H
#define AUDIOBROADCAST_H

#include <QtGui/QWidget>
#include "ui_audiobroadcast.h"

class AudioBroadcast : public QWidget
{
    Q_OBJECT

public:
    AudioBroadcast(QWidget *parent = 0);
    ~AudioBroadcast();

private:
    Ui::AudioBroadcastClass ui;
};

#endif // AUDIOBROADCAST_H

