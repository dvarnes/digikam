/* ============================================================
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2005-06-15
 * Copyright 2005 by Renchi Raju
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
 * ============================================================ */

#ifndef DIMGPRIVATE_H
#define DIMGPRIVATE_H

// QT includes.

#include <qshared.h>
#include <qstring.h>
#include <qcstring.h>
#include <qvariant.h>
#include <qmap.h>

// Local includes.

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT DImgPrivate : public QShared
{
public:

    DImgPrivate()
    {
        null       = true;
        width      = 0;
        height     = 0;
        data       = 0;
        alpha      = false;
        sixteenBit = false;
    }

    ~DImgPrivate()
    {
        delete [] data;
    }

    bool                    null;
    bool                    alpha;
    bool                    sixteenBit;    
    bool                    isReadOnly;    
    
    unsigned int            width;
    unsigned int            height;
    
    unsigned char*          data;
    
    QByteArray              ICCProfil;
    
    QMap<int, QByteArray>   metaData;
    QMap<QString, QVariant> attributes;
    QMap<QString, QString>  embeddedText;

    /** Save informations about camera witch taking the picture. */
    QString                 cameraModel;
    QString                 cameraConstructor;

};

}  // NameSpace Digikam

#endif /* DIMGPRIVATE_H */
