/*

	BUILD

	gcc `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -o example-1 example-1.c \
			`xml2-config --cflags` \
			`xml2-config --libs` \
			`xslt-config --cflags` \
			`xslt-config --libs` \
			`pkg-config --cflags --libs webkit2gtk-3.0`
*/

// to build: gcc `pkg-config --cflags gtk+-3.0` -o example-1 example-1.c `pkg-config --libs gtk+-3.0` `xml2-config --cflags` `xml2-config --libs` `xslt-config --cflags` `xslt-config --libs`
// to build(old):  gcc `pkg-config --cflags gtk+-3.0` -o example-1 example-1.c `pkg-config --libs gtk+-3.0`


#include <gtk/gtk.h>

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <webkit2/webkit2.h>


enum
{
	COL_HEADER,
	COL_EDUCATION,
	COL_AWARDS,
	COL_SCHOLARSHIPS,
	COL_PUBLICATIONS,
	COL_DESIGNATIONS
};
typedef struct telephone
{
        char *name;
        int number;
}TELEPHONE;

typedef struct identity
{
	char *name;
	TELEPHONE phone_num;
}IDENTITY;

typedef struct education
{
	char *institution_name;
	int year;
	int month;
	int day;
	struct education *nxtptr;
}EDUCATION;

// hackish?
GtkWidget *viewtext;
WebKitWebView *webView;
GtkWidget *window;





