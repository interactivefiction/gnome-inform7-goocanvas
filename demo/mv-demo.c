/*
 * GooCanvas Demo. Copyright (C) 2005 Damon Chaplin.
 * Released under the GNU LGPL license. See COPYING for details.
 *
 * main.c - demo app.
 */
#include <config.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <cairo.h>
#if CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>
#endif
#include <goocanvas.h>

static GooCanvasItemModel *ellipse2, *textitem;

static gboolean dragging = FALSE;
static double drag_x, drag_y;

GtkWidget *create_canvas_fifteen (void);
GtkWidget *create_canvas_features (void);
GtkWidget *create_canvas_arrowhead (void);
GtkWidget *create_canvas_scalability (void);
GtkWidget *create_grabs_page (void);
GtkWidget *create_events_page (void);
GtkWidget *create_paths_page (void);
GtkWidget *create_focus_page (void);
GtkWidget *create_animation_page (void);
GtkWidget *create_clipping_page (void);
GtkWidget *create_table_page (void);

#if CAIRO_HAS_PDF_SURFACE
static void
write_pdf_clicked (GtkWidget *button, GooCanvas *canvas)
{
  cairo_surface_t *surface;
  cairo_t *cr;

  g_print ("In write_pdf_clicked\n");

  surface = cairo_pdf_surface_create ("demo.pdf", 9 * 72, 10 * 72);
  cr = cairo_create (surface);

  /* Place it in the middle of our 9x10 page. */
  cairo_translate (cr, 20, 130);

  goo_canvas_render (canvas, cr, NULL, 1.0);

  cairo_show_page (cr);

  cairo_surface_destroy (surface);
  cairo_destroy (cr);
}
#endif

static void
zoom_changed (GtkAdjustment *adj, GooCanvas *canvas)
{
  goo_canvas_set_scale (canvas, adj->value);
}


static void
center_toggled (GtkToggleButton *button, gpointer data)
{
  /*foo_canvas_set_center_scroll_region (data,
                                   gtk_toggle_button_get_active (button));*/
}


static void
anchor_toggled (GtkWidget *button, GooCanvas *canvas)
{
  GtkAnchorType anchor;

  anchor = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (button), "anchor"));

  if (GTK_TOGGLE_BUTTON (button)->active)
    g_object_set (canvas, "anchor", anchor, NULL);
}


static void
scroll_to_50_50_clicked (GtkWidget *button, GooCanvas *canvas)
{
  goo_canvas_scroll_to (canvas, 50, 50);
}


static void
scroll_to_500_500_clicked (GtkWidget *button, GooCanvas *canvas)
{
  goo_canvas_scroll_to (canvas, 500, 500);
}


static void
scroll_to_250_250_clicked (GtkWidget *button, GooCanvas *canvas)
{
  goo_canvas_scroll_to (canvas, 250, 250);
}


static void
animate_ellipse_clicked (GtkWidget *button, GooCanvas *canvas)
{
#if 1
  goo_canvas_item_model_animate (ellipse2, 100, 100, 1, 90, TRUE, 1000, 40,
				 GOO_CANVAS_ANIMATE_BOUNCE);
#endif
#if 0
  goo_canvas_item_model_animate (textitem, -300, -200, 1, 90, TRUE, 1000, 40,
				 GOO_CANVAS_ANIMATE_BOUNCE);
#endif
}


static void
stop_animation_clicked (GtkWidget *button, GooCanvas *canvas)
{
  goo_canvas_item_model_stop_animation (ellipse2);
}


static void
change_bounds_clicked (GtkWidget *button, GooCanvas *canvas)
{
  static gdouble bounds[4][4] = {
    { -100, -100, 200, 200 },
    { 100, 100, 200, 200 },
    { 0, 0, 300, 300 },
    { 0, 0, 604, 454 }
  };
  static gint bounds_num = 0;

  gboolean automatic_bounds = TRUE;
  gboolean bounds_from_origin = FALSE;
  gdouble bounds_padding = 50.0;

  if (bounds_num < 4)
    {
      automatic_bounds = FALSE;
      goo_canvas_set_bounds (canvas,
			     bounds[bounds_num][0], bounds[bounds_num][1],
			     bounds[bounds_num][2], bounds[bounds_num][3]);
    }
  else if (bounds_num == 4)
    {
      bounds_from_origin = TRUE;
      bounds_padding = 50.0;
    }

  g_object_set (G_OBJECT (canvas),
		"automatic-bounds", automatic_bounds,
		"bounds-from-origin", bounds_from_origin,
		"bounds-padding", bounds_padding,
		NULL);

  bounds_num = (bounds_num + 1) % 6;
}


