#A Simple Project Test
project('XeRelease', 'cpp',
        default_options : ['c_std=c17', 'cpp_std=c++17'])

#Initalize variants
gnome = import('gnome')

#Compile Resource 
gresources = gnome.compile_resources(
    'resources', 'res/xerelease.gresource.xml',
    source_dir: 'res',
    c_name: 'resources'
)

#source files
src=['src/main.cc', 'src/MyWin.cc', 'src/MyPrefs.cc', 
     'src/xeapi.cc', 'src/xerelease.cc']

#The Gtkmm Library as a dependency
gtkdep = dependency('gtkmm-4.0')

#Additional include dirs
dir_include = include_directories('..')

#Use Different Build Opinions in windows and Linux
if host_machine.system() == 'windows'
    win=import('windows')
    icon_res=win.compile_resources('icon.rc')
executable('XeRelease', icon_res, src, gresources ,dependencies : gtkdep, 
    win_subsystem : 'windows', include_directories : dir_include)
else
    executable('XeRelease', src, gresources, dependencies : gtkdep, include_directories : dir_include)
endif
