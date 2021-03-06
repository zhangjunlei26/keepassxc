/*
 *  Copyright (C) 2019 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ReportsPageStatistics.h"

#include "ReportsWidgetStatistics.h"
#include "core/FilePath.h"

#include <QApplication>

QString ReportsPageStatistics::name()
{
    return QApplication::tr("Statistics");
}

QIcon ReportsPageStatistics::icon()
{
    return FilePath::instance()->icon("actions", "statistics");
}

QWidget* ReportsPageStatistics::createWidget()
{
    return new ReportsWidgetStatistics();
}

void ReportsPageStatistics::loadSettings(QWidget* widget, QSharedPointer<Database> db)
{
    ReportsWidgetStatistics* settingsWidget = reinterpret_cast<ReportsWidgetStatistics*>(widget);
    settingsWidget->loadSettings(db);
}

void ReportsPageStatistics::saveSettings(QWidget* widget)
{
    ReportsWidgetStatistics* settingsWidget = reinterpret_cast<ReportsWidgetStatistics*>(widget);
    settingsWidget->saveSettings();
}
