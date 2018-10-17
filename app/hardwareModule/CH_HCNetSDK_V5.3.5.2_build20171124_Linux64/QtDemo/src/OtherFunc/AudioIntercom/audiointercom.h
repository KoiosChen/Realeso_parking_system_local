/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   audiointercom.h
 *        Description                   :   audio intercom
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef AUDIOINTERCOM_H
#define AUDIOINTERCOM_H

#include <QtGui/QWidget>
#include "ui_audiointercom.h"

class AudioIntercom : public QWidget
{
    Q_OBJECT

public:
    AudioIntercom(QWidget *parent = 0);
    ~AudioIntercom();

private:
    Ui::AudioIntercomClass ui;
};

#endif // AUDIOINTERCOM_H

