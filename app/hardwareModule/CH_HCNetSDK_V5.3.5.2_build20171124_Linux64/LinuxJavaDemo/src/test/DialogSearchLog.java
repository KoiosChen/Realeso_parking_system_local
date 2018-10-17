package test;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;

import org.omg.CORBA.PUBLIC_MEMBER;

import com.sun.java_cup.internal.runtime.lr_parser;
import com.sun.jna.NativeLong;
import com.sun.org.apache.bcel.internal.generic.NEWARRAY;

import javafx.scene.control.ComboBox;

import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JComboBox;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JTextField;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class DialogSearchLog extends JDialog
{
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	
	private final JPanel contentPanel = new JPanel();
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
	private JTable tableLog;
	private JComboBox comboBoxMainType;
	private JComboBox comboBoxSubType;
	private JComboBox comboBoxSearchMode;
	private JLabel labelStatus;
	private JButton btnSearch;
	
	private NativeLong m_lLogHandle;
	private NativeLong m_lUserID;
	private boolean m_bSearch;
	private int m_LogNum;
	private SearchThread searchThread;
	
	private HashMap<Integer,String>m_subTypeMap;
	private HashMap<String,Integer>m_subTypeMap2;


	/**
	 * Create the dialog.
	 */
	public DialogSearchLog(NativeLong lUserID)
	{
		super();
		setTitle("Log Search");
		InitComponent();
		InitialDialog();
		m_lLogHandle = new NativeLong(-1);
		m_lUserID = lUserID;
		m_bSearch = false;
		m_LogNum = 0;
		m_subTypeMap = new HashMap<Integer,String>();
		m_subTypeMap2=new HashMap<String,Integer>();
		AddMinorType();
		setModal(true);
	}
	
	public void InitComponent()
	{
		setBounds(100, 100, 577, 534);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);
		{
			JLabel lblNewLabel = new JLabel("Main Type");
			lblNewLabel.setBounds(10, 10, 54, 15);
			contentPanel.add(lblNewLabel);
		}
		{
			comboBoxMainType = new JComboBox();
			comboBoxMainType.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					ComboxSelectChangeActionPerformed(e);
				}
			});
			comboBoxMainType.setModel(new DefaultComboBoxModel(new String[] {"All", "Alarm", "Exception", "Operation", "Info"}));
			comboBoxMainType.setBounds(73, 7, 95, 21);
			contentPanel.add(comboBoxMainType);
		}
		{
			JLabel lblNewLabel_1 = new JLabel("Sub Type");
			lblNewLabel_1.setBounds(178, 10, 54, 15);
			contentPanel.add(lblNewLabel_1);
		}
		{
			comboBoxSubType = new JComboBox();
			comboBoxSubType.setBounds(233, 7, 112, 21);
			contentPanel.add(comboBoxSubType);
		}
		{
			JLabel lblNewLabel_2 = new JLabel("Start Time");
			lblNewLabel_2.setBounds(10, 52, 65, 15);
			contentPanel.add(lblNewLabel_2);
		}
		{
			textFieldsYear = new JTextField();
			textFieldsYear.setColumns(10);
			textFieldsYear.setBounds(87, 49, 45, 21);
			contentPanel.add(textFieldsYear);
		}
		{
			textFieldsMonth = new JTextField();
			textFieldsMonth.setColumns(10);
			textFieldsMonth.setBounds(159, 49, 45, 21);
			contentPanel.add(textFieldsMonth);
		}
		{
			JLabel lblY = new JLabel("Y");
			lblY.setBounds(139, 52, 24, 15);
			contentPanel.add(lblY);
		}
		{
			JLabel lblM = new JLabel("M");
			lblM.setBounds(211, 52, 24, 15);
			contentPanel.add(lblM);
		}
		{
			textFieldsDay = new JTextField();
			textFieldsDay.setColumns(10);
			textFieldsDay.setBounds(233, 49, 45, 21);
			contentPanel.add(textFieldsDay);
		}
		{
			JLabel lblD = new JLabel("D");
			lblD.setBounds(282, 52, 24, 15);
			contentPanel.add(lblD);
		}
		{
			textFieldsHour = new JTextField();
			textFieldsHour.setColumns(10);
			textFieldsHour.setBounds(305, 49, 45, 21);
			contentPanel.add(textFieldsHour);
		}
		{
			JLabel lblH = new JLabel("H");
			lblH.setBounds(353, 52, 24, 15);
			contentPanel.add(lblH);
		}
		{
			textFieldsMinute = new JTextField();
			textFieldsMinute.setColumns(10);
			textFieldsMinute.setBounds(377, 49, 45, 21);
			contentPanel.add(textFieldsMinute);
		}
		{
			JLabel lblM_1 = new JLabel("M");
			lblM_1.setBounds(429, 52, 24, 15);
			contentPanel.add(lblM_1);
		}
		{
			textFieldsSecond = new JTextField();
			textFieldsSecond.setColumns(10);
			textFieldsSecond.setBounds(451, 49, 45, 21);
			contentPanel.add(textFieldsSecond);
		}
		{
			JLabel lblS = new JLabel("S");
			lblS.setBounds(503, 52, 24, 15);
			contentPanel.add(lblS);
		}
		{
			JLabel lblEndTime = new JLabel("End Time");
			lblEndTime.setBounds(12, 77, 65, 15);
			contentPanel.add(lblEndTime);
		}
		{
			textFieldeYear = new JTextField();
			textFieldeYear.setColumns(10);
			textFieldeYear.setBounds(87, 77, 45, 21);
			contentPanel.add(textFieldeYear);
		}
		{
			JLabel label = new JLabel("Y");
			label.setBounds(139, 80, 24, 15);
			contentPanel.add(label);
		}
		{
			textFieldeMonth = new JTextField();
			textFieldeMonth.setColumns(10);
			textFieldeMonth.setBounds(159, 77, 45, 21);
			contentPanel.add(textFieldeMonth);
		}
		{
			JLabel label = new JLabel("M");
			label.setBounds(211, 80, 24, 15);
			contentPanel.add(label);
		}
		{
			textFieldeDay = new JTextField();
			textFieldeDay.setColumns(10);
			textFieldeDay.setBounds(233, 77, 45, 21);
			contentPanel.add(textFieldeDay);
		}
		{
			JLabel label = new JLabel("D");
			label.setBounds(282, 80, 24, 15);
			contentPanel.add(label);
		}
		{
			textFieldeHour = new JTextField();
			textFieldeHour.setColumns(10);
			textFieldeHour.setBounds(305, 77, 45, 21);
			contentPanel.add(textFieldeHour);
		}
		{
			JLabel label = new JLabel("H");
			label.setBounds(353, 80, 24, 15);
			contentPanel.add(label);
		}
		{
			textFieldeMinute = new JTextField();
			textFieldeMinute.setColumns(10);
			textFieldeMinute.setBounds(377, 77, 45, 21);
			contentPanel.add(textFieldeMinute);
		}
		{
			JLabel label = new JLabel("M");
			label.setBounds(429, 80, 24, 15);
			contentPanel.add(label);
		}
		{
			textFieldeSecond = new JTextField();
			textFieldeSecond.setColumns(10);
			textFieldeSecond.setBounds(451, 77, 45, 21);
			contentPanel.add(textFieldeSecond);
		}
		{
			JLabel label = new JLabel("S");
			label.setBounds(503, 80, 24, 15);
			contentPanel.add(label);
		}
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 124, 529, 319);
		contentPanel.add(scrollPane);
		
		tableLog = new JTable();
		tableLog.setModel(initialTableModel());
		scrollPane.setViewportView(tableLog);
		
		btnSearch = new JButton("Search");
		btnSearch.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				StartSearchActionPerformed(e);
			}
		});
		btnSearch.setBounds(242, 463, 75, 23);
		contentPanel.add(btnSearch);
		
		JButton btnNewButton_1 = new JButton("Exit");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				ExitActionPerformed(e);
			}
		});
		btnNewButton_1.setBounds(402, 463, 65, 23);
		contentPanel.add(btnNewButton_1);
		
		JLabel lblSearchType = new JLabel("Search Type");
		lblSearchType.setBounds(355, 10, 74, 15);
		contentPanel.add(lblSearchType);
		
		comboBoxSearchMode = new JComboBox();
		comboBoxSearchMode.setModel(new DefaultComboBoxModel(new String[] {"All", "Time", "Type", "Time & Type"}));
		comboBoxSearchMode.setBounds(429, 7, 112, 21);
		contentPanel.add(comboBoxSearchMode);
		{
			labelStatus = new JLabel("Search Status......");
			labelStatus.setBounds(34, 467, 134, 15);
			contentPanel.add(labelStatus);
		}
	}
	
    //Init table mode
    public DefaultTableModel initialTableModel()
    {
        String tabeTile[];
        tabeTile = new String[]{"Num", "Log Time", "Main Type" ,"Sub Type","Remote Host","Description"};
        DefaultTableModel LogTableModel = new DefaultTableModel(tabeTile, 0);
        return LogTableModel;
    }
    
    private void InitialDialog()
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

        this.addWindowListener(new WindowAdapter()
        {
           @Override
            public void windowClosing(WindowEvent e)
            {
                StopSearch();
            }
        });
    }
    
	public void StartSearchActionPerformed(ActionEvent event) {
		if (!m_bSearch) 
		{
			HCNetSDK.NET_DVR_TIME struStartTime = new HCNetSDK.NET_DVR_TIME();
			HCNetSDK.NET_DVR_TIME struStopTime = new HCNetSDK.NET_DVR_TIME();

			struStartTime.dwYear = Integer.parseInt(textFieldsYear.getText());// start
																				// time
			struStartTime.dwMonth = Integer.parseInt(textFieldsMonth.getText());
			struStartTime.dwDay = Integer.parseInt(textFieldsDay.getText());
			struStartTime.dwHour = Integer.parseInt(textFieldsHour.getText());
			struStartTime.dwMinute = Integer.parseInt(textFieldsMinute.getText());
			struStartTime.dwSecond = Integer.parseInt(textFieldsSecond.getText());
			struStopTime.dwYear = Integer.parseInt(textFieldeYear.getText());// end
																				// time
			struStopTime.dwMonth = Integer.parseInt(textFieldeMonth.getText());
			struStopTime.dwDay = Integer.parseInt(textFieldeDay.getText());
			struStopTime.dwHour = Integer.parseInt(textFieldeHour.getText());
			struStopTime.dwMinute = Integer.parseInt(textFieldeMinute.getText());
			struStopTime.dwSecond = Integer.parseInt(textFieldeSecond.getText());

			int iSelectMode = comboBoxSearchMode.getSelectedIndex();
			NativeLong lSelectMode = new NativeLong(iSelectMode);
			String sMainType = (String)comboBoxMainType.getSelectedItem();
			String sSubType =(String)comboBoxSubType.getSelectedItem();
			int iMainType = comboBoxMainType.getSelectedIndex();
			int iSubType = m_subTypeMap2.get(sSubType);
			m_lLogHandle = hCNetSDK.NET_DVR_FindDVRLog_V30(m_lUserID, lSelectMode, iMainType, iSubType, struStartTime,
					struStopTime, false);
			if (m_lLogHandle.intValue() < 0) 
			{
				JOptionPane.showMessageDialog(this, "search log fail");
				return;
			}

			m_bSearch = true;
			((DefaultTableModel) tableLog.getModel()).getDataVector().removeAllElements();
			((DefaultTableModel) tableLog.getModel()).fireTableStructureChanged();
			m_LogNum = 0;
			searchThread = new SearchThread();
			searchThread.start();
			btnSearch.setText("Stop");
		}
		else
		{
			if(!hCNetSDK.NET_DVR_FindLogClose_V30(m_lLogHandle))
			{
				JOptionPane.showMessageDialog(this, "NET_DVR_FindLogClose_V30 fail");
			}
			m_lLogHandle = new NativeLong(-1);
			labelStatus.setText("Search Log");
			m_bSearch = false;
			m_LogNum = 0;
			searchThread.m_bRun = false;
			btnSearch.setText("Search");

		}
	}

	public void ExitActionPerformed(ActionEvent event)
	{
		StopSearch();
		this.dispose();
	}
	
	public void StopSearch()
	{
		if (m_bSearch) {
			searchThread.m_bRun = false;
		}
		hCNetSDK.NET_DVR_FindClose_V30(m_lLogHandle);
	}
	
	class SearchThread extends Thread
	{
		public boolean m_bRun = true;
		public void run()
		{
			NativeLong lRet = new NativeLong(-1);
			HCNetSDK.NET_DVR_LOG_V30 struFileInfo = new HCNetSDK.NET_DVR_LOG_V30();
			while(m_bRun)
			{
				lRet = hCNetSDK.NET_DVR_FindNextLog_V30(m_lLogHandle, struFileInfo);
				if(lRet.intValue() == hCNetSDK.NET_DVR_FILE_SUCCESS)
				{
					DefaultTableModel LogTableModel=((DefaultTableModel)tableLog.getModel());
					
					//Vector<String> newRow=new Vector<>();
					String [] newRow = new String[6];
					for(int i=0;i<6;i++)
					{
						newRow[i]=new String();
					}
			
					m_LogNum++;
					newRow[0]=m_LogNum+"";
					
					newRow[1]=struFileInfo.strLogTime.toStringTime();
					
					
					switch (struFileInfo.dwMajorType) {
					case 1:
						newRow[2]="Alarm";
						break;
					case 2:
						newRow[2]="Abnormal";
						break;
					case 3:
						newRow[2]="Operation";
						break;
					case 4:
						newRow[2]="Additional information";
						break;
					default:
						newRow[2]="Unknow";
						break;
					}
					
					String csTmp="";
					if (HCNetSDK.MAJOR_INFORMATION == struFileInfo.dwMajorType)
					{
						csTmp=InfoMinorTypeMap(struFileInfo);
					}
					else if(HCNetSDK.MAJOR_OPERATION == struFileInfo.dwMajorType)
					{
						
						csTmp=OperationMinorTypeMap(struFileInfo);
					}
					else if(HCNetSDK.MAJOR_ALARM == struFileInfo.dwMajorType)
					{
						System.out.println("MAJOR_ALARM ");
						csTmp=AlarmMinorTypeMap(struFileInfo);
					}
					else if(HCNetSDK.MAJOR_EXCEPTION == struFileInfo.dwMajorType)
					{
						csTmp=ExceptionMinorTypeMap(struFileInfo);
					}
					else {
						csTmp="UnKnown";
					}
					newRow[3]=csTmp;
					
					String sHost = "";
					if(struFileInfo.sNetUser[0]!='\0')
					{
						String sUserName = new String(struFileInfo.sNetUser);
						sHost+=sUserName.trim();
					}
					
					if(struFileInfo.sPanelUser[0]!='\0')
					{
						String sPanelUser =new String(struFileInfo.sPanelUser);
						sHost+="-"+sPanelUser.trim();
					}
					
					if (struFileInfo.dwChannel > 0)
					{
						sHost+="chan["+struFileInfo.dwChannel+"]";
					}
					
					if (struFileInfo.dwAlarmInPort != 0)
					{
						sHost+="AlarmIn["+struFileInfo.dwChannel+"]";
					}
					
					if (struFileInfo.dwAlarmOutPort != 0)
					{
						sHost+="AlarmOut["+struFileInfo.dwAlarmOutPort+"]";
					}

					String strIP = new String(struFileInfo.struRemoteHostAddr.sIpV4);
					sHost += "-" + strIP;
					
					
					//newRow.add(sHost);
					newRow[4]=sHost;

					String sDescription = "UnKnown";
					if(struFileInfo.dwInfoLen > 0)
					{
						sDescription = struFileInfo.sInfo.toString();
					}
					newRow[5]=sDescription;
					LogTableModel.insertRow(m_LogNum-1, newRow);
					//((DefaultTableModel)tableLog.getModel()).fireTableStructureChanged();
					
				}
				else
				{
					if(lRet.intValue() == hCNetSDK.NET_DVR_ISFINDING)
					{
						labelStatus.setText("Seraching ......");
						try {
							sleep(5);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						continue;
					}
					if((lRet.intValue() == hCNetSDK.NET_DVR_NOMOREFILE) || (lRet.intValue() == hCNetSDK.NET_DVR_FILE_NOFIND))
					{
						btnSearch.setText("Search");
						labelStatus.setText("Search log Ending");
						break;
					}
					else
					{
						btnSearch.setText("Search");
						labelStatus.setText("Since the server is busy, or network failure, search the log abnormal termination");
						break;
					}
				}
			}
			hCNetSDK.NET_DVR_FindClose_V30(m_lLogHandle);
		}
	}
	
	public String InfoMinorTypeMap(HCNetSDK.NET_DVR_LOG_V30 stLogInfo)
	{
		String szTemp="";
	    switch(stLogInfo.dwMinorType)
	    {
	    case HCNetSDK.MINOR_HDD_INFO:
	    	szTemp = "HDD_INFO";
	        break;
	    case HCNetSDK.MINOR_SMART_INFO:
	        szTemp="SMART_INFO";
	        break;
	    case HCNetSDK.MINOR_REC_START:
	        szTemp="REC_START";
	        break;
	    case HCNetSDK.MINOR_REC_STOP:
	        szTemp="REC_STOP";
	        break;
	    case HCNetSDK.MINOR_REC_OVERDUE:
	        szTemp="REC_OVERDUE";
	        break;	 
	    case HCNetSDK.MINOR_LINK_START:
	        szTemp="LINK_START";
	        break;	 
	    case HCNetSDK.MINOR_LINK_STOP:
	        szTemp="LINK_STOP";
	        break;	 
		case HCNetSDK.MINOR_NET_DISK_INFO:
			szTemp="NET_DISK_INFO";
	        break;
	    case HCNetSDK.MINOR_RAID_INFO:
	        szTemp="RAID_INFO";
	        break;
		case HCNetSDK.MINOR_RUN_STATUS_INFO:
			szTemp="RUN_STATUS_INFO";
			break;		
	    default:
	        szTemp="unKnown";
	        break;
	    }
	    return szTemp;
	}
	
	public String OperationMinorTypeMap(HCNetSDK.NET_DVR_LOG_V30 stLogInfo)
	{
		String szTemp;
	    switch(stLogInfo.dwMinorType)
	    {
	        //operation
	    case HCNetSDK.MINOR_START_DVR:
	        szTemp="START_DVR";
	        break;
	    case HCNetSDK.MINOR_STOP_DVR:
	        szTemp="STOP_DVR";
	        break;
	    case HCNetSDK.MINOR_STOP_ABNORMAL:
	        szTemp="STOP_ABNORMAL";
	        break;
	    case HCNetSDK.MINOR_REBOOT_DVR:
	        szTemp="REBOOT_DVR";
	        break;
	    case HCNetSDK.MINOR_LOCAL_LOGIN:
	        szTemp="LOCAL_LOGIN";
	        break;
	    case HCNetSDK.MINOR_LOCAL_LOGOUT:
	        szTemp="LOCAL_LOGOUT";
	        break;
	    case HCNetSDK.MINOR_LOCAL_CFG_PARM:
	        szTemp="LOCAL_CFG_PARM";
	        break;
	    case HCNetSDK.MINOR_LOCAL_PLAYBYFILE:
	        szTemp="LOCAL_PLAYBYFILE";
	        break;
	    case HCNetSDK.MINOR_LOCAL_PLAYBYTIME:
	        szTemp="LOCAL_PLAYBYTIME";
	        break;
	    case HCNetSDK.MINOR_LOCAL_START_REC:
	        szTemp="LOCAL_START_REC";
	        break;
		default:
	        szTemp="UnKnown";
	        break;
		}
	    return szTemp;
	}
	
	public String AlarmMinorTypeMap(HCNetSDK.NET_DVR_LOG_V30 stLogInfo)
	{
		String szTemp="";
	    switch(stLogInfo.dwMinorType)
	    {
			//alarm
	    case HCNetSDK.MINOR_ALARM_IN:
	        szTemp="ALARM_IN["+stLogInfo.dwAlarmInPort+"]";
	        break;
	    case HCNetSDK.MINOR_ALARM_OUT:
	        szTemp="ALARM_OUT["+stLogInfo.dwAlarmOutPort+"]";
	        break;
	    case HCNetSDK.MINOR_MOTDET_START:
	        szTemp="MOTDET_START["+stLogInfo.dwChannel+"]";
	        break;
	    case HCNetSDK.MINOR_MOTDET_STOP:
	        szTemp="MOTDET_STOP";
	        break;
	    case HCNetSDK.MINOR_HIDE_ALARM_START:
	        szTemp="HIDE_ALARM_START";
	        break;
	    case HCNetSDK.MINOR_HIDE_ALARM_STOP:
	        szTemp="HIDE_ALARM_STOP";
	        break;
	    case HCNetSDK.MINOR_VCA_ALARM_START:
	        szTemp="VCA_ALARM_START";
	        break;
	    case HCNetSDK.MINOR_VCA_ALARM_STOP:
	        szTemp="VCA_ALARM_STOP";
	        break;
		case HCNetSDK.MINOR_NETALARM_START:
	        szTemp="NETALARM_START";
	        break;
		case HCNetSDK.MINOR_NETALARM_STOP:
	        szTemp="NETALARM_STOP";
	        break;
		case HCNetSDK.MINOR_WIRELESS_ALARM_START:
	        szTemp="WIRELESS_ALARM_START";
	        break;
		case HCNetSDK.MINOR_WIRELESS_ALARM_STOP:
	        szTemp="WIRELESS_ALARM_STOP";
	        break;
		case HCNetSDK.MINOR_PIR_ALARM_START:
	        szTemp="PIR_ALARM_START";
	        break;
		case HCNetSDK.MINOR_PIR_ALARM_STOP:
	        szTemp="PIR_ALARM_STOP";
	        break;
		case HCNetSDK.MINOR_CALLHELP_ALARM_START:
	        szTemp="CALLHELP_ALARM_START";
	        break;
		case HCNetSDK.MINOR_CALLHELP_ALARM_STOP:
	        szTemp="CALLHELP_ALARM_STOP";
	        break;
		case HCNetSDK.MINOR_DETECTFACE_ALARM_START:
	        szTemp="DETECTFACE_ALARM_START";
	        break;
		case HCNetSDK.MINOR_DETECTFACE_ALARM_STOP:
	        szTemp="DETECTFACE_ALARM_STOP";
	        break;
		case HCNetSDK.MINOR_VQD_ALARM_START:
			szTemp="VQD Alarm Start";
			break;
		case HCNetSDK.MINOR_VQD_ALARM_STOP:
			szTemp="VQD Alarm Stop";
			break;
	    case HCNetSDK.MINOR_VCA_SECNECHANGE_DETECTION://场景变更侦测日志查询2013-07-17
	        szTemp="VCA_SECNECHANGE_DETECTION";
			break;
	    case HCNetSDK.MINOR_SMART_REGION_EXITING_BEGIN:
	        szTemp="Region Exiting Begin";
			break;
	    case HCNetSDK.MINOR_SMART_REGION_EXITING_END:
	        szTemp="Region Exiting End";
			break;
	    case HCNetSDK.MINOR_SMART_LOITERING_BEGIN:
	        szTemp="Loitering Begin";
			break;
	    case HCNetSDK.MINOR_SMART_LOITERING_END:
	        szTemp="Loitering End";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_LINE_DETECTION_BEGIN:
			szTemp="TRAVERSE_PLANE_DETECTION START";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_LINE_DETECTION_END:
			szTemp="TRAVERSE_PLANE_DETECTION Stop";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_INTRUDE_BEGIN:
			szTemp="NTRUDE Start";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_INTRUDE_END:
			szTemp="NTRUDE Stop";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_AUDIOINPUT:
			szTemp="AudioInput Exception";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_AUDIOABNORMAL:
			szTemp="AudioInput Stop";
			break;
		case HCNetSDK.MINOR_VCA_DEFOCUS_DETECTION_BEGIN:
			szTemp="defous detection Start";
			break;
		case HCNetSDK.MINOR_VCA_DEFOCUS_DETECTION_END:
			szTemp="defous detection Stop";
			break;
		case HCNetSDK.MINOR_EXT_ALARM:
			szTemp="external alarm";
			break;
		case HCNetSDK.MINOR_VCA_FACE_ALARM_BEGIN:
			szTemp="SMD face alarm start";
			break;
	    case HCNetSDK.MINOR_SMART_REGION_ENTRANCE_BEGIN:
	        szTemp="Region Entrance Begin";
	        break;
	    case HCNetSDK.MINOR_SMART_REGION_ENTRANCE_END:
	        szTemp="Region Entrance End";
	        break;
	    case HCNetSDK.MINOR_SMART_PEOPLE_GATHERING_BEGIN:
	        szTemp="People Gathering Begin";
			break;
	    case HCNetSDK.MINOR_SMART_PEOPLE_GATHERING_END:
	        szTemp="People Gathering End";
			break;
	    case HCNetSDK.MINOR_SMART_FAST_MOVING_BEGIN:
	        szTemp="Fast Moving Begin";
			break;
	    case HCNetSDK.MINOR_SMART_FAST_MOVING_END:
	        szTemp="Fast Moving End";
			break;
		case HCNetSDK.MINOR_VCA_FACE_ALARM_END:
			szTemp="SMD face alarm end";
			break;
		case HCNetSDK.MINOR_VCA_SCENE_CHANGE_ALARM_BEGIN:
			szTemp="SMD scene change alarm start";
			break;
		case HCNetSDK.MINOR_VCA_SCENE_CHANGE_ALARM_END:
			szTemp="SMD scene change alarm stop";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_AUDIOINPUT_BEGIN:
			szTemp="SMD alarm audio input start";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_AUDIOINPUT_END:
			szTemp="SMD alarm audio input stop";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_AUDIOABNORMAL_BEGIN:
			szTemp="SMD alarm audio abnormal start";
			break;
		case HCNetSDK.MINOR_VCA_ALARM_AUDIOABNORMAL_END:
			szTemp="SMD alarm audio abnormal end";
			break;
		case HCNetSDK.MINOR_VCA_LECTURE_DETECTION_BEGIN:
			szTemp="Lecture Start";
			break;
		case HCNetSDK.MINOR_VCA_LECTURE_DETECTION_END:
			szTemp="Lecture End";
			break;
		case HCNetSDK.MINOR_VCA_ANSWER_DETECTION_BEGIN:
			szTemp="Answering Questions Start";
			break;
		case HCNetSDK.MINOR_VCA_ANSWER_DETECTION_END:
			szTemp="Answering Questions End";
			break;
	    case HCNetSDK.MINOR_SMART_PARKING_BEGIN:
	        szTemp="Parking Begin";
			break;
	    case HCNetSDK.MINOR_SMART_PARKING_END:
	        szTemp="Parking End";
			break;
	    case HCNetSDK.MINOR_SMART_UNATTENDED_BAGGAGE_BEGIN:
	        szTemp="Unattended Baggage Begin";
			break;
	    case HCNetSDK.MINOR_SMART_UNATTENDED_BAGGAGE_END:
	        szTemp="Unattended Baggage End";
			break;
	    case HCNetSDK.MINOR_SMART_OBJECT_REMOVAL_BEGIN:
	        szTemp="Object Removal Begin";
			break;
	    case HCNetSDK.MINOR_SMART_OBJECT_REMOVAL_END:
	        szTemp="Object Removal End";
			break;
	    case HCNetSDK.MINOR_THERMAL_FIREDETECTION:
	        szTemp="Thermal Fire Detection Begin";
			break;
		case HCNetSDK.MINOR_THERMAL_FIREDETECTION_END:
	        szTemp="Thermal Fire Detection End";
			break;
	    case HCNetSDK.MINOR_ALARMIN_SHORT_CIRCUIT:
	        szTemp="MINOR_ALARMIN_SHORT_CIRCUIT";
	        break;
	    case HCNetSDK.MINOR_ALARMIN_BROKEN_CIRCUIT:
	        szTemp="MINOR_ALARMIN_BROKEN_CIRCUIT";
	        break;
	    case HCNetSDK.MINOR_ALARMIN_EXCEPTION:
	        szTemp="MINOR_ALARMIN_EXCEPTION";
	        break;
	    case HCNetSDK.MINOR_ALARMIN_RESUME:
	        szTemp="MINOR_ALARMIN_RESUME";
	        break;
	    case HCNetSDK.MINOR_HOST_DESMANTLE_ALARM:
	        szTemp="MINOR_HOST_DESMANTLE_ALARM";
	        break;
	    case HCNetSDK.MINOR_HOST_DESMANTLE_RESUME:
	        szTemp="MINOR_HOST_DESMANTLE_RESUME";
	        break;
	    case HCNetSDK.MINOR_CARD_READER_DESMANTLE_ALARM:
	        szTemp="MINOR_CARD_READER_DESMANTLE_ALARM";
	        break;
	    case HCNetSDK.MINOR_CARD_READER_DESMANTLE_RESUME:
	        szTemp="MINOR_CARD_READER_DESMANTLE_RESUME";
	        break;
	    case  HCNetSDK.MINOR_CASE_SENSOR_ALARM:
	        szTemp=" MINOR_CASE_SENSOR_ALARM";
	        break;
	    case HCNetSDK.MINOR_CASE_SENSOR_RESUME:
	        szTemp="MINOR_CASE_SENSOR_RESUME";
	        break;
	    case HCNetSDK.MINOR_STRESS_ALARM:
	        szTemp="MINOR_STRESS_ALARM";
	        break;
	    case  HCNetSDK.MINOR_OFFLINE_ECENT_NEARLY_FULL:
	        szTemp=" MINOR_OFFLINE_ECENT_NEARLY_FULL";
	        break;
	    case  HCNetSDK.MINOR_CARD_MAX_AUTHENTICATE_FAIL:
	        szTemp=" MINOR_CARD_MAX_AUTHENTICATE_FAIL";
	        break;
	    case  HCNetSDK.MINOR_SD_CARD_FULL:
	        szTemp=" MINOR_SD_CARD_FULL";
	        break;
	    case  HCNetSDK.MINOR_LINKAGE_CAPTURE_PIC:
	        szTemp=" MINOR_LINKAGE_CAPTURE_PIC";
	        break;
	    case HCNetSDK.MINOR_SMART_VANDALPROOF_BEGIN:
	        szTemp="VandalProof Detection Begin";
	        break;
	    case HCNetSDK.MINOR_SMART_VANDALPROOF_END:
	        szTemp="VandalProof Detection End";
			break;
	    default:
	    	szTemp="unKnown";
	        break;
	    }
	    return szTemp;
	}
	
	public String ExceptionMinorTypeMap(HCNetSDK.NET_DVR_LOG_V30 stLogInfo)
	{
		String szTemp="";
	    switch(stLogInfo.dwMinorType)
	    {
	        //exception
		case HCNetSDK.MINOR_RAID_ERROR:
	        szTemp="RAID_ERROR";
	        break;
	    case HCNetSDK.MINOR_VI_LOST:
	        szTemp="VI_LOST";
	        break;
	    case HCNetSDK.MINOR_ILLEGAL_ACCESS:
	        szTemp="ILLEGAL_ACCESS";
	        break;
	    case HCNetSDK.MINOR_HD_FULL:
	        szTemp="HD_FULL["+stLogInfo.dwDiskNumber+"]";
	        break;
	    case HCNetSDK.MINOR_HD_ERROR:
	        szTemp="HD_ERROR["+stLogInfo.dwDiskNumber+"]";
	        break;
	    case HCNetSDK.MINOR_REC_ERROR:
	        szTemp="REC_ERROR";
	        break;
	    case HCNetSDK.MINOR_DCD_LOST:
	        szTemp="DCD_LOST";
	        break;
	    case HCNetSDK.MINOR_IPC_NO_LINK:
	        szTemp="IPC_NO_LINK";
	        break;
	    case HCNetSDK.MINOR_IP_CONFLICT:
	        szTemp="IP_CONFLICT";
	        break;
	    case HCNetSDK.MINOR_NET_BROKEN:
	        szTemp="NET_BROKEN";
	        break;
		default:
			szTemp="unKnown";
	        break;
	    }
	    return szTemp;
	}
	public void InitMap()
	{
		m_subTypeMap.put(0, "All");
		
		//Alarm
		m_subTypeMap.put(HCNetSDK.MINOR_ALARM_IN,"ALARM_IN");	       
		m_subTypeMap.put(HCNetSDK.MINOR_ALARM_OUT,"ALARM_OUT");
		m_subTypeMap.put(HCNetSDK.MINOR_MOTDET_START,"MOTDET_START");
		m_subTypeMap.put(HCNetSDK.MINOR_MOTDET_STOP,"MOTDET_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_HIDE_ALARM_START, "HIDE_ALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_HIDE_ALARM_STOP, "HIDE_ALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_START, "VCA_ALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_STOP, "VCA_ALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_NETALARM_START, "NETALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_NETALARM_STOP, "NETALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_WIRELESS_ALARM_START, "WIRELESS_ALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_WIRELESS_ALARM_STOP, "WIRELESS_ALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_PIR_ALARM_START, "PIR_ALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_PIR_ALARM_STOP, "PIR_ALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_CALLHELP_ALARM_START, "CALLHELP_ALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_CALLHELP_ALARM_STOP, "CALLHELP_ALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_DETECTFACE_ALARM_START, "DETECTFACE_ALARM_START");
		m_subTypeMap.put(HCNetSDK.MINOR_DETECTFACE_ALARM_STOP, "DETECTFACE_ALARM_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_VQD_ALARM_START, "VQD Alarm Start");
		m_subTypeMap.put(HCNetSDK.MINOR_VQD_ALARM_STOP, "VQD Alarm Stop");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_SECNECHANGE_DETECTION, "VCA_SECNECHANGE_DETECTION");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_REGION_EXITING_BEGIN, "Region Exiting Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_REGION_EXITING_END, "Region Exiting End");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_LOITERING_BEGIN, "Loitering Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_LOITERING_END, "Loitering End");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_LINE_DETECTION_BEGIN, "TRAVERSE_PLANE_DETECTION START");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_LINE_DETECTION_END, "TRAVERSE_PLANE_DETECTION Stop");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_INTRUDE_BEGIN, "NTRUDE Start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_INTRUDE_END, "NTRUDE Stop");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_AUDIOINPUT, "AudioInput Exception");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_AUDIOABNORMAL, "AudioInput Stop");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_DEFOCUS_DETECTION_BEGIN, "defous detection Start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_DEFOCUS_DETECTION_END, "defous detection Stop");
		m_subTypeMap.put(HCNetSDK.MINOR_EXT_ALARM, "external alarm");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_FACE_ALARM_BEGIN, "SMD face alarm start");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_REGION_ENTRANCE_BEGIN, "Region Entrance Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_REGION_ENTRANCE_END, "Region Entrance End");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_PEOPLE_GATHERING_BEGIN, "People Gathering Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_PEOPLE_GATHERING_END, "People Gathering End");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_FAST_MOVING_BEGIN, "Fast Moving Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_FAST_MOVING_END, "Fast Moving End");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_FACE_ALARM_END, "SMD face alarm end");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_SCENE_CHANGE_ALARM_BEGIN, "SMD scene change alarm start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_SCENE_CHANGE_ALARM_END, "SMD scene change alarm stop");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_AUDIOINPUT_BEGIN, "SMD alarm audio input start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_AUDIOINPUT_END, "SMD alarm audio input stop");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_AUDIOABNORMAL_BEGIN, "SMD alarm audio abnormal start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ALARM_AUDIOABNORMAL_END, "SMD alarm audio abnormal end");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_LECTURE_DETECTION_BEGIN, "Lecture Start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_LECTURE_DETECTION_END, "Lecture End");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ANSWER_DETECTION_BEGIN, "Answering Questions Start");
		m_subTypeMap.put(HCNetSDK.MINOR_VCA_ANSWER_DETECTION_END, "Answering Questions End");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_PARKING_BEGIN, "Parking Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_PARKING_END, "Parking End");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_UNATTENDED_BAGGAGE_BEGIN, "Unattended Baggage Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_UNATTENDED_BAGGAGE_END, "Unattended Baggage End");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_OBJECT_REMOVAL_BEGIN, "Object Removal Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_OBJECT_REMOVAL_END, "Object Removal End");
		m_subTypeMap.put(HCNetSDK.MINOR_THERMAL_FIREDETECTION, "Thermal Fire Detection Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_THERMAL_FIREDETECTION_END, "Thermal Fire Detection End");
		m_subTypeMap.put(HCNetSDK.MINOR_ALARMIN_SHORT_CIRCUIT, "MINOR_ALARMIN_SHORT_CIRCUIT");
		m_subTypeMap.put(HCNetSDK.MINOR_ALARMIN_BROKEN_CIRCUIT, "MINOR_ALARMIN_BROKEN_CIRCUIT");
		m_subTypeMap.put(HCNetSDK.MINOR_ALARMIN_EXCEPTION, "MINOR_ALARMIN_EXCEPTION");
		m_subTypeMap.put(HCNetSDK.MINOR_ALARMIN_RESUME, "MINOR_ALARMIN_RESUME");
		m_subTypeMap.put(HCNetSDK.MINOR_HOST_DESMANTLE_ALARM, "MINOR_HOST_DESMANTLE_ALARM");
		m_subTypeMap.put(HCNetSDK.MINOR_HOST_DESMANTLE_RESUME, "MINOR_HOST_DESMANTLE_RESUME");
		m_subTypeMap.put(HCNetSDK.MINOR_CARD_READER_DESMANTLE_ALARM, "MINOR_CARD_READER_DESMANTLE_ALARM");
		m_subTypeMap.put(HCNetSDK.MINOR_CARD_READER_DESMANTLE_RESUME, "MINOR_CARD_READER_DESMANTLE_RESUME");
		m_subTypeMap.put(HCNetSDK.MINOR_CASE_SENSOR_ALARM, " MINOR_CASE_SENSOR_ALARM");
		m_subTypeMap.put(HCNetSDK.MINOR_CASE_SENSOR_RESUME, "MINOR_CASE_SENSOR_RESUME");
		m_subTypeMap.put(HCNetSDK.MINOR_STRESS_ALARM, "MINOR_STRESS_ALARM");
		m_subTypeMap.put(HCNetSDK.MINOR_OFFLINE_ECENT_NEARLY_FULL, " MINOR_OFFLINE_ECENT_NEARLY_FULL");
		m_subTypeMap.put(HCNetSDK.MINOR_CARD_MAX_AUTHENTICATE_FAIL, " MINOR_CARD_MAX_AUTHENTICATE_FAIL");
		m_subTypeMap.put(HCNetSDK.MINOR_SD_CARD_FULL, " MINOR_SD_CARD_FULL");
		m_subTypeMap.put(HCNetSDK.MINOR_LINKAGE_CAPTURE_PIC, " MINOR_LINKAGE_CAPTURE_PIC");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_VANDALPROOF_BEGIN, "VandalProof Detection Begin");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_VANDALPROOF_END, "VandalProof Detection End");
		
		//Exception
		m_subTypeMap.put(HCNetSDK.MINOR_RAID_ERROR, "RAID_ERROR");
		m_subTypeMap.put(HCNetSDK.MINOR_VI_LOST, "VI_LOST");
		m_subTypeMap.put(HCNetSDK.MINOR_ILLEGAL_ACCESS, "ILLEGAL_ACCESS");
		m_subTypeMap.put(HCNetSDK.MINOR_HD_FULL, "HD_FULL");
		m_subTypeMap.put(HCNetSDK.MINOR_HD_ERROR, "HD_ERROR");
		m_subTypeMap.put(HCNetSDK.MINOR_REC_ERROR, "REC_ERROR");
		m_subTypeMap.put(HCNetSDK.MINOR_DCD_LOST, "DCD_LOST");
		m_subTypeMap.put(HCNetSDK.MINOR_IPC_NO_LINK, "IPC_NO_LINK");
		m_subTypeMap.put(HCNetSDK.MINOR_IP_CONFLICT, "IP_CONFLICT");
		m_subTypeMap.put(HCNetSDK.MINOR_NET_BROKEN, "NET_BROKEN");
		
		// operation
		m_subTypeMap.put(HCNetSDK.MINOR_START_DVR, "START_DVR");
		m_subTypeMap.put(HCNetSDK.MINOR_STOP_DVR, "STOP_DVR");
		m_subTypeMap.put(HCNetSDK.MINOR_STOP_ABNORMAL, "STOP_ABNORMAL");
		m_subTypeMap.put(HCNetSDK.MINOR_REBOOT_DVR, "REBOOT_DVR");
		m_subTypeMap.put(HCNetSDK.MINOR_LOCAL_LOGIN, "LOCAL_LOGIN");
		m_subTypeMap.put(HCNetSDK.MINOR_LOCAL_LOGOUT, "LOCAL_LOGOUT");
		m_subTypeMap.put(HCNetSDK.MINOR_LOCAL_CFG_PARM, "LOCAL_CFG_PARM");
		m_subTypeMap.put(HCNetSDK.MINOR_LOCAL_PLAYBYFILE, "LOCAL_PLAYBYFILE");
		m_subTypeMap.put(HCNetSDK.MINOR_LOCAL_PLAYBYTIME, "LOCAL_PLAYBYTIME");
		m_subTypeMap.put(HCNetSDK.MINOR_LOCAL_START_REC, "LOCAL_START_REC");
		
		//Info
		m_subTypeMap.put(HCNetSDK.MINOR_HDD_INFO, "HDD_INFO");
		m_subTypeMap.put(HCNetSDK.MINOR_SMART_INFO, "SMART_INFO");
		m_subTypeMap.put(HCNetSDK.MINOR_REC_START, "REC_START");
		m_subTypeMap.put(HCNetSDK.MINOR_REC_STOP, "REC_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_REC_OVERDUE, "REC_OVERDUE");
		m_subTypeMap.put(HCNetSDK.MINOR_LINK_START, "LINK_START");
		m_subTypeMap.put(HCNetSDK.MINOR_LINK_STOP, "LINK_STOP");
		m_subTypeMap.put(HCNetSDK.MINOR_NET_DISK_INFO, "NET_DISK_INFO");
		m_subTypeMap.put(HCNetSDK.MINOR_RAID_INFO, "RAID_INFO");
		m_subTypeMap.put(HCNetSDK.MINOR_RUN_STATUS_INFO, "RUN_STATUS_INFO");

	}

	public void AddMinorType()
	{
		comboBoxSubType.removeAllItems();
		InitMap();
		Iterator it = m_subTypeMap.keySet().iterator();
		while (it.hasNext()) {
			Integer key = (Integer) it.next();
			String val = (String) m_subTypeMap.get(key);
			comboBoxSubType.addItem(val);
			m_subTypeMap2.put(val, key);
		}
	}
	
	public void ComboxSelectChangeActionPerformed(ActionEvent event)
	{
		int iSel = comboBoxMainType.getSelectedIndex();
		Iterator it = m_subTypeMap.keySet().iterator();
		if(iSel == 1)
		{
			comboBoxSubType.removeAllItems();
			comboBoxSubType.addItem("All");
			while (it.hasNext()) {
				Integer key = (Integer) it.next();
				if ((key >= HCNetSDK.MINOR_ALARM_IN) && (key <= HCNetSDK.MINOR_SMART_VANDALPROOF_END)) 
				{
					String val = (String) m_subTypeMap.get(key);
					comboBoxSubType.addItem(val);
				}
			}
		}
		else if(iSel == 2)
		{
			comboBoxSubType.removeAllItems();
			comboBoxSubType.addItem("All");
			while (it.hasNext()) {
				Integer key = (Integer) it.next();
				if ((key >= HCNetSDK.MINOR_RAID_ERROR) && (key <= HCNetSDK.MINOR_NET_BROKEN)) 
				{
					String val = (String) m_subTypeMap.get(key);
					comboBoxSubType.addItem(val);
				}
			}
		}
		else if(iSel == 3)
		{
			comboBoxSubType.removeAllItems();
			comboBoxSubType.addItem("All");
			while (it.hasNext()) {
				Integer key = (Integer) it.next();
				if ((key >= HCNetSDK.MINOR_START_DVR) && (key <= HCNetSDK.MINOR_LOCAL_START_REC)) 
				{
					String val = (String) m_subTypeMap.get(key);
					comboBoxSubType.addItem(val);
				}
			}
		}
		else if(iSel == 4)
		{
			comboBoxSubType.removeAllItems();
			comboBoxSubType.addItem("All");
			while (it.hasNext()) {
				Integer key = (Integer) it.next();
				if ((key >= HCNetSDK.MINOR_HDD_INFO) && (key <= HCNetSDK.MINOR_RUN_STATUS_INFO)) 
				{
					String val = (String) m_subTypeMap.get(key);
					comboBoxSubType.addItem(val);
				}
			}
		}
		else{
			comboBoxSubType.removeAllItems();
			while (it.hasNext()) {
				Integer key = (Integer) it.next();
				String val = (String) m_subTypeMap.get(key);
				comboBoxSubType.addItem(val);
			}
		}
	}

}
