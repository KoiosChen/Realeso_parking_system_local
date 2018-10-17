package test;

import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;
import javax.swing.table.DefaultTableModel;


import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.examples.win32.W32API.HWND;
import com.sun.jna.ptr.IntByReference;


import javax.swing.UIManager;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JComboBox;
import javax.swing.JTextField;
import javax.swing.JRadioButton;
import javax.swing.ButtonGroup;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JSlider;
import javax.swing.JToolBar;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.util.Vector;
import java.util.logging.Logger;
import java.awt.event.ActionEvent;
import javax.swing.border.EtchedBorder;
import java.awt.Font;
import java.awt.Panel;

import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JProgressBar;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

@SuppressWarnings("serial")
public class DialogPlayByFile extends JDialog
{
	protected  final Logger log=Logger.getLogger(this.getClass().getName());
	
	 static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
     NativeLong m_lPlayBackHandle;//playback handle
     NativeLong m_lDownloadHandle;//download handle
     NativeLong m_lUserID;//loginID
     Timer Downloadtimer;
     Timer Playbacktimer;
    
     IntByReference m_nFileTime;
     IntByReference m_nTotalFrames;
     
     int m_nTotalSecond;
     int m_nTotalMinute;
     int m_nTotalHour;
     
     boolean m_bGetMaxTime;
     boolean m_bSaveFile;
     
	private JTextField textFieldChannelNum;
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
	private JTable table;
	private JTextField textFieldPlayTime;
	private JTextField textFieldFileName;
	private JTextField textFieldTotalBytes;
	private JProgressBar progressBar;
	private Panel panelPlayBack;
	private JSlider sliderPlayBack;
	@SuppressWarnings("rawtypes")
	private JComboBox comboBoxRecordType;
	private JButton btnNewButtonDownload;
	private JRadioButton RadioForward;
	private JRadioButton RadioReverse;
	 
