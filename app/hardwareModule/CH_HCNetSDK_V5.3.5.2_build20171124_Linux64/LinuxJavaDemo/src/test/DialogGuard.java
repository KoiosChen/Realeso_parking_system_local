package test;

import java.awt.BorderLayout;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.tree.DefaultTreeModel;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;


import javax.swing.tree.DefaultMutableTreeNode;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.Font;

public class DialogGuard extends JDialog
{

	private final JPanel contentPanel = new JPanel();
	private JTree tree;
	private DefaultMutableTreeNode m_DeviceRoot;
	private AlarmThread paAlarmThread;
	private FMSGCallBack fMSFCallBack;// alarm callback
	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;

	/**
	 * Create the dialog.
	 */
	public DialogGuard()
	{
		InitComponent();
		AddNode();
		setModal(true);
	}

	public void InitComponent()
	{
		setBounds(100, 100, 343, 332);
		getContentPane().setLayout(new BorderLayout());
		contentPanel.setBorder(new EmptyBorder(5, 5, 5, 5));
		getContentPane().add(contentPanel, BorderLayout.CENTER);
		contentPanel.setLayout(null);

		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 10, 302, 138);
		contentPanel.add(scrollPane);

		tree = new JTree();
		scrollPane.setViewportView(tree);
		tree.setModel(InitialTreeModel());

		JButton btnNewButton = new JButton("Guard/DisGuard");
		btnNewButton.setFont(new Font("宋体", Font.PLAIN, 10));
		btnNewButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				actionPerformedGuard(e);
			}
		});
		btnNewButton.setBounds(21, 198, 117, 23);
		contentPanel.add(btnNewButton);

		JButton btnNewButton_1 = new JButton("Exit");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				actionPerformedExit(e);
			}
		});
		btnNewButton_1.setBounds(177, 198, 93, 23);
		contentPanel.add(btnNewButton_1);
		tree.addMouseListener(new CheckBoxTreeNodeSelectionListener());

		{
			JPanel buttonPane = new JPanel();
			getContentPane().add(buttonPane, BorderLayout.SOUTH);
			buttonPane.setLayout(null);
			{
				JButton okButton = new JButton("OK");
				okButton.setBounds(319, 5, 45, 23);
				okButton.setActionCommand("OK");
				buttonPane.add(okButton);
				getRootPane().setDefaultButton(okButton);
			}
			{
				JButton cancelButton = new JButton("Cancel");
				cancelButton.setBounds(369, 5, 69, 23);
				cancelButton.setActionCommand("Cancel");
				buttonPane.add(cancelButton);
			}
		}
	}

	public DefaultTreeModel InitialTreeModel()
	{
		m_DeviceRoot = new CheckBoxTreeNode("Device Tree");
		DefaultTreeModel checkBoxDefaultTreeModel = new DefaultTreeModel(m_DeviceRoot);
		return checkBoxDefaultTreeModel;
	}

	public void AddNode()
	{

		Iterator iterator = JavaDemo.m_hashDeviceInfo.keySet().iterator(); 
		while (iterator.hasNext())
		{
			String sIp = (String) iterator.next();
			DefaultTreeModel TreeModel = ((DefaultTreeModel) tree.getModel());
			CheckBoxTreeNode TreeIP = new CheckBoxTreeNode(sIp);
			m_DeviceRoot.add(TreeIP);
			TreeModel.reload();
			System.out.println(sIp);
			tree.setCellRenderer(new CheckBoxTreeCellRenderer());
		}
	}

	public void actionPerformedGuard(ActionEvent event)
	{
		for (Enumeration enu = m_DeviceRoot.children(); enu.hasMoreElements();)
		{
			CheckBoxTreeNode node = (CheckBoxTreeNode) enu.nextElement();
			if (node.isSelected)
			{
				String sIP = node.toString();
				DeviceInfo deviceInfo = JavaDemo.m_hashDeviceInfo.get(sIP);
				NativeLong lUserID = deviceInfo.GetNUserID();
				if (lUserID.intValue() == -1)
				{
					DialogMessage dlg = new DialogMessage("Please login");
					dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
					dlg.setVisible(true);
					return;
				}
				if (deviceInfo.GetAlarmHandle().intValue() == -1) 
				{
					if (fMSFCallBack == null)
					{
						fMSFCallBack = new FMSGCallBack();
						Pointer pUser = null;

						if (!hCNetSDK.NET_DVR_SetDVRMessageCallBack_V30(fMSFCallBack, pUser))
						{
							DialogMessage dlg = new DialogMessage("Set callBack fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
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
					}
					NativeLong lAlarmHandle = hCNetSDK.NET_DVR_SetupAlarmChan_V30(lUserID);
					deviceInfo.SetAlarmHandle(lAlarmHandle);
					if (lAlarmHandle.intValue() == -1)
					{
						DialogMessage dlg = new DialogMessage("Guarding fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
						dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
						dlg.setVisible(true);
						return;
					}
				}
				else
				{
					if (deviceInfo.GetAlarmHandle().intValue() != -1)
					{
						if (!hCNetSDK.NET_DVR_CloseAlarmChan_V30(deviceInfo.GetAlarmHandle()))
						{
							DialogMessage dlg = new DialogMessage("Disguarding fail,error code:" + hCNetSDK.NET_DVR_GetLastError());
							dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
							dlg.setVisible(true);
							deviceInfo.SetAlarmHandle(new NativeLong(-1));
							return;
						}
						else
						{
							deviceInfo.SetAlarmHandle(new NativeLong(-1));
						}
					}
				}
			}

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
		}
	}
	
	public void actionPerformedExit(ActionEvent event)
	{
		this.dispose();
	}
}