static void
move_ellipse_clicked (GtkWidget *button, GooCanvas *canvas)
{
  static int last_state = 0;

#if 0
  g_print ("Moving ellipse\n");
#endif

  goo_canvas_item_model_set_transform (ellipse2, NULL);

  if (last_state == 0)
    {
      g_object_set (ellipse2,
		    "center-x", 300.0,
		    "center-y", 70.0,
		    "radius-x", 45.0,
		    "radius-y", 30.0,
		    "fill-color", "red",
		    "stroke-color", "midnightblue",
		    "line-width", 4.0,
		    "title", "A red ellipse",
		    NULL);
      last_state = 1;
    }
  else if (last_state == 1)
    {
      g_object_set (ellipse2,
		    "center-x", 390.0,
		    "center-y", 150.0,
		    "radius-x", 45.0,
		    "radius-y", 40.0,
		    "fill-color", "brown",
		    "stroke-color", "midnightblue",
		    "line-width", 4.0,
		    "title", "A brown ellipse",
		    NULL);
      last_state = 2;
    }
  else if (last_state == 2)
    {
      g_object_set (ellipse2,
		    "center-x", 0.0,
		    "center-y", 0.0,
		    "radius-y", 30.0,
		    NULL);
      goo_canvas_item_model_set_simple_transform (ellipse2, 100, 100, 1, 0);
      last_state = 3;
    }
  else if (last_state == 3)
    {
      goo_canvas_item_model_set_simple_transform (ellipse2, 200, 100, 2, 0);
      last_state = 4;
    }
  else if (last_state == 4)
    {
      goo_canvas_item_model_set_simple_transform (ellipse2, 200, 200, 1, 45);
      last_state = 5;
    }
  else if (last_state == 5)
    {
      goo_canvas_item_model_set_simple_transform (ellipse2, 50, 50, 0.2, 225);
      last_state = 6;
    }
  else
    {
      g_object_set (ellipse2,
		    "center-x", 335.0,
		    "center-y", 70.0,
		    "radius-x", 45.0,
		    "radius-y", 30.0,
		    "fill-color", "purple",
		    "stroke-color", "midnightblue",
		    "line-width", 4.0,
		    "title", "A purple ellipse",
		    NULL);
      last_state = 0;
    }
}


static gboolean
on_motion_notify (GooCanvasItem *item,
		  GooCanvasItem *target,
		  GdkEventMotion *event,
		  gpointer data)
{
  GooCanvasItemModel *model = goo_canvas_item_get_model (item);

#if 0
  g_print ("received 'motion-notify' signal at %g, %g\n",
	   event->x, event->y);
#endif

  if (dragging && (event->state & GDK_BUTTON1_MASK))
    {
      double new_x = event->x;
      double new_y = event->y;

#if 0
      g_print ("Translating by %g, %g\n", new_x - drag_x, new_y - drag_y);
#endif
      goo_canvas_item_model_translate (model, new_x - drag_x, new_y - drag_y);
    }

  return TRUE;
}


static gboolean
on_button_press (GooCanvasItem *item,
		 GooCanvasItem *target,
		 GdkEventButton *event,
		 gpointer data)
{
  GooCanvasItemModel *model = goo_canvas_item_get_model (item);
  GooCanvas *canvas;
  GdkCursor *fleur;

#if 0
  g_print ("%p received 'button-press' signal at %g, %g (root: %g, %g)\n",
	   item, event->x, event->y, event->x_root, event->y_root);
#endif

  switch (event->button)
    {
    case 1:
      if (event->state & GDK_SHIFT_MASK)
	{
	  goo_canvas_item_model_remove (model);
	}
      else
	{
	  drag_x = event->x;
	  drag_y = event->y;

	  fleur = gdk_cursor_new (GDK_FLEUR);
	  canvas = goo_canvas_item_get_canvas (item);
	  goo_canvas_pointer_grab (canvas, item,
				   GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_RELEASE_MASK,
				   fleur,
				   event->time);
	  gdk_cursor_unref (fleur);
	  dragging = TRUE;
	}
      break;

    case 2:
      goo_canvas_item_model_lower (model, NULL);
      break;

    case 3:
      goo_canvas_item_model_raise (model, NULL);
      break;

    default:
      break;
    }

  return TRUE;
}


static gboolean
on_button_release (GooCanvasItem *item,
		   GooCanvasItem *target,
		   GdkEventButton *event,
		   gpointer data)
{
  GooCanvas *canvas;

#if 0
  g_print ("received 'button-release' signal\n");
#endif

  canvas = goo_canvas_item_get_canvas (item);
  goo_canvas_pointer_ungrab (canvas, item, event->time);
  dragging = FALSE;

  return TRUE;
}


static gboolean
on_background_button_press (GooCanvasItem *item,
			    GooCanvasItem *target,
			    GdkEventButton *event,
			    gpointer data)
{
#if 1
  g_print ("background item view received 'button-press' signal\n");
#endif

  return TRUE;
}


static void
on_item_created (GooCanvas          *canvas,
		 GooCanvasItem      *item,
		 GooCanvasItemModel *model,
		 gpointer            data)
{
  if (!goo_canvas_item_model_get_parent (model))
    {
      g_signal_connect (item, "button_press_event",
			G_CALLBACK (on_background_button_press), NULL);
    }
  else if (!g_object_get_data (G_OBJECT (model), "skip-signal-connection"))
    {
      g_signal_connect (item, "motion_notify_event",
			G_CALLBACK (on_motion_notify), NULL);
      g_signal_connect (item, "button_press_event",
			G_CALLBACK (on_button_press), NULL);
      g_signal_connect (item, "button_release_event",
			G_CALLBACK (on_button_release), NULL);
    }
}


