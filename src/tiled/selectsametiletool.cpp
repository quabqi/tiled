/*
 * selectsametiletool.cpp
 * Copyright 2015, Mamed Ibrahimov <ibramlab@gmail.com>
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

#include "selectsametiletool.h"

#include "brushitem.h"
#include "mapdocument.h"
#include "changeselectedarea.h"

#include <QApplication>

using namespace Tiled;
using namespace Tiled::Internal;

SelectSameTileTool::SelectSameTileTool(QObject *parent)
    : AbstractTileTool(tr("Select Same Tile"),
                       QIcon(QLatin1String(
                               ":images/22x22/stock-tool-by-color-select.png")),
                       QKeySequence(tr("S")),
                       parent)
{
}

void SelectSameTileTool::tilePositionChanged(const QPoint &tilePos)
{
    // Make sure that a tile layer is selected and contains current tile pos.
    TileLayer *tileLayer = currentTileLayer();
    if (!tileLayer)
        return;

    QRegion resultRegion;
    if (tileLayer->contains(tilePos)) {
        const Cell &matchCell = tileLayer->cellAt(tilePos);
        resultRegion = tileLayer->region([&] (const Cell &cell) { return cell == matchCell; });
    }
    mSelectedRegion = resultRegion;
    brushItem()->setTileRegion(mSelectedRegion);
}

void SelectSameTileTool::mousePressed(QGraphicsSceneMouseEvent *event)
{
    const Qt::MouseButton button = event->button();
    const Qt::KeyboardModifiers modifiers = event->modifiers();

    if (button != Qt::LeftButton && button != Qt::RightButton)
        return;

    MapDocument *document = mapDocument();

    QRegion selection;

    // Left button modifies selection, right button clears selection
    if (button == Qt::LeftButton) {
        selection = document->selectedArea();

        if (modifiers == Qt::ShiftModifier)
            selection += mSelectedRegion;
        else if (modifiers == Qt::ControlModifier)
            selection -= mSelectedRegion;
        else if (modifiers == (Qt::ControlModifier | Qt::ShiftModifier))
            selection &= mSelectedRegion;
        else
            selection = mSelectedRegion;
    }

    if (selection != document->selectedArea()) {
        QUndoCommand *cmd = new ChangeSelectedArea(document, selection);
        document->undoStack()->push(cmd);
    }
}

void SelectSameTileTool::mouseReleased(QGraphicsSceneMouseEvent *)
{
}

void SelectSameTileTool::languageChanged()
{
    setName(tr("Select Same Tile"));
    setShortcut(QKeySequence(tr("S")));
}
