// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DGCharacter.h"

#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"

#include "DGInventoryItem.h"
#include "DGBrowseableActor.h"
#include "DGInteractable.h"
#include "DrawDebugHelpers.h"
#include "DGGameInstance.h"
#include "DGInventoryComponent.h"
#include "DGEntriesManager.h"
#include "DGInvestigationSubject.h"

ADGCharacter::ADGCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("DGCharacterCameraComponent"));
	if (CameraComponent != nullptr)
	{
		CameraComponent->SetupAttachment(RootComponent);
		CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 65.0f));
		CameraComponent->bUsePawnControlRotation = true;
	}

	BrowseModeBoundComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DGCharacterViewModeBoundComponent"));
	if (BrowseModeBoundComponent != nullptr)
	{
		BrowseModeBoundComponent->SetupAttachment(CameraComponent);
		BrowseModeBoundComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BrowseModeBoundComponent->SetBoxExtent(FVector(100.0f, 50.0f, 50.0f));
		BrowseModeBoundComponent->SetRelativeLocation(FVector(95.0f, 0.0f, 0.0f));
		BrowseModeBoundComponent->SetHiddenInGame(true);
	}

	BrowseItemComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DGCharacterViewItemComponent"));
	if (BrowseItemComponent != nullptr)
	{
		BrowseItemComponent->SetupAttachment(BrowseModeBoundComponent);
		BrowseItemComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BrowseItemComponent->CastShadow = false;
		BrowseItemComponent->bRenderCustomDepth = true;
		BrowseItemComponent->bUseAttachParentBound = true;
	}

	InventoryComponent = CreateDefaultSubobject<UDGInventoryComponent>(TEXT("DGCharacterInventoryComponent"));
}

void ADGCharacter::BeginPlay()
{
	Super::BeginPlay();

	ResetBrowsing();
}

void ADGCharacter::OnNewEntriesDiscovered(UDGEntriesManager* InEntriesManager, const TScriptInterface<IDGInvestigationSubject> InNewEntry)
{
	if(InEntriesManager == nullptr 
		|| InNewEntry == nullptr)
	{

		return;
	}

	const FText SubjectName = InNewEntry->GetInvestigationSubjectName();

	UE_LOG(LogTemp, Warning, TEXT("New Entrie Descovered: %s"), *SubjectName.ToString());
}

void ADGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADGCharacter::MoveForward_Input);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADGCharacter::MoveRight_Input);
	PlayerInputComponent->BindAxis("Turn", this, &ADGCharacter::Turn_Input);
	PlayerInputComponent->BindAxis("LookUp", this, &ADGCharacter::LookUp_Input);
	PlayerInputComponent->BindAxis("ZoomItem", this, &ADGCharacter::ZoomItem_Input);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADGCharacter::OnInteract_Input);
	PlayerInputComponent->BindAction("ViewFirstMesh", IE_Pressed, this, &ADGCharacter::OnBrowseItem_Input);
}

void ADGCharacter::MoveForward_Input(float value)
{
	if (!IsBrowsingAnyItem())
	{
		if (Controller != nullptr)
		{
			FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
			AddMovementInput(Direction, value);
		}
	}
	else if (BrowseItemComponent != nullptr)
	{
		BrowseItemComponent->AddRelativeRotation(FQuat(FRotator(-value, 0.0f, 0.0f)));
	}
}

void ADGCharacter::MoveRight_Input(float value)
{
	if (!IsBrowsingAnyItem())
	{
		if (Controller != nullptr)
		{
			FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
			AddMovementInput(Direction, value);
		}
	}
	else if (BrowseItemComponent != nullptr)
	{
		BrowseItemComponent->AddRelativeRotation(FQuat(FRotator(0.0f, -value, 0.0f)));
	}
}

void ADGCharacter::LookUp_Input(float value)
{
	if (!IsBrowsingAnyItem())
	{
		AddControllerPitchInput(value);
	}
}

void ADGCharacter::Turn_Input(float value)
{
	if (!IsBrowsingAnyItem())
	{
		AddControllerYawInput(value);
	}
}

void ADGCharacter::ZoomItem_Input(float value)
{
	if (IsBrowsingAnyItem() && BrowseItemComponent != nullptr && BrowseItem != nullptr)
	{
		FVector Current = BrowseItemComponent->GetRelativeTransform().GetLocation();

		float NewX = FMath::Clamp(Current.X + value, BrowseItem->MinZoomInBrowseValue, BrowseItem->MaxZoomInBrowseValue);
		BrowseItemComponent->SetRelativeLocation(FVector(NewX, Current.Y, Current.Z));
	}
}

void ADGCharacter::OnInteract_Input()
{
	if (!IsBrowsingAnyItem())
	{
		TArray<FHitResult> OutHit;
		MakeLineTrace(OutHit, ECR_Overlap, ECollisionChannel::ECC_Pawn, UDGInteractable::StaticClass());

		if (OutHit.Num() > 0)
		{
			//Execute interaction only on first occurred interact object
			IDGInteractable::Execute_Interact(OutHit[0].Actor.Get(), this);
		}
	}
}

