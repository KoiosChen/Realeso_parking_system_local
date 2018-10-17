package test;

import javax.swing.JPanel;
import javax.swing.border.TitledBorder;

import com.sun.jna.NativeLong;

import test.HCNetSDK.NET_DVR_PTZ_PATTERN;

import java.awt.Font;
import javax.swing.JButton;
import javax.swing.border.EtchedBorder;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JComboBox;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.logging.Logger;
import javax.swing.DefaultComboBoxModel;
import javax.swing.UIManager;
import java.awt.Color;

public class PanelPTZ extends JPanel
{

	protected final Logger log = Logger.getLogger(this.getClass().getName());
	private NativeLong m_lRealHandle; // preview handle
	private NativeLong m_lUserID; // LogID
	private int m_iChannelNum; // channel num

	private JComboBox comboBoxPresetPoint;
	private JComboBox comboBoxCruiseRoute;
	private JComboBox comboBoxSpeed;
	private JComboBox comboBoxPatternScan;

	/**
	 * Create the panel.
	 */
	public PanelPTZ()
	{
		InitCompnent();
		InitPTZParam();
		
	}

	public void SetParam(NativeLong lUserID, NativeLong lRealHandle, int iChannelNum)
	{
		m_lUserID = lUserID;
		m_lRealHandle = lRealHandle;
		m_iChannelNum = iChannelNum;
	}

