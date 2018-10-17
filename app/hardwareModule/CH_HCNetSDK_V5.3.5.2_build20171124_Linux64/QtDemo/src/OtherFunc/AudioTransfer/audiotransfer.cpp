/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   audiotransfer.cpp
 *        Description                   :   audio transfer
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "audiotransfer.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include "DemoPublic.h"

#include "qtclientdemo.h"
extern QtClientDemo* gqtclinetdemo;

#ifndef _WIN32
extern unsigned int seg_isram; 
#endif
/*****************************************************************************
*
*  Routines to determine byte order (of this machine) and swap bytes
*
*****************************************************************************/





enum byte_order { order_unknown, order_bigEndian, order_littleEndian };

static enum byte_order DetermineByteOrder (void)
{
	char s[sizeof (long) + 1];
	union 
	{
		long longval;
		char charval[sizeof (long)];
	} probe;
	probe.longval = 0x41424344L;	/* ABCD in ASCII */
	strncpy (s, probe.charval, sizeof (long));
	s[sizeof (long)] = '\0';
	/* fprintf( stderr, "byte order is %s\n", s ); */
	
	if (strcmp (s, "ABCD") == 0)
		return order_bigEndian;
	else if (strcmp (s, "DCBA") == 0)
		return order_littleEndian;
	else
		return order_unknown;
}

static void SwapBytesInWords (short *loc, int words)
{
	int i;
	short thisval;
	char *dst, *src;
	src = (char *) &thisval;
	for (i = 0; i < words; i++) 
	{
		thisval = *loc;
		dst = (char *) loc++;
		dst[0] = src[1];
		dst[1] = src[0];
	}
}




/*****************************************************************************
*
*  Read in specified number of samples
*
*****************************************************************************/

int wave_get_samples (wave_info_t *wave_info, short int pcm[], int numSamples)
{
	int samples_read;
	FILE *file = wave_info->soundfile;
	
	samples_read = fread (pcm, sizeof(short int), numSamples, file);
	    
	/*if(samples_read < numSamples)
    {
        fseek(file, 0, SEEK_SET);
    	samples_read = fread (pcm, sizeof(short int), numSamples, file);
    }*/
		
	if (wave_info->byteswap) 
	{
		SwapBytesInWords (pcm, samples_read);
	}
	
	return (samples_read/wave_info->channels);
}




/*****************************************************************************
*
*  parse the wave header.
*  returns NULL if not open failed
*  returns a wave_info_t * if wave header successfully parsed.
*  needs to fill in : samplerate + channels
*  
*  POST: wave_info->soundfile is set to be at the start of the
*        PCM audio data 
*
*****************************************************************************/

