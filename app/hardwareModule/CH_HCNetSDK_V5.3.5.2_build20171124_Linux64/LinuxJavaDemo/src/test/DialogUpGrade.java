package test;

import java.awt.BorderLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JComboBox;
import javax.swing.DefaultComboBoxModel;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

import test.HCNetSDK.ENUM_UPGRADE_TYPE;

import javax.swing.JTextField;
import javax.swing.JProgressBar;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;
import java.awt.event.ActionEvent;

public class DialogUpGrade extends JDialog
{

	private final JPanel contentPanel = new JPanel();
	private JTextField textFieldFilePath;
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private JComboBox comboBoxNetEnv;
	private JComboBox comboBoxUpGradeType;
	private JComboBox comboBoxChan;
	private JLabel labelChan;
	private JComboBox comboBoxAuxDev;
	private JProgressBar progressBar;
	private JLabel labelStatus;
	
	NativeLong m_lUserID;
	int m_iChanNum;
	Timer time;



	/**
	 * Create the dialog.
	 */
	public DialogUpGrade(NativeLong lUserID,int iChanNum)
	{
		super();
		setTitle("Remote Upgrade");
		InitComponent();
		m_lUserID = lUserID;
		m_iChanNum = iChanNum;
        progressBar.setMinimum(0);
        progressBar.setMaximum(100);
        setModal(true);
	}
	
