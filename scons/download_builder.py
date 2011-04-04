#!SCons
# $Id: download_builder.py 23 2007-06-09 10:52:55Z ascii $
# pylint: disable-msg=W0312,C0103,W0613,F0401

"""
SCons builder for download a file. Written in pure Python.

Changelog:
16/05/2007 - Initial release.
"""

__version__ = "$Rev: 23 $"
__author__ = "Alexandre Garcia"
__email__ = "alexou (dot) garcia (at) gmail (dot) com"
__copyright__ = "Copyright (C) 2007 - Alexandre Garcia"
__license__ = """
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
"""

import os, time, urllib
from SCons.Script import Action, Builder, Value

def downloadAction(target, source, env):
	""" Download an URL to a file.
	Called by SCons.Action internally.

	@return: True if the download failed, else return False
	@rtype: bool
	
	@todo: Check exception from urllib.urlretrieve()
	@todo; Remove the dirty call to basename() for archive
	"""
	
	archive = os.path.basename(str(target[0]))
	url = source[0].get_contents()

	print 'Downloading', url

	# Create a global var for downloadHook()
	startTime = 0

	def downloadHook(currentBlock, blockSize, fileSize):
		""" Download hook used by urllib.urlretrieve().
		
		@todo: eta can be improve.
		"""
		global startTime

		# Download start
		if currentBlock == 0:
			startTime = time.time()
			return
		
		elapsedTime = time.time() - startTime
		downloaded = currentBlock * blockSize
		eta = (elapsedTime * (float(fileSize) / downloaded)) - elapsedTime

		print "Downloading: %d/%d" % (downloaded, fileSize),
		print "speed: %.2f Ko/s" % (downloaded / 1000 / elapsedTime),
		print "eta: %d:%.2d\r" % (int(eta / 60), eta % 60),

	print "Connecting to server...\r",
	filename, headers = urllib.urlretrieve(url, archive, downloadHook)
	print
	
	# Check for error
	if not filename or (not os.path.exists(archive)) or (filename != archive):
		return True
	else:
		return False

def strFunction(target, source, env):
	""" Use to draw a text when the action is executed. But downloadAction
	already showing text. So this function must do nothing.
	Called by SCons.Action internally.
	"""
	pass

def generate(env, **kw):
	""" Add 'Download' builder to the environment.
	Called by SCons internally.
	"""
	
	env['BUILDERS']['Download'] = Builder(
		action=Action(
			downloadAction,
			strfunction=strFunction,
			chdir=True
		),
		single_source=True,
		source_factory=Value
	)

def exists():
	""" Check if this builder is available. 'Download' builder is in pure
	Python, so this builder is always available.
	Called by SCons internally.
	"""
	return True
