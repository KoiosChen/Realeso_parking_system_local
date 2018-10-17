/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * JDialogCheckTime.java
 *
 * Created on 2009-10-9, 14:41:20
 */
/**
 *
 * @author  Administrator
 */
package test;

import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;
import java.util.Calendar;
import java.util.Date;
import javax.swing.JOptionPane;
import javax.swing.GroupLayout.Alignment;
import javax.swing.GroupLayout;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.UIManager;
import javax.swing.JLabel;
public class DialogCheckTime extends javax.swing.JDialog
{

    static HCNetSDK hCNetSDK = HCNetSDK.INSTANCE;
    private NativeLong m_lUserId;//LogID

    public DialogCheckTime(java.awt.Frame parent, boolean modal, NativeLong userId)
    {
        super(parent, modal);
        initComponents();
        m_lUserId = userId;
        Date today = new Date();
        Calendar now = Calendar.getInstance();//get current time  
        now.setTime(today); 
        jTextFieldYear.setText(now.get(Calendar.YEAR) + "");
        jTextFieldMonth.setText((now.get(Calendar.MONTH) + 1) + "");
        jTextFieldDay.setText(now.get(Calendar.DATE) + "");
        jTextFieldHour.setText(now.get(Calendar.HOUR_OF_DAY) + "");
        jTextFieldMinute.setText(now.get(Calendar.MINUTE) + "");
        jTextFieldSecond.setText(now.get(Calendar.SECOND) + "");
        setModal(true);

    }

    @SuppressWarnings("unchecked")
    private void initComponents() {

        jPanel1 = new javax.swing.JPanel();
        jToggleButtonExit = new javax.swing.JToggleButton();
        jTextFieldDay = new javax.swing.JTextField();
        jToggleButtonCheckTime = new javax.swing.JToggleButton();
        jLabel2 = new javax.swing.JLabel();
        jLabel2.setFont(UIManager.getFont("CheckBox.font"));
        jTextFieldHour = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();
        jTextFieldMonth = new javax.swing.JTextField();
        jLabel6 = new javax.swing.JLabel();
        jTextFieldYear = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        jTextFieldMinute = new javax.swing.JTextField();
        jLabel4 = new javax.swing.JLabel();
        jTextFieldSecond = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("CheckTime");

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder(""));

