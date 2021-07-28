import wx
import util

class ItemDetail(wx.Panel):
    """
    Detail panel for editing items
    """
    def __init__(self, parent, game):
        wx.Panel.__init__(self, parent)
        self.game = game
        wx.StaticText(
            self,
            wx.ID_ANY,
            "This is not implemented right now sorry"
        )

    def getList(self):
        return self.game.items
    
    def select(self, index):
        pass
    
    def rename(self, index, value):
        pass
    
    def delete(self, index):
        pass

    def add(self):
        pass

class ListEditor(wx.Panel):
    """
    Generic control that places a list of named items on one side, and a detail
    pane on the other in which you can edit selected items. The idea is you
    pass the type of detail pane that you want to edit. All of them need to
    have a constructor that takes a parent and a reference to the game object
    and nothing else.
    """
    def __init__(self, parent, detailClass, game):
        wx.Panel.__init__(self, parent)
        grid = wx.GridBagSizer(3, 3)
        addButton = wx.Button(self, wx.ID_ANY, "+")
        removeButton = wx.Button(self, wx.ID_ANY, "-")
        nameLabel = wx.StaticText(self, wx.ID_ANY, "name")
        self.name = wx.TextCtrl(self)
        self.itemList = wx.ListBox(self, style=wx.LB_SINGLE)
        self.detail = detailClass(self, game)
        grid.Add(addButton, pos=(0, 0))
        grid.Add(self.itemList, pos=(1, 0), flag=wx.EXPAND)
        grid.Add(removeButton, pos=(2, 0))
        grid.Add(nameLabel, pos=(0, 1))
        grid.Add(self.name, pos=(0, 2), flag=wx.EXPAND|wx.RIGHT)
        grid.Add(self.detail, pos=(1, 1), span=(2, 2), flag=wx.EXPAND|wx.BOTTOM|wx.RIGHT)
        grid.AddGrowableCol(2)
        grid.AddGrowableRow(1)
        self.SetSizer(grid)
        for item in self.detail.getList():
            self.itemList.Append([item.__str__()])

class DbFrame(wx.Dialog):
    """
    Overall window of the database editor.
    """
    def __init__(self, parent, game):
        wx.Dialog.__init__(self, parent, title="Database")
        tabs = wx.Notebook(self)
        itemPanel = ListEditor(tabs, ItemDetail, game)
        battlePanel = wx.Panel(tabs)
        gamePanel = wx.Panel(tabs)
        tabs.AddPage(itemPanel, "Items")
        tabs.AddPage(battlePanel, "Battles")
        tabs.AddPage(gamePanel, "System")
        self.SetSize(600, 600)
