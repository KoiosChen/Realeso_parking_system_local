/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   audiotransfer.h
 *        Description                   :   audio transfer
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef AUDIOTRANSFER_H
#define AUDIOTRANSFER_H

#include <QtGui/QWidget>
#include "ui_audiotransfer.h"
#include <QList>
#include "DemoPublic.h"
#include "devicedata.h"

#include <stdio.h>
#include <stdlib.h>
#include <QDialog>

//平台差异
#ifdef _WIN32
#elif  defined(__linux__)
#ifndef __stdcall
#define __stdcall
#endif
#endif

#define DebugPrint printf
typedef struct wave_info_struct
{
	FILE *soundfile;			// file handle 文件句柄
	short channels;				// 1 - mono . 2 - stereo. d'uh. 通道号
	unsigned long num_samples;	// total number of samples/channel if available //总采样数
	short samplesize;			// 8 bit or 16bit. could probably just auto scale. but i think
								// we'll just ignore 8bit sound. MFC May03 //bit率
	int samplerate;				// in Hz 采样频率
	int byteswap;				//交换字节数
}
wave_info_t; //多媒体文件语音参数信息结构

typedef  struct wave_header
  {
    char riff[4];             /* "RIFF" */
    unsigned int  size;       /* length of rest of file = size of rest of header(36) + data length */
    char wave[4];             /* "WAVE" */
    char fmt[4];              /* "fmt " */
    unsigned int   fmt_len;    /* length of rest of fmt chunk = 16 */
    unsigned short tag;       /* MS PCM = 1 */
    unsigned short channels;  /* channels, mono = 1, stereo = 2 */
    unsigned int   samp_rate;  /* samples per second = 44100 */
    unsigned int   byte_rate;  /* bytes per second = samp_rate * byte_samp = 176400 */
    unsigned short byte_samp; /* block align (bytes per sample) = channels * bits_per_sample / 8 = 4 */
    unsigned short bit_samp;  /* bits per sample = 16 for MS PCM (format specific) */
    char data[4];             /* "data" */
    unsigned int  length;     /* data length (bytes) */
  } WaveFileHeader;//多媒体文件


wave_info_t *wave_init(char *inPath,WaveFileHeader *waveheader);//给定文件获取文件参数信息 和 文件头
int wave_get_samples(wave_info_t *wave_info, short int pcm[], int num_samples); //压缩文件

class AudioTransfer : public QDialog
{
    Q_OBJECT

public:
	friend void __stdcall fVoiceDataCallBack(int lVoiceComHandle,
  						char *pRecvDataBuffer, unsigned int dwBufSize,
  						unsigned char byAudioFlag,  void *pUser);
    AudioTransfer(QList<DeviceData> * tree, QDialog *parent = 0);
    ~AudioTransfer();

private slots:
	void on_pushButton_exit_clicked();
	void audioTransferDevice(QTreeWidgetItem * item, int  column);
	void on_pushButton_browse_clicked();
	void on_pushButton_talkmrsend_clicked();


	void on_pushButton_startmr_clicked();
	void on_pushButton_stopmr_clicked();
	
public:
    Ui::AudioTransferClass ui;
	QList<QTreeWidgetItem *> items;
    QList<DeviceData> *m_qlistdevicetree;
    int m_atuserid[256];
    //布防撤防标记
    int m_atuseridflag[256];
    //选中索引值
    int m_atindex;

	int m_talkmrhandle;
	void * m_encodehandle;
};

#endif // AUDIOTRANSFER_H

