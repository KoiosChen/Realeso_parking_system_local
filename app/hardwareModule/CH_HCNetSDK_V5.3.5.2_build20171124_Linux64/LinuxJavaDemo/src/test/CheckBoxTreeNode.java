package test;

import javax.swing.tree.DefaultMutableTreeNode;


public class CheckBoxTreeNode extends DefaultMutableTreeNode  
{  
    protected boolean isSelected;  
      
    public CheckBoxTreeNode()  
    {  
        this(null);  
    }  
      
    public CheckBoxTreeNode(Object userObject)  
    {  
        this(userObject, true, false);  
    }  
      
    public CheckBoxTreeNode(Object userObject, boolean allowsChildren, boolean isSelected)  
    {  
        super(userObject, allowsChildren);  
        this.isSelected = isSelected;  
    }  
 
    public boolean isSelected()  
    {  
        return isSelected;  
    }  
      
    public void setSelected(boolean _isSelected)  
    {  
        this.isSelected = _isSelected;  
          
        if(_isSelected)  
        {  
            if(children != null)  
            {  
                for(Object obj : children)  
                {  
                    CheckBoxTreeNode node = (CheckBoxTreeNode)obj;  
                    if(_isSelected != node.isSelected())  
                        node.setSelected(_isSelected);  
                }  
            }  
            CheckBoxTreeNode pNode = (CheckBoxTreeNode)parent;  
            if(pNode != null)  
            {  
                int index = 0;  
                for(; index < pNode.children.size(); ++ index)  
                {  
                    CheckBoxTreeNode pChildNode = (CheckBoxTreeNode)pNode.children.get(index);  
                    if(!pChildNode.isSelected())  
                        break;  
                }  
                if(index == pNode.children.size())  
                {  
                    if(pNode.isSelected() != _isSelected)  
                        pNode.setSelected(_isSelected);  
                }  
            }  
        }  
        else   
        {  
            if(children != null)  
            {  
                int index = 0;  
                for(; index < children.size(); ++ index)  
                {  
                    CheckBoxTreeNode childNode = (CheckBoxTreeNode)children.get(index);  
                    if(!childNode.isSelected())  
                        break;  
                }  
                if(index == children.size())  
                {  
                    for(int i = 0; i < children.size(); ++ i)  
                    {  
                        CheckBoxTreeNode node = (CheckBoxTreeNode)children.get(i);  
                        if(node.isSelected() != _isSelected)  
                            node.setSelected(_isSelected);  
                    }  
                }  
            }  
              
            CheckBoxTreeNode pNode = (CheckBoxTreeNode)parent;  
            if(pNode != null && pNode.isSelected() != _isSelected)  
                pNode.setSelected(_isSelected);  
        }  
    }  
} 