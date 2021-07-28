import wx, wx.lib.newevent, math
import util, model

class LuaEditor(wx.stc.StyledTextCtrl):
    """Styled Text Control specifically set up for editing lua with styling and
    event handlers to make it auto indent and shiet"""

    WHITE = wx.Colour(255, 255, 255)
    OFFWHITE = wx.Colour(236, 240, 241)
    DARKGREY = wx.Colour(26, 26, 26)
    LIGHTGREY = wx.Colour(85, 85, 85)
    MIDGREY = wx.Colour(96, 96, 96)
    MEDIUMLIGHTGREY = wx.Colour(42, 42, 42)
    MEDIUMDARKGREY = wx.Colour(34, 34, 34)
    GREEN = wx.Colour(46, 204, 113)
    AQUA = wx.Colour(26, 188, 156)
    RED = wx.Colour(231, 77, 60)
    BLUE = wx.Colour(52, 152, 219)
    YELLOW = wx.Colour(241, 196, 15)
    ORANGE = wx.Colour(230, 125, 34)

    def __init__(self, parent):
        wx.stc.StyledTextCtrl.__init__(self, parent)
        self.SetMarginWidth(0, 50)
        self.SetMarginType(0, wx.stc.STC_MARGIN_NUMBER)
        self.SetWrapMode(wx.stc.STC_WRAP_WORD)
        self.SetUseTabs(False)
        self.SetBackSpaceUnIndents(True)
        self.SetIndent(4)
        self.Bind(wx.stc.EVT_STC_CHARADDED, self._onInput)
        self.SetLexer(wx.stc.STC_LEX_LUA)
        self.SetCaretForeground(LuaEditor.WHITE)
        self.StyleSetForeground(wx.stc.STC_STYLE_DEFAULT, LuaEditor.OFFWHITE)
        self.StyleSetBackground(wx.stc.STC_STYLE_DEFAULT, LuaEditor.DARKGREY)
        self.StyleSetFont(wx.stc.STC_STYLE_DEFAULT, util.getFont())
        self.StyleClearAll()
        self.StyleSetForeground(wx.stc.STC_STYLE_LINENUMBER, LuaEditor.AQUA)
        self.StyleSetBackground(
            wx.stc.STC_STYLE_LINENUMBER,
            LuaEditor.MEDIUMDARKGREY
        )
        self.StyleSetForeground(wx.stc.STC_LUA_DEFAULT, LuaEditor.OFFWHITE)
        self.StyleSetForeground(wx.stc.STC_LUA_COMMENT, LuaEditor.MIDGREY)
        self.StyleSetForeground(wx.stc.STC_LUA_COMMENTLINE, LuaEditor.MIDGREY)
        self.StyleSetForeground(wx.stc.STC_LUA_COMMENTDOC, LuaEditor.MIDGREY)
        self.StyleSetForeground(wx.stc.STC_LUA_NUMBER, LuaEditor.BLUE)
        self.StyleSetForeground(wx.stc.STC_LUA_WORD, LuaEditor.RED)
        self.StyleSetForeground(wx.stc.STC_LUA_WORD2, LuaEditor.ORANGE)
        self.StyleSetForeground(wx.stc.STC_LUA_WORD3, LuaEditor.GREEN)
        self.StyleSetForeground(wx.stc.STC_LUA_WORD4, LuaEditor.BLUE)
        self.StyleSetForeground(wx.stc.STC_LUA_STRING, LuaEditor.YELLOW)
        self.StyleSetForeground(wx.stc.STC_LUA_CHARACTER, LuaEditor.YELLOW)
        self.StyleSetForeground(wx.stc.STC_LUA_LITERALSTRING, LuaEditor.YELLOW)
        self.StyleSetForeground(wx.stc.STC_LUA_PREPROCESSOR, LuaEditor.RED)
        self.StyleSetForeground(wx.stc.STC_LUA_OPERATOR, LuaEditor.RED)
        self.StyleSetForeground(wx.stc.STC_LUA_STRINGEOL, LuaEditor.YELLOW)
        self.StyleSetForeground(wx.stc.STC_LUA_LABEL, LuaEditor.GREEN)
        self.StyleSetForeground(wx.stc.STC_LUA_IDENTIFIER, LuaEditor.OFFWHITE)
        self.SetKeyWords(0, "function end do then if for else while")
        self.SetKeyWords(1, "return local")
        self.SetKeyWords(2, "goto in and or not")
        self.SetKeyWords(3, "nil true false")

    def _onInput(self, event):
        key = event.GetKey()
        if key != 10 and key != 13: return
        line = self.GetCurrentLine()
        if line == 0: return
        indentation = math.floor(self.GetLineIndentation(line - 1) / 4)
        for i in range(indentation): self.Tab()

