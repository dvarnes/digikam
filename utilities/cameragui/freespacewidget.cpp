/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2007-08-31
 * Description : a widget to display free space for a mount-point.
 * 
 * Copyright (C) 2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

// Qt includes.

#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QColor>
#include <QTimer>
#include <QFont> 
#include <QFontMetrics> 

// KDE includes.

#include <kurl.h>
#include <klocale.h>
#include <kdiskfreespace.h>
#include <kmountpoint.h>
#include <kio/global.h>

// Local includes.

#include "albumsettings.h"
#include "freespacewidget.h"
#include "freespacewidget.moc"

namespace Digikam
{

class FreeSpaceWidgetPriv
{
public:

    FreeSpaceWidgetPriv()
    {
        timer       = 0;
        isValid     = false;
        kBSize      = 0;
        kBUsed      = 0;
        kBAvail     = 0;
        dSizeKb     = 0;
        percentUsed = 0;
    }

    bool           isValid;
    
    int            percentUsed;

    unsigned long  dSizeKb;
    unsigned long  kBSize;
    unsigned long  kBUsed;
    unsigned long  kBAvail;
    
    QString        mountPoint;

    QTimer        *timer;
    
    QPixmap        pix;
};

FreeSpaceWidget::FreeSpaceWidget(QWidget* parent, int width)
               : QWidget(parent)
{
    d = new FreeSpaceWidgetPriv;
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedWidth(width);
    setMaximumHeight(fontMetrics().height()+4);
    slotTimeout();
    
    d->timer = new QTimer(this);
    
    connect(d->timer, SIGNAL(timeout()),
            this, SLOT(slotTimeout()));

    d->timer->start(10000);
}

FreeSpaceWidget::~FreeSpaceWidget()
{
    d->timer->stop();
    delete d->timer;
    delete d;
}

void FreeSpaceWidget::setEstimatedDSizeKb(unsigned long dSize)
{
    d->dSizeKb = dSize;
    updatePixmap();
    repaint();
}

unsigned long FreeSpaceWidget::estimatedDSizeKb()
{
    return d->dSizeKb;
}

bool FreeSpaceWidget::isValid()
{
    return d->isValid;
}

int FreeSpaceWidget::percentUsed() 
{
    return d->percentUsed;
}

unsigned long FreeSpaceWidget::kBSize()
{
    return d->kBSize;
}

unsigned long FreeSpaceWidget::kBUsed()
{
    return d->kBUsed;
}

unsigned long FreeSpaceWidget::kBAvail()
{
    return d->kBAvail;
}

QString FreeSpaceWidget::mountPoint()
{
    return d->mountPoint;
}

void FreeSpaceWidget::updatePixmap()
{
    d->pix = QPixmap(size());
    d->pix.fill(palette().background().color());
    
    QPainter p(&d->pix);
    p.setPen(palette().foreground().color());
    p.drawRect(0, 0, d->pix.width(), d->pix.height());

    if (isValid())
    {
        // We will compute the estimated % of space size used to download and process.
        unsigned long eUsedKb = d->dSizeKb + d->kBUsed;
        int peUsed            = (int)(100.0*((double)eUsedKb/(double)d->kBSize));
        int pClamp            = peUsed > 100 ? 100 : peUsed;
        p.setBrush(peUsed > 95 ? Qt::red : Qt::darkGreen);
        p.setPen(Qt::white);
        QRect gRect(1, 1, (int)(((double)d->pix.width()-2.0)*(pClamp/100.0)), d->pix.height()-2);
        p.drawRect(gRect);

        QRect tRect(1, 1, d->pix.width()-2, d->pix.height()-2);
        QString text = QString("%1%").arg(peUsed);
        p.setPen(palette().text().color());
        QFontMetrics fontMt = p.fontMetrics();
        QRect fontRect      = fontMt.boundingRect(tRect.x(), tRect.y(), tRect.width(), tRect.height(), 0, text);
        p.drawText(tRect, Qt::AlignCenter, text);

        QString info = i18n("<p>Capacity: <b>%1</b>"
                            "<p>Available: <b>%2</b>"
                            "<p>Require: <b>%3</b>")
                            .arg(KIO::convertSizeFromKiB(d->kBSize))
                            .arg(KIO::convertSizeFromKiB(d->kBAvail))
                            .arg(KIO::convertSizeFromKiB(d->dSizeKb));
        setWhatsThis(info);
        setToolTip(info);
    }
    
    p.end();
}    

void FreeSpaceWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawPixmap(0, 0, d->pix);
    p.end();
}

void FreeSpaceWidget::slotTimeout()
{
    QString path        = AlbumSettings::componentData()->getAlbumLibraryPath();
    KMountPoint::Ptr mp = KMountPoint::currentMountPoints().findByPath(path);
    if (mp)
    {
        KDiskFreeSpace *job = new KDiskFreeSpace;
        connect(job, SIGNAL(foundMountPoint(QString, quint64, quint64, quint64)),
                this, SLOT(slotAvailableFreeSpace(QString, quint64, quint64, quint64)));
        job->readDF(mp->mountPoint());
    }
}

void FreeSpaceWidget::slotAvailableFreeSpace(QString mountPoint, quint64 kBSize, 
                                             quint64 kBUsed, quint64 kBAvail)
{
    d->mountPoint  = mountPoint;
    d->kBSize      = kBSize;
    d->kBUsed      = kBUsed;
    d->kBAvail     = kBAvail;
    d->percentUsed = 100 - (int)(100.0*kBAvail/kBSize);
    d->isValid     = true;
    updatePixmap();
    repaint();
}

}  // namespace Digikam
