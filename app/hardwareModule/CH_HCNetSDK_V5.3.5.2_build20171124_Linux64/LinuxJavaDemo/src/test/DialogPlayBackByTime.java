package test;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;

import com.sun.javafx.scene.paint.GradientUtils.Point;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.examples.win32.W32API.HWND;
import com.sun.jna.ptr.IntByReference;

import test.HCNetSDK.NET_DVR_PLAYCOND;
import test.HCNetSDK.NET_DVR_STREAM_INFO;
import test.HCNetSDK.NET_DVR_VOD_PARA;

import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import java.awt.Panel;
import javax.swing.JToolBar;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import java.awt.Color;
import javax.swing.JSlider;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JRadioButton;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.UIManager;
import java.awt.Font;

@SuppressWarnings("serial")
public class DialogPlayBackByTime extends JDialog
{

	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	NativeLong m_lUserID;// logID
	NativeLong m_lPlayHandle;// playhandle
	NativeLong m_lLoadHandle;// downloadhandle
	String m_sDeviceIP;// deviceip
	int m_iChanShowNum;// playback channel
	boolean m_bSound;
	boolean m_bPause;
	boolean m_bTimeSave;

	Timer Downloadtimer;
	Timer Playbacktimer;
	    
	private JTextField textFieldsYear;
	private JTextField textFieldsMonth;
	private JTextField textFieldsDay;
	private JTextField textFieldsHour;
	private JTextField textFieldsMinute;
	private JTextField textFieldsSecond;
	private JTextField textFieldeYear;
	private JTextField textFieldeMonth;
	private JTextField textFieldeDay;
	private JTextField textFieldeHour;
	private JTextField textFieldeMinute;
	private JTextField textFieldeSecond;
	private JTextField textFieldChannelNum;
	private Panel panelPlay;
	private JButton buttonSave;
	private JButton buttonExit;
	private JSlider sliderAudio;
	private JButton btnNewButtonAudio;
	private JButton btnNewButtonPlay;
	private JButton btnNewButtonDownload;
	private JRadioButton RadioForward;
	private JRadioButton RadioReverse ;
	
	public DialogPlayBackByTime(java.awt.Frame parent, boolean modal, NativeLong lUserID, String sIP) {
        super(parent, modal);
        setFont(new Font("宋体", Font.PLAIN, 12));
        initComponents();
        initialDialog();
        m_lUserID = lUserID;
        m_lPlayHandle = new NativeLong(-1);
        m_lLoadHandle = new NativeLong(-1);
        m_iChanShowNum = 0;
        m_sDeviceIP = sIP;
        setModal(true);
    }
	
