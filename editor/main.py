#!/usr/bin/env python3

import wx, wx.stc, wx.propgrid, os
import control, db, util, model


ABOUT_TEXT = """Tiger Shark: Extreme Sex Edition editor
Created 2021 by Daly Barron
Using icons taken from https://icons8.com
"""


def addTool(toolbar, iconFile, tip, callback=None):
    """Adds a normal kind of tool to a toolbar and optionally adds a callback
    on it being pressed"""
    tool = toolbar.AddTool(wx.ID_ANY, "", wx.Bitmap(iconFile), tip)
    if (callback): toolbar.Bind(wx.EVT_TOOL, callback, tool)
    return tool


def addCheckTool(toolbar, iconFile, tip, callback=None):
    """Adds a tool that has a checkbox type of interaction"""
    id = wx.ID_ANY
    tool = toolbar.AddTool(id, "", wx.Bitmap(iconFile), tip, wx.ITEM_CHECK)
    if (callback): toolbar.Bind(wx.EVT_TOOL, callback, tool)
    return tool


def addRadioTool(toolbar, iconFile, tip, callback=None):
    """Adds a radio button tool to a toolbar"""
    id = wx.ID_ANY
    tool = toolbar.AddRadioTool(id, "", wx.Bitmap(iconFile), shortHelp=tip)
    if (callback): toolbar.Bind(wx.EVT_TOOL, callback, tool)
    return tool


def createProject():
    """Creates a new project in the current working directory and returns a
    default project object containing a default game object"""
    os.mkdir("sprites")
    os.mkdir("actors")
    os.mkdir("images")
    os.mkdir("battles")
    os.mkdir("ext")
    project = model.Project(model.Game(model.VERSION), model.VERSION)
    util.saveXml(project.toXml(), "project.xml")
    return project


def openProject(path):
    """Moves to the given path and tries to open the project file there. If it
    works then it will return a project object, and if not then it will return
    jack shit"""
    os.chdir(path)
    game = model.Game(model.VERSION)
    project = model.Project(game, model.VERSION)
    return project


def saveProject():
    pass


class LevelSideView(wx.Panel):
    """Controls the panel that goes on the side of the main screen which lets
    you select entities to insert, change levels to look at, and modify the
    tree of entities"""
    def __init__(self, parent, pics):
        wx.Panel.__init__(self, parent)
        topTabs = wx.Notebook(self)
        self.spriteSelector = control.SpriteSelector(topTabs, "actors", pics)
        self.entityList = control.EntityList(topTabs)
        topTabs.AddPage(self.spriteSelector, "Actor Sprites")
        topTabs.AddPage(self.entityList, "Entities")
        topTabs.SetMinSize((196, 300))
        bottomTabs = wx.Notebook(self)
        self.levelTree = control.LevelTree(bottomTabs)
        bottomTabs.AddPage(self.levelTree, "Levels")
        bottomTabs.SetMinSize((196, 300))
        vbox = wx.BoxSizer(wx.VERTICAL)
        vbox.Add(topTabs)
        vbox.Add(bottomTabs)
        self.SetSizer(vbox)


