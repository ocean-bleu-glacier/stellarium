/*
 * Pointer Coordinates plug-in for Stellarium
 *
 * Copyright (C) 2014 Alexander Wolf
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ONLINEQUERIES_HPP
#define ONLINEQUERIES_HPP

#include "StelGui.hpp"
#include "StelModule.hpp"
#include "HipOnlineQuery.hpp"

#include <QFont>
#include <QString>
#include <QPair>

class QPixmap;
class StelButton;
class OnlineQueriesDialog;

/*! @defgroup onlineQueries Online Queries Plug-in
@{
The %Online Queries plugin provides online lookup to retrieve additional data from selected web services.

<b>Configuration</b>

The plug-ins' configuration data is stored in Stellarium's main configuration
file (section [OnlineQueries]).

@}
*/

//! @class OnlineQueries
//! Main class of the %Online Queries plugin.
//! @author Georg Zotti, Alexander Wolf
//! @ingroup OnlineQueries
class OnlineQueries : public StelModule
{
	Q_OBJECT
	Q_PROPERTY(bool enabled  READ isEnabled  WRITE setEnabled  NOTIFY flagEnabledChanged )

public:
	OnlineQueries();
	virtual ~OnlineQueries();

	virtual void init();
	virtual void deinit();
	virtual void update(double) {;}
	virtual void draw(StelCore *core){Q_UNUSED(core)}
	virtual bool configureGui(bool show);

signals:
	void flagEnabledChanged(bool b);

public slots:
	//! Enable plugin usage (show dialog)
	void setEnabled(bool b);
	//! Is plugin dialog shown?
	bool isEnabled() const { return enabled; }

	//QString getPtolemyUrl() const { return ptolemyUrl; }
	//QString getAncientSkiesUrl() const { return ancientSkiesUrl; }

	//! Save the settings to the main configuration file.
	void saveConfiguration(void);

	void queryStarnames();    //!< Connect from a button that triggers information query
	void queryAncientSkies(); //!< Connect from a button that triggers information query
	void queryWikipedia();    //!< Connect from a button that triggers information query

private slots:
	//! Set up the plugin with default values.  This means clearing out the OnlineQueries section in the
	//! main config.ini (if one already exists), and populating it with default values.
	void restoreDefaultConfiguration(void);

	//! Read (or re-read) settings from the main config file.  This will be called from init and also
	//! when restoring defaults (i.e. from the configuration dialog / restore defaults button).
	void loadConfiguration(void);

	void onHipQueryStatusChanged(); //!< To be connected

private:
	void createToolbarButton() const;
	void setOutputHtml(QString html); // Forward html to GUI dialog
	OnlineQueriesDialog* dialog;
	QSettings* conf;
	bool enabled;

	StelButton* toolbarButton;

	QString ptolemyUrl;                    // settable via config.ini
	QString ancientSkiesUrl;               // settable via config.ini

	// The query target websites:
	HipOnlineQuery *starnamesHipQuery;     // Patrick Gleason's site
	HipOnlineQuery *ancientSkiesHipQuery;  // ancient-skies.org
	HipOnlineReply *hipOnlineReply;        // Common reply object
};


#include <QObject>
#include "StelPluginInterface.hpp"

//! This class is used by Qt to manage a plug-in interface
class OnlineQueriesPluginInterface : public QObject, public StelPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID StelPluginInterface_iid)
	Q_INTERFACES(StelPluginInterface)
public:
	virtual StelModule* getStelModule() const;
	virtual StelPluginInfo getPluginInfo() const;
	virtual QObjectList getExtensionList() const { return QObjectList(); }
};

#endif /* ONLINEQUERIES_HPP */
