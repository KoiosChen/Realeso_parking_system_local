package test;

import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.Panel;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.Iterator;
import java.util.logging.Logger;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.border.EmptyBorder;
import javax.swing.table.DefaultTableModel;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import com.sun.jna.examples.win32.W32API.HWND;
import com.sun.jna.ptr.IntByReference;

import test.HCNetSDK.NET_DVR_DEVICEINFO_V40;
import test.HCNetSDK.NET_DVR_USER_LOGIN_INFO;

public class JavaDemo extends JFrame
{
	protected final Logger log = Logger.getLogger(this.getClass().getName());

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
	public static NativeLong g_lVoiceHandle;// voice talk handle

	static int m_iTreeNodeNum;// tree node num
	static DefaultMutableTreeNode m_DeviceRoot;
	static HashMap<String, DeviceInfo> m_hashDeviceInfo; // login info
	private boolean m_isPlaying; // isPlaying
	private static int m_iChannelNum;// channel num
	private static NativeLong m_lLogID; // login ID
	static Show[] arrShow; // multiple windows play
	static PanelPTZ panelPTZ;

	static NativeLong m_lPreviewHandle;// preview handle
	private AlarmThread paAlarmThread;

	private JPanel contentPane;
	private Panel panelRealPlay = new Panel();
	@SuppressWarnings("rawtypes")
	private static JComboBox comboBoxWindow;
	private JButton btnButtonPlay;
	private JPanel panel;
	private JLabel lblNewLabel_4;
	private Panel panelPlay;
	static JTree treeDevice;
	private static JTable tableAlarm;
	private JScrollPane scrollPane;
	private JMenuItem MenuItemVoice;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args)
	{
		EventQueue.invokeLater(new Runnable()
		{
			public void run()
			{
				try
				{
					hCNetSDK.NET_DVR_Init();
					hCNetSDK.NET_DVR_SetLogToFile(true, null, false);
					JavaDemo frame = new JavaDemo();
					frame.setVisible(true);
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public JavaDemo()
	{
		log.info("java.home:" + System.getProperty("java.home"));
		initComponents();
		m_hashDeviceInfo = new HashMap<String, DeviceInfo>();
		m_lPreviewHandle = new NativeLong(-1);
		m_isPlaying = false;
		g_lVoiceHandle = new NativeLong(-1);
		arrShow = new Show[4];
		arrShow[0] = new Show();
		arrShow[1] = new Show();
		arrShow[2] = new Show();
		arrShow[3] = new Show();
		m_iChannelNum = -1;

	}

	// Init Component
	private void initComponents()
	{
		setTitle("JavaDemo");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 990, 696);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		panelRealPlay.setForeground(Color.YELLOW);
		panelRealPlay.setBounds(176, 116, 566, 452);
		contentPane.setLayout(null);
		contentPane.add(panelRealPlay);
		panelRealPlay.setLayout(null);

		panelPlay = new Panel();
		panelPlay.setBackground(Color.LIGHT_GRAY);
		panelPlay.setBounds(0, 0, 566, 452);

		panelRealPlay.add(panelPlay);

		panel = new JPanel();
		panel.setBounds(20, 35, 722, 64);
		contentPane.add(panel);
		panel.setLayout(null);

		btnButtonPlay = new JButton("Prieview");
		btnButtonPlay.setFocusable(false);
		btnButtonPlay.setBounds(24, 23, 98, 32);
		panel.add(btnButtonPlay);
		btnButtonPlay.setFocusable(false);

		JButton btnNewButton = new JButton("Stop Prievew");
		btnNewButton.setFocusable(false);
		btnNewButton.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				StopPreviewActionPerformed(e);
			}
		});
		btnNewButton.setBounds(132, 23, 105, 32);
		panel.add(btnNewButton);

		lblNewLabel_4 = new JLabel("Window");
		lblNewLabel_4.setBounds(316, 32, 75, 15);
		panel.add(lblNewLabel_4);

		comboBoxWindow = new JComboBox();
		comboBoxWindow.setFocusable(false);
		comboBoxWindow.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				ComboxSelectChangeActionPerformed(e);
			}

		});
		comboBoxWindow.setModel(new DefaultComboBoxModel(new String[] { "1", "4" }));
		comboBoxWindow.setBounds(417, 29, 75, 21);
		panel.add(comboBoxWindow);

		JButton btnNewButton_1 = new JButton("Exit");
		btnNewButton_1.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent arg0)
			{
				ExitActionPerformed(arg0);
			}
		});
		btnNewButton_1.setBounds(562, 25, 75, 28);
		panel.add(btnNewButton_1);

		JMenuBar menuBar = new JMenuBar();
		menuBar.setFont(new Font("Times New Roman", Font.PLAIN, 12));
		menuBar.setBounds(0, 0, 964, 32);
		contentPane.add(menuBar);

		JMenu mnNewMenu = new JMenu("PlackBack");
		menuBar.add(mnNewMenu);

		JMenuItem MenuItemPlayByFile = new JMenuItem("By File");
		MenuItemPlayByFile.addMouseListener(new java.awt.event.MouseAdapter()
		{
			public void mousePressed(java.awt.event.MouseEvent evt)
			{
				MenuItemPlayByFileMousePressed(evt);
			}
		});
		mnNewMenu.add(MenuItemPlayByFile);

		JMenuItem MenuItemPlayByTime = new JMenuItem("By Time");
		MenuItemPlayByTime.addMouseListener(new java.awt.event.MouseAdapter()
		{
			public void mousePressed(java.awt.event.MouseEvent evt)
			{
				MenuItemPlayByTimeMousePressed(evt);
			}
		});
		mnNewMenu.add(MenuItemPlayByTime);

		JMenu mnNewMenu_1 = new JMenu("Alarm");
		menuBar.add(mnNewMenu_1);

		JMenuItem mntmMenuItemClearup = new JMenuItem("Guard/DisGuard");
		mntmMenuItemClearup.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent arg0)
			{
				GuardDisGuardActionPerformed(arg0);
			}
		});
		mnNewMenu_1.add(mntmMenuItemClearup);

		JMenuItem mntmNewMenuItem_4 = new JMenuItem("Listening");
		mntmNewMenuItem_4.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				actionPerformedListening(e);
			}
		});
		mnNewMenu_1.add(mntmNewMenuItem_4);

		JMenu mnNewMenu_2 = new JMenu("Voice");
		menuBar.add(mnNewMenu_2);

		MenuItemVoice = new JMenuItem("Voice Talk");
		MenuItemVoice.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				VoiceActionPerformed(e);
			}
		});
		mnNewMenu_2.add(MenuItemVoice);

		JMenu mnNewMenu_3 = new JMenu("Manager");
		menuBar.add(mnNewMenu_3);

		JMenuItem mntmNewMenuItem = new JMenuItem("Check Time");
		mntmNewMenuItem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				CheckTimeActionPerformed(e);
			}
		});
		mnNewMenu_3.add(mntmNewMenuItem);

		JMenuItem mntmNewMenuItem_1 = new JMenuItem("UpGrade");
		mntmNewMenuItem_1.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				UpgradeActionPerformed(e);
			}
		});
		mnNewMenu_3.add(mntmNewMenuItem_1);

		JMenuItem mntmNewMenuItem_2 = new JMenuItem("Reboot");
		mntmNewMenuItem_2.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				RestartActionPerformed(e);
			}
		});
		mnNewMenu_3.add(mntmNewMenuItem_2);

		JMenuItem mntmNewMenuItem_3 = new JMenuItem("Log Search");
		mntmNewMenuItem_3.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				LogSearchActionPerformed(e);
			}
		});
		mnNewMenu_3.add(mntmNewMenuItem_3);
		
		JMenu mnNewMenu_4 = new JMenu("Configure");
		menuBar.add(mnNewMenu_4);

		JMenuItem mntmMenuItemVideo = new JMenuItem("Video");
		mntmMenuItemVideo.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				VideoConfigureActionPerformed(e);
			}
		});
		mnNewMenu_4.add(mntmMenuItemVideo);

		JMenuItem mntmMenuItemAudio = new JMenuItem("Audio");
		mntmMenuItemAudio.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				AudioConfigureActionPerformed(e);
			}
		});
		mnNewMenu_4.add(mntmMenuItemAudio);

		JMenuItem mntmMenuItemImage = new JMenuItem("Image");
		mntmMenuItemImage.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				ImageConfigureActionPerformed(e);
			}
		});
		mnNewMenu_4.add(mntmMenuItemImage);

		JMenuItem mntmMenuItemSerialTrans = new JMenuItem("Serial Trans");
		mntmMenuItemSerialTrans.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				SerialTransActionPerformed(e);
			}
		});
		mnNewMenu_4.add(mntmMenuItemSerialTrans);

		JScrollPane scrollPaneAlarmList = new JScrollPane();
		scrollPaneAlarmList.setBounds(176, 568, 788, 80);
		contentPane.add(scrollPaneAlarmList);

		tableAlarm = new JTable();
		tableAlarm.setModel(this.initialTableModel());
		scrollPaneAlarmList.setViewportView(tableAlarm);

		scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 116, 164, 532);
		contentPane.add(scrollPane);

		treeDevice = new JTree();
		scrollPane.setViewportView(treeDevice);
		treeDevice.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mouseReleased(MouseEvent arg0)
			{
				RightClickMouse(arg0);
			}

			@Override
			public void mouseClicked(MouseEvent e)
			{
				DoubleClickMouse(e);
			}
		});
		treeDevice.setFont(new Font("宋体", Font.BOLD, 12));
		treeDevice.setBackground(Color.WHITE);
		treeDevice.setModel(initialTreeModel());
		treeDevice.setFocusable(false);

		JPanel panelCloud = new JPanel();
		panelCloud.setFont(new Font("宋体", Font.PLAIN, 12));
		panelCloud.setBounds(748, 116, 216, 452);
		contentPane.add(panelCloud);
		panelCloud.setLayout(null);
		panelPTZ = new PanelPTZ();
		int iHeight = panelCloud.getHeight();
		int iWidth = panelCloud.getWidth();
		panelPTZ.setBounds(0, -8, iWidth, iHeight);
		panelCloud.add(panelPTZ);

		btnButtonPlay.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				StartPlayActionPerformed(e);
			}
		});
	}

	/**
	 * 
	 * @Title ExitActionPerformed
	 * @Description
	 * @param arg0
	 */
	private void ExitActionPerformed(ActionEvent arg0)
	{
		if (comboBoxWindow.getSelectedIndex() == 0)
		{
			if (m_lPreviewHandle.longValue() > -1) // one window preview
			{
				hCNetSDK.NET_DVR_StopRealPlay(m_lPreviewHandle);
			}
		}
		else // 如果四画面的正在预览，停止预览
		{
			for (int i = 0; i < 4; i++)
			{
				arrShow[i].StopPlay();
			}
		}
		@SuppressWarnings("rawtypes")
		Iterator iterator = m_hashDeviceInfo.keySet().iterator(); // exit all device
		while (iterator.hasNext())
		{
			NativeLong lUserID = m_hashDeviceInfo.get(iterator.next()).GetNUserID();
			hCNetSDK.NET_DVR_Logout(lUserID);
		}

		// cleanup SDK
		hCNetSDK.NET_DVR_Cleanup();
		JavaDemo.this.dispose();
	}

	/**
	 * @Title StartPlayActionPerformed
	 * @Description preview
	 * @param e
	 */
	@SuppressWarnings("deprecation")
	private void StartPlayActionPerformed(ActionEvent e)
	{
		String ip = getDeviceIP(); // get ip from device tree
		if (ip.length() == 0 || ip == "Device Tree") // not select ip or select "devicetree" root
		{
			//JOptionPane.showMessageDialog(JavaDemo.this, "Please select channel");
			DialogMessage dlg = new DialogMessage("Please select channel");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		NativeLong lUserID = m_hashDeviceInfo.get(ip).GetNUserID(); // get loginID

		if (lUserID.longValue() == -1)
		{
			//JOptionPane.showMessageDialog(JavaDemo.this, "Please login");
			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}

		int iChannelNum = getChannelNumber();
		if (iChannelNum == -1)
		{
			DialogMessage dlg = new DialogMessage("Please select channel");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		m_iChannelNum = iChannelNum;
		System.out.println(iChannelNum);
		m_lLogID = lUserID;
		HCNetSDK.NET_DVR_PREVIEWINFO strPreviewInfo = new HCNetSDK.NET_DVR_PREVIEWINFO();
		strPreviewInfo.lChannel = new NativeLong(iChannelNum);

		int i = comboBoxWindow.getSelectedIndex();
		if (i == 0) // one window play
		{
			StartPlay(lUserID, strPreviewInfo);
		}
		else // four window play
		{
			boolean bPlay = false;
			for (int j = 0; j < 4; j++)
			{
				if (arrShow[j].GetFocus())
				{
					bPlay = true;
					arrShow[j].StartPlay(lUserID, strPreviewInfo);
				}
			}
			if (bPlay == false)
			{
				arrShow[0].StartPlay(lUserID, strPreviewInfo);
			}
		}

		comboBoxWindow.disable();
	}

	/**
	 * @Title StopPreviewActionPerformed
	 * @Description stop priview
	 * @param e
	 */
	@SuppressWarnings("deprecation")
	private void StopPreviewActionPerformed(ActionEvent e)
	{
		if (comboBoxWindow.getSelectedIndex() == 0)
		{
			if (m_isPlaying)
			{
				StopPlay();
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (arrShow[i].GetFocus())
				{
					arrShow[i].StopPlay();
				}
			}
		}
		comboBoxWindow.enable();
	}

	//start play
	private void StartPlay(NativeLong lUserId, HCNetSDK.NET_DVR_PREVIEWINFO struPreviewInfo)
	{
		if (m_isPlaying)
		{
			StopPlay();
		}
		HWND hwnd = new HWND(Native.getComponentPointer(panelPlay)); // get preview handle
		struPreviewInfo.hPlayWnd = hwnd;
		m_lPreviewHandle = hCNetSDK.NET_DVR_RealPlay_V40(lUserId, struPreviewInfo, null, null);
		if(m_lPreviewHandle.longValue() == -1)
		{
			int iErr = hCNetSDK.NET_DVR_GetLastError();
			System.out.println("realplay err" + iErr);
		}
		m_isPlaying = true;
	}

	//stop play
	private void StopPlay()
	{
		if (m_isPlaying)
		{
			hCNetSDK.NET_DVR_StopRealPlay(m_lPreviewHandle);
			panelPlay.repaint();
			m_isPlaying = false;
		}
	}

	//one window and four window change
	private void ComboxSelectChangeActionPerformed(ActionEvent e)
	{
		if (comboBoxWindow.getSelectedIndex() == 1)
		{
			panelRealPlay.remove(panelPlay);
			int iwidth = panelRealPlay.getWidth();
			int iHeight = panelRealPlay.getHeight();
			iwidth /= 2;
			iHeight /= 2;

			arrShow[0].setBounds(-8, -6, iwidth + 4, iHeight + 3);
			panelRealPlay.add(arrShow[0]);

			arrShow[1].setBounds(iwidth - 10, -6, iwidth + 8, iHeight + 3);
			panelRealPlay.add(arrShow[1]);
			arrShow[2].setBounds(-8, iHeight - 6, iwidth + 4, iHeight + 3);
			panelRealPlay.add(arrShow[2]);
			arrShow[3].setBounds(iwidth - 10, iHeight - 6, iwidth + 8, iHeight + 3);
			panelRealPlay.add(arrShow[3]);

		}
		else
		{
			panelRealPlay.remove(arrShow[0]);
			panelRealPlay.remove(arrShow[1]);
			panelRealPlay.remove(arrShow[2]);
			panelRealPlay.remove(arrShow[3]);
			int iwidth = panelRealPlay.getWidth();
			int iHeight = panelRealPlay.getHeight();
			panelPlay.setBounds(0, 0, iwidth, iHeight);
			panelRealPlay.add(panelPlay);
		}
	}

	//create device tree
	private static void CreateDeviceTree(NativeLong lUserID, String sDeviceIP, HCNetSDK.NET_DVR_DEVICEINFO_V40 strInfo)
	{
		System.out.println("CreateDeviceTree before");
		HCNetSDK.NET_DVR_DEVICEINFO_V30 strDeviceInfo = new HCNetSDK.NET_DVR_DEVICEINFO_V30();
		strDeviceInfo = strInfo.struDeviceV30;
		IntByReference ibrBytesReturned = new IntByReference(0);// get ip param
		boolean bRet = false;

		HCNetSDK.NET_DVR_IPPARACFG strIpparaCfg = new HCNetSDK.NET_DVR_IPPARACFG();
//		strIpparaCfg.write();
		Pointer lpIpParaConfig = strIpparaCfg.getPointer();
		System.out.println("NET_DVR_GET_IPPARACFG before");
//		try {
//			Thread.sleep(1000);
//		} catch (InterruptedException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
		bRet = hCNetSDK.NET_DVR_GetDVRConfig(lUserID, HCNetSDK.NET_DVR_GET_IPPARACFG, new NativeLong(0), lpIpParaConfig,
				strIpparaCfg.size(), ibrBytesReturned);
		strIpparaCfg.read();
		System.out.println("NET_DVR_GET_IPPARACFG after");
		DefaultTreeModel TreeModel = ((DefaultTreeModel) treeDevice.getModel());// get tree model
		MyTreeNode TreeIP = new MyTreeNode(sDeviceIP);
		m_DeviceRoot.add(TreeIP);// add device to tree
		if (!bRet)
		{
			// device not support,has no ip camera
			for (int iChannum = 0; iChannum < strDeviceInfo.byChanNum; iChannum++)
			{
				MyTreeNode newNode = new MyTreeNode("Camera" + (iChannum + strDeviceInfo.byStartChan));
				TreeIP.add(newNode);
			}
		}
		else
		{
			// ip camera
			for (int iChannum = 0; iChannum < strDeviceInfo.byChanNum; iChannum++)
			{
				//if (strIpparaCfg.byAnalogChanEnable[iChannum] == 1)
				{
					MyTreeNode newNode = new MyTreeNode("Camera" + (iChannum + strDeviceInfo.byStartChan));
					//TreeModel.insertNodeInto(newNode, m_DeviceRoot, m_iTreeNodeNum);
					TreeIP.add(newNode);
					m_iTreeNodeNum++;
				}
			}
			for (int iChannum = 0; iChannum < /*HCNetSDK.MAX_IP_CHANNEL*/strDeviceInfo.byIPChanNum; iChannum++)
				//if (strIpparaCfg.struIPChanInfo[iChannum].byEnable == 1)
				{
					MyTreeNode newNode = new MyTreeNode("IPCamera" + (iChannum + strDeviceInfo.byStartChan));
					//TreeModel.insertNodeInto(newNode, m_DeviceRoot, m_iTreeNodeNum);
					TreeIP.add(newNode);
				}
		}
		TreeModel.reload();
		treeDevice.expandPath(new TreePath(TreeIP.getPath()));
		treeDevice.setCellRenderer(new NodeRenderer());
	}

	//init tree model
	private DefaultTreeModel initialTreeModel()
	{
		m_DeviceRoot = new MyTreeNode("Device Tree");

		DefaultTreeModel myDefaultTreeModel = new DefaultTreeModel(m_DeviceRoot);
		return myDefaultTreeModel;
	}

	//getChannelNumber
	private int getChannelNumber()
	{
		int iChannelNum = -1;
		TreePath tp = treeDevice.getSelectionPath();
		if (tp != null)
		{
			String sChannelName = ((DefaultMutableTreeNode) tp.getLastPathComponent()).toString();
			if (sChannelName.charAt(0) == 'C') // Camara
			{
				iChannelNum = Integer.parseInt(sChannelName.substring(6));
			}
			else
			{
				if (sChannelName.charAt(0) == 'I') // IPCamara
				{
					iChannelNum = Integer.parseInt(sChannelName.substring(8));
				}
				else
				{
					return -1;
				}
			}
		}
		else
		{
			return -1;
		}
		return iChannelNum;
	}

	/**
	 * @Title getDeviceIP
	 * @Description 
	 * @return
	 */
	private String getDeviceIP()
	{
		String Ip = "";
		TreePath tp = treeDevice.getSelectionPath();
		if (tp != null)
		{
			DefaultMutableTreeNode leafNode = (DefaultMutableTreeNode) tp.getLastPathComponent();
			if (leafNode.getParent() == null)
			{
				return Ip;
			}
			Ip = (leafNode.getParent()).toString();
		}
		return Ip;
	}

	// right click device tree
	@SuppressWarnings("deprecation")
	private void RightClickMouse(MouseEvent e)
	{

		TreePath path = treeDevice.getPathForLocation(e.getX(), e.getY());
		treeDevice.setSelectionPath(path);
		int n = treeDevice.getRowForLocation(e.getX(), e.getY());
		if (n < 0)
		{
			return;
		}
		if (e.getButton() == MouseEvent.BUTTON3)
		{
			TreePath selTree = treeDevice.getPathForRow(n);
			DefaultMutableTreeNode selNode = (DefaultMutableTreeNode) selTree.getLastPathComponent();

			if (selNode.isLeaf() && selNode.toString() != "Device Tree")
			{
				// leaf node
				JPopupMenu pop = new JPopupMenu();
				JMenuItem menu1 = new JMenuItem();
				JMenuItem menu2 = new JMenuItem();
				menu1.setLabel("JPEG Picture");
				menu2.setLabel("Channel Attribute");
				pop.add(menu1);
				pop.add(menu2);
				pop.show(e.getComponent(), e.getX(), e.getY());
			}
			else if (selNode.toString() == "Device Tree")
			{
				JPopupMenu pop = new JPopupMenu();
				JMenuItem addDevice = new JMenuItem();
				addDevice.setLabel("Add Device");
				addDevice.addActionListener(new ActionListener()
				{
					@Override
					public void actionPerformed(ActionEvent e)
					{
						// TODO Auto-generated method stub
						DialogLogin dlg = new DialogLogin();
						dlg.setBounds(JavaDemo.this.getX() + JavaDemo.this.getWidth() / 4,
								JavaDemo.this.getY() + JavaDemo.this.getHeight() / 4, 440, 300);
						dlg.setVisible(true);
						dlg.setModal(true);
					}
				});
				pop.add(addDevice);
				pop.show(e.getComponent(), e.getX(), e.getY());
			}
			else
			{
				JPopupMenu pop = new JPopupMenu();
				JMenuItem menuLogin = new JMenuItem();
				menuLogin.setLabel("Login");
				menuLogin.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						// TODO Auto-generated method stub
						TreeLoginActionPerformed(e);
					}
				});
				JMenuItem menuLogout = new JMenuItem();
				menuLogout.setLabel("Logout");
				menuLogout.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent arg0)
					{
						// TODO Auto-generated method stub
						LogoutActionPerformed(arg0);
					}
				});

				JMenuItem menuAttribute = new JMenuItem();
				menuAttribute.setLabel("Attribute");
				menuAttribute.addActionListener(new ActionListener()
				{
					@Override
					public void actionPerformed(ActionEvent e)
					{
						// TODO Auto-generated method stub
						AttributeActionPerformed(e);
					}
				});
				JMenuItem menuDelete = new JMenuItem();
				menuDelete.setLabel("Delete");
				menuDelete.addActionListener(new ActionListener()
				{
					public void actionPerformed(ActionEvent e)
					{
						// TODO Auto-generated method stub
						DeleteActionPerformed(e);
					}
				});
				pop.add(menuLogout);
				pop.add(menuLogin);
				pop.add(menuAttribute);
				pop.add(menuDelete);
				pop.show(e.getComponent(), e.getX(), e.getY());
			}
		}
	}

	// login device
	private void DoubleClickMouse(MouseEvent e)
	{
		TreePath path = treeDevice.getPathForLocation(e.getX(), e.getY());
		treeDevice.setSelectionPath(path);
		int n = treeDevice.getRowForLocation(e.getX(), e.getY());
		if (n < 0 || n == 0)
		{
			return;
		}

		if (e.getClickCount() == 2) 
		{
			TreePath selTree = treeDevice.getPathForRow(n);
			DefaultMutableTreeNode selNode = (DefaultMutableTreeNode) selTree.getLastPathComponent();
			if (selNode.isLeaf())
			{

			}
			else
			{
				DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) treeDevice.getLastSelectedPathComponent();
				String ip = treeNode.toString();
				DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);

				NativeLong nlUserID = LoginDevice(deviceInfo);
				deviceInfo.SetNUserID(nlUserID);
				m_hashDeviceInfo.replace(ip, deviceInfo);
				treeDevice.expandPath(treeDevice.getSelectionPath());
				MyTreeNode node = (MyTreeNode) treeNode;
				node.m_isOnLine = true;
				treeDevice.setCellRenderer(new NodeRenderer());
			}
		}
	}

	// login out
	@SuppressWarnings("deprecation")
	private void LogoutActionPerformed(ActionEvent arg0)
	{
		DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) treeDevice.getLastSelectedPathComponent();
		String ip = treeNode.toString();
		NativeLong lUserID = m_hashDeviceInfo.get(ip).GetNUserID();
		if (lUserID.longValue() == -1)
		{
			return;
		}
		hCNetSDK.NET_DVR_Logout(lUserID);
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		deviceInfo.SetNUserID(new NativeLong(-1));
		m_hashDeviceInfo.replace(ip, deviceInfo);
		if (comboBoxWindow.getSelectedIndex() == 0)
		{
			if (m_isPlaying)
			{
				StopPlay();
				comboBoxWindow.enable();
			}

		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (lUserID == arrShow[i].GetID())
				{
					arrShow[i].StopPlay();
					comboBoxWindow.enable();
				}
			}
		}
		treeDevice.collapsePath(treeDevice.getSelectionPath());
		MyTreeNode node = (MyTreeNode) treeNode;
		node.m_isOnLine = false;
		treeDevice.setCellRenderer(new NodeRenderer());

	}

	// login
	private void TreeLoginActionPerformed(ActionEvent event)
	{
		DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) treeDevice.getLastSelectedPathComponent();
		String ip = treeNode.toString();
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		if (deviceInfo.GetNUserID().longValue() != -1)
		{
			return;
		}
		NativeLong nlUserID = LoginDevice(deviceInfo);
		deviceInfo.SetNUserID(nlUserID);
		m_hashDeviceInfo.replace(ip, deviceInfo);
		treeDevice.expandPath(treeDevice.getSelectionPath());
		MyTreeNode node = (MyTreeNode) treeNode;
		node.m_isOnLine = true;
		treeDevice.setCellRenderer(new NodeRenderer());

	}

	private NativeLong LoginDevice(DeviceInfo deviceInfo)
	{
		NET_DVR_USER_LOGIN_INFO struLoginInfo = new NET_DVR_USER_LOGIN_INFO();
		NET_DVR_DEVICEINFO_V40 struDeviceInfo = new NET_DVR_DEVICEINFO_V40();
		for (int i = 0; i < deviceInfo.GetIP().length(); i++)
		{
			struLoginInfo.sDeviceAddress[i] = (byte) deviceInfo.GetIP().charAt(i);
		}
		for (int i = 0; i < deviceInfo.GetPassword().length(); i++)
		{
			struLoginInfo.sPassword[i] = (byte) deviceInfo.GetPassword().charAt(i);
		}
		for (int i = 0; i < deviceInfo.GetUserName().length(); i++)
		{
			struLoginInfo.sUserName[i] = (byte) deviceInfo.GetUserName().charAt(i);
		}
		struLoginInfo.wPort = deviceInfo.GetPort();
		struLoginInfo.write();
		return hCNetSDK.NET_DVR_Login_V40(struLoginInfo.getPointer(), struDeviceInfo.getPointer());
	}

	// delete device
	@SuppressWarnings("deprecation")
	private void DeleteActionPerformed(ActionEvent e)
	{
		DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) treeDevice.getLastSelectedPathComponent();
		DefaultTreeModel model = (DefaultTreeModel) treeDevice.getModel();
		model.removeNodeFromParent(treeNode);
		String ip = treeNode.toString();
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();

		if (lUserID.longValue() != -1)
		{
			if (comboBoxWindow.getSelectedIndex() == 0)
			{
				if (m_isPlaying)
				{
					StopPlay();
					comboBoxWindow.enable();
				}

			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (lUserID == arrShow[i].GetID())
					{
						arrShow[i].StopPlay();
						comboBoxWindow.enable();
					}
				}
			}
			hCNetSDK.NET_DVR_Logout(lUserID);
		}
		m_hashDeviceInfo.remove(ip);
	}

	// device attribute
	void AttributeActionPerformed(ActionEvent e)
	{
		// TODO Auto-generated method stub
		DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) treeDevice.getLastSelectedPathComponent();
		String ip = treeNode.toString();
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		DialogDeviceAttribute dlg = new DialogDeviceAttribute(deviceInfo);
		dlg.setBounds(this.getX() + this.getWidth() / 4, this.getY() + this.getHeight() / 4, 440, 260);
		dlg.setVisible(true);
	}

	public static int TreeAddDevice(NativeLong nlUserID, String sDeviceIP,
			HCNetSDK.NET_DVR_DEVICEINFO_V40 strDeviceInfo, DeviceInfo deviceInfo)
	{
		if (m_hashDeviceInfo.containsKey(sDeviceIP) && m_hashDeviceInfo.get(sDeviceIP).GetNUserID().longValue() != -1)
		{
			return -1;
		}
		if (m_hashDeviceInfo.containsKey(sDeviceIP))
		{
			m_hashDeviceInfo.replace(sDeviceIP, deviceInfo);

		}
		else
		{
			m_hashDeviceInfo.put(sDeviceIP, deviceInfo);
			CreateDeviceTree(nlUserID, sDeviceIP, strDeviceInfo);
		}
		return 0;
	}

	// get ip
	public String GetTreeIp()
	{
		NativeLong lUserId = new NativeLong(-1);
		String ip = "";
		DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) treeDevice.getLastSelectedPathComponent();
		if (treeNode == null)
		{
			return ip;
		}
		if (treeNode.toString() == "Device Tree")
		{
			return ip;
		}
		if (treeNode.isLeaf())
		{
			ip = getDeviceIP();
		}
		else
		{
			ip = treeNode.toString();
		}
		return ip;
	}

	// playback by file
	private void MenuItemPlayByFileMousePressed(MouseEvent evt)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			JOptionPane.showMessageDialog(this, "Please login");
			return;
		}
		DialogPlayByFile dialogPlayBack = new DialogPlayByFile(this, false, lUserID);
		dialogPlayBack.setBounds(this.getX(), this.getY(), 780, 650);
		dialogPlayBack.setVisible(true);
	}

	// play back by time
	private void MenuItemPlayByTimeMousePressed(java.awt.event.MouseEvent evt)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			JOptionPane.showMessageDialog(this, "Please login");
			return;
		}

		DialogPlayBackByTime dlgPlayTime = new DialogPlayBackByTime(this, false, lUserID, deviceInfo.GetIP());
		dlgPlayTime.setBounds(this.getX(), this.getY(), 700, 600);
		centerWindow(dlgPlayTime);
		dlgPlayTime.setVisible(true);
	}

	public static void centerWindow(Container window)
	{
		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		int w = window.getSize().width;
		int h = window.getSize().height;
		int x = (dim.width - w) / 2;
		int y = (dim.height - h) / 2;
		window.setLocation(x, y);
	}

	// listening
	private void actionPerformedListening(ActionEvent evt)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{

			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		DialogListening dlgListening = new DialogListening(ip);
		dlgListening.setBounds(this.getX(), this.getY(), 350, 300);
		centerWindow(dlgListening);
		dlgListening.setVisible(true);

	}

	// GuardDisGuard
	private void GuardDisGuardActionPerformed(ActionEvent event)
	{
		DialogGuard dlgPlayTime = new DialogGuard();
		dlgPlayTime.setBounds(this.getX(), this.getY(), 350, 300);
		centerWindow(dlgPlayTime);
		dlgPlayTime.setVisible(true);
	}

	// VoiceTalk
	private void VoiceActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 400, 200);
			dlg.setVisible(true);
			return;
		}

		DialogVoiceTalk dlgVoiceTalk = new DialogVoiceTalk(this, false, lUserID, deviceInfo.GetlpDeviceInfo());
		dlgVoiceTalk.setBounds(this.getX() + 200, this.getY() + 200, 300, 250);
		dlgVoiceTalk.setVisible(true);
	}

	// check time
	public void CheckTimeActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			//JOptionPane.showMessageDialog(this, "Please login");
			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		DialogCheckTime dlgCheckTime = new DialogCheckTime(this, false, lUserID);
		dlgCheckTime.setBounds(this.getX() + 200, this.getY() + 200, 350, 200);
		dlgCheckTime.setVisible(true);
	}

	// upgrade
	public void UpgradeActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		int iChanNum = deviceInfo.m_lpDeviceInfo.byChanNum;
		DialogUpGrade dlgUpgrade = new DialogUpGrade(lUserID, iChanNum);
		dlgUpgrade.setBounds(this.getX() + 200, this.getY() + 200, 550, 350);
		dlgUpgrade.setVisible(true);
	}

	// reboot
	public void RestartActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("Please login");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}

		DialogReboot dlgReboot = new DialogReboot(lUserID);
		dlgReboot.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
		dlgReboot.setVisible(true);
	}

	static void GetPTZParam()
	{
		int iIndex = comboBoxWindow.getSelectedIndex();
		if (iIndex == 0)
		{
			panelPTZ.SetParam(m_lLogID, m_lPreviewHandle, m_iChannelNum);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (arrShow[i].GetFocus() && arrShow[i].GetIsPlaying())
				{
					panelPTZ.SetParam(arrShow[i].GetID(), arrShow[i].GetPreviewHanedle(),
							arrShow[i].GetChannelNum().intValue());
				}
			}
		}
	}

	// Init table mode
	public DefaultTableModel initialTableModel()
	{
		String tabeTile[];
		tabeTile = new String[] { "Time", "Alarm Info", "Device Info" };
		DefaultTableModel alarmTableModel = new DefaultTableModel(tabeTile, 0);
		return alarmTableModel;
	}

	// log search
	public void LogSearchActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("log first");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		DialogSearchLog dlgLog = new DialogSearchLog(lUserID);
		dlgLog.setBounds(this.getX() + 200, this.getY() + 100, 580, 570);
		dlgLog.setVisible(true);
	}
	
	// video configure
	public void VideoConfigureActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("log first");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		VideoCfg videoConfigure = new VideoCfg(lUserID);
		videoConfigure.setBounds(this.getX() + 100, this.getY() + 100, 630, 530);
		videoConfigure.setVisible(true);
	}
	
	// audio configure
	public void AudioConfigureActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("log first");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		int iChanNum = deviceInfo.m_lpDeviceInfo.byChanNum;
		AudioCfg audioConfigure = new AudioCfg(lUserID, iChanNum);
		audioConfigure.setBounds(this.getX() + 200, this.getY() + 200, 550, 300);
		audioConfigure.setVisible(true);
	}
	
	// image configure
	public void ImageConfigureActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("log first");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		int iChanNum = deviceInfo.m_lpDeviceInfo.byChanNum;
		ImageCfg imageConfigure = new ImageCfg(lUserID, iChanNum);
		imageConfigure.setBounds(this.getX() + 100, this.getY() + 100, 780, 530);
		imageConfigure.setVisible(true);
	}
	
	// serial trans
	public void SerialTransActionPerformed(ActionEvent event)
	{
		String ip = GetTreeIp();
		if (ip == "")
		{
			return;
		}
		DeviceInfo deviceInfo = m_hashDeviceInfo.get(ip);
		NativeLong lUserID = deviceInfo.GetNUserID();
		if (lUserID.intValue() == -1)
		{
			DialogMessage dlg = new DialogMessage("log first");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
			return;
		}
		TransparentChannelCfg serialTrans = new TransparentChannelCfg(lUserID);
		serialTrans.setBounds(this.getX() + 100, this.getY() + 100, 780, 530);
		serialTrans.setVisible(true);
	}

	public static DefaultTableModel getTableModel()
	{
		return ((DefaultTableModel) tableAlarm.getModel());
	}
}
