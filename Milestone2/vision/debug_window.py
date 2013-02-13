__author__ = 'Stefan'

from Tkinter import *

class DebugWindow(object):
    def __init__(self):
        root = Tk()
        self.w = Text(root)
        self.w.pack()
        root.mainloop()

    def insert_text(self, text):
        self.w.insert(END, text+'\n')

if __name__ == '__main__':
    DebugWindow()