void ADGCharacter::OnBrowseItem_Input()
{
	if (IsBrowsingAnyItem())
	{
		ResetBrowsing();
	}
	else
	{
		SetMeshForBrowsing((uint16)0);
	}
}

void ADGCharacter::MakeLineTrace(TArray<FHitResult>& OutHit, FCollisionResponseParams CollisionResponseParams, ECollisionChannel ColisionChanel, UClass* ClassToFiltr/* = UClass::StaticClass()*/) const
{
	UWorld* World = GetWorld();
	if (World != nullptr && CameraComponent != nullptr)
	{
		FVector Start(CameraComponent->GetComponentLocation());
		FVector End(Start + GetControlRotation().Vector() * 160);

		FCollisionShape MyColSphere = FCollisionShape::MakeSphere(15.0f);

		FCollisionQueryParams ObstacleCollisionQueryParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
		ObstacleCollisionQueryParams.bTraceComplex = false;
		ObstacleCollisionQueryParams.bReturnFaceIndex = false;
		ObstacleCollisionQueryParams.bReturnPhysicalMaterial = false;

		TArray<FHitResult> OutActorsHit;
		World->SweepMultiByChannel(OutActorsHit, Start, End, FQuat::Identity, ColisionChanel, MyColSphere, ObstacleCollisionQueryParams, CollisionResponseParams);

		DrawDebugCylinder(GetWorld(), Start, End, 5.0f, 5, FColor::Green, true);

		OutHit.Empty();
		for (auto& hit : OutActorsHit)
		{
			if (hit.Actor != nullptr && (hit.Actor->IsA(ClassToFiltr) || hit.Actor->GetClass()->ImplementsInterface(ClassToFiltr)))
			{
				OutHit.Add(hit);
			}
		}
	}
}

void ADGCharacter::SetMeshForBrowsing(ADGBrowseableActor* Item)
{
	ResetBrowsing();

	if (Item != nullptr && BrowseItemComponent != nullptr)
	{
		BrowseItem = Item;
		BrowseItemComponent->SetStaticMesh(Item->GetItemMesh());	

		const int32 MaterialSlotsNum = BrowseItemComponent->GetNumMaterials();

		for(int32 i = 0; i < MaterialSlotsNum; ++i)
		{
			UMaterialInterface* MaterialForSlot = Item->GetMaterialForBrowse(i);

			if(MaterialForSlot != nullptr)
			{
				BrowseItemComponent->SetMaterial(i, MaterialForSlot);
			}			
		}
		
		float MiddlePoint = Item->MinZoomInBrowseValue + (Item->MaxZoomInBrowseValue - Item->MinZoomInBrowseValue) / 2;
		BrowseItemComponent->SetRelativeLocation(FVector(MiddlePoint, 0.0f, 0.0f));

		if (CameraComponent != nullptr)
		{
			//Background blure during item view
			//CameraComponent->PostProcessSettings.bOverride_DepthOfFieldMethod = true;
			//CameraComponent->PostProcessSettings.DepthOfFieldMethod = TEnumAsByte<EDepthOfFieldMethod>(EDepthOfFieldMethod::DOFM_CircleDOF);
			//CameraComponent->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
			//CameraComponent->PostProcessSettings.DepthOfFieldFocalDistance = DepthOfFieldValue;

			TArray<FWeightedBlendable>& Materials = CameraComponent->PostProcessSettings.WeightedBlendables.Array;
			if (Materials.Num() > 0)
			{
				Materials[0].Weight = 1;
			}
		}
	}
}

void ADGCharacter::SetMeshForBrowsing(uint16 IndexInInventory)
{
	ADGInventoryItem* Item = InventoryComponent->GetItem(IndexInInventory);
	if (Item != nullptr)
	{
		SetMeshForBrowsing(Item);
	}
}

void ADGCharacter::ResetBrowsing()
{
	BrowseItem = nullptr;
	if (BrowseItemComponent != nullptr)
	{
		BrowseItemComponent->SetMaterial(0, nullptr);
		BrowseItemComponent->SetStaticMesh(nullptr);
		BrowseItemComponent->SetRelativeLocation(FVector::ZeroVector);
		BrowseItemComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
	if (CameraComponent != nullptr)
	{
		//Background blure during item view
		//CameraComponent->PostProcessSettings.bOverride_DepthOfFieldMethod = false;
		//CameraComponent->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = false;

		TArray<FWeightedBlendable>& Materials = CameraComponent->PostProcessSettings.WeightedBlendables.Array;
		if (Materials.Num() > 0)
		{
			Materials[0].Weight = 0;
		}
	}
}

bool ADGCharacter::IsBrowsingAnyItem() const
{
	return  (BrowseItemComponent != nullptr && BrowseItemComponent->GetStaticMesh() != nullptr) && BrowseItem != nullptr;
}

ADGBrowseableActor* ADGCharacter::GetCurrentBrowsable() const
{
	return BrowseItem;
}

FText ADGCharacter::GetSuspectName() const
{
	return CharacterName;
}
