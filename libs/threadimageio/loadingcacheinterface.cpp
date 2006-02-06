/* ============================================================
 * Author: Marcel Wiesweg <marcel.wiesweg@gmx.de>
 * Date  : 2006-02-06
 * Description : shared image loading and caching
 *
 * Copyright 2005 by Marcel Wiesweg
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


#include "loadingcacheinterface.h"
#include "loadingcache.h"

namespace Digikam
{

void LoadingCacheInterface::cleanUp()
{
    LoadingCache::cleanUp();
}

void LoadingCacheInterface::cleanFromCache(const QString &filePath)
{
    LoadingCache *cache = LoadingCache::cache();
    LoadingCache::CacheLock lock(cache);
    cache->removeImage(filePath);
}

void LoadingCacheInterface::cleanCache()
{
    LoadingCache *cache = LoadingCache::cache();
    LoadingCache::CacheLock lock(cache);
    cache->removeImages();
}

void LoadingCacheInterface::setCacheOptions(int cacheSize)
{
    LoadingCache *cache = LoadingCache::cache();
    LoadingCache::CacheLock lock(cache);
    cache->setCacheSize(cacheSize);
}

}   // namespace Digikam