class SpriteSelector(wx.ScrolledWindow):
    """Creates a nice list of sprites for you to select one of. The sprites are
    populated from an external folder and you can refresh it"""

    SelectEvent, EVT_SELECT = wx.lib.newevent.NewCommandEvent()

    def __init__(self, parent, folder, pics):
        wx.ScrolledWindow.__init__(self, parent)
        self._buttons = []
        self._folder = folder
        self._selected = -1
        self._pics = pics
        self._wrap = wx.WrapSizer()
        self.SetSizer(self._wrap)
        self.SetScrollRate(5, 5)
        refreshButton = wx.Button(self, wx.ID_ANY, "Refresh")
        refreshButton.Bind(wx.EVT_BUTTON, lambda event: self.refresh())
        self._wrap.Add(refreshButton)
        self._populateButtons()

    def getSelected(self):
        """Get the file that the sprite selector has got selected right now as
        a string. Keep in mind that the sprite selector might have nothing
        selected in which case this will return None"""
        if self._selected == -1: return None
        return self._pics.sprite(self._buttons[self._selected].file)

    def deselect(self):
        """Sets all buttons as deselected"""
        self._selected = -1
        for button in self._buttons: button.SetValue(False)

    def refresh(self):
        """Refinds all sprites and puts them in new buttons and sets nothing as
        selected and sends an event saying nothing is selected"""
        self._selected = -1
        self._populateButtons()
        wx.PostEvent(
            self,
            SpriteSelector.SelectEvent(self.GetId(), index=-1)
        )

    def _populateButtons(self):
        for button in self._buttons: button.Destroy()
        self._buttons = []
        for i, file in enumerate(util.findFiles(self._folder, ".png")):
            index = i
            bitmap = self._pics.pic(file)
            button = wx.BitmapToggleButton(self, wx.ID_ANY, bitmap)
            button.SetToolTip(file)
            button.SetMaxSize((100, 100))
            self._wrap.Add(button)
            button.Bind(
                wx.EVT_TOGGLEBUTTON,
                (lambda i: lambda e: self._buttonPressed(i))(i)
            )
            button.file = file
            self._buttons.append(button)
        self._wrap.Layout()

    def _buttonPressed(self, index):
        for i, button in enumerate(self._buttons): button.SetValue(i == index)
        self._selected = index
        wx.PostEvent(
            self,
            SpriteSelector.SelectEvent(self.GetId(), index=index)
        )


class LevelView(wx.Panel):
    """The bit that actually shows the level and lets you interact with it"""

    NewEvent, EVT_NEW = wx.lib.newevent.NewCommandEvent()
    EDIT_NORMAL = 0
    EDIT_SPRITE = 1
    EDIT_SHAPE = 2
    EDIT_POINT = 3
    EDIT_DRAG = 4
    EDIT_SCALE = 5

    def __init__(self, parent, getSprite):
        wx.Panel.__init__(self, parent)
        self._mode = LevelView.EDIT_NORMAL
        self._showNames = False
        self._getSprite = getSprite
        self._level = None
        self._selected = -1
        self.Bind(wx.EVT_PAINT, self._onPaint)
        self.Bind(wx.EVT_LEFT_DOWN, self._onClick)

    def setMode(self, mode):
        """Sets the level view's mode and does updating stuff that happens when
        the mode changes"""
        self._mode = mode
        self.Refresh()

    def getMode(self):
        """Tells you the current edit mode"""
        return self._mode

    def setShowNames(self, show):
        """Sets whether the level should show entity names or not, then
        rerenders"""
        self._showNames = show
        self.Refresh()

    def setLevel(self, level):
        self._level = level
        self.Refresh()

    def _onPaint(self, event):
        dc = wx.PaintDC(self)
        if self._level:
            for i, entity in enumerate(self._level.getEntities()):
                entity.render(dc, self._selected == i, self._showNames)
            if self._mode == LevelView.EDIT_SPRITE:
                dc.DrawText("SPRITE INSERT MODE", 0, 0)
            elif self._mode == LevelView.EDIT_SHAPE:
                dc.DrawText("SHAPE INSERT MODE", 0, 0)
            elif self._mode == LevelView.EDIT_POINT:
                dc.DrawText("POINT INSERT MODE", 0, 0)
            elif self._mode == LevelView.EDIT_DRAG:
                dc.DrawText("DRAG MODE", 0, 0)
            elif self._mode == LevelView.EDIT_SCALE:
                dc.DrawText("SCALE MODE", 0, 0)
        else:
            dc.DrawText("NO LEVEL OPEN", 0, 0)

    def _onClick(self, event):
        if not self._level: return
        dc = wx.ClientDC(self)
        pos = event.GetLogicalPosition(dc)
        if self._mode == LevelView.EDIT_POINT:
            entity = model.Entity()
            entity.pos = pos
            self._addEntity(entity)
        elif self._mode == LevelView.EDIT_SPRITE:
            sprite = self._getSprite()
            if sprite == None:
                pass
            else:
                entity = model.SpriteEntity(sprite)
                entity.pos = pos
            self._addEntity(entity)

    def _addEntity(self, entity):
        self._level.addEntity(entity)
        self._selected = len(self._level.getEntities()) - 1
        wx.PostEvent(self, LevelView.NewEvent(self.GetId(), entity=entity))
        self.Refresh()