wave_info_t *wave_init (char *inPath, WaveFileHeader *waveheader)
{
	unsigned char wave_header_buffer[40];	//HH fixed
	int wave_header_read   = 0;
	int wave_header_stereo = -1;
	int wave_header_16bit  = -1;
	unsigned long samplerate;
	enum byte_order NativeByteOrder = order_unknown;

	
	wave_info_t *wave_info = NULL;
	FILE *file;
	
	if ( (file = fopen(inPath, "rb"))==NULL)
	{
		DebugPrint("WAV: cannot open input file: %s\n", inPath);
		return(NULL);
	}
	
	
	/************** WAVE ************************/
	/*   Nick Burch <The_Leveller@newmail.net>  */
	/********************************************/
	/* Wave File Headers:   (Dec)				*/
	/* 8-11 = "WAVE"							*/
	/* 22 = Stereo / Mono						*/
	/*		01 = mono, 02 = stereo				*/
	/* 24 = Sampling Frequency					*/
	/* 32 = Data Rate							*/
	/*		01 = x1 (8bit Mono)					*/
	/*		02 = x2 (8bit Stereo or				*/
	/*				 16bit Mono)				*/
	/*		04 = x4 (16bit Stereo)				*/
	/********************************************/
	fread(waveheader,1 , sizeof(WaveFileHeader),file);
	fseek (file, 0, SEEK_SET);
	fread (wave_header_buffer, 1, 40, file);
	
	if (wave_header_buffer[8]  == 'W' && wave_header_buffer[9] == 'A'
	 && wave_header_buffer[10] == 'V' && wave_header_buffer[11] == 'E')
	{
		DebugPrint("Parsing Wave File Header\n");
		if (NativeByteOrder == order_unknown) 
		{
			NativeByteOrder = DetermineByteOrder ();
			if (NativeByteOrder == order_unknown) 
			{
				DebugPrint ("byte order not determined\n");
				fclose(file); return(NULL);
			}
		}
		
		if (NativeByteOrder == order_littleEndian)
		{   
		#ifdef _WIN32
			samplerate = *(unsigned long *) (&wave_header_buffer[24]);   
		#else
		     samplerate = *(unsigned int *) (&wave_header_buffer[24]); 
		#endif
		} 
		else 
		{
			samplerate = wave_header_buffer[27] +
			(wave_header_buffer[26] << 8) +
			(wave_header_buffer[25] << 16) + (wave_header_buffer[24] << 24);
		}
		
		/* Wave File */
		wave_header_read = 1;
		switch (samplerate) 
		{
			case 44100:
			case 48000:
			case 32000:
			case 24000:
			case 22050:
			case 16000:
				DebugPrint (">>> %ld Hz sampling freq selected\n", samplerate);
			break;
			default:
				/* Unknown Unsupported Frequency */
				DebugPrint (">>> Unknown samp freq %ld Hz in Wave Header\n",
				samplerate);
				DebugPrint (">>> Default 44.1 kHz samp freq selected\n");
				samplerate = 44100;
			break;
		}
		
		if ((long) wave_header_buffer[22] == 1) 
		{
			DebugPrint (">>> Input Wave File is Mono\n");
			wave_header_stereo = 0;
		}
		if ((long) wave_header_buffer[22] == 2) 
		{
			DebugPrint (">>> Input Wave File is Stereo\n");
			wave_header_stereo = 1;
		}

		if ((long) wave_header_buffer[32] == 1) 
		{
			DebugPrint (">>> Input Wave File is 8 Bit\n");
			wave_header_16bit = 0;
			DebugPrint("Input File must be 16 Bit! Please Re-sample");
			fclose(file); return(NULL);
		}
		if ((long) wave_header_buffer[32] == 2) 
		{
			if (wave_header_stereo == 1) 
			{
				DebugPrint(">>> Input Wave File is 8 Bit\n");
				wave_header_16bit = 0;
				DebugPrint("Input File must be 16 Bit! Please Re-sample");
				fclose(file); return(NULL);
			} 
			else 
			{
				fprintf(stderr,  ">>> Input Wave File is 16 Bit\n" ); 
				wave_header_16bit = 1;
			}
		}

		if ((long) wave_header_buffer[32] == 4) 
		{
			/* fprintf(stderr,  ">>> Input Wave File is 16 Bit\n" ); */
			wave_header_16bit = 1;
		}

		/* should probably use the wave header to determine size here FIXME MFC Feb 2003 */
		if (fseek (file, 44, SEEK_SET) != 0) 
		{
			/* there's a way of calculating the size of the
			   wave header. i'll just jump 44 to start with */
			DebugPrint( "Could not seek to PCM sound data in \"%s\".\n", inPath);
			fclose(file); return(NULL);
		}
		
		// Successfully processed the wave header
		wave_info = (wave_info_t *)calloc(1, sizeof(wave_info_t));
		wave_info->soundfile = file;
		if (wave_header_stereo == 1)
				wave_info->channels = 2;
		else	
            wave_info->channels = 1;
        
		wave_info->samplerate = samplerate;
		
		// UNKNOWN. But we really should be able to work 
		// it out. FIX THIS. MFC May03.
		wave_info->num_samples = -1;
		
		// Enable byte swap for big endian machines
		if (NativeByteOrder == order_bigEndian)
				wave_info->byteswap = 1;
		else	wave_info->byteswap = 0;
			
		return(wave_info);
	}
	
	// not a wave file
	fclose(file);  
	return(NULL);
}


