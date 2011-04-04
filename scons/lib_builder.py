#!SCons
# $Id: lib_builder.py 23 2007-06-09 10:52:55Z ascii $
# pylint: disable-msg=W0312,C0103,W0613,F0401

"""
SCons Builder for install an external library. This builder depends on
'Download' and 'Extract' builders.

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

import os, re
from SCons.Script import Alias, Builder, Dir, Exit, File, Help
from SCons.Script import Options, PathOption, Value

def _substituteVars(lib):
	""" Replace all $(PROPERTIES) by the correct value.

	@todo: This function works but looks too dirty.
	"""
	# Compile the regexp only one time
	propertyRe = re.compile('\$\(([A-Z0-9_]+)\)')

	def _subst(text):
		""" Substitute $(PROPERTY) by is value. """
		# Only on string
		if type(text) != str:
			return text, False

		# Search the pattern by regexp
		m = propertyRe.search(text)
		if not m:
			return text, False

		# Replace the value
		return text.replace(m.group(0), lib[m.group(1)]), True
	
	# w00t
	finish = False
	while not finish:
		finish = True
		# Main key
		for key, value in lib.iteritems():
			lib[key], found = _subst(value)
			if found:
				finish = False
		# 'CMDS' can have value to substitute too
		for cmd in lib['CMDS']:
			for key, value in cmd.iteritems():
				# 'CMDS' value can be a list
				if type(value) == list:
					for index, cm in enumerate(value):
						value[index], found = _subst(cm)
						if found:
							finish = False
				else:
					cmd[key], found = _subst(value)
					if found:
						finish = False

def emitter(target, source, env):
	""" Add all dependencies to this library.
	Called by SCons.Builder internally.

	@todo: Check for 'TARGET' and 'CMD' in all commands.
	@todo: Add "Value('CREATE_FOLDER')" check for extract
	"""
	# Check if this library definition exists
	if not env['LIB_BUILDER'].has_key(source[0].get_contents()):
		print 'Error:', source[0].get_contents(), 'is not defined.'
		Exit(1)
	
	lib = env['LIB_BUILDER'][source[0].get_contents()]
	
	# Add all lib properties as source value dependency
	source.append(Value(lib))
	
	_substituteVars(lib)
	
	if lib.has_key('URL'):
		download = env.Download(
			target=lib['ARCHIVE'],
			source=lib['URL']
		)
	else:
		download = lib['ARCHIVE']
	
	extract = env.Extract(
		target=lib['FOLDER'],
		source=download
	)
	
	# Add dependencies for all CMDS
	for cmd in lib['CMDS']:
		# Get command working directory
		if cmd.has_key('CWD'):
			folder = os.path.join(Dir('.').path, cmd['CWD'])
		else:
			folder = os.path.join(Dir('.').path, lib['FOLDER'])

		if isinstance(cmd['TARGET'], list):
			cmdTarget = map(File, cmd['TARGET'])
		else:
			# TODO: Find why SCons add wierd dependencies when 'File' is use 
			# instead of 'Value'
			cmdTarget = Value(cmd['TARGET'])

		source.append(
			env.Command(
				action=cmd['CMD'],
				target=cmdTarget,
				source=extract,
				chdir=folder
			)
		)

	return target, source

def action(target, source, env):
	""" Do nothing. This builder only add sources. """
	return None

def generate(env, **kw):
	""" Add this builder to the 'env'.
	Called by SCons internally.
	"""
	# Check for needed builders, else add it
	if not env['BUILDERS'].has_key('Download'):
		env.Tool('download_builder', toolpath=['../scons'])
	if not env['BUILDERS'].has_key('Extract'):
		env.Tool('extract_builder', toolpath=['../scons'])

	# Add command line option to the environment
	opts = Options(None)
	opts.Add(
		PathOption(
			'LIBS_PREFIX',
			help='Where all libraries are installed',
			default=os.path.join(Dir('.').abspath, 'local'),
			validator=PathOption.PathAccept
		)
	)
	opts.Update(env)
	# Generate command line help
	Help(opts.GenerateHelpText(env))

	# Init LibBuilder properties repository
	if not env.has_key('LIB_BUILDER'):
		env['LIB_BUILDER'] = dict()
	
	# Add this builder to the env
	env['BUILDERS']['LibBuilder'] = Builder(
		action=action,
		emitter=emitter,
		target_factory=Alias,
		source_factory=Value,
	)

def exists():
	""" Check if this builder is available. 'LibBuilder' builder is in pure
	Python, so this builder is always available.
	Called by SCons internally.
	"""
	return True
