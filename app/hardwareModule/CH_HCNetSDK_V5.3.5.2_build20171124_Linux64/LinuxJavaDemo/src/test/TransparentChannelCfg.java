package test;

import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.event.ItemEvent;

public class TransparentChannelCfg extends JDialog {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private NativeLong m_lUserID;
	private NativeLong m_lSerialHandle = new NativeLong(-1);
	
	private JTextField m_textFieldSerialNumber;
	private JTextField m_textFieldSendData;
	private JTable m_tableReceiveContent;
	JComboBox<String> m_cmbChannelNumber;
	private String[] m_sChannelNumber_232 = {"Serial1", "Serial2"};
	private String[] m_sChannelNumber_485 = {"Channel1"};
	private HCNetSDK.NET_DVR_SERIALSTART_V40 m_sSerialStart_V40 = new HCNetSDK.NET_DVR_SERIALSTART_V40();
	private int m_iSelectSendCommand;

	/**
	 * Create the panel.
	 */
	public TransparentChannelCfg(NativeLong lUserID) {
		setTitle("Serial Trans");
		getContentPane().setLayout(null);
		m_lUserID = lUserID;
		
		this.addWindowListener(new WindowAdapter()
        {
           @Override
            public void windowClosing(WindowEvent e)
            {
        	   if (-1 == m_lSerialHandle.longValue())
				{
					return;
				}
				if (false == hCNetSDK.NET_DVR_SerialStop(m_lSerialHandle))
				{
					int iErr = hCNetSDK.NET_DVR_GetLastError();
					DialogMessage dlg = new DialogMessage("stop direct transmission failed,error code is: "+iErr);
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				else
				{
					m_lSerialHandle = new NativeLong(-1);
				}
            }
        });
		
		JLabel lblSerialType = new JLabel("Serial Type");
		lblSerialType.setHorizontalAlignment(SwingConstants.CENTER);
		lblSerialType.setBounds(37, 24, 99, 15);
		getContentPane().add(lblSerialType);
		
		JLabel lblChannelNumber = new JLabel("Channel Number");
		lblChannelNumber.setHorizontalAlignment(SwingConstants.CENTER);
		lblChannelNumber.setBounds(238, 24, 99, 15);
		getContentPane().add(lblChannelNumber);
		
		JLabel lblSerialNumber = new JLabel("Serial Number");
		lblSerialNumber.setHorizontalAlignment(SwingConstants.CENTER);
		lblSerialNumber.setBounds(450, 24, 99, 15);
		getContentPane().add(lblSerialNumber);
		
		final JComboBox<String> cmbSerialType = new JComboBox<String>();
		cmbSerialType.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent arg0) {
				String strSerialType = (String)cmbSerialType.getSelectedItem();
				switch (strSerialType) {
				case "232":
					m_sSerialStart_V40.read();
					m_sSerialStart_V40.dwSerialType = 1;
					m_sSerialStart_V40.write();
					m_cmbChannelNumber.setModel(new DefaultComboBoxModel<String>(m_sChannelNumber_232));
					break;
				case "485":
					m_sSerialStart_V40.read();
					m_sSerialStart_V40.dwSerialType = 2;
					m_sSerialStart_V40.write();
					m_cmbChannelNumber.setModel(new DefaultComboBoxModel<String>(m_sChannelNumber_485));
					break;
				default:
					DialogMessage dlg = new DialogMessage("select send command failed!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					break;
				}
			}
		});
		String[] sSerialType = {"232", "485"};
		cmbSerialType.setModel(new DefaultComboBoxModel<String>(sSerialType));
		cmbSerialType.setSelectedItem(sSerialType[0]);
		cmbSerialType.setBounds(135, 21, 93, 21);
		getContentPane().add(cmbSerialType);
		