static void print_identity( IDENTITY inId, char *szTemp)
{
	sprintf(szTemp, "%s %s %d", inId.name, inId.phone_num.name, inId.phone_num.number);
}
static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
	TELEPHONE index;
	index.name = "Jane Doe";
	index.number = 12345;
	printf("Name: %s\n", index.name);
	printf("Telephone number: %d\n", index.number);
	
	IDENTITY myId;
	myId.name = "Greg Marti";
	myId.phone_num = index;
	char szTemp[255];
	print_identity(myId, szTemp);
	printf("Telephone numm: %s\n", szTemp);

	EDUCATION ed1 = {0};
	ed1.institution_name = "UWO";

	EDUCATION ed2 = {0};
	ed2.institution_name = "UPHO";
	ed2.year = 2004;
	ed2.month = 3;
	ed2.day=6;

	EDUCATION ed3 = {0};
	ed3.institution_name = "GUMP";

	ed1.nxtptr = &ed2;
	ed2.nxtptr = &ed3;

	EDUCATION *edTemp;

	edTemp = ed1.nxtptr;	
	while(edTemp)
	{
		printf("Next Edu: %s\n", edTemp->institution_name);
		printf("Date start %d/%d/%d\n", edTemp->year, edTemp->month, edTemp->day);
		edTemp = edTemp->nxtptr;
	};

}
GtkWidget *dialog_education_new(GtkWidget *parent)
{
	GtkWidget *content_area;
	GtkWidget *grid;
	GtkWidget *label, *username_entry, *password_entry;;
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	GtkWidget *newdialog;
	newdialog = gtk_dialog_new_with_buttons ("My dialog",
                                      	GTK_WINDOW(parent),
                                       flags,
                                       "_OK",
                                       GTK_RESPONSE_ACCEPT,
                                       "_Cancel",
                                       GTK_RESPONSE_REJECT,
                                       NULL);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG (newdialog));
	grid = gtk_grid_new ();
	g_object_set (grid, "margin", 6, NULL);
	gtk_grid_set_column_spacing (GTK_GRID (grid), 6);
	gtk_grid_set_row_spacing (GTK_GRID (grid), 4);
	gtk_box_pack_start (GTK_BOX (content_area), grid, TRUE, TRUE, 0);
	gtk_widget_show (grid);
	label = gtk_label_new ("User name:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	username_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (username_entry, TRUE);
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(username_entry), GTK_ENTRY_ICON_PRIMARY, "mail-message-new");
	gtk_grid_attach (GTK_GRID (grid), username_entry,
                   1, 0, 1, 1);
 
	label = gtk_label_new ("Password:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
	password_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (password_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (password_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), password_entry,
		           1, 1, 1, 1);

	g_object_set_data(G_OBJECT(content_area), "uname1", username_entry);
	g_object_set_data(G_OBJECT(content_area), "pword1", password_entry);

	gtk_widget_show_all (content_area);

	return newdialog;
}
gboolean treeselectionfunction (GtkTreeSelection *selection,
                         GtkTreeModel *model,
                         GtkTreePath *path,
                         gboolean path_currently_selected,
                         gpointer data)
{
	GtkTreeIter iter;
	if (gtk_tree_model_get_iter(model, &iter, path))
	{
		gboolean *sensitive;
		gtk_tree_model_get(model, &iter, 4, &sensitive, -1);
		if (!sensitive)
		{
			return FALSE;
		}		
	}
	return TRUE;
}
static void
tree_selection_changed_cb (GtkTreeSelection *selection, gpointer data)
{
        GtkTreeIter iter;
        GtkTreeModel *model, *listmodel;
		GtkTreeView *listview;
        

        if (gtk_tree_selection_get_selected (selection, &model, &iter))
        {
		//this should probably get an ID or something...
                gtk_tree_model_get (model, &iter, 8, &listmodel, -1);
				printf ("Get listmodel: %d\n", listmodel);
                gtk_tree_model_get (model, &iter, 6, &listview, -1);
				printf ("Get listview: %d\n", listview);

				gtk_tree_view_set_model (GTK_TREE_VIEW (listview), GTK_TREE_MODEL (listmodel));

				printf("OK Set model.\n");
				int typething;
                gtk_tree_model_get (model, &iter, 7, &typething, -1);
				switch (typething)
				{
					case COL_EDUCATION:
					{
						g_print("Global selection now must become EDUCATION.\n");
						break;
					}
					case COL_SCHOLARSHIPS:
					{
						g_print("Global selection now must become SCHOLARSHIPS.\n");
						break;
					}
					default:
					{
						g_print("OK, nothing. %d.\n", typething);
						break;
					}
				}
				//clear main tree
				//fill main tree
				//gtk_tree_view_set_model //now I need to pass in a model.

		//the id should correspond to a model, which should be applied to the main tree.

		//alternatively, if the view is different, you could associate a new view. But I imagine
		//you'd still need to load the data...although this could occur at start time, not run time.
		// so...if I load a resume file, I could populate all the models, and then display the various
		// "forms" when each was chosen from the sidebar

                
        }
}
static void view_onRowActivated(GtkTreeView        *treeview, GtkTreePath        *path,  GtkTreeViewColumn  *col,  gpointer            userdata)
{
//do I care what I get back here? I'm just adding it to the grid.

/*
	For each row in the grid, I have a pointer to its dialog box (which was created during
	the initialization of the program).
	I also have a pointer to the data structure that corresponds to the row
	So ultimately, I need to:
		1. Apply the values of the data structure to the dialog
		2. Display the dialog
		3. Update the values of the data structure based on dialog input
*/
printf("I'm here %s", "here");

	GtkTreeModel *model;
	GtkTreeIter iter;
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model, &iter, path);

	GtkDialog *dialog2;
	EDUCATION *education;
	//Get Education
	gtk_tree_model_get(model, &iter, 3, &education, -1);
	// dialog2 is a pointer to a dialog.
	gtk_tree_model_get(model, &iter, 4, &dialog2, -1);

	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog2));

	GtkEntry *user_entry = g_object_get_data (G_OBJECT (content_area), "uname1");
	GtkEntry *pword_entry = g_object_get_data (G_OBJECT (content_area), "pword1");

	gtk_entry_set_text(GTK_ENTRY (user_entry), "Not Set");
	gtk_entry_set_text(GTK_ENTRY (pword_entry), "Not Set");

	gtk_entry_set_text(GTK_ENTRY (pword_entry), education->institution_name);
	gint result = gtk_dialog_run (GTK_DIALOG (dialog2));

	switch (result)
    {
		case GTK_RESPONSE_ACCEPT:
		{
			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (pword_entry))); 
			education->institution_name = szBuffer;
			break;
		}
		default:
         printf("Cancelled");
         break;
    }
 // gtk_widget_destroy (dialog);
	gtk_widget_hide(GTK_WIDGET(dialog2));

}
void add_new()
{
/*
EDUCATION *education2 = malloc(sizeof(EDUCATION));
			education2->year = 2004;
			education2->month = 12;
			education2->day = 31;

			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (pword_entry))); 
			education2->institution_name = szBuffer;

			GtkTreeIter iter;
			GtkTreeModel *model;

			gtk_tree_store_append (treemodellist, &iter, NULL);
			gtk_tree_store_set (treemodellist, &iter,
						0, NULL,
						              1, "ACADEMICS",
						2, 700,		
						3, education2,
						4, dialog2,	
						              -1);
			gtk_tree_view_set_model (GTK_TREE_VIEW (list), GTK_TREE_MODEL (treemodellist));
*/
}
void setuplist(GtkWidget **list)
{

GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;

col = gtk_tree_view_column_new();
gtk_tree_view_column_set_title(col, "First Name");

   renderer = gtk_cell_renderer_pixbuf_new();	
    gtk_tree_view_column_pack_start(col, renderer, FALSE);
    gtk_tree_view_column_set_attributes(col, renderer,
                                        "icon-name", 0,
                                        NULL);

renderer = gtk_cell_renderer_text_new();
 g_object_set (renderer,
                      "ellipsize", PANGO_ELLIPSIZE_END,
                      NULL);

 gtk_tree_view_column_pack_start(col, renderer, TRUE);
 gtk_tree_view_column_set_attributes (col, renderer,
                                             "text", 1,
                                            "weight", 2, 								
                                             NULL);

gtk_tree_view_append_column(GTK_TREE_VIEW(*list), col);
	printf("OK setuplist");

}

