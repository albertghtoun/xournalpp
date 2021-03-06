/*
 * Xournal++
 *
 * The recent opened files
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <Path.h>
#include <XournalType.h>

#include <gtk/gtk.h>

class RecentManagerListener
{
public:
	virtual ~RecentManagerListener();

	/**
	 * This function is called whenever some file
	 * from the recent menu is opened
	 */
	virtual void fileOpened(const gchar* uri) = 0;
};

/**
 * @brief Handles the GtkMenu displaying the recent files
 */
class RecentManager
{
public:
	RecentManager();
	virtual ~RecentManager();

public:

	/**
	 * Adds a file to the underlying GtkRecentManager
	 * without altering the menu
	 */
	void addRecentFileFilename(Path filename);

	/**
	 * Removes a file from the underlying GtkRecentManager
	 * without altering the menu
	 */
	void removeRecentFileFilename(Path filename);

	/**
	 * Removes all of the menu items corresponding to recent files
	 */
	void freeOldMenus();

	/**
	 * Updates the menu of recent files
	 */
	void updateMenu();

	/**
	 * Returns the maximal number of recent files to be displayed
	 */
	int getMaxRecent();

	/**
	 * Sets the maximal number of recent files to be displayed
	 */
	void setMaxRecent(int maxRecent);

	/**
	 * Notifies all RecentManagerListener%s that a new
	 * file is opened
	 */
	void openRecent(Path p);

	/**
	 * Returns the root menu containing all the items
	 * corresponding to the recent files
	 */
	GtkWidget* getMenu();

	/**
	 * Adds a new RecentManagerListener to be notified
	 * of opened files
	 */
	void addListener(RecentManagerListener* listener);

private:

	/**
	 * Filters a list of GtkRecentInfo according to their file types
	 *
	 * @param items A pointer to a GList containing GtkRecentInfo%s
	 * @param xoj   Returns xoj files if xoj is set, pdf files otherwise
	 *
	 * @return      A pointer to a GList containing the relevant GtkRecentInfo%s sorted according to their
	 *              modification dates
	 */
	GList* filterRecent(GList* items, bool xoj);
	void addRecentMenu(GtkRecentInfo* info, int i);

	/**
	 * This callback function is triggered whenever a new
	 * file is added to the recent manager to recreate
	 * all of the menu items
	 */
	static void recentManagerChangedCallback(GtkRecentManager* manager, RecentManager* recentManager);

	/**
	 * This callback function is triggered whenever one of
	 * the items corresponding to recent files is activated
	 */
	static void recentsMenuActivateCallback(GtkAction* action, RecentManager* recentManager);

	/**
	 * This function serves as a comparator to sort different
	 * GtkRecentInfo%s according to their modification date
	 */
	static int sortRecentsEntries(GtkRecentInfo* a, GtkRecentInfo* b);

private:
	XOJ_TYPE_ATTRIB;


	int maxRecent;
	int recentHandlerId;

	std::vector<RecentManagerListener*> listener;

	GtkWidget* menu;
	std::vector<GtkWidget*> menuItemList;
};