GtkWidget *
create_canvas_primitives (GooCanvasItemModel *model,
			  GooCanvasItemModel *static_model)
{
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *w;
	GtkWidget *scrolled_win, *canvas;
	GtkAdjustment *adj;
	GSList *group = NULL;

	vbox = gtk_vbox_new (FALSE, 4);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 4);
	gtk_widget_show (vbox);

	w = gtk_label_new ("Drag an item with button 1.  Click button 2 on an item to lower it, or button 3 to raise it.");
	gtk_box_pack_start (GTK_BOX (vbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);

	hbox = gtk_hbox_new (FALSE, 4);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	/* Create the canvas */

	canvas = goo_canvas_new ();
	g_object_set (G_OBJECT (canvas),
		      "automatic-bounds", TRUE,
		      "bounds-from-origin", FALSE,
		      "bounds-padding", 4.0,
		      "background-color-rgb", 0xC3C3FF,
 		      "has-tooltip", TRUE,
#if 0
		      "redraw-when-scrolled", TRUE,
#endif
		      NULL);
	goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 604, 454);

	g_signal_connect (canvas, "item_created",
			  G_CALLBACK (on_item_created), NULL);

	/* Zoom */

	w = gtk_label_new ("Zoom:");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);

	adj = GTK_ADJUSTMENT (gtk_adjustment_new (1.00, 0.05, 50.00, 0.05, 0.50, 0.0));
	w = gtk_spin_button_new (adj, 0.0, 2);
	g_signal_connect (adj, "value_changed",
			  G_CALLBACK (zoom_changed),
			  canvas);
	gtk_widget_set_size_request (w, 50, -1);
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);

	/* Center: */
	
	w = gtk_check_button_new_with_label("Center scroll region");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	/*gtk_widget_show (w);*/
	g_signal_connect (w, "toggled",
			  G_CALLBACK (center_toggled),
			  canvas);

	/* Move Ellipse */

	w = gtk_button_new_with_label("Move Ellipse");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK  (move_ellipse_clicked),
			  canvas);

	/* Animate Ellipse */

	w = gtk_button_new_with_label("Animate Ellipse");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (animate_ellipse_clicked),
			  canvas);
	
	/* Stop Animation */

	w = gtk_button_new_with_label("Stop Animation");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (stop_animation_clicked),
			  canvas);
	

#if CAIRO_HAS_PDF_SURFACE
	/* Create PDF */

	w = gtk_button_new_with_label("Write PDF");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (write_pdf_clicked),
			  canvas);
#endif

	w = gtk_button_new_with_label("Change Bounds");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (change_bounds_clicked),
			  canvas);

	hbox = gtk_hbox_new (FALSE, 4);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show (hbox);

	/* Scroll to */

	w = gtk_label_new ("Scroll To:");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);

	w = gtk_button_new_with_label("50,50");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (scroll_to_50_50_clicked),
			  canvas);

	w = gtk_button_new_with_label("250,250");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (scroll_to_250_250_clicked),
			  canvas);

	w = gtk_button_new_with_label("500,500");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "clicked",
			  G_CALLBACK (scroll_to_500_500_clicked),
			  canvas);

	/* Scroll anchor */

	w = gtk_label_new ("Anchor:");
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);

	w = gtk_radio_button_new_with_label (group, "NW");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_NW));

	w = gtk_radio_button_new_with_label (group, "N");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_N));

	w = gtk_radio_button_new_with_label (group, "NE");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_NE));

	w = gtk_radio_button_new_with_label (group, "W");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_W));

	w = gtk_radio_button_new_with_label (group, "C");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_CENTER));

	w = gtk_radio_button_new_with_label (group, "E");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_E));

	w = gtk_radio_button_new_with_label (group, "SW");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_SW));

	w = gtk_radio_button_new_with_label (group, "S");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_S));

	w = gtk_radio_button_new_with_label (group, "SE");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (w));
	gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
	gtk_widget_show (w);
	g_signal_connect (w, "toggled",
			  G_CALLBACK (anchor_toggled),
			  canvas);
	g_object_set_data (G_OBJECT (w), "anchor",
			   GINT_TO_POINTER (GTK_ANCHOR_SE));

	
	/* Layout the stuff */

	scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolled_win);
	gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);

	gtk_widget_show (canvas);
	gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

	/* Set the model on the canvas view. The canvas view will then create
	   item views to render each of the items. */
	goo_canvas_set_root_item_model (GOO_CANVAS (canvas), model);

	goo_canvas_set_static_root_item_model (GOO_CANVAS (canvas), static_model);

#if 0
	g_signal_connect_after (canvas, "key_press_event",
				G_CALLBACK (key_press),
				NULL);

	GTK_WIDGET_SET_FLAGS (canvas, GTK_CAN_FOCUS);
	gtk_widget_grab_focus (canvas);
#endif

	return vbox;
}