		m_cmbChannelNumber = new JComboBox<String>();
		String[] sChannelNumber = {"Serial1", "Serial2"};
		m_cmbChannelNumber.setSelectedItem(sChannelNumber[0]);
		m_cmbChannelNumber.setModel(new DefaultComboBoxModel<String>(sChannelNumber));
		m_cmbChannelNumber.setBounds(347, 21, 93, 21);
		getContentPane().add(m_cmbChannelNumber);
		
		m_textFieldSerialNumber = new JTextField();
		m_textFieldSerialNumber.setText("0");
		m_textFieldSerialNumber.setColumns(10);
		m_textFieldSerialNumber.setBounds(552, 21, 86, 21);
		getContentPane().add(m_textFieldSerialNumber);
		
		JLabel lblSendData = new JLabel("Send Data");
		lblSendData.setHorizontalAlignment(SwingConstants.CENTER);
		lblSendData.setBounds(37, 72, 99, 15);
		getContentPane().add(lblSendData);
		
		JLabel lblSendInterface = new JLabel("Send Interface");
		lblSendInterface.setHorizontalAlignment(SwingConstants.CENTER);
		lblSendInterface.setBounds(37, 116, 99, 15);
		getContentPane().add(lblSendInterface);
		
		m_textFieldSendData = new JTextField();
		m_textFieldSendData.setColumns(10);
		m_textFieldSendData.setBounds(135, 69, 503, 21);
		getContentPane().add(m_textFieldSendData);
		