	public void InitComponent()
	{
		setBounds(100, 100, 573, 326);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);
		{
			JLabel lblNewLabel = new JLabel("Network Type");
			lblNewLabel.setBounds(10, 20, 83, 15);
			contentPanel.add(lblNewLabel);
		}
		{
			comboBoxNetEnv = new JComboBox();
			comboBoxNetEnv.setModel(new DefaultComboBoxModel(new String[] {"LAN"}));
			comboBoxNetEnv.setBounds(92, 17, 96, 21);
			contentPanel.add(comboBoxNetEnv);
		}
		{
			JButton btnNewButton = new JButton("Set NetWork Type");
			btnNewButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					SetNetEnvironment(e);
				}
			});
			btnNewButton.setBounds(380, 16, 149, 23);
			contentPanel.add(btnNewButton);
		}
		{
			JLabel lblNewLabel_1 = new JLabel("Upgrade Type");
			lblNewLabel_1.setBounds(10, 63, 72, 15);
			contentPanel.add(lblNewLabel_1);
		}
		{
			comboBoxUpGradeType = new JComboBox();
			comboBoxUpGradeType.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					SelectedChangeActionPerformed(e);
				}
			});
			comboBoxUpGradeType.setModel(new DefaultComboBoxModel(new String[] {"DVR", "Adapter", "Vcalib", "ACS", "IDS", "LED"}));
			comboBoxUpGradeType.setBounds(92, 60, 96, 21);
			contentPanel.add(comboBoxUpGradeType);
		}
		{
			JLabel lblAuxDev = new JLabel("Aux Dev");
			lblAuxDev.setBounds(366, 63, 54, 15);
			contentPanel.add(lblAuxDev);
		}
		{
			comboBoxAuxDev = new JComboBox();
			comboBoxAuxDev.setModel(new DefaultComboBoxModel(new String[] {"Keyboard", "Movement"}));
			comboBoxAuxDev.setBounds(430, 60, 96, 21);
			contentPanel.add(comboBoxAuxDev);
		}
		{
			JLabel lblNewLabel_2 = new JLabel("File");
			lblNewLabel_2.setBounds(10, 108, 54, 15);
			contentPanel.add(lblNewLabel_2);
		}
		{
			textFieldFilePath = new JTextField();
			textFieldFilePath.setText("c:\\digicap");
			textFieldFilePath.setBounds(74, 105, 357, 21);
			contentPanel.add(textFieldFilePath);
			textFieldFilePath.setColumns(10);
		}
		{
			JButton btnNewButton_1 = new JButton("Scan");
			btnNewButton_1.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					ScanActionPerformed(e);
				}
			});
			btnNewButton_1.setBounds(456, 104, 70, 23);
			contentPanel.add(btnNewButton_1);
		}
		
		progressBar = new JProgressBar();
		progressBar.setBounds(10, 204, 519, 14);
		contentPanel.add(progressBar);
		
		JButton btnNewButton_3 = new JButton("Upgrade");
		btnNewButton_3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				UpGradeActionPerformed(e);
			}
		});
		btnNewButton_3.setBounds(45, 247, 93, 23);
		contentPanel.add(btnNewButton_3);
		
		JButton btnNewButton_4 = new JButton("Exit");
		btnNewButton_4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				ExitActionPerformed(e);
			}
		});
		btnNewButton_4.setBounds(311, 247, 93, 23);
		contentPanel.add(btnNewButton_4);
		{
			labelStatus = new JLabel("State");
			labelStatus.setBounds(10, 169, 54, 15);
			contentPanel.add(labelStatus);
		}
		
		labelChan = new JLabel("Type");
		labelChan.setVisible(false);
		labelChan.setBounds(201, 63, 31, 15);
		contentPanel.add(labelChan);
		
		comboBoxChan = new JComboBox();
		comboBoxChan.setVisible(false);
		comboBoxChan.setBounds(229, 60, 107, 21);
		contentPanel.add(comboBoxChan);
	}
	
	//set net work environment
	public void SetNetEnvironment(ActionEvent event)
	{
		int iEnvLevel = comboBoxNetEnv.getSelectedIndex();
		if (!hCNetSDK.NET_DVR_SetNetworkEnvironment(iEnvLevel))
        {
            DialogMessage dlg = new DialogMessage("set net environment failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
            return;
        }
	}
	
	//scan file
	public void ScanActionPerformed(ActionEvent event)
	{
		JFileChooser jFileChooser = new JFileChooser("c:/digicap");
		if(JFileChooser.APPROVE_OPTION == jFileChooser.showOpenDialog(this))
		{
			openFile(jFileChooser.getSelectedFile().getPath());
			String filePath = jFileChooser.getSelectedFile().getPath();
			textFieldFilePath.setText(filePath);
		}
	}
	
	 void openFile(String fileName)
	    {
	        try
	        {
	            File file = new File(fileName);
	            int size = (int) file.length();
	            int chars_read = 0;
	            FileReader in = new FileReader(file);
	            char[] data = new char[size];
	            while (in.ready())
	            {
	                chars_read += in.read(data, chars_read, size - chars_read);
	      
	            }
	            in.close();
	        } catch (IOException e)
	        {
	            System.out.println(e.toString());
	        }
	    }
	 
	 //exit
	 public void ExitActionPerformed(ActionEvent event)
	 {
		 this.dispose();
	 }
	 
	// upgrade
	public void UpGradeActionPerformed(ActionEvent event) {
		NativeLong lUpgradeHandle = new NativeLong(-1);
		File fileUpgrade = new File(textFieldFilePath.getText());
		if (!fileUpgrade.canRead()) {
			DialogMessage dlg = new DialogMessage("error file");
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			JOptionPane.showMessageDialog(this, "无效文件");
			return;
		}
		if (fileUpgrade.length() == 0) {
			DialogMessage dlg = new DialogMessage("file is null");
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		int iType = comboBoxUpGradeType.getSelectedIndex();
		switch (iType) {
		case 0:// DVR
			lUpgradeHandle = hCNetSDK.NET_DVR_Upgrade(m_lUserID, textFieldFilePath.getText());
			break;
		case 1: // adapter
			lUpgradeHandle = hCNetSDK.NET_DVR_AdapterUpgrade(m_lUserID, textFieldFilePath.getText());
			break;
		case 2: // vca lib
			int iChannel = comboBoxChan.getSelectedIndex() + 1;
			lUpgradeHandle = hCNetSDK.NET_DVR_VcalibUpgrade(m_lUserID, new NativeLong(iChannel),
					textFieldFilePath.getText());
			break;
		case 3:
			int iDevNo = comboBoxChan.getSelectedIndex();
			IntByReference intRef = new IntByReference(iDevNo);
			Pointer lpPointer = intRef.getPointer();
			lUpgradeHandle = hCNetSDK.NET_DVR_Upgrade_V40(m_lUserID, ENUM_UPGRADE_TYPE.ENUM_UPGRADE_ACS,
					textFieldFilePath.getText(), lpPointer, 4);
			break;
		case 4:
			HCNetSDK.NET_DVR_AUXILIARY_DEV_UPGRADE_PARAM struAuxDevParam = new HCNetSDK.NET_DVR_AUXILIARY_DEV_UPGRADE_PARAM();
			struAuxDevParam.dwSize = struAuxDevParam.size();
			// struAuxiliaryDevUpgradeParam.byDevType = 0;
			struAuxDevParam.byDevType = (byte) comboBoxAuxDev.getSelectedIndex();
			struAuxDevParam.dwDevNo = comboBoxChan.getSelectedIndex();
			struAuxDevParam.write();
			Pointer lpAuxDevParam = struAuxDevParam.getPointer();
			lUpgradeHandle = hCNetSDK.NET_DVR_Upgrade_V40(m_lUserID, ENUM_UPGRADE_TYPE.ENUM_UPGRADE_AUXILIARY_DEV,
					textFieldFilePath.getText(), lpAuxDevParam, struAuxDevParam.size());
			struAuxDevParam.read();
			break;
		case 5:
			int iCardType = comboBoxChan.getSelectedIndex() + 1;
			IntByReference intRefe = new IntByReference(iCardType);
			Pointer lpPoint = intRefe.getPointer();
			lUpgradeHandle = hCNetSDK.NET_DVR_Upgrade_V40(m_lUserID, ENUM_UPGRADE_TYPE.ENUM_UPGRADE_LED,
					textFieldFilePath.getText(), lpPoint, 4);
			break;
		default:
			break;
		}
		if(lUpgradeHandle.intValue() < 0)
		{
			DialogMessage dlg = new DialogMessage("Upgrade failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
		}
		else
		{
			 time = new Timer();
	         time.schedule(new StatusTask(lUpgradeHandle), 0, 500);
		}

	 }
	 
	 public void SelectedChangeActionPerformed(ActionEvent event)
	 {
		 int iUpgradeType = comboBoxUpGradeType.getSelectedIndex();
		 switch(iUpgradeType)
		 {
		 case 2:
			 labelChan.setVisible(true);
			 comboBoxChan.setVisible(true);
			 comboBoxChan.removeAllItems();
			 for(int i=0;i< m_iChanNum;i++)
			 {
				 comboBoxChan.addItem("Chan "+(i+1));
			 }
			 break;
		 case 3:
			 labelChan.setVisible(true);
			 labelChan.setText("Dev");
			 comboBoxChan.setVisible(true);
			 comboBoxChan.removeAllItems();
			 for(int i=0;i<9;i++)
			 {
				 comboBoxChan.addItem("Dev "+i);
			 }
			 break;
		 case 4:
			 labelChan.setVisible(true);
			 labelChan.setText("Dev");
			 comboBoxChan.setVisible(true);
			 comboBoxChan.removeAllItems();
			 for(int i=0;i<32;i++)
			 {
				 comboBoxChan.addItem("Dev "+i);
			 }
			 break;
		 case 5:
			 labelChan.setVisible(true);
			 labelChan.setText("Type");
			 comboBoxChan.setVisible(true);
			 comboBoxChan.removeAllItems();
			 comboBoxChan.addItem("Send Card");
			 comboBoxChan.addItem("Rev Card");
			 break;
		default:
			 labelChan.setVisible(false);
			 comboBoxChan.setVisible(false);
			 break;	 
		 }

	 }
	 
	 class StatusTask extends TimerTask
	 {
		 private NativeLong m_lUpgradeHandle;
		 public StatusTask(NativeLong lUpgradeHandle) {
			// TODO Auto-generated constructor stub
			m_lUpgradeHandle = lUpgradeHandle;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			int iUpgradeStatus = hCNetSDK.NET_DVR_GetUpgradeState(m_lUpgradeHandle);
			int iPos = hCNetSDK.NET_DVR_GetUpgradeProgress(m_lUpgradeHandle);
			
			if(iPos > 0)
			{
				progressBar.setValue(iPos);
			}
			if(iUpgradeStatus == 2)
			{
				labelStatus.setText("status:upgrading ,please wait.....");
			}
			else
			{
				switch (iUpgradeStatus) 
				{
				case -1:
					labelStatus.setText("upgrade fail!");					
					break;
				case 1:
					labelStatus.setText("status:upgrade success!");
					progressBar.setValue(100);
					break;
				case 3:
					labelStatus.setText("status:upgrade fail");
					break;
				case 4:
					labelStatus.setText("status:network disconnect,status unknow");
					break; 
				case 5:
					labelStatus.setText("status:Upgrade file language mismatch");
					break;
				default:
					break;
				}
				if(!hCNetSDK.NET_DVR_CloseUpgradeHandle(m_lUpgradeHandle))
				{
					DialogMessage dlg = new DialogMessage("NET_DVR_CloseUpgradeHandle failed,error code:"+hCNetSDK.NET_DVR_GetLastError());
					dlg.setBounds(0,0 , 370, 200);
					dlg.setVisible(true);
				}
				m_lUpgradeHandle=new NativeLong(-1);
				time.cancel();
			}
		}
		 
	 }
}