static void
setup_heading (GooCanvasItemModel *root, char *text, int pos)
{
  double x = (pos % 3) * 200 + 100;
  double y = (pos / 3) * 150 + 5;
  GooCanvasItemModel *item;

  item = goo_canvas_text_model_new (root, text, x, y, -1, GTK_ANCHOR_N,
				    "font", "Sans 12",
				    NULL);
  goo_canvas_item_model_skew_y (item, 30, x, y);
  /*goo_canvas_item_rotate (item, 30, x, y);*/
}


static void
setup_divisions (GooCanvasItemModel *root)
{
  GooCanvasItemModel *group, *item;

  group = goo_canvas_group_model_new (root, NULL);
  goo_canvas_item_model_translate (group, 2, 2);

  item = goo_canvas_rect_model_new (group, 0, 0, 600, 450,
				    "line-width", 4.0,
				    NULL);
  g_object_set_data (G_OBJECT (item), "skip-signal-connection", "TRUE");

  item = goo_canvas_polyline_model_new_line (group, 0, 150, 600, 150,
					     "line-width", 4.0,
					     NULL);
  g_object_set_data (G_OBJECT (item), "skip-signal-connection", "TRUE");

  item = goo_canvas_polyline_model_new_line (group, 0, 300, 600, 300,
					     "line-width", 4.0,
					     NULL);
  g_object_set_data (G_OBJECT (item), "skip-signal-connection", "TRUE");

  item = goo_canvas_polyline_model_new_line (group, 200, 0, 200, 450,
					     "line-width", 4.0,
					     NULL);
  g_object_set_data (G_OBJECT (item), "skip-signal-connection", "TRUE");

  item = goo_canvas_polyline_model_new_line (group, 400, 0, 400, 450,
					     "line-width", 4.0,
					     NULL);
  g_object_set_data (G_OBJECT (item), "skip-signal-connection", "TRUE");

#if 1
  setup_heading (group, "Rectangles", 0);
  setup_heading (group, "Ellipses", 1);
  setup_heading (group, "Texts", 2);
  setup_heading (group, "Images", 3);
  setup_heading (group, "Lines", 4);
  /*setup_heading (group, "No Curves", 5);*/
  /*setup_heading (group, "Arcs", 6);*/
  setup_heading (group, "Polygons", 7);
  /*setup_heading (group, "Widgets", 8);*/
#endif
}


/* FIXME: I think we may need to check byte order. */
static cairo_pattern_t*
create_stipple (const char *color_name, guchar stipple_data[16])
{
  cairo_surface_t *surface;
  cairo_pattern_t *pattern;
  GdkColor color;

  gdk_color_parse (color_name, &color);
  stipple_data[2] = stipple_data[14] = color.red >> 8;
  stipple_data[1] = stipple_data[13] = color.green >> 8;
  stipple_data[0] = stipple_data[12] = color.blue >> 8;
  surface = cairo_image_surface_create_for_data (stipple_data,
						 CAIRO_FORMAT_ARGB32,
						 2, 2, 8);
  pattern = cairo_pattern_create_for_surface (surface);
  cairo_surface_destroy (surface);
  cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);

  return pattern;
}

static void
setup_rectangles (GooCanvasItemModel *root)
{
  GooCanvasItemModel *item;
  cairo_pattern_t *pattern;
  static guchar stipple_data[16] = {
    0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255
  };

  item = goo_canvas_rect_model_new (root, 20, 30, 50, 30,
				    "stroke-color", "red",
				    "line-width", 8.0,
				    "tooltip", "Red stroked rectangle",
				    NULL);

  pattern = create_stipple ("mediumseagreen", stipple_data);
  item = goo_canvas_rect_model_new (root, 90, 40, 90, 60,
				    "fill-pattern", pattern,
				    "stroke-color", "black",
				    "line-width", 4.0,
				    "tooltip", "Medium Sea Green stippled rectangle",
				    NULL);
  cairo_pattern_destroy (pattern);

  item = goo_canvas_rect_model_new (root, 10, 80, 70, 60,
				    "fill-color", "steelblue",
				    /*"fill-pattern", NULL,*/
				    "tooltip", "Steel Blue rectangle",
				    NULL);

  item = goo_canvas_rect_model_new (root, 20, 90, 70, 60,
				    "fill-color-rgba", 0x3cb37180,
				    "stroke-color", "blue",
				    "line-width", 2.0,
				    "tooltip", "Partially transparent rectangle",
				    NULL);

  item = goo_canvas_rect_model_new (root, 110, 80, 50, 30,
				    "radius-x", 20.0,
				    "radius-y", 10.0,
				    "stroke-color", "yellow",
				    "fill-color-rgba", 0x3cb3f180,
				    "tooltip", "Rectangle with rounded corners",
				    NULL);
}


