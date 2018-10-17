/************************************************************************
 *        	Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        	FileName                      	:   	playback.h
 *        	Description                   	:   	playback header file 
 *			Modification History			:      
 *  		<version >		<time>     		<author>      	<desc>
 *  		V1.0			2010-1,2		wanggongpu   	creating
************************************************************************/

#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <QtGui/QWidget>
#include <QString>
#include <QTimer>
#include <QList>
#include "ui_playback.h"
#include "channeldata.h"
#include "devicedata.h"
#include "DemoPublic.h"

#include "common.h"
#include "logtable.h"

#define DOWNLOAD_FILE_PATH "./PlayBackFileSaveDir/"
#define CAPTURE_PICTURE_PATH "./PlayBackPictureDir/"

class PlayBack : public QWidget
{
    Q_OBJECT
		
public:
	
	//friend class QtClientDemo;
    PlayBack( QList<DeviceData> * tree, int * usrid, 
    	NET_DVR_DEVICEINFO_V30 * devinfo, int * channelnum, LogTable * table, QWidget * parent = 0 );
    ~PlayBack();
	//drawarea init
	void DrawAreaInit();
	//insert find files results
	void insertrow( NET_DVR_FINDDATA_V30 & dvrfile );
	//control play
    void playbackcontrol( int command );

private slots:
	//table page change and common slots func
	void tabPageChanged( int index );
	void updateTime();
	//page 0
	void prepareDownload( int row,int column );
	void preparePlay( int row, int column );
    void on_dateTimeEdit_1_start_dateTimeChanged ( const QDateTime & datetime );
    void on_dateTimeEdit_1_stop_dateTimeChanged ( const QDateTime & datetime );  
	void on_pushButton_1_search_clicked();
    void on_pushButton_1_lock_clicked();
    void on_pushButton_1_download_clicked(); 
	void on_pushButton_1_downsave_clicked();
    void on_pushButton_1_downstopsave_clicked();
   	void on_comboBox_1_filetype_currentIndexChanged ( int index );
    void on_comboBox_1_fileattr_currentIndexChanged ( int index );
	void on_pushButton_playback_start_1_clicked();
	void on_pushButton_playback_stop_1_clicked();
	void on_pushButton_playback_fast_1_clicked();
	void on_pushButton_playback_slow_1_clicked();
	void on_pushButton_playback_picture_1_clicked();
	void on_pushButton_playback_pos_1_clicked();
	void on_pushButton_playback_frame_1_clicked();
	void on_pushButton_playback_volume_1_clicked();	
	void on_horizontalSlider_1_playbackprocess_sliderReleased();
	void on_horizontalSlider_playback_volume_1_sliderReleased();
	//page 1
	void localtablebeenclicked( int row, int column );
	void updatelocalprocess();
    void on_dateTimeEdit_2_start_dateTimeChanged ( const QDateTime & datetime );
    void on_dateTimeEdit_2_stop_dateTimeChanged ( const QDateTime & datetime ); 
    void on_pushButton_2_search_clicked();
    void on_pushButton_2_del_clicked();
    void on_pushButton_2_delall_clicked();
	void on_checkBox_2_all_stateChanged( int state );
	void on_pushButton_playback_start_2_clicked();
	void on_pushButton_playback_stop_2_clicked();
	void on_pushButton_playback_fast_2_clicked();
	void on_pushButton_playback_slow_2_clicked();
	void on_pushButton_playback_picture_2_clicked();
	void on_pushButton_playback_frame_2_clicked();
	void on_pushButton_playback_pos_2_clicked();
	void on_pushButton_playback_volume_2_clicked();
	void on_comboBox_2_filetype_currentIndexChanged ( int index );
	void on_horizontalSlider_playback_process_2_sliderReleased(  );
	void on_horizontalSlider_playback_volume_2_sliderReleased( );	
	//page 2 
    void on_dateTimeEdit_3_start_dateTimeChanged ( const QDateTime & datetime );
    void on_dateTimeEdit_3_stop_dateTimeChanged ( const QDateTime & datetime );
    void on_pushButton_3_download_clicked();
    void on_pushButton_3_save_clicked();
	void on_pushButton_playback_start_3_clicked();
	void on_pushButton_playback_stop_3_clicked();
	void on_pushButton_playback_fast_3_clicked();
	void on_pushButton_playback_slow_3_clicked();
	void on_pushButton_playback_picture_3_clicked();
	void on_pushButton_playback_pos_3_clicked();
	void on_pushButton_playback_frame_3_clicked();	
	void on_pushButton_playback_volume_3_clicked();
	void on_horizontalSlider_playback_volume_3_sliderReleased();
    void OnSingleClickPlayWindow();

    void on_changeWndBtn_clicked();

private:
    Ui::PlayBackClass ui;
	//log table 
	LogTable * m_pblogtable;
	//device list tree 
	QList<DeviceData>* m_qlistdevicetree;
    //current user id
    int* m_pbUserID;    
    //current device info struct
    NET_DVR_DEVICEINFO_V30* m_pbCurrDevInfo;
    //current valid channel
    int* m_pbCurrChannel;  
	//timer for play-process 
	QTimer *m_pbqtimer;
	//qtablewidget page index
	int m_pbpageindex;
	//page index backup 
	int m_pbcurrentpageindex;
	//for play

#ifdef __linux__
    INITINFO m_pbinitinfo; 
#endif
 
	//play area
    QFrame m_pbframe;
    //file type for search filelist
    unsigned int m_pbfiletype;
	//file attr for search filelist
    unsigned int m_pbfileattr;
	//for file search
    LPNET_DVR_FILECOND m_pbpDvrFile;
    NET_DVR_FINDDATA_V30 m_pbFileData;
    //remote file playback datetime
    NET_DVR_TIME m_pbremotestarttime;
    NET_DVR_TIME m_pbremotestoptime;
    //local file playback datetime
    QString m_pblocalstarttime;
    QString m_pblocalstoptime; 
    //play by time datetime info 
    NET_DVR_TIME m_pbplaystarttime;
    NET_DVR_TIME m_pbplaystoptime;
	//lock/unlock file
    int m_pblockflag;
	//play handle
    int m_pbhandle;
	//for play pause stop controling play
    int m_pbcontrollabel_1;
	int m_pbcontrollabel_3;
	//tmp variants in 0,2 page
    QString m_pbdstarttime;
    QString m_pbdstoptime;
	//audio open/close flag
	int m_pbvolumeflag;
	//downloading state 
	int m_pbdownloading;
	//saving state
	int m_pbsaving;
	//doawnload handle
	int m_pbdownloadhandle;
	//local file page 1
	int m_pblocalalltype; 
	//0 normal 1 alarm 2 download
	int m_pblocaltype;
	//for play local file
	QString m_pblocalfilepath;
	//playM4 handle
	int m_pblocalportnum;
};

#endif // PLAYBACK_H

