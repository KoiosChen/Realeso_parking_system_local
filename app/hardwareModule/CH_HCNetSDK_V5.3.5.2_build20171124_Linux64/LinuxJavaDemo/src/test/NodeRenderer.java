package test;
import java.awt.Color;
import java.awt.Component;

import javax.swing.JTree;
import javax.swing.tree.DefaultTreeCellRenderer;

@SuppressWarnings("serial")
public class NodeRenderer extends DefaultTreeCellRenderer
{
	public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded,
			boolean leaf, int row, boolean hasFocus)
	{
		super.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
		MyTreeNode node = (MyTreeNode) value;
		if (node.m_isOnLine == true)
		{
			setForeground(Color.BLACK);

		}
		else
		{
			setForeground(Color.RED);
		}
		return this;
	}
}
