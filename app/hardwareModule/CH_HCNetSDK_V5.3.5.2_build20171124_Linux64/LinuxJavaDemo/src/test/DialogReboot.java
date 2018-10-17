package test;

import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

import com.sun.jna.NativeLong;

import javax.swing.JLabel;
import java.awt.Font;
import java.awt.Color;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class DialogReboot extends JDialog
{

	private final JPanel contentPanel = new JPanel();
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	private NativeLong m_lUserID;



	/**
	 * Create the dialog.
	 */
	public DialogReboot( NativeLong lUserId)
	{
		InitComponent();
		m_lUserID = lUserId;
		
	}
	
	public void InitComponent()
	{
		setBounds(100, 100, 414, 235);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);
		{
			JLabel lblNewLabel = new JLabel("Are you sure to reboot?");
			lblNewLabel.setBounds(145, 36, 189, 43);
			contentPanel.add(lblNewLabel);
		}
		
		JLabel lblNewLabel_1 = new JLabel("?");
		lblNewLabel_1.setForeground(Color.RED);
		lblNewLabel_1.setBackground(Color.RED);
		lblNewLabel_1.setFont(new Font("宋体", Font.PLAIN, 20));
		lblNewLabel_1.setBounds(111, 34, 24, 43);
		contentPanel.add(lblNewLabel_1);
		{
			JButton okButton = new JButton("OK");
			okButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					RebootActionPerformed(e);
				}
			});
			okButton.setBounds(111, 114, 57, 23);
			contentPanel.add(okButton);
			okButton.setActionCommand("OK");
			getRootPane().setDefaultButton(okButton);
		}
		{
			JButton cancelButton = new JButton("Cancel");
			cancelButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					ExitActionPerformed(e);
				}
			});
			cancelButton.setBounds(220, 114, 69, 23);
			contentPanel.add(cancelButton);
			cancelButton.setActionCommand("Cancel");
		}
		{
			JPanel buttonPane = new JPanel();
			buttonPane.setLayout(new FlowLayout(FlowLayout.RIGHT));
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
		}
	}
	
	public void RebootActionPerformed(ActionEvent event)
	{
		if (!hCNetSDK.NET_DVR_RebootDVR(m_lUserID))
		{
			DialogMessage dlg = new DialogMessage("Reboot fail,error code="+ hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}
	
	public void ExitActionPerformed(ActionEvent event)
	{
		this.dispose();
	}
}
