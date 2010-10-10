srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.check_cfg(package='gtk+-2.0', uselib_store='GTK', args='--cflags --libs')
  conf.check_cfg(package='glib-2.0', args='--cflags --libs', uselib_store='GLIB')
  conf.check_cfg(package='glibmm-2.4', args='--cflags --libs', uselib_store='GLIBMM')
  conf.env.append_value('CXXFLAGS', 
    '-DNDEBUG -DGTK -DGTK2 -DSCI_LEXER -DG_THREADS_IMPL_NONE'.split())

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon", uselib = 'GTK')
  obj.cxxflags = ["-Wall", "-ansi", "-pedantic"]
  obj.target = "gtk"
  obj.source = """
               src/ngtk_window.cc
               src/ngtk_message_dialog.cc
               src/ngtk_button.cc
               src/ngtk.cc
               """
  obj.uselib = "GTK GLIB GLIBMM"
