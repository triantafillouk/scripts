import gtk, gobject, cairo
from random import randint, random
from math import pi
 
class drawingWindow (gtk.Window):
 
    __gsignals__ = {
            "expose-event"  : "override",
            "screen-changed": "override"
            }
 
    count = 0
    timeout = 1000

def __init__ (self, filename, timeout = 400):
	self.timeout = timeout
	# Create a popup window which won't take focus
	gtk.Window.__init__ (self, gtk.WINDOW_POPUP)
	# Required to paint the background
	self.set_app_paintable (True)
	# Load image file
	self.image_surface = cairo.ImageSurface.create_from_png (filename)
	self.image_width = self.image_surface.get_width ()
	self.image_height = self.image_surface.get_height ()
	# Make window as large as possible
	screen = self.get_screen ()
	width = min (1280, screen.get_width ())
	height = min (1024, screen.get_height ())
	self.set_default_size (width, height)
	# Set the colormap
	self.do_screen_changed ()
	width, height = self.get_size ()
	self.surface = cairo.ImageSurface (cairo.FORMAT_ARGB32, width, height)
	# Plug signals
	self.connect ("delete-event", gtk.main_quit)
	self.connect ("key-press-event", self.on_key_press_event)
 
	# Show the window and plug required stuff
def show (self):
	# Show the window
	gtk.Window.show (self)
	# Start timer
	gobject.timeout_add (self.timeout, self.add_image)
	# Grab keyboard
	while gtk.gdk.keyboard_grab (self.window) != gtk.gdk.GRAB_SUCCESS:
		sleep (0.1)
 
# Add a new rotated version of the image to the global surface
def add_image (self):
	cr = cairo.Context (self.surface)
	width, height = self.get_size ()
	x = randint (0, width - self.image_width)
	y = randint (0, height - self.image_height)
	cr.set_operator (cairo.OPERATOR_OVER)
	cr.set_source_surface (self.rotate_image (self.image_surface), x, y)
	cr.paint ()
	self.queue_draw ()
	return True
 
# Create a new surface containing a rotated version of the source surface
def rotate_image (self, source_surface):
	width = int (1.5 * source_surface.get_width ())
	height = int (1.5 * source_surface.get_height ())
	surface = cairo.ImageSurface (cairo.FORMAT_ARGB32, width, height)
	cr = cairo.Context (surface)
	cr.scale (width, height)
	cr.translate (0.5, 0.5)
	cr.rotate (random () * 2.0 * pi)
	cr.translate (- 0.5, - 0.5)
	cr.scale (1.0 / width, 1.0 / height)
	cr.set_source_surface (source_surface, 1.0 / 6 * width, 1.0 / 6 * height)
	cr.paint ()
	return surface 
 
# Copy the surface to the current window pixmap
def do_expose_event (self, event):
	cr = self.window.cairo_create ()
	cr.set_operator (cairo.OPERATOR_SOURCE)
	cr.set_source_rgba (0, 0, 0, 0)
	cr.paint ()
	cr.set_source_surface (self.surface)
	cr.paint ()
 
# Update colormap
def do_screen_changed (self, old_screen = None):
	screen = self.get_screen ()
	colormap = screen.get_rgba_colormap ()
	if not colormap:
		colormap = screen.get_rbg_colormap ()
	self.set_colormap (colormap)
 
# Quit when Escape key is pressed
def on_key_press_event (self, widget, event):
	if event.hardware_keycode == 9:
		gtk.main_quit ()
 
dW = drawingWindow ("heart.png")
dW.show ()
try:
	gtk.main ()
except KeyboardInterrupt:
	raise SystemExit