/************************************************************************
 *        Function            :  AudioTransfer
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
AudioTransfer::AudioTransfer(QList<DeviceData> * tree,QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	m_qlistdevicetree =tree;
	
    for (int j=0;j<256;j++)
    {
        m_atuserid[j]=-1;
    }
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(); it != (*m_qlistdevicetree).end(); ++it)
    {
        if ((*it).getUsrID()>=0)
        {
            m_atuserid[i]=(*it).getUsrID();//i和index一一对应
            i++;
            items.append(new QTreeWidgetItem((QTreeWidget*)0, 
				QStringList((*it).getDeviceName())));
        }
    }
    ui.treeWidget->insertTopLevelItems(0, items);

	connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *, 
		int)),this,SLOT(audioTransferDevice(QTreeWidgetItem *, int)));
	m_atindex = 0;
	m_talkmrhandle = -1;
	m_encodehandle = NULL;
	
}



/************************************************************************
 *        Function            :  ~AudioTransfer
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
AudioTransfer::~AudioTransfer()
{

}

void AudioTransfer::audioTransferDevice(QTreeWidgetItem * item, int  column)
{
	QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; it != (*m_qlistdevicetree).end(); 
	++it)
    {
    	
        if ((*it).getDeviceName()==item->text(column))
        {
        	m_atindex =i;
			break;
        }
		if ((*it).getUsrID()>=0)
    	{
			i++;
		}
		
    }

}

void AudioTransfer::on_pushButton_browse_clicked()
{
	 QString directory = QFileDialog::getOpenFileName(this,\
           tr("Find Update Files DIR and Filename"), QDir::currentPath());
     if (!directory.isEmpty()) 
	 {
         ui.lineEdit_sendfilepath->insert(directory);
     }
	 return;

}

/*******************************************************************
      Function:   fVoiceDataCallBack
   Description:   语音转发回调函数，用于获取设备的语音数据
     Parameter:   (IN)   int lVoiceComHandle  
                  (IN)   char *pRecvDataBuffer  
                  (IN)   unsigned int dwBufSize  
                  (IN)   unsigned char byAudioFlag  
                  (IN)   void *pUser  
        Return:   HPR_OK--成功，HPR_ERROR--失败。  HPR_OK--成功，HPR_ERROR--失败。   
**********************************************************************/
void  __stdcall fVoiceDataCallBack(LONG lVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, BYTE byAudioFlag, void *pUser)
{
    //存入表格
    QString currentdatetime = QDateTime::currentDateTime().toString(" yyyy-MM-dd hh:mm:ss");
   	QString datastring = QString(QLatin1String(pRecvDataBuffer));
	QString handle = QString::number(lVoiceComHandle);
	QString totalsize = QString::number(dwBufSize);
	QString audioflag = QString::number(int(byAudioFlag));
	QString userinfo = QString((char *)pUser);
	
	if (gqtclinetdemo->m_audiotransfer!=NULL)
	{
		QTableWidgetItem *currnettime = new QTableWidgetItem(currentdatetime);	
		QTableWidgetItem *handleitem = new QTableWidgetItem(handle);	
		QTableWidgetItem *sizeitem = new QTableWidgetItem(totalsize);	
		QTableWidgetItem *flagitem = new QTableWidgetItem(audioflag);	
		QTableWidgetItem *usrinfoitem = new QTableWidgetItem(userinfo);	
		int row = gqtclinetdemo->m_audiotransfer->ui.tableWidget->rowCount();
		gqtclinetdemo->m_audiotransfer->ui.tableWidget->insertRow(row);
		gqtclinetdemo->m_audiotransfer->ui.tableWidget->setItem(row, 0, currnettime);
		gqtclinetdemo->m_audiotransfer->ui.tableWidget->setItem(row, 1, handleitem);
		gqtclinetdemo->m_audiotransfer->ui.tableWidget->setItem(row, 2, sizeitem);
		gqtclinetdemo->m_audiotransfer->ui.tableWidget->setItem(row, 3, flagitem);
		gqtclinetdemo->m_audiotransfer->ui.tableWidget->setItem(row, 4, usrinfoitem);
	}

    //Send data to device after getting data.
    char pSendData[80];
    memset(pSendData, 0, 80);
    NET_DVR_VoiceComSendData(lVoiceComHandle, pSendData, 80);
}