static void
setup_ellipses (GooCanvasItemModel *root)
{
  GooCanvasItemModel *ellipse1, *ellipse3;
  cairo_pattern_t *pattern;
  static guchar stipple_data[16] = {
    0, 0, 0, 255,   0, 0, 0, 0,
    0, 0, 0, 0,     0, 0, 0, 255
  };

  ellipse1 = goo_canvas_ellipse_model_new (root, 245, 45, 25, 15,
					   "stroke-color", "goldenrod",
					   "line-width", 8.0,
					   NULL);

#if 1
  ellipse2 = goo_canvas_ellipse_model_new (root, 335, 70, 45, 30,
					   "fill-color", "wheat",
					   "stroke-color", "midnightblue",
					   "line-width", 4.0,
					   "title", "An ellipse",
					   NULL);
#if 0
  goo_canvas_item_model_rotate (ellipse2, 45, 335, 70);
#endif

  pattern = create_stipple ("cadetblue", stipple_data);
  ellipse3 = goo_canvas_ellipse_model_new (root, 245, 110, 35, 30,
					   "fill-pattern", pattern,
					   "stroke-color", "black",
					   "line-width", 1.0,
					   NULL);
  cairo_pattern_destroy (pattern);
#endif

#if 0
 {
   GooCanvasItem *clone = goo_canvas_clone_model_new (root, ellipse1);
   goo_canvas_item_translate (clone, 100, 100);
 }
#endif
}


#define VERTICES 10
#define RADIUS 60.0

static void
polish_diamond (GooCanvasItemModel *root)
{
  GooCanvasItemModel *group, *item;
  int i, j;
  double a, x1, y1, x2, y2;

  group = goo_canvas_group_model_new (root,
				      "line-width", 1.0,
				      "line-cap", CAIRO_LINE_CAP_ROUND,
				      NULL);
  goo_canvas_item_model_translate (group, 270, 230);

  for (i = 0; i < VERTICES; i++) {
    a = 2.0 * M_PI * i / VERTICES;
    x1 = RADIUS * cos (a);
    y1 = RADIUS * sin (a);

    for (j = i + 1; j < VERTICES; j++) {
      a = 2.0 * M_PI * j / VERTICES;
      x2 = RADIUS * cos (a);
      y2 = RADIUS * sin (a);
      item = goo_canvas_polyline_model_new_line (group, x1, y1, x2, y2, NULL);
      g_object_set_data (G_OBJECT (item), "skip-signal-connection", "TRUE");
    }
  }
}


#define SCALE 7.0

static void
make_hilbert (GooCanvasItemModel *root)
{
  char hilbert[] = "urdrrulurulldluuruluurdrurddldrrruluurdrurddldrddlulldrdldrrurd";
  char *c;
  double *pp, *p;
  GooCanvasItemModel *item;
  GooCanvasPoints *points;
  cairo_pattern_t *pattern;
  static guchar stipple_data[16] = {
    0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255
  };

  points = goo_canvas_points_new (strlen (hilbert) + 1);
  points->coords[0] = 340.0;
  points->coords[1] = 290.0;

  pp = points->coords;
  for (c = hilbert, p = points->coords + 2; *c; c++, p += 2, pp += 2)
    switch (*c) {
    case 'u':
      p[0] = pp[0];
      p[1] = pp[1] - SCALE;
      break;

    case 'd':
      p[0] = pp[0];
      p[1] = pp[1] + SCALE;
      break;

    case 'l':
      p[0] = pp[0] - SCALE;
      p[1] = pp[1];
      break;

    case 'r':
      p[0] = pp[0] + SCALE;
      p[1] = pp[1];
      break;
    }

  pattern = create_stipple ("red", stipple_data);
  item = goo_canvas_polyline_model_new (root, FALSE, 0,
					"points", points,
					"line-width", 4.0,
					"stroke-pattern", pattern,
					"line-cap", CAIRO_LINE_CAP_SQUARE,
					"line-join", CAIRO_LINE_JOIN_MITER,
					NULL);
  cairo_pattern_destroy (pattern);

  goo_canvas_points_unref (points);
}

static void
setup_lines (GooCanvasItemModel *root)
{
  GooCanvasItemModel *polyline1, *polyline2, *polyline3, *polyline4, *polyline5;

  polish_diamond (root);
  make_hilbert (root);

  /* Arrow tests */

  polyline1 = goo_canvas_polyline_model_new (root, FALSE, 4,
					     340.0, 170.0,
					     340.0, 230.0,
					     390.0, 230.0,
					     390.0, 170.0,
					     "stroke-color", "midnightblue",
					     "line-width", 3.0,
					     "start-arrow", TRUE,
					     "end-arrow", TRUE,
					     "arrow-tip-length", 3.0,
					     "arrow-length", 4.0,
					     "arrow-width", 3.5,
					     NULL);

  polyline2 = goo_canvas_polyline_model_new (root, FALSE, 2,
					     356.0, 180.0,
					     374.0, 220.0,
					     "stroke-color", "blue",
					     "line-width", 1.0,
					     "start-arrow", TRUE,
					     "end-arrow", TRUE,
					     "arrow-tip-length", 5.0,
					     "arrow-length", 6.0,
					     "arrow-width", 6.0,
					     NULL);

  polyline3 = goo_canvas_polyline_model_new (root, FALSE, 2,
					     356.0, 220.0,
					     374.0, 180.0,
					     "stroke-color", "blue",
					     "line-width", 1.0,
					     "start-arrow", TRUE,
					     "end-arrow", TRUE,
					     "arrow-tip-length", 5.0,
					     "arrow-length", 6.0,
					     "arrow-width", 6.0,
					     NULL);

  /* Test polyline without any coords. */
  polyline4 = goo_canvas_polyline_model_new (root, FALSE, 0,
					     NULL);

  /* Test polyline with 1 coord and arrows. */
  polyline5 = goo_canvas_polyline_model_new (root, FALSE, 1,
					     356.0, 220.0,
					     "start-arrow", TRUE,
					     "end-arrow", TRUE,
					     NULL);
}