	public DialogPlayByFile(java.awt.Frame parent, boolean modal, NativeLong lUserID) {
        super(parent, modal);
        initComponents();
        initialDialog();
        m_lUserID = lUserID;
        m_lPlayBackHandle = new NativeLong(-1);
        m_lDownloadHandle = new NativeLong(-1);
        m_nFileTime = new IntByReference(0);
        m_nTotalFrames = new IntByReference(0);
        setModal(true);
    }
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public void initComponents()
	{
		setTitle("PlayBack By File");
		getContentPane().setLayout(null);
		
		JPanel panel = new JPanel();
		panel.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Search Info", TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel.setBounds(10, 10, 283, 410);
		getContentPane().add(panel);
		panel.setLayout(null);
		
		JLabel lblNewLabel = new JLabel("Video Type");
		lblNewLabel.setBounds(10, 23, 68, 15);
		panel.add(lblNewLabel);
		
		JLabel lblNewLabel_2 = new JLabel("Chan Num");
		lblNewLabel_2.setBounds(10, 66, 68, 15);
		panel.add(lblNewLabel_2);
		
		comboBoxRecordType = new JComboBox();
		comboBoxRecordType.setModel(new DefaultComboBoxModel(new String[] {"All", "Timing video", "Motion detecting", "Alarm triggere", "Alarm|Motion", "Alarm&Motion", "Command trigger", "Manual video"}));
		comboBoxRecordType.setBounds(88, 20, 96, 21);
		panel.add(comboBoxRecordType);
		
		textFieldChannelNum = new JTextField();
		textFieldChannelNum.setText("1");
		textFieldChannelNum.setBounds(88, 63, 96, 21);
		panel.add(textFieldChannelNum);
		textFieldChannelNum.setColumns(10);
		
		JPanel panelTime = new JPanel();
		panelTime.setBorder(new TitledBorder(null, "", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		panelTime.setBounds(10, 200, 257, 162);
		panel.add(panelTime);
		panelTime.setLayout(null);
		
		JLabel lblNewLabel_5 = new JLabel("Start Time");
		lblNewLabel_5.setBounds(10, 24, 60, 15);
		panelTime.add(lblNewLabel_5);
		
		JLabel lblNewLabel_6 = new JLabel("End Time");
		lblNewLabel_6.setBounds(10, 89, 54, 15);
		panelTime.add(lblNewLabel_6);
		
		textFieldsYear = new JTextField();
		textFieldsYear.setBounds(77, 21, 31, 21);
		panelTime.add(textFieldsYear);
		textFieldsYear.setColumns(10);
		
		JLabel lblNewLabel_7 = new JLabel("Y");
		lblNewLabel_7.setBounds(112, 24, 18, 15);
		panelTime.add(lblNewLabel_7);
		
		textFieldsMonth = new JTextField();
		textFieldsMonth.setColumns(10);
		textFieldsMonth.setBounds(136, 21, 31, 21);
		panelTime.add(textFieldsMonth);
		
		JLabel lblM = new JLabel("M");
		lblM.setBounds(170, 24, 18, 15);
		panelTime.add(lblM);
		
		textFieldsDay = new JTextField();
		textFieldsDay.setColumns(10);
		textFieldsDay.setBounds(192, 21, 31, 21);
		panelTime.add(textFieldsDay);
		
		JLabel lblD = new JLabel("D");
		lblD.setBounds(227, 24, 18, 15);
		panelTime.add(lblD);
		
		textFieldsHour = new JTextField();
		textFieldsHour.setColumns(10);
		textFieldsHour.setBounds(77, 52, 31, 21);
		panelTime.add(textFieldsHour);
		
		JLabel lblH = new JLabel("H");
		lblH.setBounds(112, 55, 18, 15);
		panelTime.add(lblH);
		
		textFieldsMinute = new JTextField();
		textFieldsMinute.setColumns(10);
		textFieldsMinute.setBounds(136, 52, 31, 21);
		panelTime.add(textFieldsMinute);
		
		JLabel lblM_1 = new JLabel("M");
		lblM_1.setBounds(170, 55, 18, 15);
		panelTime.add(lblM_1);
		
		textFieldsSecond = new JTextField();
		textFieldsSecond.setColumns(10);
		textFieldsSecond.setBounds(192, 52, 31, 21);
		panelTime.add(textFieldsSecond);
		
		JLabel lblS = new JLabel("S");
		lblS.setBounds(227, 55, 18, 15);
		panelTime.add(lblS);
		
		textFieldeYear = new JTextField();
		textFieldeYear.setColumns(10);
		textFieldeYear.setBounds(77, 89, 31, 21);
		panelTime.add(textFieldeYear);
		
		JLabel lblY = new JLabel("Y");
		lblY.setBounds(112, 92, 18, 15);
		panelTime.add(lblY);
		
		textFieldeMonth = new JTextField();
		textFieldeMonth.setColumns(10);
		textFieldeMonth.setBounds(136, 89, 31, 21);
		panelTime.add(textFieldeMonth);
		
		JLabel lblM_2 = new JLabel("M");
		lblM_2.setBounds(170, 92, 18, 15);
		panelTime.add(lblM_2);
		
		textFieldeDay = new JTextField();
		textFieldeDay.setColumns(10);
		textFieldeDay.setBounds(192, 89, 31, 21);
		panelTime.add(textFieldeDay);
		
		JLabel lblD_1 = new JLabel("D");
		lblD_1.setBounds(227, 92, 18, 15);
		panelTime.add(lblD_1);
		
		textFieldeHour = new JTextField();
		textFieldeHour.setColumns(10);
		textFieldeHour.setBounds(77, 131, 31, 21);
		panelTime.add(textFieldeHour);
		
		JLabel lblH_1 = new JLabel("H");
		lblH_1.setBounds(112, 134, 18, 15);
		panelTime.add(lblH_1);
		
		textFieldeMinute = new JTextField();
		textFieldeMinute.setColumns(10);
		textFieldeMinute.setBounds(136, 131, 31, 21);
		panelTime.add(textFieldeMinute);
		
		JLabel lblM_3 = new JLabel("M");
		lblM_3.setBounds(170, 134, 18, 15);
		panelTime.add(lblM_3);
		
		textFieldeSecond = new JTextField();
		textFieldeSecond.setColumns(10);
		textFieldeSecond.setBounds(192, 131, 31, 21);
		panelTime.add(textFieldeSecond);
		
		JLabel lblS_1 = new JLabel("S");
		lblS_1.setBounds(227, 134, 18, 15);
		panelTime.add(lblS_1);
		
		JButton btnSearchFile = new JButton("Search");
		btnSearchFile.setFont(new Font("宋体", Font.PLAIN, 12));
		btnSearchFile.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SearchFileActionPerformed(e);
			}
		});
		btnSearchFile.setBounds(77, 377, 96, 23);
		panel.add(btnSearchFile);
		
