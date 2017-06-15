/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#include "settings.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

Settings::Settings(QObject *parent) : QObject(parent)
{
    settings = new QSettings(QDir(configPath()).filePath("config.conf"), QSettings::IniFormat);
    
    groupName = "settings";
    
}

Settings::~Settings()
{
    delete settings;
}

QString Settings::configPath()
{
    return QDir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first()).filePath(qApp->organizationName())).filePath(qApp->applicationName());
}

QVariant Settings::getOption(const QString &key)
{
    settings->beginGroup(groupName);
    QVariant result;
    if (settings->contains(key)) {
        result = settings->value(key);
    } else {
        result = QVariant();
    }
    settings->endGroup();

    return result;
}

void Settings::init()
{
    if (getOption("process_tab_index").isNull()) {
        setOption("process_tab_index", 0);
    }
    
    if (getOption("process_columns").isNull()) {
        setOption("process_columns", "cpu,memory,disk_write,disk_read,download,upload,pid");
    }
    
    if (getOption("theme_style").isNull()) {
        setOption("theme_style", "light");
    }
}

void Settings::setOption(const QString &key, const QVariant &value) {
    settings->beginGroup(groupName);
    settings->setValue(key, value);
    settings->endGroup();

    settings->sync();
}
