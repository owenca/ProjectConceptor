#include <app/Clipboard.h>
#include <support/Debug.h>

#include "ProjectConceptorDefs.h"
#include "Copy.h"
#include "Indexer.h"


Copy::Copy():PCommand()
{
}

void Copy::Undo(PDocument *doc,BMessage *undo)
{
	// nothing to do :-)
}

BMessage* Copy::Do(PDocument *doc, BMessage *settings)
{
	BMessage 	*clip				= NULL;
	BMessage	*node				= NULL;
	BMessage	*from				= NULL;
	BMessage	*to					= NULL;
	bool		fselect				= false;
	bool		tselect				= false;
	BMessage	*copyMessage		= new BMessage();
	BList		*selected			= doc->GetSelected();
	BList		*allConnections		= doc->GetAllConnections();

	int32		i					= 0;
	Indexer		*indexer			= new Indexer(doc);
	if (doc->Lock()) {
		for (i=0;i<selected->CountItems();i++) {
			if (node=(BMessage *)selected->ItemAt(i)) {
				if (node->what != P_C_CONNECTION_TYPE)
					copyMessage->AddMessage("node",indexer->IndexNode(node));
			}
		}
		for (i=0;i<allConnections->CountItems();i++ ) {
			node = (BMessage *)allConnections->ItemAt(i);
			if ( (node->FindPointer(P_C_NODE_CONNECTION_FROM,(void **)&from) == B_OK) &&
				 (node->FindPointer(P_C_NODE_CONNECTION_TO,(void **)&to) == B_OK) ) {
				if ((from->FindBool(P_C_NODE_SELECTED,&fselect)==B_OK) && (to->FindBool(P_C_NODE_SELECTED,&tselect) == B_OK) ) {
					if (fselect && tselect)
						copyMessage->AddMessage("node",indexer->IndexConnection(node,true));
					else if (fselect || tselect)
						copyMessage->AddMessage("node",indexer->IndexConnection(node));
				}
			}

		}	
		doc->Unlock();
	}
	if (be_clipboard->Lock()) {
		be_clipboard->Clear();
		if (clip = be_clipboard->Data())  {
			clip->AddData("application/x-vnd.projectconceptor-document", B_MIME_TYPE, copyMessage, sizeof(copyMessage));
			clip->AddMessage("test",copyMessage);
			PRINT_OBJECT(*clip);
			be_clipboard->Commit();
		}
		be_clipboard->Unlock();
	}
	settings = PCommand::Do(doc,settings);
	return settings;
}



void Copy::AttachedToManager(void)
{
}

void Copy::DetachedFromManager(void)
{
}