	private void initComponents() {
		setTitle("Playback By Time");
		getContentPane().setLayout(null);
		
		JPanel panel = new JPanel();
		panel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Video Time", TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel.setBounds(10, 10, 663, 113);
		getContentPane().add(panel);
		panel.setLayout(null);
		
		JLabel lblNewLabel = new JLabel("Start Time");
		lblNewLabel.setBounds(10, 24, 66, 15);
		panel.add(lblNewLabel);
		
		textFieldsYear = new JTextField();
		textFieldsYear.setBounds(76, 21, 45, 21);
		panel.add(textFieldsYear);
		textFieldsYear.setColumns(10);
		
		JLabel lblNewLabel_1 = new JLabel("Y");
		lblNewLabel_1.setBounds(128, 24, 24, 15);
		panel.add(lblNewLabel_1);
		
		textFieldsMonth = new JTextField();
		textFieldsMonth.setColumns(10);
		textFieldsMonth.setBounds(148, 21, 45, 21);
		panel.add(textFieldsMonth);
		
		JLabel lblMonth = new JLabel("M");
		lblMonth.setBounds(200, 24, 24, 15);
		panel.add(lblMonth);
		
		textFieldsDay = new JTextField();
		textFieldsDay.setColumns(10);
		textFieldsDay.setBounds(222, 21, 45, 21);
		panel.add(textFieldsDay);
		
		JLabel lblD = new JLabel("D");
		lblD.setBounds(271, 24, 24, 15);
		panel.add(lblD);
		
		textFieldsHour = new JTextField();
		textFieldsHour.setColumns(10);
		textFieldsHour.setBounds(294, 21, 45, 21);
		panel.add(textFieldsHour);
		
		JLabel lblH = new JLabel("H");
		lblH.setBounds(342, 24, 24, 15);
		panel.add(lblH);
		
		textFieldsMinute = new JTextField();
		textFieldsMinute.setColumns(10);
		textFieldsMinute.setBounds(366, 21, 45, 21);
		panel.add(textFieldsMinute);
		
		JLabel lblM = new JLabel("M");
		lblM.setBounds(418, 24, 24, 15);
		panel.add(lblM);
		
		textFieldsSecond = new JTextField();
		textFieldsSecond.setColumns(10);
		textFieldsSecond.setBounds(440, 21, 45, 21);
		panel.add(textFieldsSecond);
		
		JLabel lblS = new JLabel("S");
		lblS.setBounds(492, 24, 24, 15);
		panel.add(lblS);
		
		JLabel lblStopTime = new JLabel("Stop Time");
		lblStopTime.setBounds(10, 66, 54, 15);
		panel.add(lblStopTime);
		
		textFieldeYear = new JTextField();
		textFieldeYear.setColumns(10);
		textFieldeYear.setBounds(76, 63, 45, 21);
		panel.add(textFieldeYear);
		
		JLabel lblY = new JLabel("Y");
		lblY.setBounds(128, 66, 24, 15);
		panel.add(lblY);
		
		textFieldeMonth = new JTextField();
		textFieldeMonth.setColumns(10);
		textFieldeMonth.setBounds(148, 63, 45, 21);
		panel.add(textFieldeMonth);
		
		JLabel lblM_1 = new JLabel("M");
		lblM_1.setBounds(200, 66, 24, 15);
		panel.add(lblM_1);
		
		textFieldeDay = new JTextField();
		textFieldeDay.setColumns(10);
		textFieldeDay.setBounds(222, 63, 45, 21);
		panel.add(textFieldeDay);
		
		JLabel lblD_1 = new JLabel("D");
		lblD_1.setBounds(271, 66, 24, 15);
		panel.add(lblD_1);
		
		textFieldeHour = new JTextField();
		textFieldeHour.setColumns(10);
		textFieldeHour.setBounds(294, 63, 45, 21);
		panel.add(textFieldeHour);
		
		JLabel lblH_1 = new JLabel("H");
		lblH_1.setBounds(342, 66, 24, 15);
		panel.add(lblH_1);
		
		textFieldeMinute = new JTextField();
		textFieldeMinute.setColumns(10);
		textFieldeMinute.setBounds(366, 63, 45, 21);
		panel.add(textFieldeMinute);
		
		JLabel lblM_2 = new JLabel("M");
		lblM_2.setBounds(418, 66, 24, 15);
		panel.add(lblM_2);
		
		textFieldeSecond = new JTextField();
		textFieldeSecond.setColumns(10);
		textFieldeSecond.setBounds(440, 63, 45, 21);
		panel.add(textFieldeSecond);
		
		JLabel lblS_1 = new JLabel("S");
		lblS_1.setBounds(492, 66, 24, 15);
		panel.add(lblS_1);
		
		JLabel lblNewLabel_2 = new JLabel("Chan Num");
		lblNewLabel_2.setBounds(514, 24, 66, 15);
		panel.add(lblNewLabel_2);
		
		textFieldChannelNum = new JTextField();
		textFieldChannelNum.setText("1");
		textFieldChannelNum.setBounds(577, 21, 66, 21);
		panel.add(textFieldChannelNum);
		textFieldChannelNum.setColumns(10);
		
		JPanel panel_1 = new JPanel();
		panel_1.setBorder(new TitledBorder(null, "", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		panel_1.setBounds(10, 123, 663, 375);
		getContentPane().add(panel_1);
		panel_1.setLayout(null);
		
		panelPlay = new Panel();
		panelPlay.setBackground(new Color(204, 255, 255));
		panelPlay.setBounds(10, 5, 643, 303);
		panel_1.add(panelPlay);
		
		JToolBar toolBar = new JToolBar();
		toolBar.setBounds(117, 325, 401, 38);
		panel_1.add(toolBar);
		
		JLabel lblNewLabel_8 = new JLabel("  ");
		toolBar.add(lblNewLabel_8);
		
		btnNewButtonPlay = new JButton("Play");
		btnNewButtonPlay.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				PlayActionPerformed(e);
			}
		});
		btnNewButtonPlay.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonPlay);
		
		JLabel lblNewLabel_3 = new JLabel("      ");
		toolBar.add(lblNewLabel_3);
		
		JButton btnNewButtonStop = new JButton("Stop");
		btnNewButtonStop.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				StopActionPerformed(e);
			}
		});
		btnNewButtonStop.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonStop);
		
		JLabel lblNewLabel_4 = new JLabel("      ");
		toolBar.add(lblNewLabel_4);
		
		JButton btnNewButtonFast = new JButton("Quick");
		btnNewButtonFast.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				FastActionPerformed(e);
			}
		});
		btnNewButtonFast.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonFast);
		
		JLabel lblNewLabel_5 = new JLabel("      ");
		toolBar.add(lblNewLabel_5);
		
		JButton btnNewButtonSlow = new JButton("Slow");
		btnNewButtonSlow.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SlowActionPerformed(e);
			}
		});
		btnNewButtonSlow.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonSlow);
		
		JLabel lblNewLabel_6 = new JLabel("      ");
		toolBar.add(lblNewLabel_6);
		
		JButton btnNewButtonCapture = new JButton("JPEG");
		btnNewButtonCapture.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CaptureActionPerformed(e);
			}
		});
		btnNewButtonCapture.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonCapture);
		
		JLabel lblNewLabel_7 = new JLabel("      ");
		toolBar.add(lblNewLabel_7);
		
		btnNewButtonAudio = new JButton("Volum");
		btnNewButtonAudio.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				AudioActionPerformed(e);
			}
		});
		btnNewButtonAudio.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonAudio);
		
		sliderAudio = new JSlider();
		sliderAudio.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseReleased(MouseEvent arg0) {
				SliderAudioMouseReleased(arg0);
			}
		});
		sliderAudio.setBounds(528, 333, 123, 15);
		panel_1.add(sliderAudio);
		
		 RadioForward = new JRadioButton("Forward");
		 RadioForward.setSelected(true);
		RadioForward.setBounds(10, 325, 88, 23);
		panel_1.add(RadioForward);

		 RadioReverse = new JRadioButton("Reverse");
		RadioReverse.setBounds(10, 350, 94, 23);
		panel_1.add(RadioReverse);
		
		ButtonGroup buttonGroup = new ButtonGroup();
		buttonGroup.add(RadioForward);
		buttonGroup.add(RadioReverse);
		

		
		btnNewButtonDownload = new JButton("Download");
		btnNewButtonDownload.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				DownloadActionPerformed(e);
			}
		});
		btnNewButtonDownload.setBounds(63, 520, 91, 23);
		getContentPane().add(btnNewButtonDownload);
		
		buttonSave = new JButton("Save");
		buttonSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SaveActionPerformed(e);
			}
		});
		buttonSave.setBounds(277, 520, 77, 23);
		getContentPane().add(buttonSave);
		
		buttonExit = new JButton("Exit");
		buttonExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				ExitActionPerformed(e);
			}
		});
		buttonExit.setBounds(501, 520, 77, 23);
		getContentPane().add(buttonExit);
	}
	

     private void initialDialog()
     {
         Date today = new Date();
         Calendar now = Calendar.getInstance();
         now.setTime(today); 

         //start time
         textFieldsYear.setText(now.get(Calendar.YEAR) + "");
         textFieldsMonth.setText((now.get(Calendar.MONTH) + 1) + "");
         textFieldsDay.setText(1 + "");
         textFieldsHour.setText("0");
         textFieldsMinute.setText("0");
         textFieldsSecond.setText("0");

         //endtime
         textFieldeYear.setText(now.get(Calendar.YEAR) + "");
         textFieldeMonth.setText((now.get(Calendar.MONTH) + 1) + "");
         textFieldeDay.setText(now.get(Calendar.DATE) + "");
         textFieldeHour.setText("23");
         textFieldeMinute.setText("59");
         textFieldeSecond.setText("59");

         this.addWindowListener(new WindowAdapter()
         {
            @Override
             public void windowClosing(WindowEvent e)
             {
                 StopPlay();
             }
         });
     }
     

     //stop play
	private void StopPlay()
	{
		if (m_lPlayHandle.intValue() >= 0)
		{
			if (m_bTimeSave)
			{
				hCNetSDK.NET_DVR_StopPlayBackSave(m_lPlayHandle);
				buttonSave.setText("Save");
				m_bTimeSave = false;
			}

			hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYSTOPAUDIO, 0, null);
			hCNetSDK.NET_DVR_StopPlayBack(m_lPlayHandle);
			m_lPlayHandle.setValue(-1);

			Playbacktimer.cancel();
			panelPlay.repaint();
		}
	}

	//play
	private void PlayActionPerformed(ActionEvent event)
	{
		HCNetSDK.NET_DVR_TIME struStartTime;
		HCNetSDK.NET_DVR_TIME struStopTime;

		if (m_lPlayHandle.intValue() == -1)
		{
			m_iChanShowNum = Integer.parseInt(textFieldChannelNum.getText());

			struStartTime = new HCNetSDK.NET_DVR_TIME();
			struStopTime = new HCNetSDK.NET_DVR_TIME();

			struStartTime.dwYear = Integer.parseInt(textFieldsYear.getText());// 开始时间
			struStartTime.dwMonth = Integer.parseInt(textFieldsMonth.getText());
			struStartTime.dwDay = Integer.parseInt(textFieldsDay.getText());
			struStartTime.dwHour = Integer.parseInt(textFieldsHour.getText());
			struStartTime.dwMinute = Integer.parseInt(textFieldsMinute.getText());
			struStartTime.dwSecond = Integer.parseInt(textFieldsSecond.getText());
			struStopTime.dwYear = Integer.parseInt(textFieldeYear.getText());// 结束时间
			struStopTime.dwMonth = Integer.parseInt(textFieldeMonth.getText());
			struStopTime.dwDay = Integer.parseInt(textFieldeDay.getText());
			struStopTime.dwHour = Integer.parseInt(textFieldeHour.getText());
			struStopTime.dwMinute = Integer.parseInt(textFieldeMinute.getText());
			struStopTime.dwSecond = Integer.parseInt(textFieldeSecond.getText());

			HWND hwnd = new HWND(Native.getComponentPointer(panelPlay));
			NET_DVR_VOD_PARA  struVoidParam = new NET_DVR_VOD_PARA();
			struVoidParam.dwSize = struVoidParam.size();
			struVoidParam.struBeginTime = struStartTime;
			struVoidParam.struEndTime = struStopTime;
			struVoidParam.hWnd = hwnd;
			NET_DVR_STREAM_INFO struInfo = new NET_DVR_STREAM_INFO ();
			struInfo.dwSize = struInfo.size();
			struInfo.dwChannel = m_iChanShowNum;
			struVoidParam.struIDInfo = struInfo;
			if(RadioForward.isSelected())
			{
				m_lPlayHandle = hCNetSDK.NET_DVR_PlayBackByTime_V40(m_lUserID, struVoidParam);
			}
			else
			{
				NET_DVR_PLAYCOND struPlayCond = new NET_DVR_PLAYCOND();
				struPlayCond.dwChannel = m_iChanShowNum;
				struPlayCond.struStartTime = struStartTime;
				struPlayCond.struStopTime = struStopTime;
				m_lPlayHandle = hCNetSDK.NET_DVR_PlayBackReverseByTime_V40(m_lUserID,hwnd,struPlayCond);
			}

			if (m_lPlayHandle.intValue() == -1)
			{
				DialogMessage dlg = new DialogMessage("Plackback by time fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
				dlg.setVisible(true);
				return;
			}
			else
			{
				hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYSTART, 0, null);
			}

			if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYSTARTAUDIO, 0, null))
			{
				m_bSound = true;
				hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYAUDIOVOLUME, (0xffff) / 2, null);
				sliderAudio.setValue(50);
				btnNewButtonAudio.setText("Volum");
			}
			else
			{
				m_bSound = false;
				btnNewButtonAudio.setText("no volum");
			}

	
			Playbacktimer = new Timer();
			Playbacktimer.schedule(new PlaybackTask(), 0, 5000);
		}
		else
		{
			if (m_bPause)
			{
				if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYRESTART, 0, null))
				{
					btnNewButtonPlay.setText("Pause");
					m_bPause = false;
				}
			}
			else
			{
				if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYPAUSE, 0, null))
				{
					m_bPause = true;
					btnNewButtonPlay.setText("Start");
				}
			}
		}
	}
	
	//StopPlay
	private void StopActionPerformed(ActionEvent event)
	{
		StopPlay();
	}
	
	//quick
	@SuppressWarnings("static-access")
	private void FastActionPerformed(ActionEvent event)
	{
		hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, hCNetSDK.NET_DVR_PLAYFAST, 0, null);
	}
	
	//slow
	@SuppressWarnings("static-access")
	private void SlowActionPerformed(ActionEvent event)
	{
		hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, hCNetSDK.NET_DVR_PLAYSLOW, 0, null);
	}
	
	//jpeg
	private void CaptureActionPerformed(ActionEvent event)
	{
		if (m_lPlayHandle.intValue() == -1)
		{
			return;
		}
		String sPicName = "C:/Picture/" + m_iChanShowNum + System.currentTimeMillis() + ".bmp";
		if (hCNetSDK.NET_DVR_PlayBackCaptureFile(m_lPlayHandle, sPicName))
		{
			return;
		}
		else
		{
			DialogMessage dlg = new DialogMessage("JPEG fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
		}
	}
	
	//audio
	private void AudioActionPerformed(ActionEvent event)
	{
		if (m_lPlayHandle.intValue() >= 0)
		{
			if (m_bSound)
			{
				if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYSTOPAUDIO, 0, null))
				{
					m_bSound = false;
					btnNewButtonAudio.setText("no volum");
				}
				else
				{
					System.out.println("Stop audio failed!");
				}
			}
			else
			{
				if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYSTARTAUDIO, 0, null))
				{
					btnNewButtonAudio.setText("Volum");
					m_bSound = true;
				}
				else
				{
					System.out.println("Start audio failed!");
				}
			}
		}
	}
	
	//SliderAudio
	private void SliderAudioMouseReleased(MouseEvent event)
	{
		if (m_lPlayHandle.intValue() >= 0)
		{
			if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYAUDIOVOLUME,
					sliderAudio.getValue(), null))
			{
				System.out.println(" Set volume Succeed!");
			}
			else
			{
				System.out.println(" Set volume Failed!");
			}
		}
	}
	
	//download
	private void DownloadActionPerformed(ActionEvent event)
	{
		 if (m_lLoadHandle.intValue() == -1)
	        {
	            HCNetSDK.NET_DVR_TIME struStartTime;
	            HCNetSDK.NET_DVR_TIME struStopTime;

	            struStartTime = new HCNetSDK.NET_DVR_TIME();
	            struStopTime = new HCNetSDK.NET_DVR_TIME();
	            struStartTime.dwYear = Integer.parseInt(textFieldsYear.getText());
	            struStartTime.dwMonth = Integer.parseInt(textFieldsMonth.getText());
	            struStartTime.dwDay = Integer.parseInt(textFieldsDay.getText());
	            struStartTime.dwHour = Integer.parseInt(textFieldsHour.getText());
	            struStartTime.dwMinute = Integer.parseInt(textFieldsMinute.getText());
	            struStartTime.dwSecond = Integer.parseInt(textFieldsSecond.getText());
	            struStopTime.dwYear = Integer.parseInt(textFieldeYear.getText());
	            struStopTime.dwMonth = Integer.parseInt(textFieldeMonth.getText());
	            struStopTime.dwDay = Integer.parseInt(textFieldeDay.getText());
	            struStopTime.dwHour = Integer.parseInt(textFieldeHour.getText());
	            struStopTime.dwMinute = Integer.parseInt(textFieldeMinute.getText());
	            struStopTime.dwSecond = Integer.parseInt(textFieldeSecond.getText());

	            m_iChanShowNum = Integer.parseInt(textFieldChannelNum.getText());

	            String sFileName = "c:/DownLoad/" + m_sDeviceIP + m_iChanShowNum + struStartTime.toStringTitle() + struStopTime.toStringTitle() + ".mp4";
	            System.out.println(sFileName);
	           
	            NET_DVR_PLAYCOND struDownloadCond = new NET_DVR_PLAYCOND();
	            struDownloadCond.dwChannel = m_iChanShowNum;
	            struDownloadCond.struStartTime = struStartTime;
	            struDownloadCond.struStopTime = struStopTime;
	            m_lLoadHandle = hCNetSDK.NET_DVR_GetFileByTime_V40(m_lUserID,sFileName,struDownloadCond);
	            if (m_lLoadHandle.intValue() >= 0)
	            {
	                hCNetSDK.NET_DVR_PlayBackControl(m_lLoadHandle, HCNetSDK.NET_DVR_PLAYSTART, 0, null);
	                btnNewButtonDownload.setText("Stop Download");
	                Downloadtimer = new Timer();
	                Downloadtimer.schedule(new DownloadTask(), 0, 5000);
	            } else
	            {
	            	DialogMessage dlg = new DialogMessage("Download byTime fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
					dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
					dlg.setVisible(true);
	                System.out.println("laste error " + hCNetSDK.NET_DVR_GetLastError());
	                return;
	            }
	        } 
	        else
	        {
	            hCNetSDK.NET_DVR_StopGetFile(m_lLoadHandle);
	            btnNewButtonDownload.setText("Download");
	            Downloadtimer.cancel();
	        }
	}
	
	//save
	private void SaveActionPerformed(ActionEvent event)
	{
		if (m_lPlayHandle.intValue() == -1)
        {
			DialogMessage dlg = new DialogMessage("Please play file" );
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
            return;
        }
        if (!m_bTimeSave)
        {
            JFileChooser myJFileChooser = new JFileChooser();
            myJFileChooser.showSaveDialog(this);
            String sFileName = myJFileChooser.getSelectedFile() + ".mp4";
            if (hCNetSDK.NET_DVR_PlayBackSaveData(m_lPlayHandle, sFileName))
            {
                m_bTimeSave = true;
                buttonSave.setText("Stop Save");
            }
        } else
        {
            hCNetSDK.NET_DVR_StopPlayBackSave(m_lPlayHandle);
            m_bTimeSave = false;
            buttonSave.setText("Save");
        }
	}
	
	//exit
	private void ExitActionPerformed(ActionEvent event)
	{
		StopPlay();
		dispose();
	}
	

	class PlaybackTask extends java.util.TimerTask
	{
		
		@Override
		public void run()
		{
			IntByReference nPos = new IntByReference(0);
			if (m_lPlayHandle.intValue() >= 0)
			{
				if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayHandle, HCNetSDK.NET_DVR_PLAYGETPOS, 0, nPos))
				{
					System.out.println("回放进度" + nPos.getValue());
				}
				else
				{
					System.out.println("获取回放进度失败,错误码为:"+hCNetSDK.NET_DVR_GetLastError());
				}

				if (nPos.getValue() > 100)
				{
					StopPlay();
					
					DialogMessage dlg = new DialogMessage("NetWork or DVR busing,playBack exception stop");
					dlg.setBounds(0,0 , 370, 200);
					dlg.setVisible(true);
				}
				if (nPos.getValue() == 100)
				{
					StopPlay();
					
					DialogMessage dlg = new DialogMessage("PlayBack by time end");
					dlg.setBounds(0,0 , 370, 200);
					dlg.setVisible(true);
				}
			}
		}
	}
	

    class DownloadTask extends java.util.TimerTask
    {
        
        @Override
        public void run()
        {
            IntByReference nPos = new IntByReference(0);
            hCNetSDK.NET_DVR_PlayBackControl(m_lLoadHandle, HCNetSDK.NET_DVR_PLAYGETPOS, 0, nPos);
            if (nPos.getValue() > 100)
            {
                hCNetSDK.NET_DVR_StopGetFile(m_lLoadHandle);
                m_lLoadHandle.setValue(-1);
                btnNewButtonDownload.setText("Save");
                Downloadtimer.cancel();

                DialogMessage dlg = new DialogMessage("NetWork or DVR busing,download exception stop");
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
            }
            if (nPos.getValue() == 100)
            {
                hCNetSDK.NET_DVR_StopGetFile(m_lLoadHandle);
                m_lLoadHandle.setValue(-1);
                btnNewButtonDownload.setText("Download");
                Downloadtimer.cancel();

                DialogMessage dlg = new DialogMessage("Download by time end");
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
            }
        }
    }
}
