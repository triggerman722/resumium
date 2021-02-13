/*

	BUILD

	gcc `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -o example-1 example-1.c \
			`xml2-config --cflags` \
			`xml2-config --libs` \
			`xslt-config --cflags` \
			`xslt-config --libs` \
			`pkg-config --cflags --libs webkit2gtk-4.0`
*/

// to build: gcc `pkg-config --cflags gtk+-3.0` -o example-1 example-1.c `pkg-config --libs gtk+-3.0` `xml2-config --cflags` `xml2-config --libs` `xslt-config --cflags` `xslt-config --libs`
// to build(old):  gcc `pkg-config --cflags gtk+-3.0` -o example-1 example-1.c `pkg-config --libs gtk+-3.0`


#include <gtk/gtk.h>

#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
//#include <libxml/DOCBparser.h>
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
	COL_DESIGNATIONS,
	COL_EXPERIENCE
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

typedef struct experience
{
	char *job_title;
	int year;
	int month;
	int day;
	struct experience *nxtptr;
}EXPERIENCE;

typedef struct designation
{
	char *name;
	// the part you put after your name, such as Benny Magnum, PI
	char *postnominal;
	char *grantingagency;
	int year;
	int month;
	int day;
} DESIGNATION;

typedef struct award
{
	int year;
	int month;
	int day;
	char *name;
char *postnominal;
	char *grantingagency;
	float amount;
		
} AWARD;

