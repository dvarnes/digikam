/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2010-05-03
 * Description : a batch tool to addd film grain to images.
 *
 * Copyright (C) 2010 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "filmgrain.moc"

// Qt includes

#include <QLabel>
#include <QGridLayout>
#include <QWidget>

// KDE includes

#include <kiconloader.h>
#include <klocale.h>
#include <kdialog.h>

// Local includes

#include "dimg.h"
#include "filmgrainfilter.h"
#include "filmgrainsettings.h"

namespace Digikam
{

FilmGrain::FilmGrain(QObject* parent)
         : BatchTool("FilmGrain", BaseTool, parent)
{
    setToolTitle(i18n("Film Grain"));
    setToolDescription(i18n("A tool to add film grain"));
    setToolIcon(KIcon(SmallIcon("filmgrain")));

    QWidget* box   = new QWidget;
    m_settingsView = new FilmGrainSettings(box);
    setSettingsWidget(box);

    connect(m_settingsView, SIGNAL(signalSettingsChanged()),
            this, SLOT(slotSettingsChanged()));
}

FilmGrain::~FilmGrain()
{
}

BatchToolSettings FilmGrain::defaultSettings()
{
    BatchToolSettings prm;
    FilmGrainContainer defaultPrm = m_settingsView->defaultSettings();

    prm.insert("lum_sensibility",    (int)defaultPrm.lum_sensibility);
    prm.insert("lum_shadows",        (int)defaultPrm.lum_shadows);
    prm.insert("lum_midtones",       (int)defaultPrm.lum_midtones);
    prm.insert("lum_highlights",     (int)defaultPrm.lum_highlights);
    prm.insert("chroma_sensibility", (int)defaultPrm.chroma_sensibility);
    prm.insert("chroma_shadows",     (int)defaultPrm.chroma_shadows);
    prm.insert("chroma_midtones",    (int)defaultPrm.chroma_midtones);
    prm.insert("chroma_highlights",  (int)defaultPrm.chroma_highlights);

    return prm;
}

void FilmGrain::slotAssignSettings2Widget()
{
    FilmGrainContainer prm;
    prm.lum_sensibility    = settings()["lum_sensibility"].toInt();
    prm.lum_shadows        = settings()["lum_shadows"].toInt();
    prm.lum_midtones       = settings()["lum_midtones"].toInt();
    prm.lum_highlights     = settings()["lum_highlights"].toInt();
    prm.chroma_sensibility = settings()["chroma_sensibility"].toInt();
    prm.chroma_shadows     = settings()["chroma_shadows"].toInt();
    prm.chroma_midtones    = settings()["chroma_midtones"].toInt();
    prm.chroma_highlights  = settings()["chroma_highlights"].toInt();
    m_settingsView->setSettings(prm);
}

void FilmGrain::slotSettingsChanged()
{
    BatchToolSettings prm;
    FilmGrainContainer currentPrm = m_settingsView->settings();

    prm.insert("lum_sensibility",    (int)currentPrm.lum_sensibility);
    prm.insert("lum_shadows",        (int)currentPrm.lum_shadows);
    prm.insert("lum_midtones",       (int)currentPrm.lum_midtones);
    prm.insert("lum_highlights",     (int)currentPrm.lum_highlights);
    prm.insert("chroma_sensibility", (int)currentPrm.chroma_sensibility);
    prm.insert("chroma_shadows",     (int)currentPrm.chroma_shadows);
    prm.insert("chroma_midtones",    (int)currentPrm.chroma_midtones);
    prm.insert("chroma_highlights",  (int)currentPrm.chroma_highlights);

    setSettings(prm);
}

bool FilmGrain::toolOperations()
{
    if (!loadToDImg())
        return false;

    FilmGrainContainer prm;
    prm.lum_sensibility    = settings()["lum_sensibility"].toInt();
    prm.lum_shadows        = settings()["lum_shadows"].toInt();
    prm.lum_midtones       = settings()["lum_midtones"].toInt();
    prm.lum_highlights     = settings()["lum_highlights"].toInt();
    prm.chroma_sensibility = settings()["chroma_sensibility"].toInt();
    prm.chroma_shadows     = settings()["chroma_shadows"].toInt();
    prm.chroma_midtones    = settings()["chroma_midtones"].toInt();
    prm.chroma_highlights  = settings()["chroma_highlights"].toInt();

    FilmGrainFilter fg(&image(), 0L, prm);
    fg.startFilterDirectly();
    image().putImageData(fg.getTargetImage().bits());

    return savefromDImg();
}

} // namespace Digikam
