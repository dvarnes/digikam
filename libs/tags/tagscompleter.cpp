/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2010-06-12
 * Description : Completion Box for tags
 *
 * Copyright (C) 2010       by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
 * Copyright (C) 1997       by Sven Radej (sven.radej@iname.com)
 * Copyright (c) 1999       by Patrick Ward <PAT_WARD@HP-USA-om5.om.hp.com>
 * Copyright (c) 1999       by Preston Brown <pbrown@kde.org>
 * Copyright (c) 2000, 2001 by Dawit Alemayehu <adawit@kde.org>
 * Copyright (c) 2000, 2001 by Carsten Pfeiffer <pfeiffer@kde.org>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "tagscompleter.h"

// Qt includes

#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStandardItem>

// KDE includes

#include <klocalizedstring.h>

// Local includes

#include "digikam_debug.h"
#include "album.h"
#include "albumfiltermodel.h"
#include "albummanager.h"
#include "albummodel.h"
#include "albumthumbnailloader.h"
#include "albumtreeview.h"
#include "taggingactionfactory.h"
#include "tagscache.h"

namespace Digikam
{

enum
{
    TaggingActionRole = Qt::UserRole + 1,
    CompletionRole    = Qt::UserRole + 2
};

class TagCompleter::Private : public TaggingActionFactory::ConstraintInterface
{
public:

    Private()
        : model(0),
          supportingModel(0),
          filterModel(0),
          expectingTextChange()
    {
    }
    ~Private() {}

    QStandardItemModel*  model;
    TaggingActionFactory factory;

    TagModel*            supportingModel;
    AlbumFilterModel*    filterModel;

    bool                 expectingTextChange;

    QModelIndex indexForAlbum(int id)
    {
        if (!supportingModel)
        {
            return QModelIndex();
        }
        TAlbum* talbum = AlbumManager::instance()->findTAlbum(id);
        return supportingModel->indexForAlbum(talbum);
    }

    virtual bool matches(int id)
    {
        TAlbum* talbum = AlbumManager::instance()->findTAlbum(id);
        return filterModel->indexForAlbum(talbum).isValid();
    }
};

TagCompleter::TagCompleter(QObject* parent)
    : QCompleter(parent),
      d(new Private)
{
    d->model = new QStandardItemModel(this);
    setModel(d->model);
    setCompletionRole(CompletionRole);
    setCompletionMode(UnfilteredPopupCompletion);
    setCompletionColumn(0);

    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(slotActivated(QModelIndex)));
    connect(this, SIGNAL(highlighted(QModelIndex)), this, SLOT(slotHighlighted(QModelIndex)));
}

TagCompleter::~TagCompleter()
{
    delete d;
}

// just to make the method private
void TagCompleter::setModel(QAbstractItemModel *model)
{
    QCompleter::setModel(model);
}

void TagCompleter::setTagFilterModel(AlbumFilterModel* filterModel)
{
    d->filterModel = filterModel;
    d->factory.setConstraintInterface(d->filterModel ? d : 0);
}

void TagCompleter::setSupportingTagModel(TagModel* model)
{
    d->supportingModel = model;
}

void TagCompleter::setContextParentTag(int parentTagId)
{
    d->factory.setParentTag(parentTagId);
}

void TagCompleter::update(const QString& fragment)
{
    if (fragment == d->factory.fragment())
    {
        return;
    }

    d->factory.setFragment(fragment);
    d->model->clear();

    QList<TaggingAction> actions = d->factory.actions();

    QList<QStandardItem*> items;
    foreach (const TaggingAction& action, actions)
    {
        QStandardItem* item = new QStandardItem;
        // Text, implemented by TaggingActionFactory
        item->setText(d->factory.suggestedUIString(action));
        // Action, via user data
        item->setData(QVariant::fromValue(action), TaggingActionRole);
        // Icon and completion role
        if (action.shallCreateNewTag())
        {
            item->setData(fragment, CompletionRole);
            item->setIcon(AlbumThumbnailLoader::instance()->getNewTagIcon());
        }
        else
        {
            item->setData(TagsCache::instance()->tagName(action.tagId()), CompletionRole);
            QModelIndex index = d->indexForAlbum(action.tagId());
            if (index.isValid())
            {
                item->setData(index.data(Qt::DecorationRole), Qt::DecorationRole);
            }
            else
            {
                item->setIcon(AlbumThumbnailLoader::instance()->getStandardTagIcon());
            }
        }
        items << item;
    }

    d->model->appendColumn(items);
}

void TagCompleter::setWidget(QLineEdit *le)
{
    if (widget())
    {
        disconnect(widget(), SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
        disconnect(widget(), SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
    }

    le->setCompleter(this);

    connect(le, &QLineEdit::textChanged, this, &TagCompleter::textChanged);
    connect(le, &QLineEdit::textEdited, this, &TagCompleter::textEdited);
}

void TagCompleter::slotActivated(const QModelIndex &index)
{
    emit activated(index.data(TaggingActionRole).value<TaggingAction>());
}

void TagCompleter::slotHighlighted(const QModelIndex &index)
{
    qDebug() << "highlighted" << index;
    emit highlighted(index.data(TaggingActionRole).value<TaggingAction>());
}

void TagCompleter::textChanged(const QString &)
{
    // do not update here (?)
}

void TagCompleter::textEdited(const QString &text)
{
    update(text);
}

} // namespace Digikam