typedef struct scholarship
{
	float amount;
	char *name;
	char *description;
	char *grantingagency;
	int year;
	int month;
	int day;
} SCHOLARSHIP;

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
GtkWidget *dialog_designation_new(GtkWidget *parent)
{
	GtkWidget *content_area;
	GtkWidget *grid;
	GtkWidget *label, *designation_name_entry, *designation_postnominal_entry, *designation_grantingagency_entry;
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	GtkWidget *newdialog;
	newdialog = gtk_dialog_new_with_buttons ("Professional Designations",
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
	label = gtk_label_new ("Name:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	designation_name_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (designation_name_entry, TRUE);
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(designation_name_entry), GTK_ENTRY_ICON_PRIMARY, "mail-message-new");
	gtk_grid_attach (GTK_GRID (grid), designation_name_entry,
                   1, 0, 1, 1);
 
	label = gtk_label_new ("Post Nominal:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
	designation_postnominal_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (designation_postnominal_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (designation_postnominal_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), designation_postnominal_entry,
		           1, 1, 1, 1);


	label = gtk_label_new ("Granting Agency:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
	designation_grantingagency_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (designation_grantingagency_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (designation_grantingagency_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), designation_grantingagency_entry,
		           1, 2, 1, 1);

	g_object_set_data(G_OBJECT(content_area), "designation_name_entry", designation_name_entry);
	g_object_set_data(G_OBJECT(content_area), "designation_postnominal_entry", designation_postnominal_entry);
	g_object_set_data(G_OBJECT(content_area), "designation_grantingagency_entry", designation_grantingagency_entry);

	gtk_widget_show_all (content_area);

	return newdialog;
}
GtkWidget *dialog_award_new(GtkWidget *parent)
{
	GtkWidget *content_area;
	GtkWidget *grid;
	GtkWidget *label, *award_name_entry, *award_postnominal_entry, *award_grantingagency_entry;
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	GtkWidget *newdialog;
	newdialog = gtk_dialog_new_with_buttons ("Professional awards",
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
	label = gtk_label_new ("Name:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	award_name_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (award_name_entry, TRUE);
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(award_name_entry), GTK_ENTRY_ICON_PRIMARY, "mail-message-new");
	gtk_grid_attach (GTK_GRID (grid), award_name_entry,
                   1, 0, 1, 1);
 
	label = gtk_label_new ("Post Nominal:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
	award_postnominal_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (award_postnominal_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (award_postnominal_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), award_postnominal_entry,
		           1, 1, 1, 1);


	label = gtk_label_new ("Granting Agency:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
	award_grantingagency_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (award_grantingagency_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (award_grantingagency_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), award_grantingagency_entry,
		           1, 2, 1, 1);

	g_object_set_data(G_OBJECT(content_area), "award_name_entry", award_name_entry);
	g_object_set_data(G_OBJECT(content_area), "award_postnominal_entry", award_postnominal_entry);
	g_object_set_data(G_OBJECT(content_area), "award_grantingagency_entry", award_grantingagency_entry);

	gtk_widget_show_all (content_area);

	return newdialog;
}

GtkWidget *dialog_scholarship_new(GtkWidget *parent)
{
	GtkWidget *content_area;
	GtkWidget *grid;
	GtkWidget *label, *scholarship_name_entry, *scholarship_description_entry, *scholarship_grantingagency_entry;
	GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
	GtkWidget *newdialog;
	newdialog = gtk_dialog_new_with_buttons ("Professional scholarships",
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
	label = gtk_label_new ("Name:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 1, 1);
	scholarship_name_entry = gtk_entry_new ();
	gtk_widget_set_hexpand (scholarship_name_entry, TRUE);
	gtk_entry_set_icon_from_icon_name (GTK_ENTRY(scholarship_name_entry), GTK_ENTRY_ICON_PRIMARY, "mail-message-new");
	gtk_grid_attach (GTK_GRID (grid), scholarship_name_entry,
                   1, 0, 1, 1);
 
	label = gtk_label_new ("Description:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
	scholarship_description_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (scholarship_description_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (scholarship_description_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), scholarship_description_entry,
		           1, 1, 1, 1);


	label = gtk_label_new ("Granting Agency:");
	gtk_widget_set_halign (label, GTK_ALIGN_END);
	gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
	scholarship_grantingagency_entry = gtk_entry_new ();
	
	gtk_widget_set_hexpand (scholarship_grantingagency_entry, TRUE);

	gtk_entry_set_activates_default (GTK_ENTRY (scholarship_grantingagency_entry),
                                   TRUE);
	gtk_grid_attach (GTK_GRID (grid), scholarship_grantingagency_entry,
		           1, 2, 1, 1);

	g_object_set_data(G_OBJECT(content_area), "scholarship_name_entry", scholarship_name_entry);
	g_object_set_data(G_OBJECT(content_area), "scholarship_description_entry", scholarship_description_entry);
	g_object_set_data(G_OBJECT(content_area), "scholarship_grantingagency_entry", scholarship_grantingagency_entry);

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
                gtk_tree_model_get (model, &iter, 6, &listview, -1);

				gtk_tree_view_set_model (GTK_TREE_VIEW (listview), GTK_TREE_MODEL (listmodel));
        }
}
// The callback (and others like it) will need to be data specific. So this will
// deal with education, another with experience, etc. Think of this as handling data output.
static void show_education_dialog_cb (GtkWidget *widget, gpointer   user_data)
{
	printf ("Me show\n");
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (widget));

	// Because I am in a data specific handler, I can make assumptions about the dialog (such as: its the education dialog)
	GtkEntry *user_entry = g_object_get_data (G_OBJECT (content_area), "uname1");
	GtkEntry *pword_entry = g_object_get_data (G_OBJECT (content_area), "pword1");

	EDUCATION *medu;
	medu = user_data;
	gtk_entry_set_text(GTK_ENTRY (pword_entry), medu->institution_name);
}
static void response_education_dialog_cb (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	printf ("Me response\n");
	switch (response_id)
    {
		case GTK_RESPONSE_ACCEPT:
		{
			//Need to extract the data back out of the dialog.
			GtkWidget *content_area = gtk_dialog_get_content_area (dialog);
			GtkEntry *user_entry = g_object_get_data (G_OBJECT (content_area), "uname1");
			GtkEntry *pword_entry = g_object_get_data (G_OBJECT (content_area), "pword1");

			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (pword_entry)));

			// Need to assign it to the original data structure (or create new)

			EDUCATION *medu;
			medu = user_data; 
			medu->institution_name = szBuffer;

			break;
		}
		default:
         printf("Cancelled\n");
         break;
    }

}

static void show_experience_dialog_cb (GtkWidget *widget, gpointer   user_data)
{
	printf ("Me exp show\n");
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (widget));

	GtkEntry *user_entry = g_object_get_data (G_OBJECT (content_area), "uname1");
	GtkEntry *pword_entry = g_object_get_data (G_OBJECT (content_area), "pword1");

	gtk_entry_set_text(GTK_ENTRY (user_entry), "Not Set");
	gtk_entry_set_text(GTK_ENTRY (pword_entry), "Not Set");

	EXPERIENCE *medu;
	medu = user_data;
	gtk_entry_set_text(GTK_ENTRY (pword_entry), medu->job_title);
}
static void response_experience_dialog_cb (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	printf ("Me exp response\n");
	switch (response_id)
    {
		case GTK_RESPONSE_ACCEPT:
		{
			//Need to extract the data back out of the dialog.
			GtkWidget *content_area = gtk_dialog_get_content_area (dialog);
			GtkEntry *user_entry = g_object_get_data (G_OBJECT (content_area), "uname1");
			GtkEntry *pword_entry = g_object_get_data (G_OBJECT (content_area), "pword1");

			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (pword_entry)));

			// Need to assign it to the original data structure (or create new)

			EXPERIENCE *medu;
			medu = user_data; 
			medu->job_title = szBuffer;

			break;
		}
		default:
         printf("Cancelled\n");
         break;
    }

}
static void show_designation_dialog_cb (GtkWidget *widget, gpointer   user_data)
{
	printf ("Me show\n");
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (widget));

	// Because I am in a data specific handler, I can make assumptions about the dialog (such as: its the education dialog)
	GtkEntry *designation_name_entry = g_object_get_data (G_OBJECT (content_area), "designation_name_entry");
	GtkEntry *designation_postnominal_entry = g_object_get_data (G_OBJECT (content_area), "designation_postnominal_entry");
	GtkEntry *designation_grantingagency_entry = g_object_get_data (G_OBJECT (content_area), "designation_grantingagency_entry");

	DESIGNATION *user_data_set;
	user_data_set = user_data;
	gtk_entry_set_text(GTK_ENTRY (designation_name_entry), user_data_set->name);
	gtk_entry_set_text(GTK_ENTRY (designation_postnominal_entry), user_data_set->postnominal);
	gtk_entry_set_text(GTK_ENTRY (designation_grantingagency_entry), user_data_set->grantingagency);

}
static void show_award_dialog_cb (GtkWidget *widget, gpointer   user_data)
{
	printf ("Me show\n");
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (widget));

	// Because I am in a data specific handler, I can make assumptions about the dialog (such as: its the education dialog)
	GtkEntry *award_name_entry = g_object_get_data (G_OBJECT (content_area), "award_name_entry");
	GtkEntry *award_postnominal_entry = g_object_get_data (G_OBJECT (content_area), "award_postnominal_entry");
	GtkEntry *award_grantingagency_entry = g_object_get_data (G_OBJECT (content_area), "award_grantingagency_entry");

	AWARD *user_data_set;
	user_data_set = user_data;
	gtk_entry_set_text(GTK_ENTRY (award_name_entry), user_data_set->name);
	gtk_entry_set_text(GTK_ENTRY (award_postnominal_entry), user_data_set->postnominal);
	gtk_entry_set_text(GTK_ENTRY (award_grantingagency_entry), user_data_set->grantingagency);

}
static void response_award_dialog_cb (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	printf ("Me response\n");
	switch (response_id)
    {
		case GTK_RESPONSE_ACCEPT:
		{
			//Need to extract the data back out of the dialog.
			GtkWidget *content_area = gtk_dialog_get_content_area (dialog);
			GtkEntry *award_name_entry = g_object_get_data (G_OBJECT (content_area), "award_name_entry");
			GtkEntry *award_postnominal_entry = g_object_get_data (G_OBJECT (content_area), "award_postnominal_entry");
			GtkEntry *award_grantingagency_entry = g_object_get_data (G_OBJECT (content_area), "award_grantingagency_entry");

			AWARD *medu;
			medu = user_data; 

			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (award_name_entry)));
			// Need to assign it to the original data structure (or create new)
			medu->name = szBuffer;

			char *szBuffer2 =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer2,gtk_entry_get_text(GTK_ENTRY (award_postnominal_entry)));
			// Need to assign it to the original data structure (or create new)
			medu->postnominal = szBuffer2;

			char *szBuffer3 =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer3,gtk_entry_get_text(GTK_ENTRY (award_grantingagency_entry)));
			// Need to assign it to the original data structure (or create new)
			medu->grantingagency = szBuffer3;

			//TODO: Dates

			break;
		}
		default:
         printf("Cancelled\n");
         break;
    }

}

