#include <math.h>
#include <stdlib.h>
#include <goocanvas.h>

#if 1
#define N_GROUP_COLS 25
#define N_GROUP_ROWS 200
#define N_COLS 10
#define N_ROWS 10
#define ITEM_WIDTH 400
#define PADDING 10
#if 1
#define ROTATE
#endif
#else
/* This tests a very wide canvas, nearly up to the 31-bit GDK window size
   limit. */
#define N_GROUP_COLS 100000
#define N_GROUP_ROWS 1
#define N_COLS 10
#define N_ROWS 1
#define ITEM_WIDTH 1000
#define PADDING 100
#endif

#define N_TOTAL_ID_ITEMS (N_GROUP_COLS * N_GROUP_ROWS) * (N_COLS * N_ROWS)

/* The maximum length of a string identifying an item (i.e. its coords). */
#define MAX_ID_LEN 20

#if 1
#define SET_STYLE
#endif

#if 1
#define USE_TEXT
#endif

#if 1
#define SET_IDS
#endif

#if 0
#define USE_PIXMAP 
#endif

#if 0
#define ONLY_ONE
#endif

double total_width, total_height;
double left_offset, top_offset;
char ids[N_TOTAL_ID_ITEMS][MAX_ID_LEN];

GdkPixbuf *pixbuf = NULL;
cairo_pattern_t *pattern = NULL;
double item_width, item_height;
double cell_width, cell_height;
double group_width, group_height;

static clock_t start;

static gboolean
on_motion_notify (GooCanvasItem *item,
		  GooCanvasItem *target,
		  GdkEventMotion *event,
		  gpointer data)
{
  GooCanvasItem *ancestor = target;
  gchar *id;

  while (ancestor)
    {
      id = g_object_get_data (G_OBJECT (ancestor), "id");
      if (id)
	{
	  g_print ("%s item received 'motion-notify' signal\n", id);
	  return TRUE;
	}

      ancestor = goo_canvas_item_get_parent (ancestor);
    }

  return TRUE;
}


static void
init_ids (void)
{
  int group_i, group_j, i, j;
  int id_item_num = 0;;
	
  for (group_i = 0; group_i < N_GROUP_COLS; group_i++)
    {
      for (group_j = 0; group_j < N_GROUP_ROWS; group_j++)
	{
	  double group_x = left_offset + (group_i * group_width);
	  double group_y = top_offset + (group_j * group_height);

	  for (i = 0; i < N_COLS; i++)
	    {
	      for (j = 0; j < N_ROWS; j++)
		{
		  double item_x = (i * cell_width) + PADDING;
		  double item_y = (j * cell_height) + PADDING;

		  sprintf (ids[id_item_num++], "%.10g, %.10g",
			   group_x + item_x, group_y + item_y);
		}
	    }
	}
    }
}