        jToggleButtonExit.setText("Exit");
        jToggleButtonExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonExitActionPerformed(evt);
            }
        });

        jToggleButtonCheckTime.setText("Check");
        jToggleButtonCheckTime.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jToggleButtonCheckTimeActionPerformed(evt);
            }
        });

        jLabel2.setText("  M");

        jLabel3.setText("D");

        jLabel6.setText("S");

        jLabel1.setText("Y");

        jLabel4.setText("H");
        
        JLabel lblM = new JLabel();
        lblM.setText("  M");
        lblM.setFont(UIManager.getFont("CheckBox.font"));

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1Layout.setHorizontalGroup(
        	jPanel1Layout.createParallelGroup(Alignment.TRAILING)
        		.addGroup(jPanel1Layout.createSequentialGroup()
        			.addGap(17)
        			.addGroup(jPanel1Layout.createParallelGroup(Alignment.LEADING)
        				.addGroup(jPanel1Layout.createParallelGroup(Alignment.TRAILING)
        					.addGroup(jPanel1Layout.createSequentialGroup()
        						.addComponent(jTextFieldHour, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE)
        						.addPreferredGap(ComponentPlacement.UNRELATED)
        						.addComponent(jLabel4, GroupLayout.PREFERRED_SIZE, 27, GroupLayout.PREFERRED_SIZE))
        					.addGroup(jPanel1Layout.createSequentialGroup()
        						.addComponent(jTextFieldYear, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE)
        						.addPreferredGap(ComponentPlacement.UNRELATED)
        						.addComponent(jLabel1, GroupLayout.PREFERRED_SIZE, 27, GroupLayout.PREFERRED_SIZE)))
        				.addComponent(jToggleButtonCheckTime, 0, 0, Short.MAX_VALUE))
        			.addPreferredGap(ComponentPlacement.RELATED)
        			.addGroup(jPanel1Layout.createParallelGroup(Alignment.LEADING)
        				.addGroup(jPanel1Layout.createSequentialGroup()
        					.addComponent(jTextFieldMonth, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE)
        					.addGap(3)
        					.addComponent(jLabel2, GroupLayout.PREFERRED_SIZE, 34, GroupLayout.PREFERRED_SIZE)
        					.addPreferredGap(ComponentPlacement.UNRELATED)
        					.addComponent(jTextFieldDay, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE))
        				.addGroup(jPanel1Layout.createSequentialGroup()
        					.addComponent(jTextFieldMinute, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE)
        					.addGap(2)
        					.addComponent(lblM, GroupLayout.PREFERRED_SIZE, 34, GroupLayout.PREFERRED_SIZE)
        					.addPreferredGap(ComponentPlacement.UNRELATED)
        					.addComponent(jTextFieldSecond, GroupLayout.DEFAULT_SIZE, 41, Short.MAX_VALUE))
        				.addComponent(jToggleButtonExit, Alignment.TRAILING))
        			.addPreferredGap(ComponentPlacement.RELATED)
        			.addGroup(jPanel1Layout.createParallelGroup(Alignment.LEADING)
        				.addComponent(jLabel6, GroupLayout.PREFERRED_SIZE, 43, GroupLayout.PREFERRED_SIZE)
        				.addComponent(jLabel3, GroupLayout.PREFERRED_SIZE, 27, GroupLayout.PREFERRED_SIZE))
        			.addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
        	jPanel1Layout.createParallelGroup(Alignment.LEADING)
        		.addGroup(jPanel1Layout.createSequentialGroup()
        			.addGap(20)
        			.addGroup(jPanel1Layout.createParallelGroup(Alignment.BASELINE)
        				.addComponent(jTextFieldYear, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        				.addComponent(jLabel1)
        				.addComponent(jTextFieldMonth, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        				.addComponent(jLabel2)
        				.addComponent(jTextFieldDay, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        				.addComponent(jLabel3))
        			.addPreferredGap(ComponentPlacement.UNRELATED)
        			.addGroup(jPanel1Layout.createParallelGroup(Alignment.BASELINE)
        				.addComponent(jTextFieldHour, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        				.addComponent(jLabel4)
        				.addComponent(jTextFieldMinute, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        				.addComponent(jLabel6)
        				.addComponent(jTextFieldSecond, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
        				.addComponent(lblM))
        			.addGap(29)
        			.addGroup(jPanel1Layout.createParallelGroup(Alignment.BASELINE)
        				.addComponent(jToggleButtonExit)
        				.addComponent(jToggleButtonCheckTime))
        			.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel1.setLayout(jPanel1Layout);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        getAccessibleContext().setAccessibleName(null);

        pack();
    }

    //exit
    private void jToggleButtonExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonExitActionPerformed
        this.dispose();
    }

 
    //check
    private void jToggleButtonCheckTimeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jToggleButtonCheckTimeActionPerformed
        HCNetSDK.NET_DVR_TIME strCurTime = new HCNetSDK.NET_DVR_TIME();
        strCurTime.dwYear = Integer.parseInt(jTextFieldYear.getText());
        strCurTime.dwMonth = Integer.parseInt(jTextFieldMonth.getText());
        strCurTime.dwDay = Integer.parseInt(jTextFieldDay.getText());
        strCurTime.dwHour = Integer.parseInt(jTextFieldHour.getText());
        strCurTime.dwMinute = Integer.parseInt(jTextFieldMinute.getText());
        strCurTime.dwSecond = Integer.parseInt(jTextFieldSecond.getText());
        strCurTime.write();
        Pointer lpPicConfig = strCurTime.getPointer();
        boolean setDVRConfigSuc = hCNetSDK.NET_DVR_SetDVRConfig(m_lUserId, HCNetSDK.NET_DVR_SET_TIMECFG,
                new NativeLong(0), lpPicConfig, strCurTime.size());
        strCurTime.read();
        if (setDVRConfigSuc != true)
        {
			DialogMessage dlg = new DialogMessage("check time fail");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
            return;
        } else
        {
        	DialogMessage dlg = new DialogMessage("check time success");
			dlg.setBounds(this.getX()+this.getWidth()/3,this.getY()+this.getHeight()/3 , 370, 200);
			dlg.setVisible(true);
            return;
        }

    }
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JTextField jTextFieldDay;
    private javax.swing.JTextField jTextFieldHour;
    private javax.swing.JTextField jTextFieldMinute;
    private javax.swing.JTextField jTextFieldMonth;
    private javax.swing.JTextField jTextFieldSecond;
    private javax.swing.JTextField jTextFieldYear;
    private javax.swing.JToggleButton jToggleButtonCheckTime;
    private javax.swing.JToggleButton jToggleButtonExit;
}
