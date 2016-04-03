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
/*
 * =====================================================================================
 *        Filename:  selectpagesdialog.cc
 *         Created:  03/12/2006 07:41:44 PM CET
 *          Author:  jmisutka ()
 * =====================================================================================
 */

#include "selectpagesdialog.h"
#include "pdfutil.h"
#include "util.h"
#include <QVariant>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include QLISTBOX
#include <QListWidget>
#include <QListWidgetItem>
#include <QLayout>
#include <kernel/cpdf.h>

//=====================================================================================
namespace gui {
//=====================================================================================

using namespace std;
using namespace pdfobjects;


//=====================================================================================
namespace {
//=====================================================================================

	/**
	 * List box item.
	 * Inherits from QListBoxText item type and adds NodeData data field.
	 */
	struct ListItem : public Q_ListBoxText {
			typedef size_t Position;

		protected:
			Position _position;

			//
			// Ctor
			//
		public:
			ListItem (Position pos, Q_ListBox* _parent, const QString& text = QString::null)
                : Q_ListBoxText(_parent),_position(pos) 	{}
			ListItem (Position pos, const QString&  text = QString::null)
                : Q_ListBoxText(),_position(pos) {}
			ListItem (Position pos, Q_ListBox* _parent, const QString& text, Q_ListBoxItem* after)
                : Q_ListBoxText(_parent),_position(pos) {}

