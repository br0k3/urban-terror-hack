#!/usr/bin/python

"""	Scons script for build urth - Urban Terror Hack. """

__author__ = "*nixCoders team"
__url__ = "http://www.nixcoders.org/"
__copyright__ = "Copyright (C) 2007 - *nixCoders team"
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

env = Environment()

# Add urth version to the environment
env['URTH_VERSION'] = open('version.def').read()

# Add needed tools
env.Tool('disttar_builder', toolpath=['scons'])

# Export env for others SCons scripts
Export('env')

# Add all others SCons scripts
env.SConscript(dirs='libs')
env.SConscript(
	dirs='src',
	build_dir='build',
	duplicate=False
)

# For create an dist archive of ghf
env.Append(
	DISTTAR_EXCLUDEEXTS=['.pyc'],
	DISTTAR_EXCLUDEDIRS=['.svn']
)
Alias('dist', [
		env.DistTar(
			target=env.subst('urth-${URTH_VERSION}-src.tar.gz'),
			source=[
				Dir('src'),
				Dir('scons'),
				"libs/SConscript",
				"libs/ghf-0.8-0.tar.gz",
				"SConstruct",
				"version.def"
			]
		),
		env.Install('#', 'build/libURTH.so'),
		env.DistTar(
			target=env.subst('urth-${URTH_VERSION}-bin.tar.gz'),
			source=[
				'build/libURTH.so',
				'README'
			]
		)
	]
)
