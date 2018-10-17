#ifndef CCOMPRESSAUDIO_H
#define CCOMPRESSAUDIO_H

#include <QtGui/QDialog>
#include "ui_ccompressaudio.h"

class CCompressAudio : public QDialog
{
    Q_OBJECT

public:
    CCompressAudio(QWidget *parent = 0);
    ~CCompressAudio();

    int setUserIDAndChannel(long lUserID, long lChannel);

    //get rtsp info
      int getCompressAudioInfo();
      //set rtsp info
      int setCompressAudioInfo();

      //event
       void showEvent ( QShowEvent * event );

  public slots:
     void clickSave();
     void clickRefresh();
     void on_btnExit_clicked();

private:
    Ui::CCompressAudioClass ui;

    long m_lUserID;
    long m_lChannel;
};

#endif // CCOMPRESSAUDIO_H