static void response_designation_dialog_cb (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	printf ("Me response\n");
	switch (response_id)
    {
		case GTK_RESPONSE_ACCEPT:
		{
			//Need to extract the data back out of the dialog.
			GtkWidget *content_area = gtk_dialog_get_content_area (dialog);
			GtkEntry *designation_name_entry = g_object_get_data (G_OBJECT (content_area), "designation_name_entry");
			GtkEntry *designation_postnominal_entry = g_object_get_data (G_OBJECT (content_area), "designation_postnominal_entry");
			GtkEntry *designation_grantingagency_entry = g_object_get_data (G_OBJECT (content_area), "designation_grantingagency_entry");

			DESIGNATION *medu;
			medu = user_data; 

			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (designation_name_entry)));
			// Need to assign it to the original data structure (or create new)
			medu->name = szBuffer;

			char *szBuffer2 =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer2,gtk_entry_get_text(GTK_ENTRY (designation_postnominal_entry)));
			// Need to assign it to the original data structure (or create new)
			medu->postnominal = szBuffer2;

			char *szBuffer3 =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer3,gtk_entry_get_text(GTK_ENTRY (designation_grantingagency_entry)));
			// Need to assign it to the original data structure (or create new)
			medu->grantingagency = szBuffer3;

			//TODO: Dates

			break;
		}
		default:
         printf("Cancelled\n");
         break;
    }

}

