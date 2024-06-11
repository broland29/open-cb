#pragma once

#include <QVector>
#include <QString>
#include <spdlog/spdlog.h>
#include <../ip/headers/Parameters.h>

// "Standard behavior: Windows file system treats file and directory names as case-insensitive"
//  so, unfortunately, cannot use P and p, must use WP and BP
// these encodings will be used for folders, for passing around, for everything, to avoid endless mapping.
// can get std::string easily. if classifier uses other labels, it has to encode/decode for itself
static QVector<QString> ENCODINGS =
{
    "WF", "WP", "WB", "WN", "WR", "WQ", "WK",
    "BF", "BP", "BB", "BN", "BR", "BQ", "BK",
};