			/**
			 * Returns position.
			 */
			Position position () const
				{ return _position; }
	};

//=====================================================================================
} // namespace
//=====================================================================================


//=====================================================================================
// SelectPagesDialog
//=====================================================================================

//
// Constructor
//
SelectPagesDialog::SelectPagesDialog (const QString& filename) : QDialog (0, 0) {
//	setName ("Select page dialog.");
    setModal(false);
    SelectPagesDialogLayout = new QGridLayout( this/*, 1, 1, 11, 6, "SelectPagesDialogLayout"*/);
    SelectPagesDialogLayout->setMargin(11);
    SelectPagesDialogLayout->setSpacing(6);
    //QGridLayout ( QWidget * parent, int nRows = 1, int nCols = 1
     //       , int margin = 11, int space = 6, const char * name = 0 ) qt3

    layout48 = new QVBoxLayout();
    layout48->setMargin(0);
    layout48->setSpacing(6);
    layout27 = new QHBoxLayout();
    layout27->setMargin(0);
    layout27->setSpacing(6);

    textLabel1 = new QLabel( this);
	textLabel1->setText (filename);
    textLabel1->setSizePolicy( QSizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0)) );
    layout27->addWidget( textLabel1 );
    layout48->addLayout( layout27 );
    layout47 = new QHBoxLayout;
    layout47->setMargin(0);
    layout47->setSpacing(6);
	// selectedList
    selectedList = new Q_ListBox( this);
    selectedList->setEnabled( false );
    layout47->addWidget( selectedList );
    layout4 = new QVBoxLayout;
    layout4->setMargin(0);
    layout4->setSpacing(6);
	// Add buttno
    addBtn = new QPushButton( this);
    addBtn->setEnabled( false );
    layout4->addWidget( addBtn );
	// Remove button
    removeBtn = new QPushButton( this);
    removeBtn->setEnabled( false );
    layout4->addWidget( removeBtn );
    spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout4->addItem( spacer3 );
    layout47->addLayout( layout4 );
	// Filelist
    originalList = new Q_ListBox( this);
    originalList->setEnabled( false );
    originalList->setFrameShape( Q_ListBox::StyledPanel );
    originalList->setSelectionMode( Q_ListBox::ExtendedSelection );
    layout47->addWidget( originalList );
    layout48->addLayout( layout47 );
    layout6 = new QHBoxLayout;
    layout6->setMargin(0);
    layout6->setSpacing(5);
    spacer5 = new QSpacerItem( 61, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer5 );
	// Ok button
    okBtn = new QPushButton( this);
    okBtn->setEnabled( false );
    layout6->addWidget( okBtn );
    spacer6 = new QSpacerItem( 41, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout6->addItem( spacer6 );
	// Cancel button
    cancelBtn = new QPushButton( this);
    layout6->addWidget( cancelBtn );
    layout48->addLayout( layout6 );
    SelectPagesDialogLayout->addLayout( layout48, 0, 0 );

    languageChange();
    resize( QSize(759, 380).expandedTo(minimumSizeHint()) );
//    clearWState( WState_Polished );

    // signals and slots connections
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( okBtn, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( addBtn, SIGNAL( clicked() ), this, SLOT( addBtn_clicked() ) );
    connect( removeBtn, SIGNAL( clicked() ), this, SLOT( removeBtn_clicked() ) );
#ifdef QT3
    connect( selectedList, SIGNAL( currentChanged(QListBoxItem*) ), this, SLOT( selectedList_changed(QListBoxItem*) ) );
    connect( originalList, SIGNAL( currentChanged(QListBoxItem*) ), this, SLOT( originalList_changed(QListBoxItem*) ) );
#else
    connect( selectedList, SIGNAL( currentItemChanged(QListBoxItem*, QListBoxItem*) ), this, SLOT( selectedList_changed(QListBoxItem*) ) );
    connect( originalList, SIGNAL( currentItemChanged(QListBoxItem*, QListBoxItem*) ), this, SLOT( originalList_changed(QListBoxItem*) ) );
#endif

    // tab order
    setTabOrder( addBtn, removeBtn );
    setTabOrder( removeBtn, okBtn );
    setTabOrder( okBtn, cancelBtn );

	//
	// Init pages
	//
	init (filename);
    selectedList->setEnabled (true);
    originalList->setEnabled (true);
    okBtn->setEnabled (true);
}


//
// Event handlers
//

//
//
//
void SelectPagesDialog::originalList_changed (Q_ListBoxItem* item) {
    ListItem * listItem=dynamic_cast<ListItem *>(item);
	if(listItem==NULL)
			return;
			
    addBtn->setEnabled(true);
}


//
//
//
void SelectPagesDialog::selectedList_changed (Q_ListBoxItem*) {
	// allways enable add button when something is selected
    removeBtn->setEnabled(true);
}

//
// !source code copied from MergeDialog!
//
void SelectPagesDialog::addBtn_clicked () {
	int oldPos=-1;
    ListItem * oldItem=NULL;

	for(int i=originalList->count()-1; i>=0; --i)
	{
			// skips unselected items
            //if(!originalList->isSelected()) Fixed on QT3
            //		continue;
			
            ListItem * fileItem=dynamic_cast<ListItem *>(originalList->item(i));
			if(!fileItem)
					continue;

			// removes fileItem from originalList
            oldPos=originalList->row(fileItem);
			oldItem=fileItem;
            originalList->removeItemWidget(fileItem);
			if(oldPos>=(int)(originalList->count()))
					// correction for last item in list
					--oldPos;

			// gets position of selected item in selectedList
            int pos=selectedList->currentRow();

			// insert fileItem before currently selected node in selectedList
            selectedList->insertItem((pos<0)?0:pos,fileItem);
            //selectedList->setSelected(false); Fixed Qt3
	}

	// sets new currentItem in selectedList to oldItem
	if(oldItem)
	{
			selectedList->setCurrentItem(oldItem);
            //selectedList->setSelected(true); Fixed qt3
	}

	// nothing has left in originalList box
	if(!originalList->count())
	{
            addBtn->setEnabled(false);
			return;
	}
	
	// select and mark as current element behind last added
	// to selectedList
	if(oldPos>=0)
	{
            originalList->setCurrentRow(oldPos);
            //originalList->setSelected(true);  Fixed QT3
			return;
	}
}


//
// !source code copied from MergeDialog!
//
void SelectPagesDialog::removeBtn_clicked () {
//	if(selectedList->selectedItem()) Fixed Qt3
//	{
            ListItem * mergeItem=dynamic_cast<ListItem *>(selectedList->currentItem());
			if(!mergeItem)
			{
					// nothing selected
                    removeBtn->setEnabled(false);
					return;
			}
			// removes item from selectedList
            int oldPos=selectedList->row(mergeItem);
            selectedList->removeItemWidget(mergeItem);

			// insert to correct position in originalList - keeps ordering
			int pos=0;
			while(Q_ListBoxItem * item=originalList->item(pos))
			{
                    ListItem * i=dynamic_cast<ListItem *>(item);
					if(!i)
							// bad type, this should not happen
							continue;
					// finish with first node with higer position
					// mergeItem will be stored before this position
					if(i->position() > mergeItem->position())
							break;
					++pos;
			}

			// inserts mergeItem to correct position and unselect it
            originalList->insertItem(pos, mergeItem);
			originalList->setCurrentItem(mergeItem);
            selectedList->setCurrentRow(oldPos);
            //selectedList->setSelected(true);   Fixed QT3
//	}else
//            removeBtn->setEnabled(false); Fixed QT3
}


//
// Helper functions
//

template<typename Container> void SelectPagesDialog::getResult (Container& cont) const {
    for (int pos = 0; pos < selectedList->count(); ++pos)
	{
        ListItem* item = dynamic_cast<ListItem*>(selectedList->item(pos));
		if(!item)
			continue;
		cont.push_back (item->position());
	}
}
// Explicit instantiation
template void SelectPagesDialog::getResult<list<size_t> > (list<size_t>& cont) const;

//
//
//
void SelectPagesDialog::init (size_t count) {
 QString itemLabel;
 for(size_t i=1; i<=count; ++i) {
  snprintf(itemLabel.toLatin1().data(), 127, "Page%d", (int)i);
  new ListItem (i, originalList, itemLabel);
 }

}

//
//
//
bool SelectPagesDialog::init (const QString& fileName) {
	boost::shared_ptr<CPdf> document;
	CPdf::OpenMode mode = CPdf::ReadOnly;

	try {

		guiPrintDbg (debug::DBG_DBG,"Opening document.");
		document = util::getPdfInstance (this,fileName,mode);
		assert(document);
		guiPrintDbg (debug::DBG_DBG,"Document opened.");
	
	} catch (PdfOpenException& e) {
		std::string err;
		e.getMessage(err);
		//TODO: some messagebox?
		guiPrintDbg(debug::DBG_DBG,"Failed opening document " << err);
        return false;
	}

	// Init pages
	init (document->getPageCount());
	document.reset();
	
    return true;
}


//
//  Sets the strings of the subwidgets using the current language.
//
void SelectPagesDialog::languageChange() {
 setWindowTitle(tr("Convert pdf to xml"));
 textLabel1->setText("tr(textLabel1->text()) Fixed SelectPagesDialog.cc:383");
 addBtn->setText("<<");
 removeBtn->setText(">>");
 cancelBtn->setText(QObject::tr("&Cancel"));
 cancelBtn->setShortcut(QKeySequence("Alt+C"));
 okBtn->setText(QObject::tr("&Ok"));
 okBtn->setShortcut(QKeySequence("Alt+O"));
}

//=====================================================================================
} //namespace gui
//=====================================================================================