static void
setup_polygons (GooCanvasItemModel *root)
{
  GooCanvasItemModel *polyline1, *polyline2;
  GooCanvasPoints *points;
  cairo_pattern_t *pattern;
  static guchar stipple_data[16] = {
    0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255
  };

  points = goo_canvas_points_new (3);
  points->coords[0] = 210.0;
  points->coords[1] = 320.0;
  points->coords[2] = 210.0;
  points->coords[3] = 380.0;
  points->coords[4] = 260.0;
  points->coords[5] = 350.0;
  pattern = create_stipple ("blue", stipple_data);
  polyline1 = goo_canvas_polyline_model_new (root, TRUE, 0,
					     "line-width", 1.0,
					     "points", points,
					     "fill_pattern", pattern,
					     "stroke_color", "black",
					     NULL);
  cairo_pattern_destroy (pattern);
  goo_canvas_points_unref (points);

  points = goo_canvas_points_new (14);
  points->coords[0] = 270.0;
  points->coords[1] = 330.0;
  points->coords[2] = 270.0;
  points->coords[3] = 430.0;
  points->coords[4] = 390.0;
  points->coords[5] = 430.0;
  points->coords[6] = 390.0;
  points->coords[7] = 330.0;
  points->coords[8] = 310.0;
  points->coords[9] = 330.0;
  points->coords[10] = 310.0;
  points->coords[11] = 390.0;
  points->coords[12] = 350.0;
  points->coords[13] = 390.0;
  points->coords[14] = 350.0;
  points->coords[15] = 370.0;
  points->coords[16] = 330.0;
  points->coords[17] = 370.0;
  points->coords[18] = 330.0;
  points->coords[19] = 350.0;
  points->coords[20] = 370.0;
  points->coords[21] = 350.0;
  points->coords[22] = 370.0;
  points->coords[23] = 410.0;
  points->coords[24] = 290.0;
  points->coords[25] = 410.0;
  points->coords[26] = 290.0;
  points->coords[27] = 330.0;
  polyline2 = goo_canvas_polyline_model_new (root, TRUE, 0,
					     "points", points,
					     "fill_color", "tan",
					     "stroke_color", "black",
					     "line-width", 3.0,
					     NULL);
  goo_canvas_points_unref (points);
}


static GooCanvasItemModel *
make_anchor (GooCanvasItemModel *root, double x, double y)
{
  GooCanvasItemModel *group, *item;
  cairo_matrix_t transform = { 0.8, 0.2, -0.3, 0.5, x, y };

  group = goo_canvas_group_model_new (root, NULL);
  goo_canvas_item_model_translate (group, x, y);
#if 1
  g_object_set (group,
		"transform", &transform,
		NULL);
#endif

  item = goo_canvas_rect_model_new (group, -2.5, -2.5, 4, 4,
				    "line-width", 1.0,
				    NULL);

  return group;
}

static void 
setup_texts (GooCanvasItemModel *root)
{
  cairo_pattern_t *pattern;
  static guchar stipple_data[16] = {
    0, 0, 0, 255,   0, 0, 0, 0,   0, 0, 0, 0,     0, 0, 0, 255
  };

#if 1
  pattern = create_stipple ("blue", stipple_data);
  goo_canvas_text_model_new (make_anchor (root, 420, 20),
			     "Anchor NW", 0, 0, -1, GTK_ANCHOR_NW,
			     "font", "Sans Bold 24",
			     "fill_pattern", pattern,
			     NULL);
  cairo_pattern_destroy (pattern);

  goo_canvas_text_model_new (make_anchor (root, 470, 75),
			     "Anchor center\nJustify center\nMultiline text\nb8bit text ÅÄÖåäö",
			     0, 0, -1, GTK_ANCHOR_CENTER,
			     "font", "monospace bold 14",
			     "alignment", PANGO_ALIGN_CENTER,
			     "fill_color", "firebrick",
			     NULL);
#endif

#if 0
  goo_canvas_text_model_new (make_anchor (root, 590, 140),
			     "Clipped text\nClipped text\nClipped text\nClipped text\nClipped text\nClipped text",
			     0, 0, -1, GTK_ANCHOR_SE,
			     "font", "Sans 12",
			     /*"clip", TRUE,*/
			     /*"clip_width", 50.0,*/
			     /*"clip_height", 55.0,*/
			     /*"x_offset", 10.0,*/
			     "fill_color", "darkgreen",
			     NULL);
#endif

#if 1
  textitem = goo_canvas_text_model_new (make_anchor (root, 420, 240),
					"This is a very long paragraph that will need to be wrapped over several lines so we can see what happens to line-breaking as the view is zoomed in and out.",
					0, 0, 180, GTK_ANCHOR_W,
					"font", "Sans 12",
					"fill_color", "goldenrod",
					NULL);
#endif

#if 1
  textitem = goo_canvas_text_model_new (root,
					"Ellipsized text.",
					20, 420, 115, GTK_ANCHOR_W,
					"font", "Sans 12",
					"fill_color", "blue",
					"ellipsize", PANGO_ELLIPSIZE_END,
					NULL);
#endif
}


