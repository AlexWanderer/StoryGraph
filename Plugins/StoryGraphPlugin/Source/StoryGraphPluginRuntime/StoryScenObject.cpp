// Copyright 2016 Dmitriy Pavlov
#include "StoryGraphPluginRuntime.h"
#include "StoryScenObject.h"
#include "HUD_StoryGraph.h"


//IStoryScenObject.......................................................
void IStoryScenObject::RefreshScenObjectActive()
{
	bool IsActive = false;

	for (int i = 0; i < OwningStoryGraphObject.Num(); i++)
	{
		if (OwningStoryGraphObject[i]->IsScenObjectActive)
		{
			IsActive = true;
			break;
		}
	}
	EnabelObjectOnMap(IsActive);
}

void IStoryScenObject::SetStoryGraphObjectState(int NewState)
{
	for (int i = 0; i < OwningStoryGraphObject.Num(); i++)
	{
		if (OwningStoryGraphObject[i])
		{
			OwningStoryGraphObject[i]->SetCurentState(NewState);
		}
	}
}

//ACharecter_StoryGraph........................................................................
ACharecter_StoryGraph::ACharecter_StoryGraph()
{
	
}
void ACharecter_StoryGraph::ChangeState(ECharecterStates NewState)
{
	SetStoryGraphObjectState((int)NewState);
}

void ACharecter_StoryGraph::OpenDialog()
{
	if (OwningStoryGraphObject.Num() > 0)
	{
		AHUD_StoryGraph* HUD = Cast<AHUD_StoryGraph>(GetWorld()->GetFirstPlayerController()->GetHUD());

		HUD->OpenDialogOrOpenPlaceTriggerMessages(OwningStoryGraphObject);
	}
}

void ACharecter_StoryGraph::EnabelObjectOnMap(bool Enabel)
{
	IsEnabel = Enabel;
	SetActorHiddenInGame(!Enabel);
	SetActorEnableCollision(Enabel);
	SetActorTickEnabled(Enabel);
}

void ACharecter_StoryGraph::SendMessageToScenObject(FString Message)
{
	GetMessegeFromStoryGraph(Message);
}

FText ACharecter_StoryGraph::GetObjectName()
{
	return OwningStoryGraphObject.Num() > 0 ? OwningStoryGraphObject[0]->ObjName : FText::FromString("Non");
}

void AScenObjectActor_StoryGraph::EnabelObjectOnMap(bool Enabel)
{
	IsEnabel = Enabel;
	SetActorHiddenInGame(!Enabel);
	SetActorEnableCollision(Enabel);
	SetActorTickEnabled(Enabel);
}

//AScenObjectActor_StoryGraph....................................................

void AScenObjectActor_StoryGraph::SendMessageToScenObject(FString Message)
{
	GetMessegeFromStoryGraph(Message);
}

FText AScenObjectActor_StoryGraph::GetObjectName()
{
	return OwningStoryGraphObject.Num() > 0 ? OwningStoryGraphObject[0]->ObjName : FText::FromString("Non");
}

//APlaceTrigger_StoryGraph......................................................................



void APlaceTrigger_StoryGraph::ChangeState(EPlaceTriggerStates NewState)
{
	SetStoryGraphObjectState((int)NewState);
	
}

void APlaceTrigger_StoryGraph::Activate()
{
	TArray<UStoryGraphObjectWithScenObject*> AdvanceInteractiveTriggers;

	for (int i = 0; i < OwningStoryGraphObject.Num(); i++)
	{
		switch (((UStoryGraphPlaceTrigger*)OwningStoryGraphObject[i])->PlaceTriggerType)
		{
		case EPlaceTriggerType::UnInteractive:

			break;

		case EPlaceTriggerType::Interactive:

			OwningStoryGraphObject[i]->SetCurentState((int)EPlaceTriggerStates::Active);
			break;

		case EPlaceTriggerType::AdvanceInteractive:

		
			AdvanceInteractiveTriggers.Add(OwningStoryGraphObject[i]);
			break;
		

		}
		
	}
	if (AdvanceInteractiveTriggers.Num() > 0)
	{
		AHUD_StoryGraph* HUD = Cast<AHUD_StoryGraph>(GetWorld()->GetFirstPlayerController()->GetHUD());

		HUD->OpenDialogOrOpenPlaceTriggerMessages(AdvanceInteractiveTriggers);
	}
}

EPlaceTriggerType APlaceTrigger_StoryGraph::GetPlaceTriggerType()
{
	bool Interactive = false;
	bool AdvanceInteractive = false;

	for (int i = 0; i < OwningStoryGraphObject.Num(); i++)
	{
		
		switch (((UStoryGraphPlaceTrigger*)OwningStoryGraphObject[i])->PlaceTriggerType)
		{
		case EPlaceTriggerType::UnInteractive:

			break;

		case EPlaceTriggerType::Interactive:

			Interactive = true;
			break;

		case EPlaceTriggerType::AdvanceInteractive:

			AdvanceInteractive = true;
			break;
		}
	}

	if (AdvanceInteractive) return EPlaceTriggerType::AdvanceInteractive;
	
	if (Interactive) return EPlaceTriggerType::Interactive;
	
	return EPlaceTriggerType::UnInteractive;

}

void APlaceTrigger_StoryGraph::OpenMessageDialog()
{
	if (OwningStoryGraphObject.Num() > 0)
	{
		AHUD_StoryGraph* HUD = Cast<AHUD_StoryGraph>(GetWorld()->GetFirstPlayerController()->GetHUD());

		HUD->OpenDialogOrOpenPlaceTriggerMessages(OwningStoryGraphObject);
	}
}



//AInventoryItem_StoryGraph.............................................................................


void AInventoryItem_StoryGraph::PickUp()
{
	SetStoryGraphObjectState((int)EInventoryItemeStates::InInventory + 1);
	
	EnabelObjectOnMap(false);
		
}


void AInventoryItem_StoryGraph::RefreshScenObjectActive()
{
	
	bool IsActive = false;

	for (int i = 0; i < OwningStoryGraphObject.Num(); i++)
	{
		if (OwningStoryGraphObject[i]->IsScenObjectActive)
		{
			IsActive = true;
			break;
		}
	}
	if (IsActive && OwningStoryGraphObject[0]->GetCurentState() != (int)EInventoryItemeStates::OnLevel + 1)
	{
		IsActive = false;
	}
	EnabelObjectOnMap(IsActive);
}



