import wx, os
import xml.etree.ElementTree as et


VERSION = "0.1.0"


class Item:
    def __init__(self, name=""):
        self.name = name
        self.sprite = ""

    def __str__(self):
        return self.name


class Entity:
    """Default type of entity which is just a point with a scale and a name"""
    def __init__(self, name="point"):
        self.name = name
        self.pos = wx.Point()
        self.scale = 1
        self.id = 0

    def render(self, dc, selected=False, showName=False):
        point = wx.Point(10, 0)
        dc.DrawLine(self.pos - point , self.pos + point)
        point.y = 10
        point.x = 0
        dc.DrawLine(self.pos - point , self.pos + point)
        if showName:
            dc.DrawText(self.name, self.pos)


class SpriteEntity(Entity):
    """Entity that draws a sprite"""
    def __init__(self, sprite):
        Entity.__init__(self, sprite.name)
        self._sprite = sprite

    def render(self, dc, selected=False, showName=False):
        dc.DrawBitmap(self._sprite.bitmap, self.pos - self._sprite.bitmap.GetSize() / 2)
        if selected:
            dc.SetPen(wx.Pen('#4c4c4c', 1, wx.SOLID))
            dc.SetBrush(wx.NullBrush)
            dc.DrawRectangle(
                self.pos - self._sprite.bitmap.GetSize() / 2,
                self._sprite.bitmap.GetSize()
            )
        if showName:
            dc.DrawText(self.name, self.pos - self._sprite.bitmap.GetSize() / 2)


class Level():
    """Represents a game level, which is a background image and a bunch of
    entities arranged on top of it, plus a script which controls what the
    entities do, and some config like the music etc"""
    def __init__(self):
        self._entities = []
        self._children = []
        self.name = "brexit"
        self.script = ""
        self.background = None
        self.id = 0

    def GetId(self):
        return self.id

    def addEntity(self, entity):
        """Adds an entity to the level and makes it's name unique if it wasn't
        unique within the level"""
        entity.name = self._findNewName(entity.name, self._entities)
        self._entities.append(entity)

    def removeEntity(self, entity):
        """Removes an entity from the level if it is present there"""
        self._entities.remove(entity)

    def addChild(self, level):
        """Adds a child level to this level and finds it an appropriate name"""
        level.name = self._findNewName(level.name, self._children)
        self._children.append(level)

    def getEntities(self):
        """Just use this for iterating. don't be naughty"""
        return self._entities

    def getChildren(self):
        """Only use it for iterating be good"""
        return self._children

    def _findNewName(self, name, others):
        addition = None
        while True:
            hit = False
            newName = name
            if addition: newName += str(addition)
            for other in others:
                if other.name == newName:
                    hit = True
                    break
            if not hit:
                return newName
            elif addition == None:
                addition = 1
            else:
                addition += 1


class Game:
    """Represents the overall state of things that are to be sent to the actual
    game engine, besides stuff that is implicitly in their folders like images
    etc"""
    def __init__(self, version):
        self.version = version
        self.items = []
        self.levels = [
            Level(),
            Level(),
            Level(),
            Level()
        ]
        self.name = "new game"
        self.copyright = "by hernia"

    def toXml(self):
        """Returns an xml representation of the game"""
        game = et.Element("game", {"version": self.version})
        name = et.SubElement(game, "name")
        copyright = et.SubElement(game, "copyright")
        name.text = self.name
        copyright.text = self.copyright
        return game

class Project:
    """Represents state that is saved to disk, but that does not persist into
    the game itself"""
    def __init__(self, game, version):
        self.version = version
        self.game = game

    def toXml(self):
        """Returns an xml representation of the project"""
        project = et.Element("project", {"version": self.version})
        project.append(self.game.toXml())
        return project
