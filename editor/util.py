import os, wx
import xml.etree.ElementTree as et


class Sprite:
    def __init__(self, name, bitmap):
        self.name = name
        self.bitmap = bitmap


class PicStore():
    """Stores bitmaps so you can load them again later without getting them
    from disk"""
    def __init__(self):
        self.pics = {}

    def pic(self, name):
        """Gets a pic"""
        if not name in self.pics: self.pics[name] = wx.Bitmap(name)
        return self.pics[name]

    def sprite(self, name):
        """Same as pic but it returns a sprite object that stores the origin of
        the image so it can be recorded"""
        pic = self.pic(name)
        return Sprite(name, pic)


def empty(path):
    """Tells you if the given folder is empty, ignoring hidden files because
    I am ballsy"""
    for file in os.listdir(path):
        if not file.startswith("."): return False
    return True


def findFiles(folder, ends):
    """Finds all the files in the given folder that have one of the given
    extensions, and allows you to iterate over the lot of them"""
    if not os.path.exists(folder): return
    if not isinstance(ends, list): ends = [ends]
    for file in os.listdir(folder):
        accepted = False
        for end in ends:
            if file.endswith(end):
                accepted = True
                break
        if accepted:
            yield os.path.join(folder, file)


def getFont():
    """Gets a monospace font to use with code by looking up what monospace
    fonts the system has and just choosing the first one it finds. I guess this
    will crash if there are absolutely no monospace fonts on your system"""
    fonts = wx.FontEnumerator.GetFacenames(fixedWidthOnly=True)
    return wx.Font(wx.FontInfo(14).FaceName(fonts[0]).AntiAliased(False))


def saveXml(xml, file):
    """Saves an xml node into a file"""
    with open(file, "wb") as out: out.write(et.tostring(xml))
