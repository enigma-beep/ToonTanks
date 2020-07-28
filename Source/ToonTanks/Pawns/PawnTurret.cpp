// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"


// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
    Super::BeginPlay();
    
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true, false);

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // If the PlayerPawn reference isn't valid or it's out of range, return. 
    if(!PlayerPawn || ReturnDistanceToPlayer() > FireRange)
    {
        return;
    }
    // If we have a PlayerPawn in range then rotate the turret to follow. 
    RotateTurret(PlayerPawn->GetActorLocation());


}

void APawnTurret::CheckFireCondition()
{
    // If Player == null || is Dead THEN BAIL!!

    if(!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
    {
        return;
    }
    // If Player Player IS in range THEN FIRE!!!
    if(ReturnDistanceToPlayer() <= FireRange)
    {
        // Fire
        UE_LOG(LogTemp, Warning, TEXT("FIRE!!"));
        Fire();
    }
}

float APawnTurret::ReturnDistanceToPlayer()
{
    // Check if we have a valid reference to the player.
    if (!PlayerPawn)
    {
        return 0.0f;
    }

    float Distance = (PlayerPawn->GetActorLocation() - GetActorLocation()).Size();
    return Distance;

}
void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

