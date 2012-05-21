#include "StdAfx.h"
#include "swiftitem.h"

/// \class SwiftItem
/// \brief 
///
/// 
///
SwiftItem::SwiftItem(const QString &path) : m_path(path) {}
QString SwiftItem::path() const { return m_path; }