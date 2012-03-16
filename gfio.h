#ifndef GFIO_H
#define GFIO_H

#include <gtk/gtk.h>

struct probe_widget {
	GtkWidget *hostname;
	GtkWidget *os;
	GtkWidget *arch;
	GtkWidget *fio_ver;
};

struct multitext_widget {
	GtkWidget *entry;
	char **text;
	unsigned int cur_text;
	unsigned int max_text;
};

struct eta_widget {
	GtkWidget *names;
	struct multitext_widget iotype;
	struct multitext_widget bs;
	struct multitext_widget ioengine;
	struct multitext_widget iodepth;
	GtkWidget *jobs;
	GtkWidget *files;
	GtkWidget *read_bw;
	GtkWidget *read_iops;
	GtkWidget *cr_bw;
	GtkWidget *cr_iops;
	GtkWidget *write_bw;
	GtkWidget *write_iops;
	GtkWidget *cw_bw;
	GtkWidget *cw_iops;
};

struct gfio_graphs {
#define DRAWING_AREA_XDIM 1000
#define DRAWING_AREA_YDIM 400
	GtkWidget *drawing_area;
	struct graph *iops_graph;
	struct graph *bandwidth_graph;
};

/*
 * Main window widgets and data
 */
struct gui {
	GtkUIManager *uimanager;
	GtkRecentManager *recentmanager;
	GtkActionGroup *actiongroup;
	guint recent_ui_id;
	GtkWidget *menu;
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *thread_status_pb;
	GtkWidget *buttonbox;
	GtkWidget *notebook;
	GtkWidget *error_info_bar;
	GtkWidget *error_label;
	GtkListStore *log_model;
	GtkWidget *log_tree;
	GtkWidget *log_view;
	struct gfio_graphs graphs;
	struct probe_widget probe;
	struct eta_widget eta;
	pthread_t server_t;

	pthread_t t;
	int handler_running;

	struct flist_head list;
} main_ui;

enum {
	GE_STATE_NEW = 1,
	GE_STATE_CONNECTED,
	GE_STATE_JOB_SENT,
	GE_STATE_JOB_STARTED,
	GE_STATE_JOB_RUNNING,
	GE_STATE_JOB_DONE,
};

enum {
	GFIO_BUTTON_CONNECT = 0,
	GFIO_BUTTON_SEND,
	GFIO_BUTTON_START,
	GFIO_BUTTON_NR,
};

/*
 * Notebook entry
 */
struct gui_entry {
	struct flist_head list;
	struct gui *ui;

	GtkWidget *vbox;
	GtkWidget *job_notebook;
	GtkWidget *thread_status_pb;
	GtkWidget *buttonbox;
	GtkWidget *button[GFIO_BUTTON_NR];
	GtkWidget *notebook;
	GtkWidget *error_info_bar;
	GtkWidget *error_label;
	GtkWidget *results_window;
	GtkWidget *results_notebook;
	GtkUIManager *results_uimanager;
	GtkWidget *results_menu;
	GtkWidget *disk_util_vbox;
	GtkListStore *log_model;
	GtkWidget *log_tree;
	GtkWidget *log_view;
	struct gfio_graphs graphs;
	struct probe_widget probe;
	struct eta_widget eta;
	GtkWidget *page_label;
	gint page_num;
	unsigned int state;

	struct graph *clat_graph;
	struct graph *lat_bucket_graph;

	struct gfio_client *client;
	int nr_job_files;
	char **job_files;
};

struct end_results {
	struct group_run_stats gs;
	struct thread_stat ts;
};

struct gfio_client {
	struct gui_entry *ge;
	struct fio_client *client;
	GtkWidget *err_entry;
	struct thread_options o;

	struct end_results *results;
	unsigned int nr_results;

	struct cmd_du_pdu *du;
	unsigned int nr_du;
};

#define ARRAYSIZE(x) (sizeof((x)) / (sizeof((x)[0])))

#define GFIO_MIME	"text/fio"

#endif