class MainFrame(wx.Frame):
    """The core window of the editor as a whole"""
    def __init__(self, parent):
        wx.Frame.__init__(
            self,
            parent,
            title="Tiger Shark: Extreme Sex Edition"
        )
        self._project = None
        self.pics = util.PicStore()
        self.SetIcon(wx.Icon("icon.ico"))
        self.SetMinSize((1024, 600))
        self.SetSize((1024, 600))
        self.setUpMenus()
        self.tabs = wx.Notebook(self)
        self.sideView = LevelSideView(self, self.pics)
        self.levelView = control.LevelView(
            self.tabs,
            self.sideView.spriteSelector.getSelected
        )
        self.codeView = control.LuaEditor(self.tabs)
        self.tabs.AddPage(self.levelView, "Level")
        self.tabs.AddPage(self.codeView, "Code")
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        hbox.Add(self.sideView)
        hbox.Add(self.tabs, wx.ID_ANY, wx.EXPAND | wx.ALL)
        self.SetSizer(hbox)
        self.Bind(
            control.SpriteSelector.EVT_SELECT,
            self.onSprite,
            self.sideView.spriteSelector
        )
        self.Bind(
            wx.EVT_TREE_ITEM_ACTIVATED,
            self.onLevel,
            self.sideView.levelTree
        )
        self.Bind(
            control.EntityList.EVT_CHANGE,
            self._onLevelChange,
            self.sideView.entityList
        )
        self.Bind(control.LevelView.EVT_NEW, self._onNewEntity, self.levelView)
        self.Show(True)

    def setUpMenus(self):
        """Just sets up the menus on the main window. Essentially an extension
        of the constructor with a lot of state"""
        self.CreateStatusBar()
        fileMenu = wx.Menu()
        openItem = fileMenu.Append(wx.ID_OPEN, "&Open", "Open script")
        aboutItem = fileMenu.Append(
            wx.ID_ABOUT,
            "&About",
            "Information about the program"
        )
        fileMenu.AppendSeparator()
        exitItem = fileMenu.Append(wx.ID_EXIT, "E&xit", "Terminate")
        menuBar = wx.MenuBar()
        menuBar.Append(fileMenu, "&File")
        self.Bind(wx.EVT_MENU, self._onOpen, openItem)
        self.Bind(wx.EVT_MENU, self._onAbout, aboutItem)
        self.Bind(wx.EVT_MENU, self.onExit, exitItem)
        self.SetMenuBar(menuBar)
        self.toolbar = self.CreateToolBar()
        addTool(self.toolbar, "newIcon.png", "create a new tiger shark project", self._onNew)
        addTool(self.toolbar, "openIcon.png", "open an existing tiger shark project", self._onOpen)
        addTool(self.toolbar, "saveIcon.png", "save the current project")
        self.toolbar.AddSeparator()
        addTool(self.toolbar, "cursorIcon.png", "normal pointer", self.onCursor)
        addTool(self.toolbar, "dragIcon.png", "drag selected entity", self.onDrag)
        addTool(self.toolbar, "scaleIcon.png", "scale selected entity", self.onScale)
        addTool(self.toolbar, "pointIcon.png", "insert point", self.onPoint)
        addTool(self.toolbar, "shapeIcon.png", "insert shape", self.onShape)
        addTool(self.toolbar, "splitIcon.png", "split selected shape edge")
        addTool(self.toolbar, "joinIcon.png", "join selected shape edge with next one")
        self.toolbar.AddSeparator()
        addCheckTool(self.toolbar, "iconShowNames.png", "show entity names", self.onShowNames)
        addTool(self.toolbar, "centreIcon.png", "centre")
        addTool(self.toolbar, "zoomOutIcon.png", "Zoom Out")
        addTool(self.toolbar, "zoomInIcon.png", "Zoom In")
        self.toolbar.AddSeparator()
        addTool(self.toolbar, "dbIcon.png", "Database", self.onDatabase)
        addTool(self.toolbar, "playIcon.png", "Run Game", self.onPlay)
        self.toolbar.Realize()

    def setProject(self, project):
        self._project = project
        self.sideView.spriteSelector.refresh()
        self.sideView.levelTree.setGame(project.game)
        # TODO: probably gonna have to set some stuff up here

    def setViewMode(self, mode):
        """Sets the mode of the level view and if it's not sprite mode also
        deselects everything in the sprite selector"""
        if mode != control.LevelView.EDIT_SPRITE:
            self.sideView.spriteSelector.deselect()
        self.levelView.setMode(mode)

    def warn(self, *lines):
        """Sends the user a warning message"""
        message = "\n".join(lines)
        dialog = wx.MessageDialog(self, message, "Warning", wx.OK)
        dialog.ShowModal()
        dialog.Destroy()

    def warnNoProject(self):
        self.warn(
            "You are not editing a project",
            "Create a new Project or open an existing one"
        )

    def newProject(self, path):
        """Moves to the given path and creates a project there unless there is
        already shit there in which case it complains and stops"""
        if not util.empty(path):
            self.warn("New project must be created in empty folder")
            return
        os.chdir(path)
        self.setProject(createProject())

    def _onAbout(self, event):
        dialog = wx.MessageDialog(self, ABOUT_TEXT, "about", wx.OK)
        dialog.ShowModal()
        dialog.Destroy()

    def _onNew(self, event):
        dialog = wx.DirDialog(self, "Choose project directory (must be empty)")
        if dialog.ShowModal() == wx.ID_OK: self.newProject(dialog.GetPath())
        dialog.Destroy()

    def _onOpen(self, event):
        dialog = wx.FileDialog(
            self,
            "Choose file",
            "",
            "",
            "*.xml",
            wx.FD_OPEN
        )
        if dialog.ShowModal() == wx.ID_OK:
            dirname = dialog.GetDirectory()
            self.setProject(openProject(dialog.GetDirectory()))
        dialog.Destroy()

    def _onLevelChange(self, event):
        self.levelView.Refresh()

    def _onNewEntity(self, event):
        self.sideView.entityList.addEntity(event.entity)

    def onSave(self, event):
        pass

    def onExit(self, event):
        self.Close(True)

    def onCursor(self, event):
        self.setViewMode(control.LevelView.EDIT_NORMAL)

    def onSprite(self, event):
        if event.index == -1:
            if self.levelView.getMode() == control.LevelView.EDIT_SPRITE:
                self.setViewMode(control.LevelView.EDIT_NORMAL)
        else:
            self.setViewMode(control.LevelView.EDIT_SPRITE)

    def onLevel(self, event):
        level = self.sideView.levelTree.GetItemData(event.GetItem())
        self.levelView.setLevel(level)
        self.sideView.entityList.setLevel(level)

    def onShape(self, event):
        self.setViewMode(control.LevelView.EDIT_SHAPE)

    def onPoint(self, event):
        self.setViewMode(control.LevelView.EDIT_POINT)

    def onDrag(self, event):
        self.setViewMode(control.LevelView.EDIT_DRAG)

    def onScale(self, event):
        self.setViewMode(control.LevelView.EDIT_SCALE)

    def onShowNames(self, event):
        self.levelView.setShowNames(event.IsChecked())

    def onZoomOut(self, event):
        pass

    def onZoomIn(self, event):
        pass

    def onDatabase(self, event):
        if self._project:
            dialog = db.DbFrame(self, self._project.game)
            dialog.ShowModal()
            dialog.Destroy()
        else:
            self.warnNoProject()

    def onPlay(self, event):
        if self._project:
            pass
        else:
            self.warnNoProject()


app = wx.App(False)
frame = MainFrame(None)
app.MainLoop()
