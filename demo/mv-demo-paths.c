#include <config.h>
#include <string.h>
#include <gtk/gtk.h>
#include <goocanvas.h>


static GooCanvasItemModel *path1;

static GooCanvasItemModel*
create_canvas_model (void)
{
  GooCanvasItemModel *root, *path;

  root = goo_canvas_group_model_new (NULL, NULL);

  /* Test the simple commands like moveto and lineto: MmZzLlHhVv. */
  path1 = goo_canvas_path_model_new (root, "M 20 20 L 40 40", NULL);
  path = goo_canvas_path_model_new (root, "M30 20 l20, 20", NULL);
  path = goo_canvas_path_model_new (root, "M 60 20 H 80", NULL);
  path = goo_canvas_path_model_new (root, "M60 40 h20", NULL);
  path = goo_canvas_path_model_new (root, "M 100,20 V 40", NULL);
  path = goo_canvas_path_model_new (root, "M 120 20 v 20", NULL);
  path = goo_canvas_path_model_new (root, "M 140 20 h20 v20 h-20 z", NULL);
  path = goo_canvas_path_model_new (root,
				    "M 180 20 h20 v20 h-20 z m 5,5 h10 v10 h-10 z",
				    "fill-color", "red",
				    "fill-rule", CAIRO_FILL_RULE_EVEN_ODD,
				    NULL);

  path = goo_canvas_path_model_new (root, "M 220 20 L 260 20 L 240 40 z",
				    "fill-color", "red",
				    "stroke-color", "blue",
				    "line-width", 3.0,
				    NULL);

  /* Test the bezier curve commands: CcSsQqTt. */
  path = goo_canvas_path_model_new (root,
				    "M20,100 C20,50 100,50 100,100 S180,150 180,100",
				    NULL);

  path = goo_canvas_path_model_new (root,
				    "M220,100 c0,-50 80,-50 80,0 s80,50 80,0",
				    NULL);

  path = goo_canvas_path_model_new (root,
				    "M20,200 Q60,130 100,200 T180,200",
				    NULL);

  path = goo_canvas_path_model_new (root,
				    "M220,200 q40,-70 80,0 t80,0",
				    NULL);

  /* Test the elliptical arc commands: Aa. */
  path = goo_canvas_path_model_new (root,
				    "M200,500 h-150 a150,150 0 1,0 150,-150 z",
				    "fill-color", "red",
				    "stroke-color", "blue",
				    "line-width", 5.0,
				    NULL);

  path = goo_canvas_path_model_new (root,
				    "M175,475 v-150 a150,150 0 0,0 -150,150 z",
				    "fill-color", "yellow",
				    "stroke-color", "blue",
				    "line-width", 5.0,
				    NULL);

  path = goo_canvas_path_model_new (root,
				    "M400,600 l 50,-25 "
				    "a25,25 -30 0,1 50,-25 l 50,-25 "
				    "a25,50 -30 0,1 50,-25 l 50,-25 "
				    "a25,75 -30 0,1 50,-25 l 50,-25 "
				    "a25,100 -30 0,1 50,-25 l 50,-25",
				    "stroke-color", "red",
				    "line-width", 5.0,
				    NULL);

  path = goo_canvas_path_model_new (root,
				    "M 525,75 a100,50 0 0,0 100,50",
				    "stroke-color", "red",
				    "line-width", 5.0,
				    NULL);
  path = goo_canvas_path_model_new (root,
				    "M 725,75 a100,50 0 0,1 100,50",
				    "stroke-color", "red",
				    "line-width", 5.0,
				    NULL);
  path = goo_canvas_path_model_new (root,
				    "M 525,200 a100,50 0 1,0 100,50",
				    "stroke-color", "red",
				    "line-width", 5.0,
				    NULL);
  path = goo_canvas_path_model_new (root,
				    "M 725,200 a100,50 0 1,1 100,50",
				    "stroke-color", "red",
				    "line-width", 5.0,
				    NULL);

  return root;
}


static void
move_path_clicked (GtkWidget *button, GooCanvas *canvas)
{
  static gint count = 0;

  gchar *data;

  if (count == 0)
    data = "M130,70 L317,70";
  else if (count == 1)
    data = "M130,170 L317,170";
  else
    data = "M130,270 L317,270";

  g_object_set (path1,
		"data", data,
		NULL);

  count = (count + 1) % 3;
}


GtkWidget *
create_paths_page (void)
{
  GtkWidget *vbox, *hbox, *w, *scrolled_win, *canvas;
  GooCanvasItemModel *root;

  vbox = gtk_vbox_new (FALSE, 4);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 4);
  gtk_widget_show (vbox);

  hbox = gtk_hbox_new (FALSE, 4);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_win),
				       GTK_SHADOW_IN);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (vbox), scrolled_win);

  canvas = goo_canvas_new ();
  gtk_widget_set_size_request (canvas, 600, 450);
  goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);
  gtk_widget_show (canvas);
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  root = create_canvas_model ();
  goo_canvas_set_root_item_model (GOO_CANVAS (canvas), root);
  g_object_unref (root);

  w = gtk_button_new_with_label ("Move Path");
  gtk_box_pack_start (GTK_BOX (hbox), w, FALSE, FALSE, 0);
  gtk_widget_show (w);
  g_signal_connect (w, "clicked",
		    G_CALLBACK (move_path_clicked), canvas);

  return vbox;
}
