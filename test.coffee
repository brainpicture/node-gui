gtk = require './build/default/gtk'

process.nextTick ->
  gtk.init()
  window = new gtk.Window
  button = new gtk.Button
  dialog = new gtk.MessageDialog(
    window
    gtk.DIALOG_DESTROY_WITH_PARENT
    gtk.MESSAGE_INFO
    gtk.BUTTONS_YES_NO
    'This is an alert!'
  )
  window.container = new gtk.Vbox()
  window.add(window.container)


  window.show()
