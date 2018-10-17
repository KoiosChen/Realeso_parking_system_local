package test;

import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

import com.sun.corba.se.impl.interceptors.InterceptorInvoker;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.IntByReference;

import jdk.internal.org.objectweb.asm.tree.IntInsnNode;

import javax.swing.JComboBox;
import javax.swing.DefaultComboBoxModel;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class DialogListening extends JDialog
{

	private final JPanel contentPanel = new JPanel();
	private JComboBox comboBoxIP;
	private JTextField textFieldPort;
	private JComboBox comboBoxIPType;
	
	
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	FMSGCallBack fMSFCallBack;// alarm callback
	private AlarmThread paAlarmThread;
	private String m_ip;
	private JTextField textFieldIPV6;


	/**
	 * Create the dialog.
	 */
	public DialogListening(String ip)
	{
		InitComponent();
		m_ip = ip;
		GetHostIP();
		setModal(true);
	}
	
	public void InitComponent()
	{
		setBounds(100, 100, 380, 300);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);
		{
			comboBoxIPType = new JComboBox();
			comboBoxIPType.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					actionPerformedIPChange(e);
				}
			});
			comboBoxIPType.setModel(new DefaultComboBoxModel(new String[] { "IPV4", "IPV6" }));
			comboBoxIPType.setBounds(33, 20, 71, 21);
			contentPanel.add(comboBoxIPType);
		}

		JLabel lblNewLabel = new JLabel("Binding IP");
		lblNewLabel.setBounds(138, 23, 54, 15);
		contentPanel.add(lblNewLabel);

		comboBoxIP = new JComboBox();
		comboBoxIP.setBounds(185, 20, 115, 21);
		contentPanel.add(comboBoxIP);

		JButton btnNewButton = new JButton("Refresh IP");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				actionPerformedRefresh(e);
			}
		});
		btnNewButton.setBounds(33, 74, 104, 23);
		contentPanel.add(btnNewButton);

		JButton btnNewButton_1 = new JButton("Binding IP");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				actionPerformedBind(e);
			}
		});
		btnNewButton_1.setBounds(185, 74, 93, 23);
		contentPanel.add(btnNewButton_1);

		JLabel lblNewLabel_1 = new JLabel("Listening Port");
		lblNewLabel_1.setBounds(40, 149, 54, 15);
		contentPanel.add(lblNewLabel_1);

		textFieldPort = new JTextField();
		textFieldPort.setText("7200");
		textFieldPort.setBounds(126, 146, 66, 21);
		contentPanel.add(textFieldPort);
		textFieldPort.setColumns(10);

		JButton btnNewButton_2 = new JButton("Start Listening");
		btnNewButton_2.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				actionPerformedListeing(e);
			}
		});
		btnNewButton_2.setBounds(40, 207, 93, 23);
		contentPanel.add(btnNewButton_2);

		JButton btnNewButton_3 = new JButton("Stop Listening");
		btnNewButton_3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				actionPerformedStop(e);
			}
		});
		btnNewButton_3.setBounds(207, 207, 93, 23);
		contentPanel.add(btnNewButton_3);
		
		textFieldIPV6 = new JTextField();
		textFieldIPV6.setBounds(190, 20, 66, 21);
		contentPanel.add(textFieldIPV6);
		textFieldIPV6.setColumns(10);
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
		}
	}

	public void actionPerformedListeing(ActionEvent e)
	{
		DeviceInfo deviceInfo = JavaDemo.m_hashDeviceInfo.get(m_ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		String sIP = "";
		if(comboBoxIPType.getSelectedIndex() == 0)
		{
			sIP = (String)comboBoxIP.getSelectedItem();
			System.out.println("sip="+sIP);
		}
		else
		{
			sIP = textFieldIPV6.getText();
		}

		if (deviceInfo.GetListenHandle().intValue() == -1)
		{
			if (fMSFCallBack == null)
			{
				fMSFCallBack = new FMSGCallBack();
			}
			Pointer pUser = null;
			if (!hCNetSDK.NET_DVR_SetDVRMessageCallBack_V30(fMSFCallBack, pUser))
			{
				DialogMessage dlg = new DialogMessage("Set callback fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
				dlg.setVisible(true);
				return;
			}
			paAlarmThread = AlarmThread.getInstance();
			if(!paAlarmThread.GetStart())
			{
				paAlarmThread.start();
				paAlarmThread.SetStart(true);
			}
			int iPort = Integer.parseInt(textFieldPort.getText());
			NativeLong lListenHandle = hCNetSDK.NET_DVR_StartListen_V30(sIP, (short) iPort, fMSFCallBack, null);
			deviceInfo.SetListenHandle(lListenHandle);
			if (lListenHandle.intValue() == -1)
			{
				DialogMessage dlg = new DialogMessage("Starting listening fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
				dlg.setVisible(true);
				return;
			}
		}

	}
	
	public void actionPerformedStop(ActionEvent event)
	{
		DeviceInfo deviceInfo = JavaDemo.m_hashDeviceInfo.get(m_ip);
		if (deviceInfo.GetListenHandle().intValue() != -1)
        {
            if (!hCNetSDK.NET_DVR_StopListen_V30(deviceInfo.GetListenHandle()))
            {
            	DialogMessage dlg = new DialogMessage("Stop listening fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
				dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
				dlg.setVisible(true);
                deviceInfo.SetListenHandle(new NativeLong(-1)); 
                return;
            }
            else
            {
            	deviceInfo.SetListenHandle(new NativeLong(-1));                    
            }
        }
	}
	
	public void actionPerformedRefresh(ActionEvent event)
	{
		GetHostIP();
	}
	
	public void GetHostIP()
	{
		byte[]sAllIP = new byte[16*16];
		IntByReference ptrdwIpNum = new IntByReference(0);
		HCNetSDK.Bind b = new HCNetSDK.Bind();
		b.write();
		Pointer ptr = b.getPointer();		
		ByteByReference ptrbBind = new ByteByReference((byte)0);
		if(!hCNetSDK.NET_DVR_GetLocalIP(sAllIP, ptrdwIpNum, ptrbBind))
		{
			DialogMessage dlg = new DialogMessage("get ip fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		b.read();
		
		System.out.println("num="+ptrdwIpNum.getValue());
		comboBoxIP.removeAllItems();
		for(int i=0;i<ptrdwIpNum.getValue();i++)
		{
			String str = new String(sAllIP, i*16, 16);
			comboBoxIP.addItem(str);
		}
		comboBoxIP.setSelectedIndex(0);
	}
	
	public void actionPerformedBind(ActionEvent event)
	{
		int index = comboBoxIP.getSelectedIndex();
		if(!hCNetSDK.NET_DVR_SetValidIP(index, true))
		{
			DialogMessage dlg = new DialogMessage("binding ip fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
		}
	}
	
	public void actionPerformedIPChange(ActionEvent event)
	{
		if(comboBoxIPType.getSelectedIndex() == 0)
		{
			comboBoxIP.setVisible(true);
			textFieldIPV6.setVisible(false);
		}
		else
		{
			textFieldIPV6.setVisible(true);
			comboBoxIP.setVisible(false);
		}
	}

	public class FMSGCallBack implements HCNetSDK.FMSGCallBack
	{
		// alarm info call back function

		public void invoke(NativeLong lCommand, HCNetSDK.NET_DVR_ALARMER pAlarmer, HCNetSDK.RECV_ALARM pAlarmInfo,
				int dwBufLen, Pointer pUser)
		{
			QueueMessage queueMessage = new QueueMessage(pAlarmInfo, lCommand, pAlarmer);
			paAlarmThread.AddMessage(queueMessage);
			System.out.println("------------1");
		}
	}
	
	
}