	public void InitCompnent()
	{
		setLayout(null);
		JPanel panel_15 = new JPanel();
		panel_15.setLayout(null);
		panel_15.setFont(new Font("Times New Roman", Font.PLAIN, 12));
		panel_15.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "PTZ Control",
				TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel_15.setBounds(0, 5, 210, 457);
		add(panel_15);

		JPanel panel_16 = new JPanel();
		panel_16.setLayout(null);
		panel_16.setBounds(10, 21, 196, 199);
		panel_15.add(panel_16);

		JButton buttonUp = new JButton("Up");
		buttonUp.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				UpMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				UpMouseRelease(e);
			}
		});
		buttonUp.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonUp.setFocusable(false);
		buttonUp.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonUp.setBounds(70, 10, 57, 23);
		panel_16.add(buttonUp);

		JButton buttonLeft = new JButton("Left");
		buttonLeft.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				LeftMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				LeftMouseRelease(e);
			}
		});
		buttonLeft.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonLeft.setFocusable(false);
		buttonLeft.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonLeft.setBounds(10, 34, 57, 23);
		panel_16.add(buttonLeft);

		JButton buttonRight = new JButton("Right");
		buttonRight.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				RightMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				RightMouseRelease(e);
			}
		});
		buttonRight.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonRight.setFocusable(false);
		buttonRight.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonRight.setBounds(129, 34, 57, 23);
		panel_16.add(buttonRight);

		JButton buttonDown = new JButton("Down");
		buttonDown.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				DownMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				DownMouseRelease(e);
			}
		});
		buttonDown.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonDown.setFocusable(false);
		buttonDown.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonDown.setBounds(70, 57, 57, 23);
		panel_16.add(buttonDown);

		JButton buttonIn = new JButton("In");
		buttonIn.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				ZoomInMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				ZoomInMouseRelease(e);
			}
		});
		buttonIn.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonIn.setFocusable(false);
		buttonIn.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonIn.setBounds(10, 87, 57, 23);
		panel_16.add(buttonIn);

		JLabel label_15 = new JLabel("Zoom");
		label_15.setFont(new Font("宋体", Font.PLAIN, 12));
		label_15.setBounds(89, 91, 38, 15);
		panel_16.add(label_15);

		JButton buttonOut = new JButton("Out");
		buttonOut.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				ZoomOutMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				ZoomOutMouseRelease(e);
			}
		});
		buttonOut.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonOut.setFocusable(false);
		buttonOut.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonOut.setBounds(129, 87, 57, 23);
		panel_16.add(buttonOut);

		JButton buttonNear = new JButton("Near");
		buttonNear.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				FocusNearMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				FocusNearMouseRelease(e);
			}
		});
		buttonNear.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonNear.setFocusable(false);
		buttonNear.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonNear.setBounds(10, 116, 57, 23);
		panel_16.add(buttonNear);

		JLabel label_16 = new JLabel("Focus");
		label_16.setFont(new Font("宋体", Font.PLAIN, 12));
		label_16.setBounds(89, 120, 38, 15);
		panel_16.add(label_16);

		JButton buttonFar = new JButton("Far");
		buttonFar.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				FocusFarMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				FocusFarMouseRelease(e);
			}
		});
		buttonFar.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonFar.setFocusable(false);
		buttonFar.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonFar.setBounds(129, 116, 57, 23);
		panel_16.add(buttonFar);

		JButton buttonOpen = new JButton("Open");
		buttonOpen.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				IrisOpenMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				IrisOpenMouseRelease(e);
			}
		});
		buttonOpen.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonOpen.setFocusable(false);
		buttonOpen.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonOpen.setBounds(10, 143, 57, 23);
		panel_16.add(buttonOpen);

		JLabel label_17 = new JLabel("Iris");
		label_17.setFont(new Font("宋体", Font.PLAIN, 12));
		label_17.setBounds(89, 147, 38, 15);
		panel_16.add(label_17);

		JButton buttonClose = new JButton("Close");
		buttonClose.addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				IrisCloseMousePress(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				IrisCloseMouseRelease(e);
			}
		});
		buttonClose.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonClose.setFocusable(false);
		buttonClose.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonClose.setBounds(129, 143, 57, 23);
		panel_16.add(buttonClose);

		JLabel label_18 = new JLabel("PTZ Speed");
		label_18.setFont(new Font("宋体", Font.PLAIN, 12));
		label_18.setBounds(13, 174, 54, 15);
		panel_16.add(label_18);

		comboBoxSpeed = new JComboBox();
		comboBoxSpeed.setFont(new Font("宋体", Font.PLAIN, 12));
		comboBoxSpeed.setFocusable(false);
		comboBoxSpeed.setBounds(80, 172, 106, 21);
		panel_16.add(comboBoxSpeed);

		JPanel panel_17 = new JPanel();
		panel_17.setLayout(null);
		panel_17.setFont(new Font("宋体", Font.PLAIN, 12));
		panel_17.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Preset Point",
				TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel_17.setBounds(10, 220, 196, 49);
		panel_15.add(panel_17);

		comboBoxPresetPoint = new JComboBox();
		comboBoxPresetPoint.setFont(new Font("宋体", Font.PLAIN, 12));
		comboBoxPresetPoint.setFocusable(false);
		comboBoxPresetPoint.setBounds(10, 20, 42, 21);
		panel_17.add(comboBoxPresetPoint);

		JButton buttonCall = new JButton("Call");
		buttonCall.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				PresetPointActionPerformed(e);
			}
		});
		buttonCall.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonCall.setFocusable(false);
		buttonCall.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonCall.setBounds(58, 19, 42, 23);
		panel_17.add(buttonCall);

		JButton buttonSet = new JButton("Set");
		buttonSet.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				SetPresetPointActionPerformed(e);
			}
		});
		buttonSet.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonSet.setFocusable(false);
		buttonSet.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonSet.setBounds(104, 19, 42, 23);
		panel_17.add(buttonSet);

		JButton buttonDel = new JButton("Del");
		buttonDel.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				DelPresetPointActionPerformed(e);
			}
		});
		buttonDel.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonDel.setFocusable(false);
		buttonDel.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonDel.setBounds(149, 19, 37, 23);
		panel_17.add(buttonDel);

		JPanel panel_18 = new JPanel();
		panel_18.setLayout(null);
		panel_18.setFont(new Font("宋体", Font.PLAIN, 12));
		panel_18.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Cruise Route",
				TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel_18.setBounds(10, 269, 196, 55);
		panel_15.add(panel_18);

		comboBoxCruiseRoute = new JComboBox();
		comboBoxCruiseRoute.setFont(new Font("宋体", Font.PLAIN, 12));
		comboBoxCruiseRoute.setFocusable(false);
		comboBoxCruiseRoute.setBounds(10, 23, 39, 21);
		panel_18.add(comboBoxCruiseRoute);

		JButton buttonCall1 = new JButton("Call");
		buttonCall1.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				CruiseRouteActionPerformed(e);
			}
		});
		buttonCall1.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonCall1.setFocusable(false);
		buttonCall1.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonCall1.setBounds(51, 23, 39, 23);
		panel_18.add(buttonCall1);

		JButton buttonDel1 = new JButton("Del");
		buttonDel1.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				DelCruiseRouteActionPerformed(e);
			}
		});
		buttonDel1.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonDel1.setFocusable(false);
		buttonDel1.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonDel1.setBounds(138, 23, 50, 23);
		panel_18.add(buttonDel1);

		JButton buttonSet1 = new JButton("Set");
		buttonSet1.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				SetCruiseRouteActionPerformed(e);
			}
		});
		buttonSet1.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonSet1.setFocusable(false);
		buttonSet1.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonSet1.setBounds(89, 23, 50, 23);
		panel_18.add(buttonSet1);

		JPanel panel_19 = new JPanel();
		panel_19.setLayout(null);
		panel_19.setFont(new Font("宋体", Font.PLAIN, 12));
		panel_19.setBorder(new TitledBorder(UIManager.getBorder("TitledBorder.border"), "Pattern Scan",
				TitledBorder.LEADING, TitledBorder.TOP, null, new Color(0, 0, 0)));
		panel_19.setBounds(10, 334, 196, 108);
		panel_15.add(panel_19);

		JLabel label_19 = new JLabel("ID");
		label_19.setFont(new Font("宋体", Font.PLAIN, 12));
		label_19.setBounds(10, 21, 21, 15);
		panel_19.add(label_19);

		comboBoxPatternScan = new JComboBox();
		comboBoxPatternScan.setModel(new DefaultComboBoxModel(new String[] { "1", "2", "3", "4" }));
		comboBoxPatternScan.setFont(new Font("宋体", Font.PLAIN, 12));
		comboBoxPatternScan.setFocusable(false);
		comboBoxPatternScan.setBounds(30, 18, 41, 21);
		panel_19.add(comboBoxPatternScan);

		JButton buttonStartMem = new JButton("Start Mem");
		buttonStartMem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				StartMemActionPerformed(e);
			}
		});
		buttonStartMem.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonStartMem.setFocusable(false);
		buttonStartMem.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonStartMem.setBounds(10, 72, 83, 23);
		panel_19.add(buttonStartMem);

		JButton buttonStopMem = new JButton("Stop Mem");
		buttonStopMem.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				StopMemActionPerformed(e);
			}
		});
		buttonStopMem.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonStopMem.setFocusable(false);
		buttonStopMem.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonStopMem.setBounds(97, 72, 89, 23);
		panel_19.add(buttonStopMem);

		JButton buttonDelOne = new JButton("Del One");
		buttonDelOne.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				DelOnePatterScanActionPerformed(e);
			}
		});
		buttonDelOne.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonDelOne.setFocusable(false);
		buttonDelOne.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonDelOne.setBounds(10, 46, 83, 23);
		panel_19.add(buttonDelOne);

		JButton buttonDelAll = new JButton("Del All");
		buttonDelAll.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				DelAllPatterScanActionPerformed(e);
			}
		});
		buttonDelAll.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonDelAll.setFocusable(false);
		buttonDelAll.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonDelAll.setBounds(97, 46, 89, 23);
		panel_19.add(buttonDelAll);

		JButton buttonRun = new JButton("Run");
		buttonRun.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				RunPatternScanActionPerformed(e);
			}
		});
		buttonRun.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonRun.setFocusable(false);
		buttonRun.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonRun.setBounds(81, 17, 48, 23);
		panel_19.add(buttonRun);

		JButton buttonStop = new JButton("Stop");
		buttonStop.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				StopPatternScanActionPerformed(e);
			}
		});
		buttonStop.setFont(new Font("宋体", Font.PLAIN, 12));
		buttonStop.setFocusable(false);
		buttonStop.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		buttonStop.setBounds(133, 17, 57, 23);
		panel_19.add(buttonStop);

	}


	private void PTZControlAll(int iPTZCommand, int iStop)
	{
		JavaDemo.GetPTZParam();
		int iSpeed = comboBoxSpeed.getSelectedIndex() + 1;
		if (!JavaDemo.hCNetSDK.NET_DVR_PTZControlWithSpeed(m_lRealHandle, iPTZCommand, iStop, iSpeed))
		{
			//JOptionPane.showMessageDialog(this,"PTZ control fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("PTZ control fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	public void InitPTZParam()
	{
		int iNum;
		for (iNum = 0; iNum < HCNetSDK.MAX_CRUISE_V30; iNum++)
		{
			comboBoxCruiseRoute.addItem(iNum + 1);
		}

		for (iNum = 0; iNum < HCNetSDK.MAX_PRESET_V30; iNum++)
		{
			comboBoxPresetPoint.addItem(iNum + 1);
		}

		for (iNum = 0; iNum < 7; iNum++)
		{
			comboBoxSpeed.addItem(iNum + 1);
		}
	}

	// up
	private void UpMousePress(MouseEvent e)
	{
		PTZControlAll(HCNetSDK.TILT_UP, 0);
	}

	private void UpMouseRelease(MouseEvent e)
	{
		PTZControlAll(HCNetSDK.TILT_UP, 1);
	}

	// left
	private void LeftMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.PAN_LEFT, 0);
	}

	private void LeftMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.PAN_LEFT, 1);
	}

	// right
	private void RightMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.PAN_RIGHT, 0);
	}

	private void RightMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.PAN_RIGHT, 1);
	}

	// down
	private void DownMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.TILT_DOWN, 0);
	}

	private void DownMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.TILT_DOWN, 1);
	}

	// zoom in
	private void ZoomInMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.ZOOM_IN, 0);
	}

	private void ZoomInMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.ZOOM_IN, 1);
	}

	// zoom out
	private void ZoomOutMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.ZOOM_OUT, 0);
	}

	private void ZoomOutMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.ZOOM_OUT, 1);
	}

	// focus near
	private void FocusNearMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.FOCUS_NEAR, 0);
	}

	private void FocusNearMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.FOCUS_NEAR, 1);
	}

	// focus far
	private void FocusFarMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.FOCUS_FAR, 0);
	}

	private void FocusFarMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.FOCUS_FAR, 1);
	}

	// Iris open
	private void IrisOpenMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.IRIS_OPEN, 0);
	}

	private void IrisOpenMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.IRIS_OPEN, 1);
	}

	// Iris close
	private void IrisCloseMousePress(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.IRIS_CLOSE, 0);
	}

	private void IrisCloseMouseRelease(MouseEvent event)
	{
		PTZControlAll(HCNetSDK.IRIS_CLOSE, 1);
	}

	// call preset point
	private void PresetPointActionPerformed(ActionEvent event)
	{
		int iPreset = comboBoxPresetPoint.getSelectedIndex() + 1;
		if (!JavaDemo.hCNetSDK.NET_DVR_PTZPreset(m_lRealHandle, HCNetSDK.GOTO_PRESET, iPreset))
		{
			//JOptionPane.showMessageDialog(this,"call preset point fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("call preset point fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// set Preset point
	public void SetPresetPointActionPerformed(ActionEvent e)
	{
		JavaDemo.GetPTZParam();
		int iPreset = comboBoxPresetPoint.getSelectedIndex() + 1;
		if (!JavaDemo.hCNetSDK.NET_DVR_PTZPreset(m_lRealHandle, HCNetSDK.SET_PRESET, iPreset))
		{
			//JOptionPane.showMessageDialog(this,"Set Preset point fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("Set preset point fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// delete Preset point
	public void DelPresetPointActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		int iPreset = comboBoxPresetPoint.getSelectedIndex() + 1;
		if (!JavaDemo.hCNetSDK.NET_DVR_PTZPreset(m_lRealHandle, HCNetSDK.CLE_PRESET, iPreset))
		{
			//JOptionPane.showMessageDialog(this,"delete Preset point fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("Delete preset point fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// call cruise route
	public void CruiseRouteActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		byte iCruise = (byte) (comboBoxCruiseRoute.getSelectedIndex() + 1);
		if (!JavaDemo.hCNetSDK.NET_DVR_PTZCruise(m_lRealHandle, HCNetSDK.RUN_SEQ, iCruise, (byte) 0, (short) 0))
		{
			//JOptionPane.showMessageDialog(this,"call cruise route fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("call cruise route fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// set cruise route
	public void SetCruiseRouteActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		DialogCruiseRoute dlgCruiseRoute = new DialogCruiseRoute(m_lRealHandle);
		dlgCruiseRoute.setBounds(this.getX() + 200, this.getY() + 200, 450, 270);
		dlgCruiseRoute.setVisible(true);
	}

	// del cruise route
	public void DelCruiseRouteActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		byte byCruise = (byte) (comboBoxCruiseRoute.getSelectedIndex() + 1);
		if (!JavaDemo.hCNetSDK.NET_DVR_PTZCruise(m_lRealHandle, HCNetSDK.DEL_SEQ, byCruise, (byte) 0, (short) 0))
		{
			//JOptionPane.showMessageDialog(this,"delete cruise route fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("delete cruise route fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// start memory
	public void StartMemActionPerformed(ActionEvent e)
	{
		JavaDemo.GetPTZParam();
		int iPatternId = comboBoxPatternScan.getSelectedIndex() + 1;
		HCNetSDK.NET_DVR_PTZ_PATTERN struPTZPattern = new NET_DVR_PTZ_PATTERN();
		struPTZPattern.dwChannel = m_iChannelNum;
		struPTZPattern.dwPatternID = iPatternId;
		struPTZPattern.dwPatternCmd = HCNetSDK.STA_MEM_CRUISE;
		struPTZPattern.dwSize = struPTZPattern.size();

		if (!JavaDemo.hCNetSDK.NET_DVR_RemoteControl(m_lUserID, HCNetSDK.NET_DVR_CONTROL_PTZ_PATTERN, struPTZPattern,
				struPTZPattern.size()))
		{
			//JOptionPane.showMessageDialog(this,"start memory fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("start memory fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}

	}

	// stop memory
	public void StopMemActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		int iPatternId = comboBoxPatternScan.getSelectedIndex() + 1;
		HCNetSDK.NET_DVR_PTZ_PATTERN struPTZPattern = new NET_DVR_PTZ_PATTERN();
		struPTZPattern.dwChannel = m_iChannelNum;
		struPTZPattern.dwPatternID = iPatternId;
		struPTZPattern.dwPatternCmd = HCNetSDK.STO_MEM_CRUISE;
		struPTZPattern.dwSize = struPTZPattern.size();

		if (!JavaDemo.hCNetSDK.NET_DVR_RemoteControl(m_lUserID, HCNetSDK.NET_DVR_CONTROL_PTZ_PATTERN, struPTZPattern,
				struPTZPattern.size()))
		{
			//JOptionPane.showMessageDialog(this,"stop memory fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("stop memory fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// start run pattern scan
	public void RunPatternScanActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		int iPatternId = comboBoxPatternScan.getSelectedIndex() + 1;
		HCNetSDK.NET_DVR_PTZ_PATTERN struPTZPattern = new NET_DVR_PTZ_PATTERN();
		struPTZPattern.dwChannel = m_iChannelNum;
		struPTZPattern.dwPatternID = iPatternId;
		struPTZPattern.dwPatternCmd = HCNetSDK.RUN_CRUISE;
		struPTZPattern.dwSize = struPTZPattern.size();

		if (!JavaDemo.hCNetSDK.NET_DVR_RemoteControl(m_lUserID, HCNetSDK.NET_DVR_CONTROL_PTZ_PATTERN, struPTZPattern,
				struPTZPattern.size()))
		{
			//JOptionPane.showMessageDialog(this,"start run pattern scan fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("start run pattern scan ,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// stop run pattern scan
	public void StopPatternScanActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		int iPatternId = comboBoxPatternScan.getSelectedIndex() + 1;
		HCNetSDK.NET_DVR_PTZ_PATTERN struPTZPattern = new NET_DVR_PTZ_PATTERN();
		struPTZPattern.dwChannel = m_iChannelNum;
		struPTZPattern.dwPatternID = iPatternId;
		struPTZPattern.dwPatternCmd = HCNetSDK.STOP_CRUISE;
		struPTZPattern.dwSize = struPTZPattern.size();

		if (!JavaDemo.hCNetSDK.NET_DVR_RemoteControl(m_lUserID, HCNetSDK.NET_DVR_CONTROL_PTZ_PATTERN, struPTZPattern,
				struPTZPattern.size()))
		{
			//JOptionPane.showMessageDialog(this,"stop run pattern scan fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("stop run pattern scan ,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// delete one pattern scan
	public void DelOnePatterScanActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		int iPatternId = comboBoxPatternScan.getSelectedIndex() + 1;
		HCNetSDK.NET_DVR_PTZ_PATTERN struPTZPattern = new NET_DVR_PTZ_PATTERN();
		struPTZPattern.dwChannel = m_iChannelNum;
		struPTZPattern.dwPatternID = iPatternId;
		struPTZPattern.dwPatternCmd = HCNetSDK.DELETE_CRUISE;
		struPTZPattern.dwSize = struPTZPattern.size();

		if (!JavaDemo.hCNetSDK.NET_DVR_RemoteControl(m_lUserID, HCNetSDK.NET_DVR_CONTROL_PTZ_PATTERN, struPTZPattern,
				struPTZPattern.size()))
		{
			//JOptionPane.showMessageDialog(this,"delete one pattern scan fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("delete one pattern scan fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

	// delete all pattern scan
	public void DelAllPatterScanActionPerformed(ActionEvent event)
	{
		JavaDemo.GetPTZParam();
		HCNetSDK.NET_DVR_PTZ_PATTERN struPTZPattern = new NET_DVR_PTZ_PATTERN();
		struPTZPattern.dwChannel = m_iChannelNum;
		struPTZPattern.dwPatternCmd = HCNetSDK.DELETE_ALL_CRUISE;
		struPTZPattern.dwSize = struPTZPattern.size();

		if (!JavaDemo.hCNetSDK.NET_DVR_RemoteControl(m_lUserID, HCNetSDK.NET_DVR_CONTROL_PTZ_PATTERN, struPTZPattern,
				struPTZPattern.size()))
		{
			//JOptionPane.showMessageDialog(this,"delete all pattern scan fail,error code=" + JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			DialogMessage dlg = new DialogMessage("delete all pattern scan fail,error code:"+JavaDemo.hCNetSDK.NET_DVR_GetLastError());
			dlg.setBounds(0,0 , 370, 200);
			dlg.setVisible(true);
			return;
		}
	}

}
