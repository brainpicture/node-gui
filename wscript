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

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon", uselib = 'GTK')
  obj.cxxflags = ["-Wall", "-ansi", "-pedantic"]
  obj.target = "gtk"
  obj.source = """
               src/ngtk_entry.cc
               src/ngtk_button.cc
               src/ngtk_message_dialog.cc
               src/ngtk_vbox.cc
               src/ngtk_hbox.cc
               src/ngtk_box.cc
               src/ngtk_window.cc
               src/ngtk_container.cc
               src/ngtk_widget.cc
               src/ngtk.cc
               """
  obj.uselib = "GTK GLIB"