		JPanel panel_1 = new JPanel();
		panel_1.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Play Control", TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel_1.setBounds(290, 10, 460, 410);
		getContentPane().add(panel_1);
		panel_1.setLayout(null);
		
		panelPlayBack = new Panel();
		panelPlayBack.setBackground(new Color(204, 255, 255));
		panelPlayBack.setBounds(10, 21, 440, 312);
		panel_1.add(panelPlayBack);
		
		sliderPlayBack = new JSlider();
		sliderPlayBack.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseReleased(MouseEvent arg0) {
				SliderPlayBackMouseReleased(arg0);
			}
		});
		sliderPlayBack.setValue(0);
		sliderPlayBack.setBounds(10, 343, 440, 23);
		panel_1.add(sliderPlayBack);
		
		JToolBar toolBar = new JToolBar();
		toolBar.setBounds(119, 376, 331, 24);
		panel_1.add(toolBar);
		
		JLabel lblNewLabel_14 = new JLabel("");
		toolBar.add(lblNewLabel_14);
		
		JButton btnNewButtonPlay = new JButton("Play");
		btnNewButtonPlay.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				PlayActionPerformed(e);
			}
		});
		btnNewButtonPlay.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonPlay.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonPlay.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonPlay);
		
		JLabel lblNewLabel_13 = new JLabel(" ");
		toolBar.add(lblNewLabel_13);
		
		JButton btnNewButtonStop = new JButton("Stop");
		btnNewButtonStop.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				StopActionPerformed(e);
			}
		});
		btnNewButtonStop.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonStop.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonStop.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonStop);
		
		JLabel lblNewLabel_8 = new JLabel("      ");
		toolBar.add(lblNewLabel_8);
		
		JButton btnNewButtonStart = new JButton("Start");
		btnNewButtonStart.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonStart.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonStart.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				RestartActionPerformed(arg0);
			}
		});
		btnNewButtonStart.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonStart);
		
		JLabel lblNewLabel_9 = new JLabel(" ");
		toolBar.add(lblNewLabel_9);
		
		JButton btnNewButtonPause = new JButton("Pause");
		btnNewButtonPause.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				PauseActionPerformed(e);
			}
		});
		btnNewButtonPause.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonPause.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonPause.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonPause);
		
		JLabel lblNewLabel_10 = new JLabel("      ");
		toolBar.add(lblNewLabel_10);
		
		JButton btnNewButtonFast = new JButton("Quick");
		btnNewButtonFast.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				FastActionPerformed(e);
			}
		});
		btnNewButtonFast.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonFast.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonFast.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonFast);
		
		JLabel lblNewLabel_12 = new JLabel(" ");
		toolBar.add(lblNewLabel_12);
		
		JButton btnNewButtonSlow = new JButton("Slow");
		btnNewButtonSlow.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SlowActionPerformed(e);
			}
		});
		btnNewButtonSlow.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonSlow.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonSlow.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonSlow);
		
		JLabel lblNewLabel_11 = new JLabel("      ");
		toolBar.add(lblNewLabel_11);
		
		JButton btnNewButtonCapture = new JButton("JPEG");
		btnNewButtonCapture.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				CaptureActionPerformed(e);
			}
		});
		btnNewButtonCapture.setFont(new Font("宋体", Font.PLAIN, 13));
		btnNewButtonCapture.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		btnNewButtonCapture.setBackground(new Color(204, 255, 255));
		toolBar.add(btnNewButtonCapture);
		
		 RadioForward = new JRadioButton("Forward");
		 RadioForward.setSelected(true);
		RadioForward.setBounds(20, 361, 93, 23);
		panel_1.add(RadioForward);
		
		 RadioReverse = new JRadioButton("Reverse");
		RadioReverse.setBounds(20, 386, 67, 23);
		panel_1.add(RadioReverse);
		
		ButtonGroup buttonGroup = new ButtonGroup();
		buttonGroup.add(RadioForward);
		buttonGroup.add(RadioReverse);
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 420, 544, 146);
		getContentPane().add(scrollPane);
		
		table = new JTable();
		table.setModel(this.initialTableModel());
        scrollPane.setViewportView(table);
		//scrollPane.setColumnHeaderView(table);
		
		JPanel panel_3 = new JPanel();
		panel_3.setBorder(new TitledBorder(null, "", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		panel_3.setBounds(557, 420, 193, 146);
		getContentPane().add(panel_3);
		panel_3.setLayout(null);
		
		textFieldPlayTime = new JTextField();
		textFieldPlayTime.setBounds(68, 7, 115, 21);
		panel_3.add(textFieldPlayTime);
		textFieldPlayTime.setColumns(10);
		
		JLabel lblNewLabel_15 = new JLabel("Time");
		lblNewLabel_15.setBounds(10, 10, 37, 15);
		panel_3.add(lblNewLabel_15);
		
		JLabel lblNewLabel_16 = new JLabel("Name");
		lblNewLabel_16.setBounds(10, 46, 48, 15);
		panel_3.add(lblNewLabel_16);
		
		textFieldFileName = new JTextField();
		textFieldFileName.setBounds(68, 43, 115, 21);
		panel_3.add(textFieldFileName);
		textFieldFileName.setColumns(10);
		
		JLabel lblNewLabel_17 = new JLabel("Byte");
		lblNewLabel_17.setBounds(10, 81, 54, 15);
		panel_3.add(lblNewLabel_17);
		
		textFieldTotalBytes = new JTextField();
		textFieldTotalBytes.setBounds(68, 78, 115, 21);
		panel_3.add(textFieldTotalBytes);
		textFieldTotalBytes.setColumns(10);
		
		JButton btnNewButtonSave = new JButton("Save");
		btnNewButtonSave.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				SaveActionPerformed(e);
			}
		});
		btnNewButtonSave.setBounds(20, 113, 64, 23);
		panel_3.add(btnNewButtonSave);
		
		JButton btnNewButtonStop2 = new JButton("Stop");
		btnNewButtonStop2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				Stop2ActionPerformed(e);
			}
		});
		btnNewButtonStop2.setBounds(114, 113, 69, 23);
		panel_3.add(btnNewButtonStop2);
		
		btnNewButtonDownload = new JButton("Download");
		btnNewButtonDownload.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				DownloadActionPerformed(e);
			}
		});
		btnNewButtonDownload.setBounds(32, 577, 89, 23);
		getContentPane().add(btnNewButtonDownload);
		
		JButton btnNewButtonExit = new JButton("Exit");
		btnNewButtonExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				ExitActionPerformed(e);
			}
		});
		btnNewButtonExit.setBounds(579, 576, 81, 23);
		getContentPane().add(btnNewButtonExit);
		
		progressBar = new JProgressBar();
		progressBar.setBounds(151, 580, 403, 20);
		getContentPane().add(progressBar);
	}
	
	  /*************************************************
    函数:      initialTableModel
    函数描述:  初始化文件列表
     *************************************************/
    private DefaultTableModel initialTableModel()
    {
        String tabeTile[];
        tabeTile = new String[]{"FileName","Size","StartTime","EndTime"};
        DefaultTableModel fileTableModel = new DefaultTableModel(tabeTile, 10);
        return fileTableModel;
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

         //end time
         textFieldeYear.setText(now.get(Calendar.YEAR) + "");
         textFieldeMonth.setText((now.get(Calendar.MONTH) + 1) + "");
         textFieldeDay.setText(now.get(Calendar.DATE) + "");
         textFieldeHour.setText("23");
         textFieldeMinute.setText("59");
         textFieldeSecond.setText("59");

         progressBar.setVisible(false);

           this.addWindowListener(new WindowAdapter()
         {
             public void windowClosing(WindowEvent e)
             {
                 StopPlay();
             }
         });
     }
     

	private void StopPlay()
	{
		if (m_lPlayBackHandle.intValue() >= 0)
		{
			if (m_bSaveFile)
			{
				if (!hCNetSDK.NET_DVR_StopPlayBackSave(m_lPlayBackHandle))
				{
					
					DialogMessage dlg = new DialogMessage("stop save file,error code:"+hCNetSDK.NET_DVR_GetLastError());
					dlg.setBounds(0,0 , 370, 200);
					dlg.setVisible(true);
					return;
				} else
				{
					m_bSaveFile = false;
				}

			}
			if (!hCNetSDK.NET_DVR_StopPlayBack(m_lPlayBackHandle))
			{
				
				DialogMessage dlg = new DialogMessage("NET_DVR_StopPlayBack failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
				return;
			} else
			{
				panelPlayBack.repaint();
				m_lPlayBackHandle.setValue(-1);
				sliderPlayBack.setValue(-1);
				Playbacktimer.cancel();
				textFieldFileName.setText("");
				textFieldTotalBytes.setText("");
				textFieldPlayTime.setText("");
				sliderPlayBack.setValue(0);
			}
		}
	}
	
	//search file
	@SuppressWarnings("unchecked")
	private void SearchFileActionPerformed(ActionEvent event)
	{
		((DefaultTableModel)table.getModel()).getDataVector().removeAllElements();
		((DefaultTableModel)table.getModel()).fireTableStructureChanged(); 
		
		HCNetSDK.NET_DVR_FILECOND_V40 strFilecond=new HCNetSDK.NET_DVR_FILECOND_V40();
		strFilecond.struStartTime=new HCNetSDK.NET_DVR_TIME();
		strFilecond.struStopTime=new HCNetSDK.NET_DVR_TIME();
		
		//start time
		strFilecond.struStartTime.dwYear=Integer.parseInt(textFieldsYear.getText());
		strFilecond.struStartTime.dwMonth=Integer.parseInt(textFieldsMonth.getText());
		strFilecond.struStartTime.dwDay=Integer.parseInt(textFieldsDay.getText());
		strFilecond.struStartTime.dwHour=Integer.parseInt(textFieldsHour.getText());
		strFilecond.struStartTime.dwMinute=Integer.parseInt(textFieldsMinute.getText());
		strFilecond.struStartTime.dwSecond=Integer.parseInt(textFieldsSecond.getText());
		
		//end time
		strFilecond.struStopTime.dwYear=Integer.parseInt(textFieldeYear.getText());
		strFilecond.struStopTime.dwMonth=Integer.parseInt(textFieldeMonth.getText());
		strFilecond.struStopTime.dwDay=Integer.parseInt(textFieldeDay.getText());
		strFilecond.struStopTime.dwHour=Integer.parseInt(textFieldeHour.getText());
		strFilecond.struStopTime.dwMinute=Integer.parseInt(textFieldeMinute.getText());
		strFilecond.struStopTime.dwSecond=Integer.parseInt(textFieldeSecond.getText());
		
		strFilecond.lChannel=new NativeLong(Integer.parseInt(textFieldChannelNum.getText()));
		int iIndex = comboBoxRecordType.getSelectedIndex();
		if(iIndex == 0)
		{
			strFilecond.dwFileType = 0xff;
		}
		else
		{
			strFilecond.dwFileType= iIndex -1;
		}
		strFilecond.dwIsLocked=0xff;
		/*strFilecond.dwUseCardNo=RadioButtonCard.isSelected()?1:0; //是否使用卡号
		if(strFilecond.dwUseCardNo == 1)
		{
			strFilecond.sCardNumber=textFieldCardNum.getText().getBytes();			
		}*/
		NativeLong lFindFile=hCNetSDK.NET_DVR_FindFile_V40(m_lUserID, strFilecond);
		HCNetSDK.NET_DVR_FINDDATA_V40 strFile=new HCNetSDK.NET_DVR_FINDDATA_V40();
		NativeLong lNext;
		while(true)
		{
			lNext=hCNetSDK.NET_DVR_FindNextFile_V40(lFindFile, strFile);
			if(lNext.longValue() == HCNetSDK.NET_DVR_FILE_SUCCESS)
			{
				DefaultTableModel FileTableModel=((DefaultTableModel)table.getModel());
				Vector<String> newRow=new Vector<>();
				
				
				String [] s=new String[2];
				s=new String(strFile.sFileName).split("\0",2);
				newRow.add(new String(s[0]));
				
				int iTemp;
				String str;
				if(strFile.dwFileSize < 1024*1024)
				{
					iTemp=(strFile.dwFileSize)/1024;
					str=iTemp+"K";
				}
				else
				{
					iTemp=(strFile.dwFileSize)/(1024*1024);
					str=iTemp+"M ";
					iTemp=((strFile.dwFileSize)%(1024*1024))/1024;
					str+=iTemp+"K";
				}
				newRow.add(str);
				newRow.add(strFile.struStartTime.toStringTime());
				newRow.add(strFile.struStopTime.toStringTime());
				FileTableModel.getDataVector().add(newRow);
				((DefaultTableModel)table.getModel()).fireTableStructureChanged();
			}
			else
			{
				if(lNext.longValue()==HCNetSDK.NET_DVR_ISFINDING)
				{
					continue;
				}
				else
				{
					if(lNext.longValue()==HCNetSDK.NET_DVR_FILE_NOFIND)
					{
						DialogMessage dlg = new DialogMessage("find no file");
						dlg.setBounds(0,0 , 370, 200);
						dlg.setVisible(true);
						return;
					}
					else
					{
						boolean flag=hCNetSDK.NET_DVR_FindClose_V30(lFindFile);
						if(flag==false)
						{
							DialogMessage dlg = new DialogMessage("search ending failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
							dlg.setBounds(0,0 , 370, 200);
							dlg.setVisible(true);
						}
						return;
					}
				}
			}
		}
	}
	
	//play
	private void PlayActionPerformed(ActionEvent event)
	{
		if(table.getSelectedRow() == -1)
		{
			DialogMessage dlg = new DialogMessage("Please select play file");
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		if(m_lPlayBackHandle.intValue() != -1)
		{
			hCNetSDK.NET_DVR_StopPlayBack(m_lPlayBackHandle);
			m_lPlayBackHandle.setValue(-1);
		}

		DefaultTableModel FileTableModel=((DefaultTableModel)table.getModel());
		String sFileName=FileTableModel.getValueAt(table.getSelectedRow(), 0).toString();
		String sSize=FileTableModel.getValueAt(table.getSelectedRow(), 1).toString();
		
		HWND hwnd=new HWND();
		hwnd.setPointer(Native.getComponentPointer(panelPlayBack)); 
		

		if(RadioForward.isSelected())
		{
			m_lPlayBackHandle=hCNetSDK.NET_DVR_PlayBackByName(m_lUserID, sFileName, hwnd);
		}
		else
		{
			m_lPlayBackHandle = hCNetSDK.NET_DVR_PlayBackReverseByName(m_lUserID,sFileName,hwnd);
		}
		if(m_lPlayBackHandle.longValue() > -1)
		{
			
		}
		else
		{
			DialogMessage dlg = new DialogMessage("playback failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		
		if(!hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYSTART, 0, null))
		{
			DialogMessage dlg = new DialogMessage("Start playback failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		else
		{
			textFieldFileName.setText(sFileName);
			textFieldTotalBytes.setText(sSize);
			
			Playbacktimer=new Timer();
			Playbacktimer.schedule(new PlaybackTask(), 0,1000);
		}
		
	}
	

	
	//stop play
	private void StopActionPerformed(ActionEvent event)
	{
		StopPlay();
	}
	
	//restart
	private void RestartActionPerformed(ActionEvent event)
	{
		if (m_lPlayBackHandle.intValue() == -1)
		{
			return;
		}

		if (!hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYNORMAL, 0, null))
		{
			DialogMessage dlg = new DialogMessage("play failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
		}
	}
	
	//pause
	private void PauseActionPerformed(ActionEvent event)
	{
		if (m_lPlayBackHandle.intValue() == -1)
		{
			return;
		}
		IntByReference intByRef = new IntByReference(0);
		Pointer lpInBuffer = intByRef.getPointer();
		if(!hCNetSDK.NET_DVR_PlayBackControl_V40(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYPAUSE, lpInBuffer,4,null,null))
		{
			DialogMessage dlg = new DialogMessage("pause failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
		}
	}
	
	//fast
	private void FastActionPerformed(ActionEvent event)
	{
		IntByReference intByRef = new IntByReference(0);
		Pointer lpInBuffer = intByRef.getPointer();
		hCNetSDK.NET_DVR_PlayBackControl_V40(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYFAST, lpInBuffer,4,null,null);
	}
	
	//slow
	private void SlowActionPerformed(ActionEvent e)
	{
		IntByReference intByRef = new IntByReference(0);
		Pointer lpInBuffer = intByRef.getPointer();
		hCNetSDK.NET_DVR_PlayBackControl_V40(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYSLOW, lpInBuffer,4,null,null);
	}
	
	//download
	private void DownloadActionPerformed(ActionEvent event)
	{
		
		if(m_lDownloadHandle.intValue() == -1)
		{
			if(table.getSelectedRow() == -1)
			{
				DialogMessage dlg = new DialogMessage("Please select download file");
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
				return;
			}
			
			DefaultTableModel FileTableModel=((DefaultTableModel)table.getModel());
			String sFileName=FileTableModel.getValueAt(table.getSelectedRow(), 0).toString();
			
			m_lDownloadHandle = hCNetSDK.NET_DVR_GetFileByName(m_lUserID, sFileName, sFileName);
			if(m_lDownloadHandle.intValue() >= 0)
			{
				IntByReference intByRef = new IntByReference(0);
				Pointer lpInBuffer = intByRef.getPointer();
				hCNetSDK.NET_DVR_PlayBackControl_V40(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYSTART, lpInBuffer,4,null,null);
				btnNewButtonDownload.setText("Sotp Download");
				progressBar.setValue(0);
				progressBar.setVisible(true);
				
				Downloadtimer=new Timer();
				Downloadtimer.schedule(new DownloadTask(), 0,1000);
			}
			else
			{
				DialogMessage dlg = new DialogMessage("Download failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
				return;
			}
		}
		else 
		{
			hCNetSDK.NET_DVR_StopGetFile(m_lDownloadHandle);
			m_lDownloadHandle.setValue(-1);
			btnNewButtonDownload.setText("Download");
			progressBar.setValue(0);
			progressBar.setVisible(false);
			Downloadtimer.cancel();
		}
	}
	
	//save
	private void SaveActionPerformed(ActionEvent event)
	{
		if (m_lPlayBackHandle.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("Please select play file");
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		JFileChooser myJFileChooser = new JFileChooser();
		myJFileChooser.showSaveDialog(this);
		String sFileName = myJFileChooser.getSelectedFile() + ".mp4";

		if (hCNetSDK.NET_DVR_PlayBackSaveData(m_lPlayBackHandle, sFileName))
		{
			m_bSaveFile = true;
		}
		else
		{
			DialogMessage dlg = new DialogMessage("Save file failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
		}
	}
	
	//stop save
	private void Stop2ActionPerformed(ActionEvent event)
	{
		if (m_bSaveFile)
        {
            hCNetSDK.NET_DVR_StopPlayBackSave(m_lPlayBackHandle);
            m_bSaveFile = false;
            DialogMessage dlg = new DialogMessage("Stop save success");
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
        }
	}
	
	//exit
	private void ExitActionPerformed(ActionEvent event)
	{
        StopPlay();
        dispose();
	}
	
	// SliderPlayBack
	private void SliderPlayBackMouseReleased(MouseEvent e)
	{
		int iPos = sliderPlayBack.getValue();
		if (m_lPlayBackHandle.intValue() >= 0)
		{
			if ((iPos >= 0) && (iPos <= 100))
			{
				if (iPos == 100)
				{
					StopPlay();
					iPos = 99;
				}
				else
				{	
					IntByReference intByRef = new IntByReference(iPos);
					Pointer lpInBuffer = intByRef.getPointer();
					if (hCNetSDK.NET_DVR_PlayBackControl_V40(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYSETPOS, lpInBuffer,4,null,null))
					{
						log.info("success");
					}
					else
					{
						DialogMessage dlg = new DialogMessage("set play progress failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
						dlg.setBounds(0,0 , 370, 200);
						dlg.setVisible(true);
					}
				}
			}
		}
	}
	
	//JPEG
	private void CaptureActionPerformed(ActionEvent event)
	{
		if (m_lPlayBackHandle.intValue() == -1)
		{
			return;
		}
		String sPicName = "C:/Picture/" + Integer.parseInt(textFieldChannelNum.getText()) + System.currentTimeMillis()
				+ ".bmp";
		if (hCNetSDK.NET_DVR_PlayBackCaptureFile(m_lPlayBackHandle, sPicName))
		{
			log.info("jpeg:" + sPicName);
			return;
		}
		else
		{
			DialogMessage dlg = new DialogMessage("JPEG failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
		}
	}
	
	
	
	class PlaybackTask extends java.util.TimerTask
	{
		@Override
		public void run()
		{
			IntByReference nCurrentTime = new IntByReference(0);
			IntByReference nCurrentFrame = new IntByReference(0);
			IntByReference nPos = new IntByReference(0);
			int nHour, nMinute, nSecond;
			if (!m_bGetMaxTime)
			{
				hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_GETTOTALTIME, 0, m_nFileTime);
				if (m_nFileTime.getValue() == 0)
				{
					return;
				}

				if (hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_GETTOTALFRAMES, 0,
						m_nTotalFrames))
				{
					if (m_nTotalFrames.getValue() == 0)
					{
						return;
					}
				} 
				else
				{
					DialogMessage dlg = new DialogMessage("get total frame failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
					dlg.setBounds(0,0 , 370, 200);
					dlg.setVisible(true);
				}

				m_nTotalHour = m_nFileTime.getValue() / 3600;
				m_nTotalMinute = (m_nFileTime.getValue() % 3600) / 60;
				m_nTotalSecond = m_nFileTime.getValue() % 60;
				m_bGetMaxTime = true;
			}

			hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYGETTIME, 0, nCurrentTime);
			if (nCurrentTime.getValue() >= m_nFileTime.getValue())
			{
				nCurrentTime.setValue(m_nFileTime.getValue());
			}
			nHour = (nCurrentTime.getValue() / 3600) % 24;
			nMinute = (nCurrentTime.getValue() % 3600) / 60;
			nSecond = nCurrentTime.getValue() % 60;
			hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYGETFRAME, 0, nCurrentFrame);
			if (nCurrentFrame.getValue() > m_nTotalFrames.getValue())
			{
				nCurrentFrame.setValue(m_nTotalFrames.getValue());
			}

			String sPlayTime;// play time
			sPlayTime = String.format("%02d:%02d:%02d/%02d:%02d:%02d ", nHour, nMinute, nSecond, m_nTotalHour,
					m_nTotalMinute, m_nTotalSecond);
			textFieldPlayTime.setText(sPlayTime);

			hCNetSDK.NET_DVR_PlayBackControl(m_lPlayBackHandle, HCNetSDK.NET_DVR_PLAYGETPOS, 0, nPos);
			if (nPos.getValue() > 100) // 200 indicates network exception
			{
				StopPlay();
				DialogMessage dlg = new DialogMessage("network or DVR busing play exception stop");
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
			}
			else
			{
				sliderPlayBack.setValue(nPos.getValue());
				if (nPos.getValue() == 100)
				{
					StopPlay();
				}
			}
		}
	}
	

	class DownloadTask extends java.util.TimerTask
	{
		@Override
		public void run()
		{
			int iPos = hCNetSDK.NET_DVR_GetDownloadPos(m_lDownloadHandle);
			log.info(""+iPos);
			if (iPos < 0) // failed
			{
				hCNetSDK.NET_DVR_StopGetFile(m_lDownloadHandle);
				progressBar.setVisible(false);
				btnNewButtonDownload.setText("Download");
				m_lDownloadHandle.setValue(-1);

				DialogMessage dlg = new DialogMessage("Get download progress failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
				Downloadtimer.cancel();
			}
			if (iPos == 100) // end download
			{
				hCNetSDK.NET_DVR_StopGetFile(m_lDownloadHandle);
				progressBar.setVisible(false);
				btnNewButtonDownload.setText("Download");
				m_lDownloadHandle.setValue(-1);
				DialogMessage dlg = new DialogMessage("Downlaod ending");
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
				Downloadtimer.cancel();
			}
			if (iPos > 100) // download exception for network problems or DVR
							// hasten
			{
				hCNetSDK.NET_DVR_StopGetFile(m_lDownloadHandle);
				progressBar.setVisible(false);
				btnNewButtonDownload.setText("Download");
				m_lDownloadHandle.setValue(-1);
				DialogMessage dlg = new DialogMessage("NetWork or DVR busing ,download exception stop");
				dlg.setBounds(0,0 , 370, 200);
				dlg.setVisible(true);
				Downloadtimer.cancel();
			}
			else
			{
				progressBar.setValue(iPos);
			}
		}
	}

}