void impresume_list_education_new(GtkTreeStore *treemodellist, GtkWidget *dialog)
{
	GtkTreeIter iter;

	EDUCATION *education = malloc(sizeof(EDUCATION));
		education->year = 2004;
		education->month = 12;
		education->day = 31;
		education->institution_name = "Default School";

	gtk_tree_store_append (treemodellist, &iter, NULL);
	gtk_tree_store_set (treemodellist, &iter,
				0, NULL,
                1, education->institution_name,
				2, 700,			
				3, education,
				4, dialog,
                -1);
	printf("OK education");
}
void createstatusbar(GtkWidget *view, GtkWidget **statusbar)
{
	char buff[20];
	gint context_id;
	static int count = 1;

	*statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(view), *statusbar, FALSE, TRUE, 0);

	context_id = gtk_statusbar_get_context_id(GTK_STATUSBAR(*statusbar),"Statusbar example");

	g_snprintf(buff, 20, "Item %d", count++);
	gtk_statusbar_push(GTK_STATUSBAR(*statusbar), context_id, buff);
	gtk_widget_show(*statusbar);
}
static void button_clicked (GtkWidget *button, gpointer data)
{
	xsltStylesheetPtr cur = NULL;
	xmlDocPtr doc, res;
	xmlChar *result;
    int len;

	xmlSubstituteEntitiesDefault(1);
	xmlLoadExtDtdDefaultValue = 1;
	cur = xsltParseStylesheetFile("cdcatalog.xsl");
	doc = xmlParseFile("cdcatalog.xml");
	res = xsltApplyStylesheet(cur, doc, NULL);
	//xsltSaveResultToFile(stdout, res, cur);
	
	xsltSaveResultToString(&result, &len, res, cur);

//	gtk_label_set_markup (GTK_LABEL(viewtext), result);
//webkit_web_view_load_string(webView, result, NULL, NULL, "http://localhost");
webkit_web_view_load_html(webView, result, NULL);

WebKitPrintOperation *wpint = webkit_print_operation_new (webView);

GtkPrintSettings *gprints = gtk_print_settings_new ();

gtk_print_settings_set_printer (gprints, "Print to File");
gtk_print_settings_set(gprints, GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT, "pdf");
//gtk_print_settings_set(gprints, GTK_PRINT_SETTINGS_OUTPUT_BASENAME, "poop");
gtk_print_settings_set(gprints, GTK_PRINT_SETTINGS_OUTPUT_URI, "file:///home/greg/Projects/anydentity/coombianana.pdf");


webkit_print_operation_set_print_settings(wpint, gprints);
webkit_print_operation_print (wpint); 





	xsltFreeStylesheet(cur);
	xmlFreeDoc(res);
	xmlFreeDoc(doc);

        xsltCleanupGlobals();
        xmlCleanupParser();
}
static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget  *popover, *gimage, *menubut, *tree, *scrollview,  *notebook, *scrollview2;
//	GtkListStore *treemodel;
	GtkTreeStore *treemodel;