/*******************************************************************
      Function:   AudioTransfer::on_pushButton_startmr_clicked
   Description:   开始语音转发btn
     Parameter:   (IN)   无参数  
        Return:   无  
**********************************************************************/
void AudioTransfer::on_pushButton_startmr_clicked()
{
	m_talkmrhandle = NET_DVR_StartVoiceCom_MR_V30(m_atuserid[m_atindex],1,fVoiceDataCallBack,0);

	if (m_talkmrhandle < 0)
	{
		QMessageBox::information(this,tr("NET_DVR_StartVoiceCom_MR_V30 error"),
			tr("m_atindex=%1 m_atuserid[m_atindex]=%2").arg(m_atindex).arg(m_atuserid[m_atindex]));

	}
	else
	{
		QMessageBox::information(this,tr("NET_DVR_StartVoiceCom_MR_V30 SUCC"),
			tr("NET_DVR_StartVoiceCom_MR_V30 SUCC"));
		ui.pushButton_startmr->setDisabled(1);
		ui.pushButton_stopmr->setDisabled(0);
	}

}
void AudioTransfer::on_pushButton_stopmr_clicked()
{
  if (!NET_DVR_StopVoiceCom(m_talkmrhandle))
  {
		QMessageBox::information(this,tr("NET_DVR_StopVoiceCom error"),
			tr("lasterror=%1").arg(NET_DVR_GetLastError()));
  }
  else
  {		
  		QMessageBox::information(this,tr("NET_DVR_StopVoiceCom SUCC"),
			tr("NET_DVR_StopVoiceCom SUCC"));
		ui.pushButton_stopmr->setDisabled(1);
		ui.pushButton_startmr->setDisabled(0);
  }

}

void AudioTransfer::on_pushButton_talkmrsend_clicked()
{
	m_encodehandle = NET_DVR_InitG722Encoder();

        if (m_encodehandle == (void*)-1)
	{
		QMessageBox::information(this,tr("NET_DVR_InitG722Encoder error"),
			tr("LAST_ERROR=%1").arg(NET_DVR_GetLastError()));

	}
	else
	{
		QMessageBox::information(this,tr("NET_DVR_InitG722Encoder SUCC"),
			tr("NET_DVR_InitG722Encoder SUCC"));
	}

	wave_info_t*  para = (wave_info_t*)malloc(sizeof(wave_info_t));
	WaveFileHeader header;
	short int pcmdata[640];
	int i;
	para = wave_init(ui.lineEdit_sendfilepath->text().toLatin1().data(), &header);

	if ((para->channels!=1)&&(para->samplesize!=16)&&(para->samplerate!=16000))
	{
		printf("the file is not correct for its parameters!\n");
		QMessageBox::information(this,tr("音频文件错误"),
			tr("音频文件参数错误"));
		return ;
	}
	unsigned char  sendcontent[80]={0};

	//int time =header.length/1280;
	//while (time>0)
	//{
		i=wave_get_samples(para,pcmdata,640);
		//printf("time=%d\n",time);
	  	
		if (!NET_DVR_EncodeG722Frame(m_encodehandle, (unsigned char*)pcmdata, sendcontent))
		{
			QMessageBox::information(this,tr("NET_DVR_EncodeG722Frame error"),
				tr("lasterror=%1").arg(NET_DVR_GetLastError()));
		}
		else
		{		
			QMessageBox::information(this,tr("NET_DVR_EncodeG722Frame SUCC"),
				tr("NET_DVR_EncodeG722Frame SUCC"));
		}
		

		if (!NET_DVR_VoiceComSendData(m_talkmrhandle,(char*)sendcontent,80))
		{
			QMessageBox::information(this,tr("NET_DVR_VoiceComSendData error"),
				tr("lasterror=%1").arg(NET_DVR_GetLastError()));
		}
		else
		{		
		
		}
		//usleep(40000);
		//time -- ;
	//}
	NET_DVR_ReleaseG722Encoder(m_encodehandle);
	QMessageBox::information(this,tr("NET_DVR_VoiceComSendData SUCC"),
				tr("NET_DVR_VoiceComSendData SUCC"));
}



/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  退出界面
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void AudioTransfer::on_pushButton_exit_clicked()
{
	close();
}