static void
setup_canvas (GtkWidget *canvas)
{
  GooCanvasItem *root, *group, *item;
  int group_i, group_j, i, j;
  int total_items = 0, id_item_num = 0;;
  GdkColor color = { 0, 0, 0, 0, };
  GooCanvasStyle *style, *style2;
  GValue tmpval = { 0 };
  cairo_matrix_t item_matrix;
  GQuark id_quark = g_quark_from_static_string ("id");

  root = goo_canvas_get_root_item (GOO_CANVAS (canvas));

  g_signal_connect (root, "motion_notify_event",
		    G_CALLBACK (on_motion_notify), NULL);

  g_object_set (G_OBJECT (root),
		"font", "Sans 8",
		NULL);

  style = goo_canvas_style_new ();
  gdk_color_parse ("mediumseagreen", &color);
  pattern = cairo_pattern_create_rgb (color.red / 65535.0,
				      color.green / 65535.0,
				      color.blue / 65535.0);
  g_value_init (&tmpval, GOO_TYPE_CAIRO_PATTERN);
  g_value_take_boxed (&tmpval, pattern);
  goo_canvas_style_set_property (style, goo_canvas_style_fill_pattern_id, &tmpval);
  g_value_unset (&tmpval);

  style2 = goo_canvas_style_new ();
  gdk_color_parse ("steelblue", &color);
  pattern = cairo_pattern_create_rgb (color.red / 65535.0,
				      color.green / 65535.0,
				      color.blue / 65535.0);
  g_value_init (&tmpval, GOO_TYPE_CAIRO_PATTERN);
  g_value_take_boxed (&tmpval, pattern);
  goo_canvas_style_set_property (style2, goo_canvas_style_fill_pattern_id, &tmpval);
  g_value_unset (&tmpval);

  for (group_i = 0; group_i < N_GROUP_COLS; group_i++)
    {
      for (group_j = 0; group_j < N_GROUP_ROWS; group_j++)
	{
	  double group_x = left_offset + (group_i * group_width);
	  double group_y = top_offset + (group_j * group_height);

	  group = goo_canvas_group_new (root, NULL);
	  total_items++;
	  goo_canvas_item_translate (group, group_x, group_y);

	  for (i = 0; i < N_COLS; i++)
	    {
	      for (j = 0; j < N_ROWS; j++)
		{
		  double item_x = (i * cell_width) + PADDING;
		  double item_y = (j * cell_height) + PADDING;
#ifdef ROTATE
		  double rotation = (i % 10 * 2) * M_PI / 180;
		  double rotation_x = item_x + item_width / 2;
		  double rotation_y = item_y + item_height / 2;
#endif

#ifdef USE_PIXMAP
		  item = goo_canvas_image_new (group, NULL, item_x, item_y,
					       "pattern", pattern,
					       "width", item_width,
					       "height", item_height,
					       NULL);
#else
		  item = goo_canvas_rect_new (group, item_x, item_y,
					      item_width, item_height,
					      NULL);
#ifdef SET_STYLE
		  goo_canvas_item_set_style (item, (j % 2) ? style : style2);
#endif
#ifdef ROTATE
		  cairo_matrix_init_identity (&item_matrix);
		  cairo_matrix_translate (&item_matrix, rotation_x, rotation_y);
		  cairo_matrix_rotate (&item_matrix, rotation);
		  cairo_matrix_translate (&item_matrix, -rotation_x, -rotation_y);
		  goo_canvas_item_set_transform (item, &item_matrix);
#endif
#endif
#ifdef SET_IDS
		  g_object_set_qdata (G_OBJECT (item), id_quark,
				      ids[id_item_num]);
#endif
#if 0
		  g_signal_connect (item, "motion_notify_event",
				    G_CALLBACK (on_motion_notify), NULL);
#endif

#ifdef USE_TEXT
		  item = goo_canvas_text_new (group, ids[id_item_num],
					      item_x + item_width / 2,
					      item_y + item_height / 2,
					      item_width, GTK_ANCHOR_CENTER,
					      "height", item_height,
					      /*"alignment", PANGO_ALIGN_CENTER,*/
					      NULL);
		  /* FIXME: This is slightly naughty, but much faster. */
		  GOO_CANVAS_TEXT (item)->text_data->alignment = PANGO_ALIGN_CENTER;

#else
		  item = goo_canvas_rect_new (group, item_x + 20, item_y + 4,
					      item_width - 40, item_height - 8,
					      NULL);
#endif

#ifdef ROTATE
		  goo_canvas_item_set_transform (item, &item_matrix);
#endif
		  id_item_num++;
		  total_items += 2;

#ifdef ONLY_ONE
		  break;
#endif
		}
#ifdef ONLY_ONE
	      break;
#endif
	    }
#ifdef ONLY_ONE
	  break;
#endif
	}
#ifdef ONLY_ONE
      break;
#endif
    }

  if (pattern)
    cairo_pattern_destroy (pattern);

  g_print ("Total items: %i\n", total_items);
}


static gboolean
on_expose_event (GtkWidget *canvas,
		 GdkEvent  *event,
		 gpointer   unused_data)
{
  static gboolean first_time = TRUE;

  if (first_time)
    {
      printf ("Update Canvas Time Used: %g\n",
	      (double) (clock() - start) / CLOCKS_PER_SEC);
      first_time = FALSE;
    }

  return FALSE;
}


GtkWidget *
create_canvas (void)
{
  GtkWidget *canvas;

#ifdef USE_PIXMAP
  pixbuf = gdk_pixbuf_new_from_file("toroid.png", NULL);
  item_width = gdk_pixbuf_get_width (pixbuf);
  item_height = gdk_pixbuf_get_height (pixbuf);
  pattern = goo_canvas_cairo_pattern_from_pixbuf (pixbuf);
#else
  pixbuf = NULL;
  item_width = ITEM_WIDTH;
  item_height = 19;
#endif
	
  cell_width = item_width + PADDING * 2;
  cell_height = item_height + PADDING * 2;

  group_width = N_COLS * cell_width;
  group_height = N_ROWS * cell_height;

  total_width = N_GROUP_COLS * group_width;
  total_height = N_GROUP_ROWS * group_height;

  /* We use -ve offsets to test if -ve coords are handled correctly. */
  left_offset = -total_width / 2;
  top_offset = -total_height / 2;

#ifdef SET_IDS
  init_ids ();
#endif

  /* Create the canvas. */
  canvas = goo_canvas_new ();
  gtk_widget_set_size_request (canvas, 600, 450);

  start = clock();
  setup_canvas (canvas);
  printf ("Create Canvas Time Used: %g\n",
	  (double) (clock() - start) / CLOCKS_PER_SEC);

  start = clock();
  goo_canvas_set_bounds (GOO_CANVAS (canvas), left_offset, top_offset,
			 left_offset + total_width, top_offset + total_height);
  gtk_widget_show (canvas);

  g_signal_connect (canvas, "expose_event",
		    G_CALLBACK (on_expose_event), NULL);

  return canvas;
}


static gboolean
on_delete_event (GtkWidget *window,
		 GdkEvent  *event,
		 gpointer   unused_data)
{
  gtk_main_quit ();
  return FALSE;
}


int
main (int argc, char *argv[])
{
  GtkWidget *window, *scrolled_win, *canvas;

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);
  gtk_widget_show (window);
  g_signal_connect (window, "delete_event", G_CALLBACK (on_delete_event),
		    NULL);

  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolled_win);
  gtk_container_add (GTK_CONTAINER (window), scrolled_win);

  canvas = create_canvas ();
  gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);

  gtk_main ();

  return 0;
}


