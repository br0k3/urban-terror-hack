#!SCons
# $Id: extract_builder.py 23 2007-06-09 10:52:55Z ascii $
# pylint: disable-msg=W0312,C0103,W0613,F0401

"""
SCons builder for extract an archive. Written in pure Python.

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

import os, tarfile, zipfile
import SCons
from SCons.Script import Action, Builder, Dir

SUPPORTED_ARCHIVE = ['.tar', '.tar.gz', '.tar.bz2', '.zip']

def extractAction(target, source, env):
	""" Extract archive to a folder. Support tar.gz, tar.bz2 and zip format.
	Called by SCons.Action internally.

	@return: True if the extract failed, else return False
	@rtype: bool
	"""

	archive = str(source[0])
	folder = str(target[0])
	
	# Check if we need to create a folder where to extract the archive
	createFolder = False
	for src in source:
		if isinstance(src, SCons.Node.Python.Value):
			if src.get_contents() == 'CREATE_FOLDER':
				createFolder = True
			else:
				raise ValueError, 'extract builder: unknown value: ' + src.get_contents()

	if not createFolder:
		folder = os.path.dirname(folder)

	print 'Extracting', archive, 'to', folder

	if archive.endswith('.tar.bz2') or archive.endswith('.tar.gz'):
		archive = tarfile.open(archive, 'r')
		
		# Extract all files
		fileCount = 1
		member = archive.next()
		while member:
			print fileCount, 'files extracted\r',
			archive.extract(member, folder)

			fileCount += 1
			member = archive.next()
		print
	elif archive.endswith('.zip'):
		archive = zipfile.ZipFile(archive, 'r')
		
		# Create folder structure
		for member in archive.infolist():
			curdir = os.path.join(folder, member.filename)
			# If directory
			if curdir.endswith('/') and not os.path.exists(curdir):
				os.makedirs(curdir)
		
		# Extract all files
		for count, member in enumerate(archive.infolist()):
			print count, 'files extracted\r',

			# If file
			if not member.filename.endswith('/'):
				outfile = open(os.path.join(folder, member.filename), 'wb')
				outfile.write(archive.read(member.filename))
				outfile.close()
		print
	else:
		raise NotImplemented, "Can't extract: " + archive

	return False

def strFunction(target, source, env):
	""" Use to draw a text when the action is executed. But extractAction
	already show text. So this function do nothing.
	Called by SCons.Action internally.
	"""
	pass

def emitter(target, source, env):
	""" Add the directory created to the SCons clean list.
	Called by SCons.Builder internally.
	"""
	env.Clean(target, target)
	return target, source

def generate(env, **kw):
	""" Add 'Extract' builder to the environment.
	Called by SCons internally.
	"""
	
	env['BUILDERS']['Extract'] = Builder(
		action=Action(
			extractAction,
			strfunction=strFunction,
		),
		emitter=emitter,
		target_factory=Dir,
	)

def exists():
	""" Check if this builder is available. 'Extract' builder is in pure
	Python, so this builder is always available.
	Called by SCons internally.
	"""
	return True