class LevelTree(wx.TreeCtrl):
    """Shows all of the levels in the game and also controls creating,
    deleting, and moving them"""

    CHOICE_INSERT = 1
    CHOICE_PROPERTIES = 2
    CHOICE_DELETE = 3

    def __init__(self, parent):
        wx.TreeCtrl.__init__(
            self,
            parent,
            style=wx.TR_EDIT_LABELS|wx.TR_HAS_BUTTONS
        )
        self._root = None
        self._game = None
        self.Bind(wx.EVT_TREE_ITEM_MENU, self._onRightClick)

    def setGame(self, game):
        """Sets the game object the level tree is showing levels for and
        regenerates the tree to show the new game's levels"""
        self._game = game
        self._root = self.AddRoot(game.name)
        for level in self._game.levels:
            level.id = self.AppendItem(self._root, level.name, data=level)
            self._addChildren(level)

    def _addChildren(self, level):
        for child in level.getChildren():
            child.id = self.AppendItem(root, child.name, data=child)
            self._addChildren(child)

    def _onRightClick(self, event):
        item = event.GetItem()
        menu = wx.Menu()
        menu.Append(LevelTree.CHOICE_INSERT, "insert level")
        if item != self._root:
            menu.Append(LevelTree.CHOICE_PROPERTIES, "level properties")
            menu.Append(LevelTree.CHOICE_DELETE, "delete level")
        selection = self.GetPopupMenuSelectionFromUser(menu)
        menu.Destroy()
        if selection == LevelTree.CHOICE_INSERT:
            level = self.GetItemData(item)
            newLevel = model.Level()
            if level:
                level.addChild(newLevel)
                self.AppendItem(item, newLevel.name, data=newLevel)
            else:
                self._game.levels.append(newLevel)
                self.AppendItem(self._root, newLevel.name, data=newLevel)
        if selection == LevelTree.CHOICE_PROPERTIES:
            print("properties")
        elif selection == LevelTree.CHOICE_DELETE:
            print("delete")


class EntityList(wx.TreeCtrl):
    """Shows all the entities in a level and lets you select and delete them"""

    SelectEvent, EVT_SELECT = wx.lib.newevent.NewCommandEvent()
    ChangeEvent, EVT_CHANGE = wx.lib.newevent.NewCommandEvent()

    def __init__(self, parent):
        wx.ListBox.__init__(
            self,
            parent,
            style=wx.TR_EDIT_LABELS|wx.TR_NO_BUTTONS|wx.TR_HIDE_ROOT
        )
        self._level = None
        self._root = self.AddRoot("")
        self.Bind(wx.EVT_TREE_ITEM_MENU, self._onRightClick)

    def setLevel(self, level):
        """Sets the level that the list is showing entities for"""
        self._level = level
        self.Unselect()
        self.DeleteChildren(self._root)
        for entity in level.getEntities():
            self.AppendItem(self._root, entity.name, data=entity)

    def addEntity(self, entity):
        """Tells the entity list to add a new entity on to the end because it
        has been added to the level"""
        self.AppendItem(self._root, entity.name, data=entity)
        # TODO: should probably make it selected by default

    def _onRightClick(self, event):
        item = event.GetItem()
        menu = wx.Menu()
        menu.Append(69, "Delete entity")
        selection = self.GetPopupMenuSelectionFromUser(menu)
        menu.Destroy()
        if selection == 69:
            self._level.removeEntity(self.GetItemData(item))
            wx.PostEvent(self, EntityList.ChangeEvent(self.GetId()))
            self.Delete(item)
