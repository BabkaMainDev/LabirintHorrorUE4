#include "DeathMenuWidget.h"
#include "Components/Button.h"
#include "LabirintHorrorCharacter.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UDeathMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerCharacter = Cast<ALabirintHorrorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // Привязка кнопки
    if (RespawnButton)
    {
        RespawnButton->OnClicked.AddDynamic(this, &UDeathMenuWidget::OnRespawnButtonClicked);
    }
}

void UDeathMenuWidget::OnRespawnButtonClicked()
{
    Respawn();
}


void UDeathMenuWidget::Respawn()
{
    if (PlayerCharacter)
    {
        PlayerCharacter->Respawn();
        RemoveFromParent();
    }
}
