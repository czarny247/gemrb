# -*-python-*-
# GemRB - Infinity Engine Emulator
# Copyright (C) 2003-2004 The GemRB Project
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# $Header: /data/gemrb/cvs2svn/gemrb/gemrb/gemrb/GUIScripts/tob/GUIJRNL.py,v 1.5 2004/10/23 13:01:43 avenger_teambg Exp $


# GUIJRNL.py - scripts to control journal/diary windows from GUIJRNL winpack

# GUIJRNL:
# 0 - main journal window
# 1 - quests window
# 2 - beasts window
# 3 - log/diary window

###################################################
import GemRB
from GUIDefines import *

###################################################
JournalWindow = None
LogWindow = None
QuestsWindow = None
Section = 0
Chapter = 0
Order = 0

###################################################
def OpenJournalWindow ():
	global JournalWindow, Chapter

	GemRB.HideGUI()


	if JournalWindow:
		if LogWindow: OpenLogWindow()
		if QuestsWindow: OpenQuestsWindow()
		
		GemRB.HideGUI()
		
		GemRB.UnloadWindow(JournalWindow)
		JournalWindow = None
		GemRB.SetVar("OtherWindow", -1)
		
		GemRB.UnhideGUI()
		return
		
	GemRB.LoadWindowPack ("GUIJRNL")
	JournalWindow = GemRB.LoadWindow (2)
	GemRB.SetVar("OtherWindow", JournalWindow)

	# prev. chapter
	Button = GemRB.GetControl (JournalWindow, 3)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "PrevChapterPress")

	# next chapter
	Button = GemRB.GetControl (JournalWindow, 4)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "NextChapterPress")

	# Quests
	Button = GemRB.GetControl (JournalWindow, 6)
	GemRB.SetVarAssoc(JournalWindow, Button, "Section", 1)
	GemRB.SetText (JournalWindow, Button, 45485)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "UpdateLogWindow")

	# Quests completed
	Button = GemRB.GetControl (JournalWindow, 7)
	GemRB.SetVarAssoc(JournalWindow, Button, "Section", 2)
	GemRB.SetText (JournalWindow, Button, 45486)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "UpdateLogWindow")

	# Journal
	Button = GemRB.GetControl (JournalWindow, 8)
	GemRB.SetVarAssoc(JournalWindow, Button, "Section", 4)
	GemRB.SetText (JournalWindow, Button, 15333)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "UpdateLogWindow")

	# User
	Button = GemRB.GetControl (JournalWindow, 9)
	GemRB.SetVarAssoc(JournalWindow, Button, "Section", 0)
	GemRB.SetText (JournalWindow, Button, 45487)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "UpdateLogWindow")

	# Order
	Button = GemRB.GetControl (JournalWindow, 10)
	GemRB.SetText (JournalWindow, Button, 4627)
	GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "ToggleOrderWindow")

	# Done
	#Button = GemRB.GetControl (JournalWindow, 3)
	#GemRB.SetText (JournalWindow, Button, 20636)
	#GemRB.SetEvent (JournalWindow, Button, IE_GUI_BUTTON_ON_PRESS, "OpenJournalWindow")

	Section = 0
	Chapter = GemRB.GetGameVar("chapter")
	GemRB.UnhideGUI()
	UpdateLogWindow()
	return

def ToggleOrderWindow ():
	global Order

	if Order:
		Order = 0
	else:
		Order = 1
	UpdateLogWindow ()
	return

def UpdateLogWindow ():

	# text area
	Window = JournalWindow

	GemRB.HideGUI()
	Section = GemRB.GetVar("Section")
	GemRB.SetToken("CurrentChapter", str(Chapter) )
	# CurrentChapter
	Label = GemRB.GetControl (JournalWindow, 0x1000000a)
	GemRB.SetText (JournalWindow, Label, 15873)
	print "Chapter ", Chapter, "Section ", Section

	Text = GemRB.GetControl (Window, 1)

	GemRB.SetText (Window, Text, "")
	for i in range (GemRB.GetJournalSize (Chapter, Section)):
		je = GemRB.GetJournalEntry (Chapter, i, Section)

		if je == None:
			continue

		# FIXME: the date computed here is wrong by approx. time
		#   of the first journal entry compared to journal in
		#   orig. game. So it's probably computed since "awakening"
		#   there instead of start of the day.

		date = str (1 + int (je['GameTime'] / 86400))
		time = str (je['GameTime'])
		
		GemRB.TextAreaAppend (Window, Text, "[color=808000]Day " + date + '  (' + time + "):[/color]", 3*i)
		GemRB.TextAreaAppend (Window, Text, je['Text'], 3*i + 1)
		GemRB.TextAreaAppend (Window, Text, "", 3*i + 2)
	GemRB.UnhideGUI()
	return

###################################################
def PrevChapterPress ():
	global Chapter 

	if Chapter > 0:
		Chapter = Chapter - 1
		GemRB.SetToken("CurrentChapter", str(Chapter) )
		UpdateLogWindow ()
	return

###################################################
def NextChapterPress ():
	global Chapter

	if Chapter < GemRB.GetGameVar("chapter"):
		Chapter = Chapter + 1
		GemRB.SetToken("CurrentChapter", str(Chapter) )
		UpdateLogWindow ()
	return

###################################################
# End of file GUIJRNL.py