static void
setup_invisible_texts (GooCanvasItemModel *root)
{
  goo_canvas_text_model_new (root, "Visible above 0.8x", 500, 330, -1,
			     GTK_ANCHOR_CENTER,
			     "visibility", GOO_CANVAS_ITEM_VISIBLE_ABOVE_THRESHOLD,
			     "visibility-threshold", 0.8,
			     NULL);
  goo_canvas_rect_model_new (root, 410.5, 322.5, 180, 15,
			     "line-width", 1.0,
			     "visibility", GOO_CANVAS_ITEM_VISIBLE_ABOVE_THRESHOLD,
			     "visibility-threshold", 0.8,
			     NULL);

  goo_canvas_text_model_new (root, "Visible above 1.5x", 500, 350, -1,
			     GTK_ANCHOR_CENTER,
			     "visibility", GOO_CANVAS_ITEM_VISIBLE_ABOVE_THRESHOLD,
			     "visibility-threshold", 1.5,
			     NULL);
  goo_canvas_rect_model_new (root, 410.5, 342.5, 180, 15,
			     "line-width", 1.0,
			     "visibility", GOO_CANVAS_ITEM_VISIBLE_ABOVE_THRESHOLD,
			     "visibility-threshold", 1.5,
			     NULL);

  goo_canvas_text_model_new (root, "Visible above 3.0x", 500, 370, -1,
			     GTK_ANCHOR_CENTER,
			     "visibility", GOO_CANVAS_ITEM_VISIBLE_ABOVE_THRESHOLD,
			     "visibility-threshold", 3.0,
			     NULL);
  goo_canvas_rect_model_new (root, 410.5, 362.5, 180, 15,
			     "line-width", 1.0,
			     "visibility", GOO_CANVAS_ITEM_VISIBLE_ABOVE_THRESHOLD,
			     "visibility-threshold", 3.0,
			     NULL);

  /* This should never be seen. */
  goo_canvas_text_model_new (root, "Always Invisible", 500, 390, -1,
			     GTK_ANCHOR_CENTER,
			     "visibility", GOO_CANVAS_ITEM_INVISIBLE,
			     NULL);
  goo_canvas_rect_model_new (root, 410.5, 350.5, 180, 15,
			     "line-width", 1.0,
			     "visibility", GOO_CANVAS_ITEM_INVISIBLE,
			     NULL);
}


static void
plant_flower (GooCanvasItemModel *root,
	      double x, double y,
	      GtkAnchorType anchor,
	      double opacity)
{
  cairo_pattern_t *pattern;
  cairo_surface_t *surface;
  GooCanvasItemModel *image;
  double w, h;

  surface = cairo_image_surface_create_from_png ("flower.png");
  w = cairo_image_surface_get_width (surface);
  h = cairo_image_surface_get_height (surface);

  pattern = cairo_pattern_create_for_surface (surface);
  cairo_surface_destroy (surface);

  image = goo_canvas_image_model_new (root, NULL, x, y,
				      "pattern", pattern,
				      "width", w * 1.5,
				      "height", h * 2,
				      "scale-to-fit", TRUE,
				      "alpha", opacity,
				      NULL);
  cairo_pattern_destroy (pattern);
}

static void
setup_images (GooCanvasItemModel *root)
{
  GdkPixbuf *im;
  GooCanvasItemModel *image;

  im = gdk_pixbuf_new_from_file ("toroid.png", NULL);

  if (im)
    {
      double w = gdk_pixbuf_get_width (im);
      double h = gdk_pixbuf_get_height (im);
      image = goo_canvas_image_model_new (root, im, 100.0 - w / 2, 225.0 - h / 2,
					  "width", w,
					  "height", h,
					  /* "anchor", GTK_ANCHOR_CENTER, */
					  NULL);
      g_object_unref(im);
    }
  else
    g_warning ("Could not find the toroid.png sample file");

  plant_flower (root,  20.0, 170.0, GTK_ANCHOR_NW, 0.3);
  plant_flower (root, 180.0, 170.0, GTK_ANCHOR_NE, 0.5);
  plant_flower (root,  20.0, 280.0, GTK_ANCHOR_SW, 0.7);
  plant_flower (root, 180.0, 280.0, GTK_ANCHOR_SE, 1.0);
}


