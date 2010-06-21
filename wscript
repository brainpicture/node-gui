srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.check_cfg(package='gtk+-2.0', atleast_version='2.8.0', uselib_store='GTK',
		mandatory=True, args='--cflags --libs')
  conf.env.append_value('CXXFLAGS', 
		'-DNDEBUG -DGTK -DGTK2 -DSCI_LEXER -DG_THREADS_IMPL_NONE'.split())

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "pthread", "node_addon",uselib = 'GTK')
  obj.packages='gtk+-2.0'
  obj.target = "node-gtk"
  obj.source = """
	       bindings/gtk-window.cc
	       bindings/gtk-hbox.cc
	       bindings/gtk-vbox.cc
	       bindings/gtk-button.cc
	       bindings/gtk-alert.cc
	       bindings/gtk-entry.cc
  	       bindings/gtk.cc
  	       """
