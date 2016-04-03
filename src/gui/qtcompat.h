/*
 * PDFedit - free program for PDF document manipulation.
 * Copyright (C) 2006-2009  PDFedit team: Michal Hocko,
 *                                        Jozef Misutka,
 *                                        Martin Petricek
 *                   Former team members: Miroslav Jahoda
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in doc/LICENSE.GPL); if not, write to the 
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 * MA  02111-1307  USA
 *
 * Project is hosted on http://sourceforge.net/projects/pdfedit
 */
#ifndef __QTCOMPAT_H__
#define __QTCOMPAT_H__

/**
 @file
 Compatibility fixes to allow Qt3 code to work in Qt4
*/

//#include <QtCore/qglobal.h>
#include <QListWidget>    // qt5
#include <QListWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeView>

//#if defined(QT_VERSION) && QT_VERSION >= 0x050000

 // QT5 or newer
//#define QT5 1

//#include <Qt>

///** qt3/qt4 compatibility typedef */
//typedef Qt::WindowFlags WFlags;

/** Macro working in QT3 and QT4, allowing to send QString to debugging output */
#define Q_OUT(x) (x.toUtf8().data())

//Type aliases
#define Q_ButtonGroup		QButtonGroup
#define Q_ComboBox		QComboBox
#define Q_Dict			QHash
#define Q_DictIterator		 QHashIterator
#define Q_GroupBox		QGroupBox
#define Q_List			QList
#define Q_ListBox		QListWidget
#define Q_ListBoxItem		QListWidgetItem
#define Q_ListBoxText		QListWidgetItem
#define Q_ListView		    QTreeWidget
#define Q_ListViewItem		QTreeWidgetItem
#define Q_ListViewItemIterator	QTreeWidgetItemIterator
#define Q_Painter		QPainter
#define Q_PopupMenu		QMenu
#define Q_PtrCollection		Q3PtrCollection
#define Q_PtrDict		QHash
#define Q_PtrDictIterator	QMultiHash
#define Q_PtrList		QList
#define Q_PtrListIterator	Q3PtrListIterator
#define Q_ScrollView		QScrollArea
#define Q_TextBrowser		QTextBrowser
#define Q_TextEdit		QTextEdit
#define QStrList		QList<QByteArray>
#define QMenuData		QMenu
#define QCString		QByteArray
#define QMemArray		QVector
#define IbeamCursor		IBeamCursor
#define TheWheelFocus		Qt::WheelFocus

//Include aliases
#define QBUTTONGROUP	<QButtonGroup>
#define QBYTEARRAY	<QByteArray>
#define QCOMBOBOX	<QComboBox>
#define QDICT		<QHash>
#define QGROUPBOX	<QGroupBox>
#define QICON		<QtGui/QIcon>
#define QLISTBOX	<QListView>
#define QLISTVIEW	<QListView>
#define QPOPUPMENU	<QMenu>
#define QPTRDICT	<QHash>
#define QPTRLIST	<QList>
#define QPTRCOLLECTION	<Q3PtrCollection>
#define QLIST		<QList>
#define QSCROLLVIEW	<QtWidgets/QScrollArea>
#define QSTRLIST	<QtCore/QList>
#define QTEXTBROWSER	<QTextBrowser>
#define QTEXTEDIT	<QTextEdit>

#endif
