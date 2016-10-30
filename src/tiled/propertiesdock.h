/*
 * propertiesdock.h
 * Copyright 2013, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROPERTIESDOCK_H
#define PROPERTIESDOCK_H

#include <QDockWidget>
#include <QVariant>

class QtBrowserItem;

namespace Tiled {

class Object;
class Tileset;

namespace Internal {

class MapDocument;
class PropertyBrowser;

class PropertiesDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit PropertiesDock(QWidget *parent = nullptr);

    /**
     * Sets the \a mapDocument on which this properties dock will act.
     */
    void setMapDocument(MapDocument *mapDocument);

public slots:
    void bringToFront();

protected:
    bool event(QEvent *event) override;

private slots:
    void currentObjectChanged(Object *object);
    void currentItemChanged(QtBrowserItem *item);
    void tilesetFileNameChanged(Tileset *tileset);

    void addProperty();
    void addProperty(const QString &name, const QVariant &value);
    void removeProperty();
    void renameProperty();
    void renameProperty(const QString &name);
    void showContextMenu(const QPoint& pos);

private:
    void retranslateUi();

    MapDocument *mMapDocument;
    PropertyBrowser *mPropertyBrowser;
    QAction *mActionAddProperty;
    QAction *mActionRemoveProperty;
    QAction *mActionRenameProperty;
};

} // namespace Internal
} // namespace Tiled

#endif // PROPERTIESDOCK_H