static void show_scholarship_dialog_cb (GtkWidget *widget, gpointer   user_data)
{
	printf ("Scholarship show\n");
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (widget));

	// Because I am in a data specific handler, I can make assumptions about the dialog (such as: its the scholarship dialog)
	GtkEntry *scholarship_description_entry = g_object_get_data (G_OBJECT (content_area), "scholarship_description_entry");

	SCHOLARSHIP *medu;
	medu = user_data;
	gtk_entry_set_text(GTK_ENTRY (scholarship_description_entry), medu->description);
}
static void response_scholarship_dialog_cb (GtkDialog *dialog, gint response_id, gpointer user_data)
{
	printf ("Scholarship response\n");
	switch (response_id)
    {
		case GTK_RESPONSE_ACCEPT:
		{
			//Need to extract the data back out of the dialog.
			GtkWidget *content_area = gtk_dialog_get_content_area (dialog);
			GtkEntry *scholarship_description_entry = g_object_get_data (G_OBJECT (content_area), "scholarship_description_entry");

			char *szBuffer =  malloc(sizeof(char) * 255); 
			strcpy(szBuffer,gtk_entry_get_text(GTK_ENTRY (scholarship_description_entry)));

			// Need to assign it to the original data structure (or create new)

			SCHOLARSHIP *medu;
			medu = user_data; 
			medu->description = szBuffer;

			break;
		}
		default:
         printf("Cancelled\n");
         break;
    }

}




