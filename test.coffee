gtk = require './build/default/gtk'

# Event loop
main = ->
  if not gtk.main()
    setTimeout ->
      main()
    , 20

window = new gtk.Window
button = new gtk.Button
dialog = new gtk.MessageDialog(
  window
  gtk.DIALOG_DESTROY_WITH_PARENT
  gtk.MESSAGE_INFO
  gtk.BUTTONS_YES_NO
  'This is an alert!'
)

# Start the loop when we show the window.
window.onShow = main
window.setTitle     'Coffee + GTK'
window.setResizable yes
window.setOpacity   0.9
window.setDefaultSize()
window.onDestroy = ->
  console.log window.getOpacity()
  console.log window.getPosition()
  console.log window.getSize()
  console.log window.getResizable()
  console.log window.getTitle()
  console.log 'OMG. Everything dies.'

button.setLabel 'Test Button.'
button.onClick = ->
  console.log 'Button was clicked'
window.add button

window.show()

dialog.run()
dialog.destroy()
