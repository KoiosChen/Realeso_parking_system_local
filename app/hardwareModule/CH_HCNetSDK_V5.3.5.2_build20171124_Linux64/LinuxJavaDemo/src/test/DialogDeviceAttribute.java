package test;

import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;

@SuppressWarnings("serial")
public class DialogDeviceAttribute extends JDialog
{
	private JTextField textFieldIP;
	private JTextField textFieldPort;
	private JTextField textFieldUserName;
	private JTextField textFieldPassword;
	private JTextField textFieldDeviceType;
	private JTextField textFieldSerialNum;
	private JTextField textFieldChannelNum;

	public DialogDeviceAttribute(DeviceInfo deviceInfo)
	{
		setTitle("Device Attribute");
		InitialDialog();
		ShowMessage(deviceInfo);
		setModal(true);
		
	}
	private void InitialDialog()
	{
		getContentPane().setLayout(null);

		JLabel label = new JLabel("IP:");
		label.setBounds(23, 13, 54, 15);
		getContentPane().add(label);

		textFieldIP = new JTextField();
		textFieldIP.setColumns(10);
		textFieldIP.setBounds(92, 10, 106, 21);
		getContentPane().add(textFieldIP);

		JLabel label_1 = new JLabel("Port：");
		label_1.setBounds(219, 13, 54, 15);
		getContentPane().add(label_1);

		textFieldPort = new JTextField();
		textFieldPort.setColumns(10);
		textFieldPort.setBounds(292, 10, 98, 21);
		getContentPane().add(textFieldPort);

		JLabel label_2 = new JLabel("UserName：");
		label_2.setBounds(23, 64, 54, 15);
		getContentPane().add(label_2);

		textFieldUserName = new JTextField();
		textFieldUserName.setColumns(10);
		textFieldUserName.setBounds(92, 61, 106, 21);
		getContentPane().add(textFieldUserName);

		JLabel label_3 = new JLabel("Password：");
		label_3.setBounds(219, 64, 54, 15);
		getContentPane().add(label_3);

		textFieldPassword = new JTextField();
		textFieldPassword.setColumns(10);
		textFieldPassword.setBounds(292, 61, 98, 21);
		getContentPane().add(textFieldPassword);

		JLabel label_4 = new JLabel("Device Type");
		label_4.setBounds(23, 115, 54, 15);
		getContentPane().add(label_4);

		textFieldDeviceType = new JTextField();
		textFieldDeviceType.setColumns(10);
		textFieldDeviceType.setBounds(92, 112, 106, 21);
		getContentPane().add(textFieldDeviceType);

		JLabel label_5 = new JLabel("Serial NO：");
		label_5.setBounds(23, 167, 63, 15);
		getContentPane().add(label_5);

		textFieldSerialNum = new JTextField();
		textFieldSerialNum.setColumns(10);
		textFieldSerialNum.setBounds(92, 164, 298, 21);
		getContentPane().add(textFieldSerialNum);

		JLabel label_6 = new JLabel("Channel NUM：");
		label_6.setBounds(219, 115, 54, 15);
		getContentPane().add(label_6);

		textFieldChannelNum = new JTextField();
		textFieldChannelNum.setColumns(10);
		textFieldChannelNum.setBounds(292, 112, 98, 21);
		getContentPane().add(textFieldChannelNum);
	}
	
	private void ShowMessage(DeviceInfo deviceInfo)
	{
		textFieldIP.setText(deviceInfo.GetIP());
		textFieldPort.setText(Short.toString(deviceInfo.GetPort()));
		textFieldUserName.setText(deviceInfo.GetUserName());
		textFieldPassword.setText(deviceInfo.GetPassword());
		textFieldDeviceType.setText(Byte.toString(deviceInfo.GetlpDeviceInfo().byDVRType));
		textFieldSerialNum.setText(new String(deviceInfo.GetlpDeviceInfo().sSerialNumber).trim());
		textFieldChannelNum.setText(Byte.toString(deviceInfo.GetlpDeviceInfo().byChanNum));
	}

}
