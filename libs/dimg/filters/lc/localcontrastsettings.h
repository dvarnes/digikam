/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2010-03-09
 * Description : Local Contrast settings view.
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

#ifndef LOCALCONTRASTSETTINGS_H
#define LOCALCONTRASTSETTINGS_H

// Local includes

#include <QWidget>

// KDE includes

#include <kconfig.h>

// Local includes

#include "digikam_export.h"
#include "tonemappingparameters.h"

namespace Digikam
{

class LocalContrastSettingsPriv;

class DIGIKAM_EXPORT LocalContrastSettings : public QWidget
{
    Q_OBJECT

public:

    LocalContrastSettings(QWidget* parent);
    ~LocalContrastSettings();

    ToneMappingParameters defaultSettings() const;
    void resetToDefault();

    ToneMappingParameters settings() const;
    void setSettings(const ToneMappingParameters& settings);

    void readSettings(KConfigGroup& group);
    void writeSettings(KConfigGroup& group);

    void loadSettings();
    void saveAsSettings();

Q_SIGNALS:

    void signalSettingsChanged();

private Q_SLOTS:

    void slotStage1Enabled(bool);
    void slotStage2Enabled(bool);
    void slotStage3Enabled(bool);
    void slotStage4Enabled(bool);

private:

    LocalContrastSettingsPriv* const d;
};

}  // namespace Digikam

#endif /* LOCALCONTRASTSETTINGS_H */
