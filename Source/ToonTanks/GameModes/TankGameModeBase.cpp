// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"



void ATankGameModeBase::BeginPlay()
{  
   TargetTurrets = GetTargetTurretCount();
   PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

   HandleGameStart();
   
   Super::BeginPlay();
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
    // Get references and game win/lose conditions.
    TSubclassOf<APawnTurret> ClassToFind;
    ClassToFind = APawnTurret::StaticClass();
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, TurretActors);
    return TurretActors.Num();
}


void ATankGameModeBase::ActorDied(AActor *DeadActor)
{
    // Check what type of Actor died. If Turret, tally. If Player -> go to lose condition.
    UE_LOG(LogTemp, Warning, TEXT("An Actor died"));
    if(DeadActor == PlayerTank)
   {
       PlayerTank->PawnDestroyed();
       HandleGameOver(false);
       if(PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }

   }
   else if(APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
   {
       
       DestroyedTurret->PawnDestroyed();

       TargetTurrets--;

       if(TargetTurrets == 0)
       {
           HandleGameOver(true);
       }
   }

} 

void ATankGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();
    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef, 
            &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }


} 

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    // See if the player has destroyed all the turrets, show win result.
    // else if turret destroyed player, show lose result. 
    // Call blueprint version GameOver();
        GameOver(PlayerWon);

}