GtkTreeSelection *treeselection;
  GtkWidget *button, *hbox;
  GtkWidget *button_box, *dialog;
GtkWidget *header;
GtkTreeIter iter, iter2;
GMenu *menmodel;
GtkStyleContext *style_context;
GtkWidget *mainsplitter, *viewsplitter;

GtkWidget *list;

GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
GdkPixbuf     *icon;
GtkWidget *statusbar, *viewbox;
const gint gweight = 400;
const gint gheight = -1;
const gboolean gsensitive = TRUE;

const gchar *s = "open-menu-symbolic";


  window = gtk_application_window_new (app);

  gtk_window_set_default_size (GTK_WINDOW (window), 1024, 768);

	header = gtk_header_bar_new();
	gtk_header_bar_set_title(GTK_HEADER_BAR(header), "He shoots, he scores");
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
	
gtk_window_set_titlebar(GTK_WINDOW(window), header);


gimage = gtk_image_new_from_icon_name("emblem-system-symbolic", GTK_ICON_SIZE_SMALL_TOOLBAR);
menubut = gtk_menu_button_new();
gtk_container_add (GTK_CONTAINER (menubut), gimage);
popover= gtk_popover_new (menubut);
gtk_menu_button_set_popover(GTK_MENU_BUTTON(menubut), popover);

menmodel = g_menu_new ();
  g_menu_append (menmodel, "New", "app.new");
  g_menu_append (menmodel, "About", "win.about");
gtk_menu_button_set_menu_model (GTK_MENU_BUTTON(menubut), G_MENU_MODEL (menmodel));

gtk_header_bar_pack_end(GTK_HEADER_BAR(header), menubut);


button = gtk_button_new_with_label("Button");
g_signal_connect (G_OBJECT (button), "clicked",
                  G_CALLBACK (button_clicked),
                  NULL);


gtk_header_bar_pack_start(GTK_HEADER_BAR(header), button);



/*
	DIALOG
	
	NOTE: ORDER MATTERS!!!! This must be set PRIOR to creating the list
	because we depend on the dialog pointer (i.e. not being NULL)
*/

dialog = dialog_education_new(window);


/*
	LIST
*/

list = gtk_tree_view_new ();
setuplist(&list);

/*
	SCROLL AREA
*/


scrollview = gtk_scrolled_window_new (NULL, NULL);
gtk_widget_set_no_show_all (scrollview, TRUE);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrollview),
                                        GTK_POLICY_NEVER,
                                        GTK_POLICY_AUTOMATIC);

scrollview2 = gtk_scrolled_window_new (NULL, NULL);
gtk_widget_set_no_show_all (scrollview2, TRUE);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrollview2),
                                        GTK_POLICY_AUTOMATIC,
                                        GTK_POLICY_AUTOMATIC);

tree = gtk_tree_view_new ();
col = gtk_tree_view_column_new();
gtk_tree_view_column_set_title(col, "First Name");

   renderer = gtk_cell_renderer_pixbuf_new();	
    gtk_tree_view_column_pack_start(col, renderer, FALSE);
    gtk_tree_view_column_set_attributes(col, renderer,
                                        "icon-name", 0,
                                        NULL);

renderer = gtk_cell_renderer_text_new();
 g_object_set (renderer,
                      "ellipsize", PANGO_ELLIPSIZE_END,
                      NULL);

 gtk_tree_view_column_pack_start(col, renderer, TRUE);
 gtk_tree_view_column_set_attributes (col, renderer,
                                             "text", 1,
                                            "weight", 2, 
						"height", 3, 	
						"sensitive", 4,				
                                             NULL);
/*
	THIS WILL NOT WORK!!!! MUST CREATE A CUSTOM RENDERER FOR "PILLS"!!!
*/
renderer = gtk_cell_renderer_text_new();
/*
 g_object_set (renderer,"ellipsize", PANGO_ELLIPSIZE_END, NULL);
g_object_set (renderer,"alignment", PANGO_ALIGN_RIGHT, NULL);
 g_object_set (renderer,"background", "#808080", NULL);
 g_object_set (renderer,"foreground", "#ffffff", NULL);
 g_object_set (renderer,"xpad", 4, NULL);
 g_object_set (renderer,"ypad", 4, NULL);
 g_object_set (renderer,"xalign", 4, NULL);
 g_object_set (renderer,"yalign", 4, NULL);
*/
 g_object_set (renderer,"visible", FALSE, NULL);