static GooCanvasItemModel*
create_static_model ()
{
  GooCanvasItemModel *static_root, *group, *item;

  static_root = goo_canvas_group_model_new (NULL, NULL);

  group = goo_canvas_group_model_new (static_root, NULL);

  item = goo_canvas_polyline_model_new_line (group,
					     40.0, 410.0,
					     40.0, 330.0,
					     "stroke-color", "midnightblue",
					     "line-width", 3.0,
					     "end-arrow", TRUE,
					     "arrow-tip-length", 3.0,
					     "arrow-length", 4.0,
					     "arrow-width", 3.5,
					     NULL);

  item = goo_canvas_polyline_model_new_line (group,
					     32.0, 370.0,
					     48.0, 370.0,
					     "stroke-color", "midnightblue",
					     "line-width", 3.0,
					     NULL);

  item = goo_canvas_text_model_new (group, "N", 40, 320, -1, GTK_ANCHOR_S,
				    "font", "Sans 12",
				    NULL);

  return static_root;
}


static void
test_simple_transforms (GooCanvasItemModel *root)
{
#if 0
  GooCanvasItemModel *item;
  int rotation, x, y;
  double scale, x_out, y_out, scale_out, rotation_out, max_error = 0.0000001;

  item = goo_canvas_rect_model_new (root, 20, 30, 50, 30,
				    NULL);

  for (rotation = 0; rotation < 360; rotation += 20)
    {
      scale = 0.1;
      while (scale < 10)
	{
	  g_print ("Rotation: %i Scale: %g\n", rotation, scale);

	  for (x = -100; x < 100; x += 10)
	    {
	      for (y = -100; y < 100; y += 10)
		{
		  goo_canvas_item_model_set_simple_transform (item, x, y, scale,
							      rotation);

		  goo_canvas_item_model_get_simple_transform (item, &x_out, &y_out,
							      &scale_out,
							      &rotation_out);

		  if (fabs (x_out - x) > max_error
		      || fabs (y_out - y) > max_error
		      || fabs (scale_out - scale) > max_error
		      || fabs (rotation_out - rotation) > max_error)
		    {
		      g_print ("IN X:%i Y:%i Scale:%g Rotation:%i -> %g, %g, %g, %g\n", x, y, scale, rotation, x_out, y_out, scale_out, rotation_out);
		    }
		}
	    }
	  scale += 0.1;
	}
    }
#endif
}


static void
setup_grids (GooCanvasItemModel *root)
{
  GooCanvasItemModel *item;

  item = goo_canvas_grid_model_new (root, 80, 310, 90, 90, 10, 10, 5, 5,
				    "stroke-color", "yellow",
				    "fill-color", "pink",
				    "border-width", 2.0,
				    "border-color", "red",
				    "vert-grid-line-color", "lightblue",
				    "horz-grid-line-width", 1.0,
				    "vert-grid-line-width", 1.0,
				    "vert-grid-lines-on-top", TRUE,
				    NULL);
}


static GooCanvasItemModel*
create_model (void)
{
  GooCanvasItemModel *root;

  root = goo_canvas_group_model_new (NULL, NULL);

  /* Setup canvas items */
#if 1
  setup_divisions (root);
  setup_rectangles (root);
  setup_ellipses (root);
  setup_lines (root);
  setup_polygons (root);
  setup_texts (root); 
  setup_images (root);
  setup_invisible_texts (root);
  setup_grids (root);
#endif
#if 0
  setup_widgets (root);
#endif

  test_simple_transforms (root);

  return root;
}


static gboolean
on_delete_event (GtkWidget *window,
		 GdkEvent *event,
		 gpointer unused_data)
{
  gtk_main_quit ();
  return FALSE;
}


static GtkWidget*
create_window (GooCanvasItemModel *model,
	       GooCanvasItemModel *static_model)
{
  GtkWidget *window, *notebook;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "delete_event", G_CALLBACK (on_delete_event),
		    NULL);

  notebook = gtk_notebook_new ();
  gtk_widget_show (notebook);
  gtk_container_add (GTK_CONTAINER (window), notebook);

  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_canvas_primitives (model, static_model),
			    gtk_label_new ("Primitives"));
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_canvas_arrowhead (),
			    gtk_label_new ("Arrowhead"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_canvas_fifteen (),
			    gtk_label_new ("Fifteen"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_canvas_features (),
			    gtk_label_new ("Features"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_canvas_scalability (),
			    gtk_label_new ("Scalability"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_grabs_page (),
			    gtk_label_new ("Grabs"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_events_page (),
			    gtk_label_new ("Events"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_paths_page (),
			    gtk_label_new ("Paths"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_focus_page (),
			    gtk_label_new ("Focus"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_animation_page (),
			    gtk_label_new ("Animation"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_clipping_page (),
			    gtk_label_new ("Clipping"));
#endif
#if 1
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
			    create_table_page (),
			    gtk_label_new ("Table"));
#endif

  return window;
}


int
main (int argc, char *argv[])
{
  GooCanvasItemModel *model, *static_model;
  GtkWidget *window;

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  model = create_model ();
  static_model = create_static_model ();

  /* Create 2 windows to show off multiple views. */
  window = create_window (model, static_model);
#if 1
  window = create_window (model, static_model);
#endif

  g_object_unref (model);
  g_object_unref (static_model);

  gtk_main ();
  return 0;
}