		m_iSelectSendCommand = 1;
		final JComboBox<String> cmbSendInterface = new JComboBox<String>();
		cmbSendInterface.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent arg0) {
				String strSendInterface = (String)cmbSendInterface.getSelectedItem();
				switch (strSendInterface) {
				case "NET_DVR_SerialSend(Transparent)":
					m_iSelectSendCommand = 1;
					break;
				case "NET_DVR_SendToSerialPort(direct)":
					m_iSelectSendCommand = 2;
					break;
				case "NET_DVR_SendTo232Port(direct)":
					m_iSelectSendCommand = 3;
					break;
				default:
					DialogMessage dlg = new DialogMessage("select send command failed!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					break;
				}
			}
		});
		String[] sSendInterface = {"NET_DVR_SerialSend(Transparent)", "NET_DVR_SendToSerialPort(direct)", "NET_DVR_SendTo232Port(direct)"};
		cmbSendInterface.setSelectedItem(sSendInterface[0]);
		cmbSendInterface.setModel(new DefaultComboBoxModel<String>(sSendInterface));
		cmbSendInterface.setBounds(135, 113, 250, 21);
		getContentPane().add(cmbSendInterface);
		
		JButton btnSend = new JButton("Send");
		btnSend.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				String sSendData = m_textFieldSendData.getText();
				if (sSendData.length() > 1016)
				{
					DialogMessage dlg = new DialogMessage("please send data less than 1016!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				DialogMessage dlg;
				switch (m_iSelectSendCommand) {
				case 1:
					if (-1 == m_lSerialHandle.longValue())
					{
						dlg = new DialogMessage("please start direct transmission first!");
						dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
						dlg.setVisible(true);
						return;
					}
					if (false == hCNetSDK.NET_DVR_SerialSend(m_lSerialHandle, new NativeLong(1), sSendData, sSendData.length()))
					{
						int iErr = hCNetSDK.NET_DVR_GetLastError();
						dlg = new DialogMessage("send data failed,error code is: "+iErr);
						dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
						dlg.setVisible(true);
						return;
					}
					break;
				case 2:
					if (false == hCNetSDK.NET_DVR_SendToSerialPort(m_lUserID, 1, 1, sSendData, sSendData.length()))
					{
						int iErr = hCNetSDK.NET_DVR_GetLastError();
						dlg = new DialogMessage("send data failed,error code is: "+iErr);
						dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
						dlg.setVisible(true);
						return;
					}
					break;
				case 3:
					if (false == hCNetSDK.NET_DVR_SendTo232Port(m_lUserID, sSendData, sSendData.length()))
					{
						int iErr = hCNetSDK.NET_DVR_GetLastError();
						dlg = new DialogMessage("send data failed,error code is: "+iErr);
						dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
						dlg.setVisible(true);
						return;
					}
					break;
				default:
					dlg = new DialogMessage("select send data command failed!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					break;
				}
				dlg = new DialogMessage("send data succeed!");
				dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
				dlg.setVisible(true);
			}
		});
		btnSend.setBounds(527, 112, 111, 23);
		getContentPane().add(btnSend);
		
		JLabel lblReceiveContent = new JLabel("Receive Content");
		lblReceiveContent.setHorizontalAlignment(SwingConstants.CENTER);
		lblReceiveContent.setBounds(37, 159, 99, 15);
		getContentPane().add(lblReceiveContent);
		
		m_tableReceiveContent = new JTable();
		m_tableReceiveContent.setModel(new DefaultTableModel(
			new Object[][] {
				{"Time", "Content", "Hex"},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
				{null, null, null},
			},
			new String[] {
				"Time", "Content", "Hex"
			}
		));
		m_tableReceiveContent.setBounds(63, 201, 592, 144);
		getContentPane().add(m_tableReceiveContent);
		
		JButton btnBuildChannel = new JButton("Build Channel");
		m_sSerialStart_V40.dwSerialType = 1;	// 1-232; 2-485
		btnBuildChannel.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				if (-1 != m_lSerialHandle.longValue())
				{
					DialogMessage dlg = new DialogMessage("please stop direct transmission first!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				long lUserID = m_lUserID.longValue();
				if (-1 == lUserID)
				{
					DialogMessage dlg = new DialogMessage("please login at first!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				m_sSerialStart_V40.dwSize = m_sSerialStart_V40.size();
				//m_sSerialStart_V40.dwSerialType = 1;	// 1-232; 2-485
				m_sSerialStart_V40.bySerialNum = 0;
				m_sSerialStart_V40.write();
				Pointer lpSerialStart_V40 = m_sSerialStart_V40.getPointer();
				m_lSerialHandle = hCNetSDK.NET_DVR_SerialStart_V40(m_lUserID, lpSerialStart_V40, new NativeLong(m_sSerialStart_V40.size()), null, null);
				m_sSerialStart_V40.read();
				long lSerialHandle = m_lSerialHandle.longValue();
				if (-1 == lSerialHandle)
				{
					int iErr = hCNetSDK.NET_DVR_GetLastError();
					DialogMessage dlg = new DialogMessage("start direct transmission failed,error code is: "+iErr);
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				else
				{
					DialogMessage dlg = new DialogMessage("start direct transmission succeed!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
				}
			}
		});
		btnBuildChannel.setBounds(406, 378, 111, 23);
		getContentPane().add(btnBuildChannel);
		
		JButton btnStopChannel = new JButton("Stop Channel");
		btnStopChannel.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				if (-1 == m_lSerialHandle.longValue())
				{
					DialogMessage dlg = new DialogMessage("direct transmission has been stopped!");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				if (false == hCNetSDK.NET_DVR_SerialStop(m_lSerialHandle))
				{
					int iErr = hCNetSDK.NET_DVR_GetLastError();
					DialogMessage dlg = new DialogMessage("stop direct transmission failed,error code is: "+iErr);
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
					return;
				}
				else
				{
					m_lSerialHandle = new NativeLong(-1);
					DialogMessage dlg = new DialogMessage("stop direct transmission succeed");
					dlg.setBounds(dlg.getX()+dlg.getWidth()/2,dlg.getY()+dlg.getHeight() , 370, 200);
					dlg.setVisible(true);
				}
			}
		});
		btnStopChannel.setBounds(527, 378, 111, 23);
		getContentPane().add(btnStopChannel);
		
		
	}
}