// g_object_set (renderer,"weight", "700", NULL);
// g_object_set (renderer,"font-size", "small", NULL);

 gtk_tree_view_column_pack_start(col, renderer, FALSE);
 gtk_tree_view_column_set_attributes (col, renderer,
                                             "text", 5,
                                             NULL);

//clickable, x-offfset, weight



gtk_tree_view_append_column(GTK_TREE_VIEW(tree), col);

treemodel = gtk_tree_store_new(9, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_INT, G_TYPE_POINTER);

 gtk_tree_store_append (treemodel, &iter, NULL);

// OK. I need to associate a tree model with this list, but it is for the main list. So when a user 
// selects a row in this tree, I will pass over the model to the other list. Confused yet?

GtkTreeStore *mainmodellist;


gtk_tree_store_set (treemodel, &iter,
			0, NULL,
                          1, "ACADEMICS",
			2, 700,
			3, 40,
			4, FALSE,
			5, "",
			6, list,
			7, COL_HEADER,
			8, NULL,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);

impresume_list_education_new(mainmodellist, dialog);

gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "go-top",
                          1, "Education",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "",
			6, list,
			7, COL_EDUCATION,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "help-about",
                          1, "Awards",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "",
			6, list,
			7, COL_AWARDS,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "mail-message-new",
                          1, "Scholarships",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_SCHOLARSHIPS,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "emblem-documents",
                          1, "Publications",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "emblem-important",
                          1, "Designations",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);


gtk_tree_store_append (treemodel, &iter, NULL);
gtk_tree_store_set (treemodel, &iter,
			0, NULL,
                          1, "PROFESSIONAL",
			2, 700,
			3, 40,
			4, FALSE,
			5, "",
			6, list,
			7, COL_HEADER,
			8, NULL,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "battery-low",
                          1, "Experience",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "image-loading",
                          1, "Certification",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "mail-replied",
                          1, "Job-training",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "network-transmit",
                          1, "Apprenticeships",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

gtk_tree_store_append (treemodel, &iter, NULL);
gtk_tree_store_set (treemodel, &iter,
			0, NULL,
                          1, "PERSONAL",
			2, 700,
			3, 40,
			4, FALSE,
			5, "",
			6, list,
			7, COL_HEADER,
			8, NULL,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "battery-low",
                          1, "Contact Information",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "image-loading",
                          1, "Objective",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "mail-replied",
                          1, "Volunteering",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "network-transmit",
                          1, "Hobbies",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "network-transmit",
                          1, "Sports",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);


gtk_tree_store_append (treemodel, &iter, NULL);
gtk_tree_store_set (treemodel, &iter,
			0, NULL,
                          1, "RESULTS",
			2, 700,
			3, 40,
			4, FALSE,
			5, "",
			6, list,
			7, COL_HEADER,
			8, NULL,
                          -1);


mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "battery-low",
                          1, "Resumes",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);

mainmodellist = gtk_tree_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_POINTER, G_TYPE_POINTER);
gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "image-loading",
                          1, "Templates",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_HEADER,
			8, mainmodellist,
                          -1);
 
gtk_tree_view_set_model (GTK_TREE_VIEW (tree), GTK_TREE_MODEL (treemodel));
g_object_unref(treemodel);
	gtk_tree_view_set_level_indentation(GTK_TREE_VIEW (tree), 25);
	gtk_tree_view_expand_all (GTK_TREE_VIEW (tree));
        gtk_tree_view_set_enable_search (GTK_TREE_VIEW (tree), FALSE);
	gtk_tree_view_set_show_expanders(GTK_TREE_VIEW (tree), FALSE);





treeselection = gtk_tree_view_get_selection(GTK_TREE_VIEW (tree));
gtk_tree_selection_set_select_function(treeselection, treeselectionfunction, NULL, NULL);
gtk_tree_selection_set_mode (treeselection, GTK_SELECTION_SINGLE);
g_signal_connect (G_OBJECT (treeselection), "changed",
                  G_CALLBACK (tree_selection_changed_cb),
                  NULL);




 gtk_container_add (GTK_CONTAINER (scrollview), tree);

mainsplitter = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);

 g_object_set (mainsplitter,"position", 250,NULL);

viewsplitter = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);

 g_object_set (viewsplitter,"position", 250,NULL);



