/*
 * tilesetmanager.h
 * Copyright 2008-2014, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2009, Edward Hutchins <eah1@yahoo.com>
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

#ifndef TILESETMANAGER_H
#define TILESETMANAGER_H

#include "tileset.h"

#include <QObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QSet>
#include <QTimer>

namespace Tiled {
namespace Internal {

class FileSystemWatcher;
class TileAnimationDriver;

/**
 * The tileset manager keeps track of all tilesets used by loaded maps. It also
 * watches the tileset images for changes and will attempt to reload them when
 * they change.
 */
class TilesetManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Requests the tileset manager. When the manager doesn't exist yet, it
     * will be created.
     */
    static TilesetManager *instance();

    /**
     * Deletes the tileset manager instance, when it exists.
     */
    static void deleteInstance();

    /**
     * Searches for a tileset matching the given file name.
     * @return a tileset matching the given file name, or 0 if none exists
     */
    SharedTileset findTileset(const QString &fileName) const;

    /**
     * Adds a tileset reference. This will make sure the tileset is watched for
     * changes and can be found using findTileset().
     */
    void addReference(const SharedTileset &tileset);

    /**
     * Removes a tileset reference. When the last reference has been removed,
     * the tileset is no longer watched for changes.
     */
    void removeReference(const SharedTileset &tileset);

    /**
     * Convenience method to add references to multiple tilesets.
     * @see addReference
     */
    void addReferences(const QVector<SharedTileset> &tilesets);

    /**
     * Convenience method to remove references from multiple tilesets.
     * @see removeReference
     */
    void removeReferences(const QVector<SharedTileset> &tilesets);

    /**
     * Returns all currently available tilesets.
     */
    QList<SharedTileset> tilesets() const;

    /**
     * Forces a tileset to reload.
     */
    void forceTilesetReload(SharedTileset &tileset);

    /**
     * Sets whether tilesets are automatically reloaded when their tileset
     * image changes.
     */
    void setReloadTilesetsOnChange(bool enabled);
    bool reloadTilesetsOnChange() const;

    void setAnimateTiles(bool enabled);
    bool animateTiles() const;
    void resetTileAnimations();

    void tilesetImageSourceChanged(const Tileset &tileset,
                                   const QString &oldImageSource);

signals:
    /**
     * Emitted when a tileset's images have changed and views need updating.
     */
    void tilesetChanged(Tileset *tileset);

    /**
     * Emitted when any images of the tiles in the given \a tileset have
     * changed. This is used to trigger repaints for displaying tile
     * animations.
     */
    void repaintTileset(Tileset *tileset);

private slots:
    void fileChanged(const QString &path);
    void fileChangedTimeout();

    void advanceTileAnimations(int ms);

private:
    Q_DISABLE_COPY(TilesetManager)

    /**
     * Constructor. Only used by the tileset manager itself.
     */
    TilesetManager();

    /**
     * Destructor.
     */
    ~TilesetManager();

    static TilesetManager *mInstance;

    /**
     * Stores the tilesets and maps them to the number of references.
     */
    QMap<SharedTileset, int> mTilesets;
    FileSystemWatcher *mWatcher;
    TileAnimationDriver *mAnimationDriver;
    QSet<QString> mChangedFiles;
    QTimer mChangedFilesTimer;
    bool mReloadTilesetsOnChange;
};

inline bool TilesetManager::reloadTilesetsOnChange() const
{ return mReloadTilesetsOnChange; }

} // namespace Internal
} // namespace Tiled

#endif // TILESETMANAGER_H
