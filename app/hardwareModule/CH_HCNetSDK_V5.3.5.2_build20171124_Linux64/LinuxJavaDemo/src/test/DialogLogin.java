package test;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;

@SuppressWarnings("serial")
public class DialogLogin extends JDialog
{
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private int m_iDeviceIndex ;
	
	
	private JTextField textFieldIP;
	private JTextField textFieldPort;
	private JTextField textFieldUserName;
	private JPasswordField passwordFieldPwd;
	
	HCNetSDK.NET_DVR_USER_LOGIN_INFO struLoginInfo ;
	HCNetSDK.NET_DVR_DEVICEINFO_V40 struDeviceInfo;
	
	public DialogLogin() 
	{
		m_iDeviceIndex = -1;
		InitComponent();
		setModal(true);
	}
	
	public void SetDeivceIndex(int i)
	{
		m_iDeviceIndex = i;
	}
	
	public void InitComponent()
	{
		setTitle("Add Device");
		getContentPane().setLayout(null);
		
		JLabel lblNewLabel = new JLabel("IP:");
		lblNewLabel.setBounds(28, 37, 54, 15);
		getContentPane().add(lblNewLabel);
		
		textFieldIP = new JTextField();
		textFieldIP.setText("10.17.133.41");
		textFieldIP.setBounds(92, 34, 89, 21);
		getContentPane().add(textFieldIP);
		textFieldIP.setColumns(10);
		
		JLabel lblNewLabel_1 = new JLabel("Port:");
		lblNewLabel_1.setBounds(202, 37, 54, 15);
		getContentPane().add(lblNewLabel_1);
		
		textFieldPort = new JTextField();
		textFieldPort.setText("8000");
		textFieldPort.setBounds(281, 34, 89, 21);
		getContentPane().add(textFieldPort);
		textFieldPort.setColumns(10);
		
		JLabel lblNewLabel_2 = new JLabel("UserName:");
		lblNewLabel_2.setBounds(28, 122, 54, 15);
		getContentPane().add(lblNewLabel_2);
		
		textFieldUserName = new JTextField();
		textFieldUserName.setText("admin");
		textFieldUserName.setBounds(93, 119, 88, 21);
		getContentPane().add(textFieldUserName);
		textFieldUserName.setColumns(10);
		
		JLabel lblNewLabel_3 = new JLabel("Password:");
		lblNewLabel_3.setBounds(202, 122, 54, 15);
		getContentPane().add(lblNewLabel_3);
		
		JButton btnNewButton = new JButton("Login");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				LoginActionPerformed(arg0);
				
			}
		});
		btnNewButton.setBounds(86, 200, 78, 23);
		getContentPane().add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("Cancel");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				ExitActionPerformed(e);
			}
		});
		btnNewButton_1.setBounds(233, 200, 78, 23);
		getContentPane().add(btnNewButton_1);
		
		passwordFieldPwd = new JPasswordField();
		passwordFieldPwd.setText("12345");
		passwordFieldPwd.setBounds(281, 119, 89, 21);
		getContentPane().add(passwordFieldPwd);
	}
	
	void LoginActionPerformed(ActionEvent event)
	{
		
		String sDeviceIP = textFieldIP.getText();
		if( textFieldPort.getText() == null || textFieldPort.getText().trim().length() == 0)
		{
			DialogMessage dlg = new DialogMessage("Port must be not null");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			
			return;
		}
		int iPort = Integer.parseInt(textFieldPort.getText());
		
		struLoginInfo = new HCNetSDK.NET_DVR_USER_LOGIN_INFO();
		struDeviceInfo = new HCNetSDK.NET_DVR_DEVICEINFO_V40();
		Pointer PointerstruDeviceInfoV40 = struDeviceInfo.getPointer();
		Pointer PointerstruLoginInfo = struLoginInfo.getPointer();
		
		for(int i=0;i<sDeviceIP.length();i++)
		{
			struLoginInfo.sDeviceAddress[i]= (byte)sDeviceIP.charAt(i);
		}
		for(int i=0;i<passwordFieldPwd.getPassword().length;i++)
		{
			struLoginInfo.sPassword[i]=(byte) passwordFieldPwd.getPassword()[i];
		}
		for(int i=0;i<textFieldUserName.getText().length();i++)
		{
			struLoginInfo.sUserName[i]=(byte)textFieldUserName.getText().charAt(i);
		}		
		struLoginInfo.wPort = (short)iPort;	
		struLoginInfo.write();
		System.out.println("NET_DVR_Login_V40 before");
		try {
			//Thread.sleep(1000);
		NativeLong nlUserID = hCNetSDK.NET_DVR_Login_V40(PointerstruLoginInfo, PointerstruDeviceInfoV40);
		System.out.println("NET_DVR_Login_V40 after");
		
		long lUserID = nlUserID.longValue();
		if (lUserID == -1)
		{
			sDeviceIP = "";
			int ierr = hCNetSDK.NET_DVR_GetLastError();
			DialogMessage dlg = new DialogMessage("login error,error code：" + ierr);
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
		} 
		else
		{
			struDeviceInfo.read();
			//Thread.sleep(300);
			DeviceInfo deviceInfo=new DeviceInfo(sDeviceIP, (short)iPort, textFieldUserName.getText(),
					new String(passwordFieldPwd.getPassword()), struDeviceInfo.struDeviceV30, nlUserID);
			int iRet = JavaDemo.TreeAddDevice(nlUserID,sDeviceIP,struDeviceInfo,deviceInfo);
			if(iRet == -1)
			{
				DialogMessage dlg = new DialogMessage("device already login" );
				dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
				dlg.setVisible(true);
			}
			this.dispose();
		
		}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	void ExitActionPerformed(ActionEvent event)
	{
		this.dispose();
	}
}