g_signal_connect(G_OBJECT(list), "row-activated", G_CALLBACK(view_onRowActivated), NULL);
gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);



GtkTextBuffer *buffer;

viewtext = gtk_label_new ("This is <strong>ome</strong>text");
/*const gchar *text =
"Go to the"
"<a href=\"http://www.gtk.org\" title=\"Our website\">"
"GTK+ website</a> for more..."
"and pass it to the gtk_container_children() function. This creates a duplicate of the original list, so it's advisable to free it up using g_list_free() after you're done with it, or to iterate on it destructively, like this:"
"The tree_owner field is defined only in subtrees, where it points to the TreeItem widget which holds the tree in question. The level field indicates how deeply nested a particular tree is; root trees have level 0, and each successive level of subtrees has a level one greater than the parent level. This field is set only after a Tree widget is actually mapped (i.e. drawn on the screen)."
"This signal is emitted when a child of the Tree is about to get deselected. As of GTK 1.0.4, this seems to only occur on calls to gtk_tree_unselect_item() or gtk_tree_unselect_child(), and perhaps on other "
"and pass it to the gtk_container_children() function. This creates a duplicate of the original list, so it's advisable to free it up using g_list_free() after you're done with it, or to iterate on it destructively, like this:"
"The tree_owner field is defined only in subtrees, where it points to the TreeItem widget which holds the tree in question. The level field indicates how deeply nested a particular tree is; root trees have level 0, and each successive level of subtrees has a level one greater than the parent level. This field is set only after a Tree widget is actually mapped (i.e. drawn on the screen)."
"This signal is emitted when a child of the Tree is about to get deselected. As of GTK 1.0.4, this seems to only occur on calls to gtk_tree_unselect_item() or gtk_tree_unselect_child(), and perhaps on other "
;
gtk_label_set_markup (GTK_LABEL(viewtext), text);
g_object_set (viewtext,"margin", 25,NULL);
g_object_set (viewtext,"xpad", 50,NULL);
g_object_set (viewtext,"ypad", 50,NULL);
g_object_set (viewtext,"wrap", TRUE,NULL);
*/

//webkit
webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

g_object_set (webView,"margin", 25,NULL);



//gtk_container_add (GTK_CONTAINER (scrollview2), viewtext);
gtk_container_add (GTK_CONTAINER (scrollview2), GTK_WIDGET(webView));

gtk_widget_set_size_request(GTK_WIDGET(webView), 600, 800);


viewbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
gtk_box_pack_start(GTK_BOX(viewbox), viewsplitter, TRUE, TRUE, 0);
//createstatusbar(viewbox, &statusbar);

gtk_paned_add1(GTK_PANED(mainsplitter), scrollview);
	gtk_paned_add2(GTK_PANED(mainsplitter), viewbox);

gtk_paned_add1(GTK_PANED(viewsplitter), list);
	gtk_paned_add2(GTK_PANED(viewsplitter), scrollview2);

GtkCssProvider *cssProvider = gtk_css_provider_new();
gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (cssProvider),
                                 " .booger {\n"
                                 "   box-shadow: 0px 0px 15px #808080;\n"
                                 "   border-radius: 3px;\n"
                                 "   background-color: #ffffff;\n"
                                 "}\n",
				 -1, NULL);


    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);


style_context = gtk_widget_get_style_context(tree);
gtk_style_context_add_class(style_context, "sidebar");
style_context = gtk_widget_get_style_context(GTK_WIDGET(webView));
gtk_style_context_add_class(style_context, "booger");
style_context = gtk_widget_get_style_context(list);
gtk_style_context_add_class(style_context, "view");


/*
	SHOW STUFF
*/





gtk_widget_show (list);
gtk_widget_show (button);
gtk_widget_show (GTK_WIDGET(webView));
gtk_widget_show (viewsplitter);


gtk_widget_show (viewbox);
gtk_widget_show (mainsplitter);

  gtk_container_add (GTK_CONTAINER (window), mainsplitter);
gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tree), FALSE);
gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree), 0);


  gtk_widget_show_all (header);
gtk_widget_show (tree);

gtk_widget_show(scrollview2);
gtk_widget_show(scrollview);
gtk_widget_show (window);

}


int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;


  app = gtk_application_new ("org.gnome.impresume", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