static void view_onRowActivated(GtkTreeView        *treeview, GtkTreePath        *path,  GtkTreeViewColumn  *col,  gpointer            userdata)
{

	GtkTreeModel *model;
	GtkTreeIter iter;
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model, &iter, path);

	GtkDialog *dialog2;

	// data_strcuture will hold a pointer to things like education, experience, etc.
	gpointer *data_structure;

	// these pointers are to call back functions for each of the show and response signals.
	gpointer *show_cb;
	gpointer *response_cb;

	//Get data_strcuture
	gtk_tree_model_get(model, &iter, 3, &data_structure, -1);

	// dialog2 is a pointer to a dialog.
	gtk_tree_model_get(model, &iter, 4, &dialog2, -1);

	//show_cb is a pointer to a show callback for the dialog
	gtk_tree_model_get(model, &iter, 5, &show_cb, -1);

	//response_cb is a pointer to a response callback for the dialog
	gtk_tree_model_get(model, &iter, 6, &response_cb, -1);

	printf("Got data %p\n", data_structure);
	printf("Got show callback %p\n", show_cb);
	printf("Got response callback %p\n", response_cb);

	// So I could change the following to a static call back, set only on init.
	// then I would need only to set the data structure to the dialog.
	// g_object_set_data(G_OBJECT(dialog), "data_structure", data_structure);
	// later (in the callback) I could get the structure via: 
	// gpointer data_structure = g_object_get_data (G_OBJECT (dialog), "data_structure");
	// this approach would ease the model, and would make a future glade based ui implmentation seem more "normal"
	// because you would connect the signals via the builder.
	// also, this would get rid of a ton of code shit.

	gint show_handler_id = g_signal_connect(G_OBJECT(dialog2), "show", G_CALLBACK(show_cb), data_structure);
	gint response_handler_id = g_signal_connect(G_OBJECT(dialog2), "response", G_CALLBACK(response_cb), data_structure);
	gint result = gtk_dialog_run (GTK_DIALOG (dialog2));

	g_signal_handler_disconnect (G_OBJECT(dialog2), show_handler_id);
	g_signal_handler_disconnect (G_OBJECT(dialog2), response_handler_id);

	gtk_widget_hide(GTK_WIDGET(dialog2));

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
				5, show_education_dialog_cb,
				6, response_education_dialog_cb,
                -1);
	printf("OK education");
}
void impresume_list_experience_new(GtkTreeStore *treemodellist, GtkWidget *dialog)
{
	GtkTreeIter iter;

	EXPERIENCE *experience = malloc(sizeof(EXPERIENCE));
		experience->year = 2004;
		experience->month = 12;
		experience->day = 31;
		experience->job_title = "Default Experience";

	gtk_tree_store_append (treemodellist, &iter, NULL);
	gtk_tree_store_set (treemodellist, &iter,
				0, NULL,
                1, experience->job_title,
				2, 700,			
				3, experience,
				4, dialog,
				5, show_experience_dialog_cb,
				6, response_experience_dialog_cb,
                -1);
	printf("OK experience\n");
}
void impresume_list_designation_new(GtkTreeStore *treemodellist, GtkWidget *dialog)
{
	GtkTreeIter iter;

	DESIGNATION *designation = malloc(sizeof(DESIGNATION));
		designation->year = 2004;
		designation->month = 12;
		designation->day = 31;
		designation->name = "Person of Humungous Dimensions";
		designation->postnominal = "PhD.";
		designation->grantingagency = "Someone";

	gtk_tree_store_append (treemodellist, &iter, NULL);
	gtk_tree_store_set (treemodellist, &iter,
				0, NULL,
                1, designation->name,
				2, 700,			
				3, designation,
				4, dialog,
				5, show_designation_dialog_cb,
				6, response_designation_dialog_cb,
                -1);
	printf("OK designation\n");
}
void impresume_list_award_new(GtkTreeStore *treemodellist, GtkWidget *dialog)
{
	GtkTreeIter iter;

	AWARD *award = malloc(sizeof(AWARD));
		award->year = 2004;
		award->month = 12;
		award->day = 31;
		award->name = "Person of Humungous award";
		award->postnominal = "PhD.";
		award->grantingagency = "Someone";

	gtk_tree_store_append (treemodellist, &iter, NULL);
	gtk_tree_store_set (treemodellist, &iter,
				0, NULL,
                1, award->name,
				2, 700,			
				3, award,
				4, dialog,
				5, show_award_dialog_cb,
				6, response_award_dialog_cb,
                -1);
	printf("OK award\n");
}
void impresume_list_scholarship_new(GtkTreeStore *treemodellist, GtkWidget *dialog)
{
	GtkTreeIter iter;

	SCHOLARSHIP *scholarship = malloc(sizeof(SCHOLARSHIP));
		scholarship->amount = 100.00;
		scholarship->year = 2004;
		scholarship->month = 12;
		scholarship->day = 31;
		scholarship->name = "Person of Humungous scholarship";
		scholarship->description = "PhD.";
		scholarship->grantingagency = "Someone";

	gtk_tree_store_append (treemodellist, &iter, NULL);
	gtk_tree_store_set (treemodellist, &iter,
				0, NULL,
                1, scholarship->name,
				2, 700,			
				3, scholarship,
				4, dialog,
				5, show_scholarship_dialog_cb,
				6, response_scholarship_dialog_cb,
                -1);
	printf("OK scholarship\n");
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

GtkTreeStore *initialize_standard_model()
{
	return gtk_tree_store_new(7,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_INT,
			G_TYPE_POINTER, //storage object
			G_TYPE_POINTER, //dialog
			G_TYPE_POINTER, //dialog show callback function
			G_TYPE_POINTER //dialog response callback function
			);
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

// Create the main list data model store
mainmodellist = initialize_standard_model();

// Add content spectific stuff to it
impresume_list_education_new(mainmodellist, dialog);

// Now add to the main navigation panel
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

mainmodellist = initialize_standard_model();
dialog = dialog_award_new(window);
impresume_list_award_new(mainmodellist, dialog);
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

mainmodellist = initialize_standard_model();
dialog = dialog_scholarship_new(window);
impresume_list_scholarship_new(mainmodellist, dialog);
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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();
dialog = dialog_designation_new(window);
// Add content spectific stuff to it
impresume_list_designation_new(mainmodellist, dialog);

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

mainmodellist = initialize_standard_model();

dialog = dialog_education_new(window);
impresume_list_experience_new(mainmodellist, dialog);

gtk_tree_store_append (treemodel, &iter2, &iter);
gtk_tree_store_set (treemodel, &iter2,
			0, "battery-low",
                          1, "Experience",
			2, gweight,
			3, gheight,
			4, gsensitive,
			5, "32",
			6, list,
			7, COL_EXPERIENCE,
			8, mainmodellist,
                          -1);

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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


mainmodellist = initialize_standard_model();

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

mainmodellist = initialize_standard_model();

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
